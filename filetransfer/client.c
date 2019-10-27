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
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(7228); 

    connect(sock,(struct sockaddr*)&server,sizeof(server)); 
    n = read(sock,buffer,sizeof(buffer));   
    
    //Write buffer contents to file 
    char fPath[15];
    int fPos=0; 
    FILE *fptr; 

    printf("\nEnter the file path: "); 
    scanf("%s",fPath); 
    fptr = fopen(fPath,"w"); 

    while (buffer[fPos]!=EOF) 
    {
        putc(buffer[fPos],fptr); 
        fPos++;  
    }
    buffer[fPos] = EOF; 

    printf("File Transfered!");  

    close(sock);
    fclose(fptr); 
    return 0; 
}