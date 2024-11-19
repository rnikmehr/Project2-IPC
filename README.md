# Project2: IPC
This is a Client/Server project that allows the client to send queries to the server by specifying the file name and search parameters and receive the result from the server.


## Included files
- domain_socket.h: Domain Socket header file -- Includes the path to the socket
- client.cc: Client code
- server.cc: Server code
- Makefile: Used to compile the client and server
- server.h: Server header file
- client.h: Client header file

## Building the client and server
```sh
make text-server
make text-client
```

## Running the server
```sh
./text-server
```

## Running the client
```sh
./text-client csv_server_sock ./filename.csv KEYWORD1 + KEYWORD2
# or
./text-client csv_server_sock ./filename.csv KEYWORD1 x KEYWORD2 x KEYWORD3
