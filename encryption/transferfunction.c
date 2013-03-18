#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "encrypt.h"

/*Function to establish connection with server and transfer data */
int transferfunction(char *host, char *port, char *filename)
{
	int sockfdc, numbytesc;  
	char *buf;
	struct addrinfo hintsc, *servinfoc, *pc;
	int rvc, result;
	struct sockaddr_in sa;
	char s[INET6_ADDRSTRLEN];
	FILE *fp;
	size_t filesize;
	int count;

	memset(&hintsc, 0, sizeof hintsc);
	hintsc.ai_family = AF_UNSPEC;
	hintsc.ai_socktype = SOCK_STREAM;

	fp=fopen(filename, "r");
	filesize = getfilesize(filename);
	buf = malloc(100 * sizeof(char));
	
	printf("Beginning Transfer\n");

	/* Get linked list of connections from kernel */
	if ((rvc = getaddrinfo(host, port, &hintsc, &servinfoc)) != 0)
	{
		printf("Getaddrinfo %s\n", gai_strerror(rvc));
		return 1;
	}

	/* loop through all the results and connect to the first we can */
	for(pc = servinfoc; pc != NULL; pc = pc->ai_next) {
		if ((sockfdc = socket(pc->ai_family, pc->ai_socktype,
				pc->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfdc, pc->ai_addr, pc->ai_addrlen) == -1) {
			close(sockfdc);
			perror("client: connect");
			continue;
		}

		break;
	}

	if (pc == NULL) {
		printf("Client failed to connect\n");
		return 2;
	}

	inet_ntop(pc->ai_family,&(sa.sin_addr),s, sizeof s);  
	printf("client connecting to %s\n", s);

	freeaddrinfo(servinfoc); 

	/* Keep sending data as until the end of file */
	while( !feof( fp ) )
   	{
     		 /* Attempt to read in 100 bytes: */
     		count = fread( buf, sizeof( char ), 100, fp );
      		if( ferror( fp ) )     
		{
         		perror( "Read error" );
        		 break;
      		}
		/* Send 100 bytes of data to server */
		if (send(sockfdc, buf, count, 0) == -1)
		{
			perror("send");
		}

	}
	printf("Client sent data to server successfully\n");

	close(sockfdc);

	return 0;
}

