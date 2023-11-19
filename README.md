An RSS.cpp and Client.cpp files to the assignment, to create a distributed system consisting of a client - server processes. The server will implement a CPU scheduling algorithm. The client will distribute tasks to servers, and you will analyze the performance based on completion time, throughput, and efficiency.

Server Process Implementation:
Create C++ file for server  RRServer.cpp.
Implement socket server code in the file, listening on 8002 ports.
the server must accept a connection, receive a task, process it (simulate processing by sleeping for the task's duration), and send back a completion confirmation.
Generate a set of tasks (use integers to represent task complexity).

Client Process Implementation:
Create a C++ file for the client (Client.cpp).
Implement socket client code to connect to all four servers.
Generate a set of tasks (use integers to represent task complexity).
