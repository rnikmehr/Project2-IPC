#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

#include "domain_socket_client.cc"

int main(int argc, char* argv[]) {
  Client client(AF_UNIX);

  if (argc < 3) {
    std::cerr << "Usage: ./text-client ./path/to/socket_name ./path/to/text-file expr1 + expr2 + ..." << std::endl;
    return 1;
  }

  client.set_socket_name(argv[1]);
  client.set_file_name(argv[2]);

  std::string search_strings;

  int sockfd = client.create_socket();
  client.connect_to_server();
  client.set_search_strings_from_args(argv, argc);
  std::string message = client.generate_message();

  client.send_message(message);

  char buffer[1024];
  int num_bytes = 0;
  std::vector<std::string> lines;
  std::string current_line;

  while (true) {
    int bytes_recv = recv(sockfd, buffer, sizeof(buffer), 0);
    if (bytes_recv == 0) {
      break;
    }

    for (int i = 0; i < bytes_recv; i++) {
      char c = buffer[i];
      if (c == '\n') {
        lines.push_back(current_line);
        current_line = "";
      } else {
        current_line += c;
        num_bytes++;
      }
    }

    if (current_line == "INVALID_FILE") {
      std::cout << current_line << std::endl;
      exit(-1);
    }
  }

  for (size_t i = 0; i < lines.size(); i++) {
    std::cout << (i + 1) << '\t' << lines[i] << std::endl;
  }

  std::clog << "BYTES RECEIVED: " << num_bytes << std::endl;

  close(sockfd);
  return 0;
}