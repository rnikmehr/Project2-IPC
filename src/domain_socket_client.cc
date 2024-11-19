/**
 * @author Rasoul Nikmehr
 */

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

#include "domain_socket.h"

class Client : public DomainSocket {
 public:
  inline Client(int family) : DomainSocket(family) {}

  void set_socket_name(char* sock_name) { sock_name_ = sock_name; }

  void set_file_name(char* file_name) { file_name_ = file_name; }

  int isOpcode(char* s) { return strcmp(s, "+") == 0 || strcmp(s, "x") == 0; }

  void set_search_strings_from_args(char* argv[], int argc) {
    for (int i = 3; i < argc; i++) {
      if (isOpcode(argv[i])) {
        if (opcode != nullptr && strcmp(argv[i], opcode) != 0) {
          std::cerr << "Mixed boolean operations not presently supported\n";
          exit(2);
        }
        opcode = argv[i];
        continue;
      }

      search_strings_ += argv[i];

      if (i != argc - 1) {
        search_strings_ += kUS;
      }
    }
  }

  int create_socket() {
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, sock_name_, sizeof(addr.sun_path) - 1);

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
      std::cerr << "Error creating socket\n";
      return 1;
    }

    return sockfd;
  }

  int connect_to_server() {
    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
      std::cerr << "Error connecting to server: " << std::strerror(errno);
      close(sockfd);
      return 1;
    }

    std::clog << "SERVER CONNECTION ACCEPTED\n";

    return 0;
  }

  std::string generate_message() {
    std::string op("-"); // Default to no op
    if (opcode != nullptr) {
      op = opcode;
    }
    
    message_ = std::string(file_name_) + kUS + op + kUS + search_strings_;
    return message_;
  }

  int send_message(std::string message) {
    if (send(sockfd, message.c_str(), message.length(), 0) == -1) {
      std::cerr << "Error sending data to server: " << std::strerror(errno)
                << std::endl;
      close(sockfd);
      return 1;
    }

    return 0;
  }

 private:
  char* file_name_ = 0;
  char* sock_name_ = 0;
  int sockfd;            // Socket file descriptor
  char* opcode = 0;      // +, x, or none (if there is only one search string)
  std::string message_;  // The string that will be sent to the server
  std::string search_strings_;
  struct sockaddr_un addr;
};
