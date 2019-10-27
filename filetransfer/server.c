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
    char buffer[1024]; 

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


    
    //putting file contents to buffer 
    FILE *fptr;
    char fileName[15]; 
    char ch; 

    printf("\nEnter file name: "); 
    scanf("%s",fileName);  
    if ((fptr=fopen(fileName,"r"))==NULL){ 
        handle("Cannot open file!");     
    } 
    printf("Test\n");
    int fpos = 0;  
    ch = fgetc(fptr);
    while (ch!=EOF) 
    {
        buffer[fpos] = ch; 
        fpos++; 
        ch = fgetc(fptr); 
    }
    buffer[fpos] = EOF; 
    printf("In buffer!");
    n = write(newfd,buffer,sizeof(buffer)); 
    close(sock);
    close(newfd);
    fclose(fptr);  

    return 0; 
}