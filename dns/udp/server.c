#include<stdio.h> 
#include<string.h> 
#include<sys/types.h> 
#include<sys/socket.h> 
#include<arpa/inet.h> 
#include<stdlib.h> 
#include<unistd.h> 

#define MAXLINE 1024 
#define PORT 8080

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
    if (found==1) 
        IP = table[i][1]; 
    else
        IP = "DNS_Lookup_Failed!";

    return IP; 
}
  
  
// Driver code 
int main() { 
    int sockfd; 
    char buffer[MAXLINE];   
    char hello[128];  
    struct sockaddr_in servaddr, cliaddr; 
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    int len, n; 
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
                &len); 
    buffer[n] = '\0'; 
    printf("Received Domain Name: %s\n", buffer); 

    char domainIP[128]; 
    strcpy(domainIP,dnsLookup(buffer)); 
    printf("%s\n",domainIP);  

    sendto(sockfd, (const char *)domainIP, strlen(domainIP),  
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
            len); 
    printf("Domain IP sent.\n");  
      
    return 0; 
} 