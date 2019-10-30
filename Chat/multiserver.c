#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<arpa/inet.h> 
#include<sys/socket.h> 
#include<sys/types.h> 
#include<unistd.h> 

void handle (char *msg) {
    printf("%s\n", msg); 
    exit(0); 
}

void main() { 
    int sock; 
    struct sockaddr_in server;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0))<0){ 
        handle("Cannot create socket!"); 
    }

    bzero(&server, sizeof(server)); 
    server.sin_family = AF_INET; 
    server.sin_port = htons(1024); 
    server.sin_addr.s_addr = INADDR_ANY; 

    if (bind(sock, (struct sockaddr *) &server, sizeof(server))<0){ 
        handle("Cannot bind!"); 
    }

    int newSock[2]; 
    struct sockaddr_in clients[2]; 
    int clientLen = sizeof(clients[0]); 

    int i; 
    for (i=0; i<2; i++){ 
        listen(sock, 2); 
        newSock[i] = accept(sock, (struct sockaddr *) &server, &clientLen); 
    }

    int n; 
    char buffer[128]; 
    while (1) 
    {
        for (i=0; i<2; i++){ 
            printf("Enter message: "); 
            scanf("%s", buffer); 
            
            if (strcmp(buffer, "bye") == 0){ 
                for (i=0; i<2; i++){ 
                    n = write(newSock[i], buffer, sizeof(buffer)); 
                }
                break; 
            }
            
            n = write(newSock[i], buffer, sizeof(buffer)); 
            n = read(newSock[i], buffer, sizeof(buffer)); 
            printf("Client %d: %s\n", i+1, buffer); 
        }

        break; 
    }

    close(sock); 
    for (i=0; i<2; i++)
        close(newSock[i]); 
    
}