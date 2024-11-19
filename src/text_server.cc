/**
 * @author Rasoul Nikmehr
 */

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "domain_socket_server.cc"
#include "server.h"
#include <sys/sysinfo.h>

// Message format:
// filename<US>opcode<US>search_string1<US>search_string2<US>...<EOT>

int main(int argc, char* argv[]) {

  if (argc < 2) {
    std::cerr << "Usage: ./text-server SOCKET-NAME" << std::endl;
    return 1;
  }

  Server server(AF_UNIX);

  // Remove the socket file if already exists
  // If it doesn't exist, the call to the remove function returns and error that is ignored by us
  std::remove(argv[1]);

  int socket_fd = server.create_socket(argv[1]);
  
  int client_fd;
  std::string opcode;
  std::vector<std::string>* search_list = new std::vector<std::string>;

  // Listen for incoming connections
  if (listen(socket_fd, get_nprocs_conf() - 1) == -1) {
    perror("listen");
    exit(1);
  }

  // Wait for a client to connect
  while (true) {
    client_fd = accept(socket_fd, NULL, NULL);

    if (search_list != nullptr) {
      search_list->clear();
    }

    if (client_fd == -1) {
      perror("Accept failed");
      exit(1);
    }

    std::clog << "SERVER STARTED" << std::endl;
    std::clog << "\tMAX CLIENTS: " << get_nprocs_conf() - 1 << std::endl;

    std::clog << "CLIENT CONNECTED" << std::endl;

    // Read input from the client
    char buffer[1024];
    ssize_t nread = read(client_fd, buffer, sizeof(buffer));
    if (nread == -1) {
      perror("Read failed");
      exit(1);
    }

    std::string input(buffer, nread);

    std::vector<std::string> message_parts = server.parseMessage(input);
    std::string text_file_path = message_parts[0];
    opcode = message_parts[1];

    for (size_t i = 2; i < message_parts.size(); i++) {
      search_list->push_back(message_parts[i]);
    }

    std::clog << "\tPATH: \"" << text_file_path << "\"" << std::endl;
    std::clog << "\tOPERATION: " << server.operatorName(opcode) << std::endl;

    std::clog << "\tSEEKING: ";
    for (size_t i = 0; i < search_list->size(); i++) {
      std::clog << search_list->at(i);
      if (i < search_list->size() - 1) {
        std::clog << ", ";
      } else {
        std::clog << std::endl;
      }
    }

    std::vector<std::string>* found = new std::vector<std::string>;
    size_t n;
    int sent_bytes = 0;

    if (server.searchInFile(text_file_path, opcode, search_list, found) != false) {
      for (size_t i = 0; i < found->size(); i++) {
        n = write(client_fd, found->at(i).data(), found->at(i).size());
        write(client_fd, "\n", 1);
        sent_bytes += n;
      }
    } else {
      write(client_fd, "INVALID_FILE", 12);
    }

    std::clog << "BYTES SENT: " << sent_bytes << std::endl;

    // Close the client connection
    close(client_fd);
  }

  // Close the server socket
  close(socket_fd);

  return 0;
}
