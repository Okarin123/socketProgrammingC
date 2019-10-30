#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
  
void handle(char *msg){ 
    printf("%s\n",msg); 
    exit(0); 
}

int main(int argc, char **argv){
    
    //create socket  
    int sock; 
    if ((sock = socket(AF_INET,SOCK_STREAM,0))<0){ 
        handle("Cannot create socket!"); 
    }

    //create address structure 
    struct sockaddr_in serverAddr; 
    bzero(&serverAddr,sizeof(serverAddr)); 
    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_port = htons(7223); 
    serverAddr.sin_addr.s_addr = INADDR_ANY; 

    int connectionStatus; 
    if ((connectionStatus = connect(sock,(struct sockaddr *) &serverAddr,sizeof(serverAddr)))<0){ 
        handle("Unable to connect properly!"); 
    }
    
    //chat portion of the code
    char serverMessage[128]; 
    char clientMessage[128];  
    int pid = fork();
    if (pid>0){ 
        while (1)
        {
            printf("Client: "); 
            scanf("%s", clientMessage); 
            send(sock, &clientMessage, sizeof(clientMessage), 0); 

            if (strcmp(clientMessage,"break") == 0){ 
                break; 
            } 
        }
    }

    else
    {
        while (1)
        {
            recv(sock, &serverMessage, sizeof(serverMessage), 0); 
            if (sizeof(serverMessage)>0){
                printf("Server: %s\n",serverMessage);
            }    
        }
    }
    
    close(sock); 
    return 0; 
}