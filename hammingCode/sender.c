#include<stdio.h> 
#include<string.h> 
#include<math.h> 
#include<sys/socket.h> 
#include<sys/types.h> 
#include<unistd.h> 
#include<stdlib.h> 
#include<netinet/in.h> 

int table[100][100]; 

void populateTable() {
    
    int i,j;
    int r=100, c=100; 
    for (i=0; i<100; i++)
        for (j=0; j<100; j++)
            table[i][j] = 0; 
    
    int temp; 
    for (i=1; i<100; i++){ 

        temp = i; 
        j=0; 
        while (temp>0){ 
            table[i][j] = temp%2; 
            temp/=2;
            j++; 
        }
    }
}

int extendedLen; 

int addIndividual(char ext[], int pos){ 

	int i, j; 
	int count=0; 

	for (i=0; i<extendedLen; i++){ 
		
		if ((ceil(log2(i+1))!=floor(log2(i+1))) && (table[i+1][log2(pos+1)])) {  
		    if (ext[i] == '1')
                count++;    
		}   
        	 
	} 

	return count;   			 
}

char extended[100];  
    
void addParity(char msg[]){ 
    int n = strlen(msg); 
    int r=0; 

    while (pow(2,r)<=r+n+1){ 
        r++; 
    }

    int i, j=0;  
    for (i=0; i<r+n; i++){
        if (ceil(log2(i+1)) == floor(log2(i+1))){ 
            extended[i] = 'x';
        } 
        else
        {
            extended[i] = msg[j]; 
            j++;    
        }
    }
	extended[n+r] = '\0'; 
	extendedLen = n+r;
    int count; 

    
	for (i=0; i<n+r; i++){ 
		if (ceil(log2(i+1)) == floor(log2(i+1))){  
			count = addIndividual(extended, i); 
            extended[i] = count%2 + '0';  
		}     
	}
}

void handle (char msg[]){ 
    printf("%s\n",msg); 
    exit(0); 
}

void main(){ 
    populateTable(); 

    char input[100]; 
    char *buffer;
	
    printf("Enter the message: ");
	scanf("%s", input); 
	addParity(input);
    printf("Message after parity generation: %s\n", extended); 
    
    int pos; 
    printf("Enter position to introduce error: "); 
    scanf("%d", &pos); 

    if (pos>strlen(extended)){ 
        handle("Incorrect input.");
    }

    if (extended[pos-1] == '1'){ 
        extended[pos-1] = '0';
    }
    else
    {
        extended[pos-1] = '1'; 
    }
    
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0))<0) { 
        handle("Cannot create socket!"); 
    }

    struct sockaddr_in server, client; 
    bzero(&server, sizeof(server)); 
    server.sin_family = AF_INET; 
    server.sin_port = htons(1024); 
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *) &server, sizeof(server))<0){
        handle("Cannot bind!"); 
    }

    printf("Waiting for connection...\n");
    listen(sock, 2); 
    int clientLen = sizeof(client); 
    int newfd = accept(sock, (struct sockaddr *) &client, &clientLen); 

    int bytes = write(newfd, extended, sizeof(extended)); 
    printf("%d bytes sent.\n", bytes);
}
