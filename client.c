#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>


#define BUFFER_SIZE 2048

/*As long as the user does not enter the good bye message,
 receives a message from the user and sends this message to the server. 
*/
void read_from_stdin_write_server(int simpleSocket, char *name){
    char buff[BUFFER_SIZE];
    for (;;){
        bzero(buff, sizeof(buff));
        printf("Enter your message (enter good bye for exit): ");
        fgets(buff, BUFFER_SIZE, stdin);
        printf("Your messages is sending %s...\n", name);
        buff[strcspn(buff, "\n")] = 0;
        write(simpleSocket, buff, sizeof(buff)); //sends message to server
        bzero(buff, sizeof(buff));
        int returnStatus = read(simpleSocket, buff, sizeof(buff)); //reads message from server
        if (returnStatus > 0){
            printf("From Server: %s\n", buff);
            if ((strcmp(buff, "good bye")) == 0){
                printf("Client exit. Good bye %s!\n", name);
                printf("********************\n");
                break;
            }
        }
        else{
            fprintf(stderr, "Return Status = %d \n", returnStatus);
        }
        

    }

}

int main(int argc, char *argv[]) {
    int simpleSocket = 0;
    int simplePort = 0;
    int returnStatus = 0;
    char buffer[256] = "";
    struct sockaddr_in simpleServer;
    if (3 != argc) {
        fprintf(stderr, "Usage: %s <server> <port>\n", argv[0]);
        exit(1);
    }
    /* create a streaming socket
    */
    simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (simpleSocket == -1) {
        fprintf(stderr, "Could not create a socket!\n");
        exit(1);
    }
    else {
        printf("Client socket number: %d\n", simpleSocket);
        fprintf(stderr, "Socket created!\n");
    }
    /* retrieve the port number for connecting */
    simplePort = atoi(argv[2]);
    /* set up the address structure */
    /* use the IP address argument for the server address
    */
    bzero(&simpleServer, sizeof(simpleServer));
    simpleServer.sin_family = AF_INET;
    simpleServer.sin_port = htons(simplePort);
    char *ip = argv[1]; 
    simpleServer.sin_addr.s_addr = inet_addr(ip);
    /*
    connect to the address and port with our socket
    */
    returnStatus = connect(simpleSocket, (struct sockaddr *)&simpleServer, sizeof(simpleServer));
    
    if (returnStatus == 0) {
        fprintf(stderr, "Connect successful!\n");
    }
    else {
        fprintf(stderr, "Could not connect to address!\n");
        close(simpleSocket);
        exit(1);
    }
    printf("********************\n");
    printf("Server IP address: %s\n", inet_ntoa(simpleServer.sin_addr)); //prints ip_adrr of server
    struct hostent *hostName;
    struct in_addr ip_addr;
    inet_aton(inet_ntoa(simpleServer.sin_addr), & ip_addr);  
    hostName = gethostbyaddr(&ip_addr, sizeof(ip_addr), AF_INET); //IP address of server convert the name
    printf("Server name: %s\n", hostName->h_name); //prints server name 
    read_from_stdin_write_server(simpleSocket, hostName->h_name);
    close(simpleSocket);
    return 0;
}