#include<stdio.h> 
#include<string.h> 
#include<sys/socket.h> 
#include<sys/types.h> 
#include<arpa/inet.h>
#include<unistd.h> 
#include<stdlib.h> 

void handle (char *msg){
    printf("%s\n",msg); 
    exit(0); 
}

int main(){ 

    int sock; 
    struct sockaddr_in serverAddr; 

    if ((sock = socket(AF_INET, SOCK_STREAM, 0))<0){ 
        handle("Cannot create socket!"); 
    }

    bzero(&serverAddr, sizeof(serverAddr)); 
    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_port = htons(1024); 
    serverAddr.sin_addr.s_addr = INADDR_ANY; 

    int connectionStatus; 
    if ((connectionStatus = connect(sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)))<0){ 
        handle("Cannot connect!"); 
    }

    char domainName[128]; 
    printf("Enter domain: "); 
    scanf("%s", domainName); 
    send(sock, &domainName, sizeof(domainName), 0); 
    printf("Waiting for response...\n"); 

    char domainIP[128]; 
    recv(sock, &domainIP, sizeof(domainIP), 0); 
    printf("%s\n", domainIP); 

    close(sock);
    return 0; 
}