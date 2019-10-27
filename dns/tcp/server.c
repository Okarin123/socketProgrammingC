#include<stdio.h> 
#include<stdlib.h> 
#include<sys/types.h> 
#include<sys/socket.h> 
#include<arpa/inet.h> 
#include<string.h> 
#include<unistd.h> 

int tableLen = 3; 
char *table[3][2] = { 
        {"www.yahoo.com","10.2.45.67"}, 
        {"www.google.com","142.89.78.66"}, 
        {"www.annauniv.edu","197.32.53.122"}
    };

void handle (char *msg){ 
    printf("%s\n",msg); 
    exit(0); 
}

void dispDNS(){ 
    int i; 
    for (i=0; i<tableLen; i++){ 
        printf("%s\t%s\n",table[i][0],table[i][1]); 
    }
}

char* dnsLookup(char *name){ 
    
    int i, found = 0; 
    for (i=0; i<tableLen; i++){ 
        if (strcmp(table[i][0],name) == 0){ 
            found = 1; 
            break;
        }
    }    

    char *IP; 
    if (found==1){ 
        IP = table[i][1]; 
    }
    else{ 
        IP = "DNS_Lookup_Failed!";
    }
    return IP; 

}

int main(){

    dispDNS();
    printf("Waiting for client to connect!\n");

    int sock; 
    struct sockaddr_in serverAddr; 

    //creating socket 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0))<0){ 
        handle("Cannot create socket!"); 
    }

    //initializing address structure 
    bzero(&serverAddr, sizeof(serverAddr)); 
    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_port = htons(1024); 
    serverAddr.sin_addr.s_addr = INADDR_ANY; 

    //binding and listening 
    if (bind (sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr))<0){ 
        handle("Bind error!"); 
    }
    listen (sock,2); 

    //accept connection
    int clientSock, clientAddrLen;  
    struct sockaddr_in clientAddr; 
    clientAddrLen = sizeof(clientAddr);
    clientSock = accept(sock, (struct sockaddr *) &clientAddr, (socklen_t *) &clientAddrLen); 
    char *clientIP = inet_ntoa(clientAddr.sin_addr); 
    printf("Connected to client %s\n",clientIP); 

    //read the domain name 
    char domainName[128]; 
    char domainIP[128];  
    recv(clientSock, &domainName, sizeof(domainName), 0);
    strcpy(domainIP,dnsLookup(domainName)); 
    send(clientSock, &domainIP, sizeof(domainIP), 0); 
    printf("Sent response!\n"); 

    close(sock); 
    close(clientSock); 
    return 0; 
}
