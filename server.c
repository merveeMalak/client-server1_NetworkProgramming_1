#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>


#define BUFFER_SIZE 2048


/*Waits for a new message from the client,
 if it comes, it prints the screen and
 sends it back to the client.
*/
void read_from_client_write_client(int simpleChildSocket, char *name){
    char buff[BUFFER_SIZE];
    for (;;){
        printf("Waiting new message...\n");
        bzero(buff, BUFFER_SIZE);
        read(simpleChildSocket, buff, sizeof(buff)); //reads message from client
        printf("From client: %s\n", buff);
        write(simpleChildSocket, buff, sizeof(buff)); //sends message to client
        if (strncmp("good bye", buff, 8) == 0){
            printf("Good bye %s!\n", name);
            break;
        }
    }
}
    int main(int argc, char *argv[]) {
    int simpleSocket = 0;
    int simplePort = 0;
    int returnStatus = 0;
    struct sockaddr_in simpleServer;

        if (2 != argc) {
            fprintf(stderr, "Usage: %s <port>\n", argv[0]);
            exit(1);
        }

        simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (simpleSocket == -1) {
            fprintf(stderr, "Could not create a socket!\n");
            exit(1);
        }
        else {
            printf("Server Socket number: %d\n", simpleSocket);
            fprintf(stderr, "Socket created!\n");
        }
        int val=1;
        if ((setsockopt(simpleSocket, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val))) < 0){
            fprintf(stderr, "Server\n");
            exit(1);
        }

        /* retrieve the port number for listening */
        simplePort = atoi(argv[1]);
        /* set up the address structure */
        /* use INADDR_ANY to bind to all local addresses
        */
        bzero(&simpleServer, sizeof(simpleServer));
        simpleServer.sin_family = AF_INET;
        simpleServer.sin_addr.s_addr = htonl(INADDR_ANY);
        simpleServer.sin_port = htons(simplePort);
        /*
        bind to the address and port with our socket
        */
        returnStatus = bind(simpleSocket,(struct sockaddr *)&simpleServer,sizeof(simpleServer));

        if (returnStatus == 0) {
            fprintf(stderr, "Bind completed!\n");
        }
        else {
            fprintf(stderr, "Could not bind to address!\n");
            close(simpleSocket);
            exit(1);
        }
    /* let's listen on the socket for connections*/
    returnStatus = listen(simpleSocket, 5);
    if (returnStatus == -1) {
        fprintf(stderr, "Cannot listen on socket!\n");
        close(simpleSocket);
        exit(1);
    }

    while (1){
        struct sockaddr_in clientName = { 0 };
        int simpleChildSocket = 0;
        int clientNameLength = sizeof(clientName);
        printf("********************\n");
        printf("Waiting new client...\n");
        /* wait here */
        simpleChildSocket = accept(simpleSocket,(struct sockaddr *)&clientName,&clientNameLength);
        
        if (simpleChildSocket == -1) {
            fprintf(stderr, "Cannot accept connections!\n");
            close(simpleSocket);
            exit(1);
        }
        
        printf("Client accepted!\n Client IP address: %s\n", inet_ntoa(clientName.sin_addr)); //print ip_adrr of client
        struct hostent *hostName;
        struct in_addr ip_addr;
        inet_aton(inet_ntoa(clientName.sin_addr), & ip_addr);  
        hostName = gethostbyaddr(&ip_addr, sizeof(ip_addr), AF_INET); //IP address of client convert the name
        printf("Host name: %s\n", hostName->h_name); //prints host name 
        read_from_client_write_client(simpleChildSocket, hostName->h_name);
        close(simpleChildSocket);
      }
        close(simpleSocket);
        return 0;
}
