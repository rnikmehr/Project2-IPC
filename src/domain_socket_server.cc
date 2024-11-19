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

class Server : public DomainSocket {
 public:
  inline Server(int family) : DomainSocket(family) {}

  void set_socket_name(char* sock_name) { sock_name_ = sock_name; }

  void set_file_name(char* file_name) { file_name_ = file_name; }

  int create_socket(const char* socket_path) {
    // Creates a new socket and sets the socket file descriptor to socket_fd
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    std::string opcode;
    std::string text_file_path;

    if (sockfd == -1) {
      perror("Failed to create a socket");
      exit(1);
    }

    // use struct sockaddr_un to specify address of our Unix Domain Socket
    struct sockaddr_un addr;
    // Zero out addr to ensure all unused fields have our default value
    memset(&addr, 0, sizeof(struct sockaddr_un));
    // Our socket uses Unix Domain Address Family (AF_UNIX)
    // AF_UNIX is used for communication between processes on the same host
    addr.sun_family = AF_UNIX;

    // Copies socket_path to addr.sun_path
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

    // Bind the socket to the address
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) ==
        -1) {
      perror("bind");
      exit(1);
    }

    return sockfd;
  }

  std::vector<std::string> parseMessage(std::string message) {
    char delimiter = '\x1F';
    size_t pos = 0;
    std::string token;
    std::vector<std::string> parts;
    while ((pos = message.find(delimiter)) != std::string::npos) {
      token = message.substr(0, pos);
      parts.push_back(token);
      message.erase(0, pos + 1);
    }
    parts.push_back(message);

    return parts;
  }

  bool searchLine(std::string line, std::string op,
                  std::vector<std::string>* search_list) {
    if (op == "+") {
      for (size_t i = 0; i < search_list->size(); i++) {
        if (line.find(search_list->at(i).data()) != std::string::npos) {
          return true;
        }
      }
    } else if (op == "x") {
      size_t num_found = 0;
      for (size_t i = 0; i < search_list->size(); i++) {
        if (line.find(search_list->at(i).data()) != std::string::npos) {
          num_found++;
        }
      }

      // Return true, only if all items inside the search_list are found
      if (num_found == search_list->size()) {
        return true;
      }
    } else {  // No op
      if (line.find(search_list->at(0)) != std::string::npos) {
        return true;
      }
    }
    return false;
  }

  bool searchInFile(std::string filepath, std::string op,
                    std::vector<std::string>* search_list,
                    std::vector<std::string>* found) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
      return false;
    }

    std::string line;
    while (std::getline(file, line)) {
      if (searchLine(line, op, search_list) == true) {
        found->push_back(line);
      }
    }

    return true;
  }

  std::string operatorName(std::string opcode) {
    if (opcode == "x") {
      return "AND";
    } else if (opcode == "+") {
      return "OR";
    } else {
      return "n/a";
    }
  }

 private:
  char* file_name_ = 0;
  char* sock_name_ = 0;
  std::string message_;  // The string that will be sent to the server
  std::string search_strings_;
  std::vector<std::string>* search_list = new std::vector<std::string>;
};
