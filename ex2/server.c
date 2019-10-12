#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 
#include<sys/socket.h> 
#include<sys/types.h> 
#include<netinet/in.h> 
#include<unistd.h> 

void handle(char *msg){ 
    printf("%s\n",msg);
    exit(0);  
}

int main(){
    int sock,newfd; 
    struct sockaddr_in server,client; 
    char buffer[128]; 

    //Creating a socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0))<0)
        handle("Cannot create socket!"); 

    //Assign values to the address structure 
    bzero(&server,sizeof(server)); 
    server.sin_family = AF_INET; 
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(7228); 

    //Bind socket 
    if (bind(sock,(struct sockaddr*)&server,sizeof(server))<0)
        handle("Cannot bind!"); 
    
    //Accept connection
    int len,n; 
    
    listen(sock,2);
    len = sizeof(client); 
    newfd = accept(sock,(struct sockaddr*)&client,&len); 
    n = read(newfd,buffer,sizeof(buffer));
    printf("%s\n",buffer);
    
    close(sock);
    close(newfd); 

    return 0; 
}