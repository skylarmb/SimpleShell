//Authors: Ryan Gliever (rgliever) and Skylar Brown (skmbrown) 
//With help from: Prof. Vinod Pillai 
//(www.vinodthebest.wordpress.com/category/c-programming/c-network- 
//programming), and www.beej.us/guide/bgnet/output/html/singlepage/ 
//bgnet.html //CMPE 150 Final Project

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

struct sockaddr_in server_address;
struct hostent* server;

//error helper function to print to stderr and exit
void error(char* message){
    fprintf(stderr,"%s\n",message);
    printf("%s\n",message);
    exit(1);
}
 
int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    //buffer and cmd are allocated. cmd will sometimes hold a copy
    //of buffer for error printing
    char buffer[1024];
    char cmd[1024];
    if (argc < 3) error("Usage: <ClientProgramName> PORT IPADDRESS");
    portno = atoi(argv[1]);
    //set socket file descriptor
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR: Opening socket");
    //try to connect to server
    server = gethostbyname(argv[2]);
    if(server == NULL) error("ERROR: Reaching host");
    memset(&server_address,0,sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy((char *) server->h_addr_list[0], 
      (char*)&server_address.sin_addr.s_addr,server->h_length);
    server_address.sin_port = htons(portno);
    //try to connect
    if (connect(sockfd,(struct sockaddr *)&server_address,
      sizeof(server_address)) < 0)
        error("ERROR: Connecting");
    //main loop for sending commands to server
    while(1) {
        //print prompt and clear buffer
        printf("$: ");
        memset(&buffer,0,sizeof(buffer));
        //read command
        fgets(buffer,1023,stdin);
        //quit if desired
        if(strcmp(buffer,"quit\n") == 0){
            printf("Bye!\n");
            //send termination message to server
            n = write(sockfd,"EOF",strlen("EOF"));
            close(sockfd);
            exit(0);
        }
        strcpy(cmd,buffer);
        //send command to server
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) {
            fprintf(stderr,"ERROR: writing to socket");
            exit(1);
        }
        //clear the buffer and prepare to recieve from server
        memset(&buffer,0,sizeof(buffer));
        sleep(1);
        n = read(sockfd,buffer,1023);
        if (n < 0){
            fprintf(stderr,"ERROR: reading from socket\n");
            printf("ERROR: reading from socket\n");
            break;
        }
        //if command not found, print appripriate error
        if(strcmp(buffer,"Command not found\n") == 0)
            printf("Command not found: %s\n",cmd); 
        //otherwise print what the server sent back
        else printf("%s\n",buffer);
    }
    return 0;
}

