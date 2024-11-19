**Project2: IPC**
This is a Client/Server project implemented using Unix Domain Sockets. It enables the client to send queries to the server by specifying a file name and search parameters and receive results. The project demonstrates inter-process communication (IPC), multithreading, and file handling in C++.
________________________________________
**Features**
•	Unix Domain Sockets: Facilitates IPC between the client and server.
•	Multithreading: Handles multiple client connections concurrently.
•	CSV File Processing: Reads large CSV datasets and performs keyword-based searches.
•	Error Handling: Gracefully handles invalid inputs and unsupported operations.
________________________________________
**Included Files**
•	domain_socket.h: Defines the domain socket class.
•	client.h: Header file for client logic.
•	server.h: Header file for server logic.
•	domain_socket_client.cc: Implements client-side socket communication.
•	domain_socket_server.cc: Implements server-side socket communication.
•	text_client.cc: Main client implementation.
•	text_server.cc: Main server implementation.
•	Makefile: Automates the build process.
•	exe.sh: Script to clean, build, and run the system.
________________________________________
**Skills Demonstrated**
•	Programming Languages: C++ for file handling, socket programming, and multithreading.
•	Systems Programming: Unix Domain Sockets for IPC.
•	Automation: Makefile and shell scripting for building and testing.
•	Debugging: Error handling and multithreaded environment management.
________________________________________
**How to Build**
To compile the client and server:
make text-server
make text-client
________________________________________
**How to Run**
Start the Server
To start the server:
./text-server csv_server_sock
Start the Client

To start the client and perform operations:
For AND Operations:
./text-client csv_server_sock ./dat/bankloan1.csv KEYWORD1 x KEYWORD2

For OR Operations:
./text-client csv_server_sock ./dat/bankloan1.csv KEYWORD1 + KEYWORD2
________________________________________
**Output Example**

Client Output:
SERVER CONNECTION ACCEPTED
1   3, 195600, 52100, 3.75, 37, industrial, farm, ftb, default
2   6, 133000, 45300, 2.94, 29, industrial, farm, ftb, default
BYTES RECEIVED: 370

Server Log:
SERVER STARTED
    MAX CLIENTS: 7
CLIENT CONNECTED
    PATH: "dat/bankloan2.csv"
    OPERATION: AND
    SEEKING: indus, farm, default
BYTES SENT: 370
________________________________________
**Testing**
Provided Test Scripts
Use the provided scripts in the Tests/ folder to validate the program's functionality:
1.	Run AND Operations Test:
bash Tests/test_and.sh
2.	Run OR Operations Test:
bash Tests/test_or.sh
These scripts execute predefined test cases and output results for validation.
