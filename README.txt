CMPE 150 Final Project: Client-Server Model in C

To use this program, server.c and client.c must be run on different machines that are connected 
through a network. Both files can be compiled with gcc. The server side must be executed first, with
the name of the executable file for server.c followed by a port number. Once the connection is open,
the client side may be executed, with the name of the executable file for client.c followed by 
the same port number the server entered, followed by the IP address of the server side. The client side
then enters a simple UNIX command (date, ls, pwd, shutdown) and the server side relays the output back
to the client. To exit, the client enters 'quit', and the server side's connection stays open for
other clients to connect.

server: ./<ServerProgramName> PortNo
client: ./<ClientProgramName> PortNo ServerIPaddress

How this program works:
Server: Binds to socket, listens for incoming connection.
Client: Sends connection request to server using port number and server IP address.
Server: Accepts connection request from client, connects to client.
Client: Writes data request to server.
Server: Reads data request sent by client, writes back information requested.
Client: Reads response from server.
The client can keep writing requests to the server, and the server will respond with the data.
This process ends when the server closes the connection (or if the client closes the connection, but
other clients can still connect to the server in that case).
