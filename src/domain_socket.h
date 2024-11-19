#ifndef DOMAIN_SOCKET_H
#define DOMAIN_SOCKET_H

class DomainSocket {
 public:
  inline DomainSocket(int family) : family_(family) {}

  inline int family() { return family_; }

  // <US> (unit separator) acts as the separator between the values sent to the
  // server
  const char kUS = static_cast<char>(31);

 private:
  int family_;
};
// #define SOCKET_PATH "./csv_server_sock"

#endif