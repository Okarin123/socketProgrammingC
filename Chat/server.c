#include<stdio.h> 
#include<sys/socket.h> 
#include<string.h> 
#include<sys/types.h> 
#include<unistd.h> 
#include<arpa/inet.h> 
#include<stdlib.h> 

void handle(char *msg){ 
    printf("%s\n",msg); 
    exit(0); 
} 

int main(){

    char serverMessage[256]; 
    char clientMessage[256]; 
    
    //socket creation
    int sock; 
    if ((sock = socket(AF_INET,SOCK_STREAM,0))<0){
        handle("Cannot create socket!"); 
    }

    //bind 
    struct sockaddr_in serverAddr; 
    bzero(&serverAddr,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_port = htons(7223);
    serverAddr.sin_addr.s_addr = INADDR_ANY; 

    if (bind(sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr))<0){ 
        handle("Cannot bind!"); 
    }

    //listen and accept 
    listen(sock,2); 

    int clientSock;
    struct sockaddr_in clientAddr; 
    int clientAddrLen = sizeof(clientAddr);   
    clientSock = accept(sock, (struct sockaddr *) &clientAddr, (socklen_t *) &clientAddrLen);     
    
    //printing connected message
    char *clientIP = inet_ntoa(clientAddr.sin_addr);  
    printf("Message sent to client %s\n", clientIP);  

    int pid = fork(); 
    if (pid>0){ 
        while (1)
        {
            scanf("%s",serverMessage);
            send(clientSock,serverMessage,sizeof(serverMessage),0);
        }
    } 
    else { 
        while(1){ 
            recv(clientSock, &clientMessage, sizeof(clientMessage), 0);
            printf("Client: %s\n", clientMessage); 

            if (strcmp("break",clientMessage) == 0){ 
                break; 
            }
        }
    }
     
    close(clientSock); 
    close(sock); 
    return 0;     
}