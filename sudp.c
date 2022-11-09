// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
	
#define PORT	 8080
#define MAXLINE 4096
	
// Driver code
int filereading(char *filename,char* buffer){
	FILE *inputfile = NULL;
	inputfile = fopen(filename, "rb");
	if(inputfile == NULL){
		perror("Opening error");
		exit(-1);
	}
	int res = fread(buffer,sizeof(buffer),1,inputfile);
	if(res <0){
		perror("Reading error");
		exit(-1);
	}
	printf("%s",buffer);
	return 0;
}
int main(int argc, char* argv[]) {
	
	int sockfd,connection;
	char buffer[MAXLINE];
	struct sockaddr_in servaddr, cliaddr, saddr, caddr;
	int tabPORT[1024];
	int len, n;
	int num =8081;
	char segment[1024];

	if(argc != 2){
		fprintf(stderr, "\nSyntax: %s <seed>\n\n", argv[0]);
		return(0);
	}

	for(int i=0; i<1024;i++){
		tabPORT[i] = 0;
	}
		
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}


	if ( (connection = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("connection socket creation failed");
		exit(EXIT_FAILURE);
	}
		
	
	memset(&saddr, 0, sizeof(saddr));
	memset(&caddr, 0, sizeof(caddr));
		
	saddr.sin_family = AF_INET; // IPv4
	saddr.sin_addr.s_addr = INADDR_ANY;
	saddr.sin_port = htons(num);

	
		
	

	if ( bind(connection, (const struct sockaddr *)&saddr,
			sizeof(saddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
		
	
	
	len = sizeof(cliaddr); //len is value/result
	
	n = recvfrom(sockfd, (char *)buffer, MAXLINE,
				MSG_WAITALL, ( struct sockaddr *) &cliaddr,
				(socklen_t *restrict)&len);

	// char str1[10] = "SYN";
	// char str2[10] = "SYN";

	if (!strcmp(buffer,"SYN")){
		char sack[20];
		printf("Client : %s\n", buffer);
		for(int i =0; i<1024; i++){
			if(tabPORT[i] == 0){
				tabPORT[i] = 1;
				break;
			}
			num += 1;
		}
		printf("%d\n",num);
		sprintf(sack,"SYN_ACK %i",num);
		saddr.sin_port = htons(num);

		sendto(sockfd, (const char *)sack, strlen(sack),
		MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
			len);

		char buffer[MAXLINE];
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,
				MSG_WAITALL, ( struct sockaddr *) &cliaddr,
				(socklen_t *restrict)&len);
		buffer[n] = '\0';
		printf("Client : %s\n", buffer);

	}else{
		perror("Cannot establish three way hand shake because SYN is missing");
		exit(EXIT_FAILURE);
	}
	

	//while(1){
		len = sizeof(caddr); 
		printf("Accepting\n");
		n = recvfrom(connection, (char *)buffer, MAXLINE,
				MSG_WAITALL, ( struct sockaddr *) &caddr,
				(socklen_t *restrict)&len);
		//while(n>0){
		printf("the value of reception is: %d\n", n);
		printf("Client : %s\n", buffer);
		
		//}
		filereading(argv[1],segment);
		int sss= sendto(connection, (char *)segment, strlen(segment),
			MSG_CONFIRM, (struct sockaddr *) &caddr,
				sizeof(caddr));
    	printf("the value of sent is: %d\n", sss);
		if(sss<0){
			perror("error sent");
		}
	//}
	close(connection);
	close(sockfd);
	// sleep(2);
	// printf("Hello message sent.\n");
		
	return 0;
}
