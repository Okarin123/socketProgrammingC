#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 
#include<sys/socket.h> 
#include<sys/types.h> 
#include<netinet/in.h> 
#include<unistd.h>
#include<arpa/inet.h> 

void handle(char *msg){ 
    printf("%s",msg); 
    exit(0); 
}

//server IP 
int main(int argc, char** argv){  
    int sock,n; 
    struct sockaddr_in server; 
    char buffer[1024];

    if ((sock = socket(AF_INET,SOCK_STREAM,0))<0)
        handle("Cannot create socket!");  
    
    bzero(&server,sizeof(server)); 
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(1024); 

    connect(sock,(struct sockaddr*)&server,sizeof(server)); 
    
    n = read(sock,buffer,sizeof(buffer));  
    printf("Received %s\n",buffer);  

    close(sock);  
 
    return 0; 
}