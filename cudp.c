// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
	
#define PORT	 8080
#define MAXLINE 1024
	
// Driver code
int n, len;
int sockfd, connection;
char buffer[MAXLINE];
struct sockaddr_in	 servaddr, saddr;
int twhs()
{

	char *hello = "SYN";		
	sendto(sockfd, (const char *)hello, strlen(hello),
		MSG_CONFIRM, (const struct sockaddr *) &servaddr,
			sizeof(servaddr));

				
	n = recvfrom(sockfd, (char *)buffer, MAXLINE,
			MSG_WAITALL, (struct sockaddr *) &servaddr,
				(socklen_t *restrict)&len);
	buffer[n] = '\0';
	char* sack = buffer;
	char * token = strtok(sack, " ");
	char * port_str = strtok(NULL, " ");
	int port_num = atoi(port_str);

	//printf("%d\n",port_num);
		
	if (!strcmp(token,"SYN_ACK"))
	{
			
			
		printf("Server : %s\n", buffer);
		char* GEN_ACK ="ACK";
		sendto(sockfd, (const char *)GEN_ACK, 3,
		MSG_CONFIRM, (const struct sockaddr *) &servaddr,
			sizeof(servaddr));
	}
	return port_num;
}
int main() {

	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	if ( (connection = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("connection socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&saddr, 0, sizeof(saddr));
		
	// Filling server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr =  INADDR_ANY;
;
		
	
	int port=twhs();
	printf("%d\n",port);

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = INADDR_ANY;


	//int cont= 1;
	char *data = "DATA";
	printf("%s\n",data);
  	//while (cont) {
    	//fgets(buffer, MAXLINE, stdin);
    	int sss= sendto(connection, (char *)data, 4,
			MSG_CONFIRM, (struct sockaddr *) &saddr,
				sizeof(saddr));
    	printf("the value of sent is: %d\n", sss);
		if(sss<0){
			perror("error sent");
		}
		// recvfrom(sockfd, (char *)buffer, MAXLINE,
		// 	MSG_WAITALL, (struct sockaddr *) &saddr,
		// 		&len);
		// printf("Server: %s\n", buffer);
    	/*recvfrom(sockfd, (char *)buffer, MAXLINE,
			MSG_WAITALL, (struct sockaddr *) &servaddr,
				&len);
    	printf("%s",buffer);
    	memset(buffer,0,MAXLINE);
    	if (strcmp(buffer,"stop\n") == 0) {
      	cont= 0; 
    	}*/
		memset(buffer,0,MAXLINE);
		recvfrom(connection, (char *)buffer, MAXLINE,
			MSG_WAITALL, (struct sockaddr *) &saddr,
				(socklen_t *restrict)&len);
		printf("%s\n",buffer);

  //}
	close(sockfd);
	return 0;
}
