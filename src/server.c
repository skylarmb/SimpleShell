//Authors: Ryan Gliever (rgliever) and Skylar Brown (skmbrown)
//With help from: Prof. Vinod Pillai 
//(www.vinodthebest.wordpress.com/category/c-programming/c-network-
//programming), and www.beej.us/guide/bgnet/output/html/singlepage/
//bgnet.html
//CMPE 150 Final Project
#define _BSD_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
struct sockaddr_in server_address, client_address;
//error function for writing error messages to stderr and stdout
void error(char *message) {
    fprintf(stderr, "%s\n", message);
    printf("%s\n", message);
    exit(1);
}
int main(int argc, char *argv[]) {
    //initialize socket file descriptor,
    //new socket file descriptor, port number, and
    //client address length
    int sockfd, newsockfd, portno, client_addr_len;
    char buffer[1024];
    int n;
    if (argc < 2) { //error if no port no.
        error("ERROR: no port provided\n");
    }
    //set sockfd accordingly
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR: on opening socket\n");
    }
    memset((char *) &server_address, 0, sizeof(server_address));
    portno = atoi(argv[1]);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(portno);
    //bind the socket to the port number
    if (bind(sockfd, (struct sockaddr *) &server_address,
sizeof(server_address)) < 0)
        error("ERROR: on binding\n");
    //listen for an incoming connection
    listen(sockfd,5);
    client_addr_len = sizeof(client_address);
    //accept the connection and set it to newsockfd
    newsockfd = accept(sockfd,
(struct sockaddr *) &client_address,
(socklen_t *) &client_addr_len);
    if (newsockfd < 0)
        error("ERROR: on accept\n");
    printf("Client has connected\n");
    char out[1024];
    while(1) {
        FILE *file;
        //set buffer values to zero
        memset(&buffer,0,sizeof(buffer));
        //read the incoming data and put it in buffer
        n = read(newsockfd,buffer,1023);
        //check if client has disconnected, and listen again 
        //if so.
        if(strcmp(buffer, "EOF") == 0){
            printf("Client has disconnected\n");
            listen(sockfd, 5);
            client_addr_len = sizeof(client_address);
            newsockfd = accept(sockfd,
(struct sockaddr *) &client_address,
(socklen_t *) &client_addr_len);
            if (newsockfd < 0) 
                error("ERROR: on accept\n");
            printf("Client has connected\n");
            continue;
        }
        if (n < 0) error("ERROR: in reading from socket\n");
        //put contents of buffer into a "file"
        file = popen(buffer, "r");
        if (file == NULL) error("ERROR: file is null\n");
        printf("%s",buffer);
        //get the output of the command
        char *get = fgets(out, sizeof(out)-1, file);
        char* err = "Command not found\n";
        if(get == NULL) {
            write(newsockfd, err, strlen(err));
            printf("Command not found: %s", buffer);
        }
        while (get != NULL) {
            //write the output to the client
            n = write(newsockfd,out,strlen(out));
            if (n < 0) error("ERROR: in writing to socket\n");
            get = fgets(out, sizeof(out)-1, file);
        }
        //close the "file"
        pclose(file);
    }
    return 0;
}

