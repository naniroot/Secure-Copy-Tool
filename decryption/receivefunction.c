#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "decrypt.h"


/* Function to establish server at specified port and collect the incoming text and write to file */
void receivefunction(char *port)
{
	int sockfds, new_fd,numbyteserv;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hintss, *servinfos, *ps;
	struct sockaddr_storage their_addrs; // connector's address information
	socklen_t sin_size;
	int yes=1;
	char *bufs, *buftemp;
	char ss[INET_ADDRSTRLEN];
	int rv;
	int numbytes;
	FILE *fp;


	bufs= malloc(sizeof(char)*100);
	
	memset(&hintss, 0, sizeof hintss);
	hintss.ai_family = AF_UNSPEC;
	hintss.ai_socktype = SOCK_STREAM;
	hintss.ai_flags = AI_PASSIVE; // use my IP

	printf("Establishing server\n");
	
	/* Get the linked list of connections from kernel */
	if ((rv = getaddrinfo(NULL, port, &hintss, &servinfos)) != 0) 
	{
		printf("Getaddrinfo error: %s\n", gai_strerror(rv));
		exit(1);
	}

	/* Loop through all the results and bind to the first we can */
	for(ps = servinfos; ps != NULL; ps = ps->ai_next) 
	{
		if ((sockfds = socket(ps->ai_family, ps->ai_socktype,ps->ai_protocol)) == -1) 
		{
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfds, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1)
		{
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfds, ps->ai_addr, ps->ai_addrlen) == -1)
		{
			close(sockfds);
			perror("server: bind");
			continue;
		}

		break;
	}

	if (ps == NULL)
	{
		fprintf(stderr, "server: failed to bind\n");
		exit(2);
	}

	freeaddrinfo(servinfos); // all done with this structure


	fp=fopen("plaintext.txt.ncsu","w+");
	fclose(fp);
	fp=fopen("plaintext.txt.ncsu","a");

	if (listen(sockfds, 1) == -1)
	{
		perror("listen");
		exit(1);
	}

	printf("\n Server: waiting for connections\n");
	

	/* outer recv loop */
	while(1) 
	{  
		sin_size = sizeof their_addrs;
		new_fd = accept(sockfds, (struct sockaddr *)&their_addrs, &sin_size);
		if (new_fd == -1)
		{
			perror("accept");
			continue;
		}

		inet_ntop(their_addrs.ss_family,(struct sockaddr *)&their_addrs,ss, sizeof ss);
		printf("Server got connection from %s\n", ss);
		
		if ((numbytes = recv(new_fd, bufs, 100, 0)) == -1)
		{
			printf("recv");
			exit(1);
		}
		fwrite(bufs,1,numbytes,fp);
		

		while(numbytes>0)
		{
			if ((numbytes = recv(new_fd, bufs, 100, 0)) == -1)
			{
				printf("recv");
				exit(1);
			}
			fwrite(bufs,1,numbytes,fp);
		}
		
		fclose(fp);
		close(new_fd); 
		break;
	}

}

