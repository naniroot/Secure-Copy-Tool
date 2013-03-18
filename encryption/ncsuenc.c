#include <stdio.h>
#include <stdlib.h>
#include <gcrypt.h>
#include <string.h>

#include "encrypt.h"

#define KDFkeylength 32
#define HSIZE 64

/* Main function to invoke all the functions */
int main(int argc, char *argv[])
{

	char *filename;
	char *encryptdata;
	char *plaintext;
	char *ciphertext;
	char password[100];
	char *key;
	char *hostname, *port, ip[200];
	char *hashtext= malloc(HSIZE);
	char *salt;
	char *iv;
	size_t filesize;
	int i;

	if(argc<2)
	{
		printf("Usage : ncsuenc <input file> [<Output IP-addr: Port>]");
		exit(0);
	}

	printf("Please enter required password \n");
	scanf("%s",password);

	filename = argv[1]; /* First parameter passed is the filename */

	key=malloc(KDFkeylength*sizeof(char));

	/*Generate Salt*/
	salt = generatesalt();
	
	/* Generate the key */
	key = generatekey(password, salt);

	/* Get the filesize of plaintext*/
	filesize = getfilesize(filename);

	/* Read the file*/
	plaintext = fileread(filename, filesize);

	/* Generate IV */
	iv = generateiv();

	/* Encrypt the plaintext using the key*/
	ciphertext = encrypt(plaintext, key, iv, filesize);

	/* Generate HMAC with ciphertext and key*/
	hashtext=hmac(ciphertext, key, filesize);

	/* Write the file with .ncsu suffix*/	
	filename= filewrite(filename, ciphertext,hashtext, filesize, salt,iv); 

	/* If network address provided transfer data to the network address specified*/
	if(argc==3)
	{
		
		/* Logic to extract hostname and port from the 3rd parameter passed */		
		strcpy(ip,argv[2]);
		hostname =strtok(ip,":");
		port = strtok(NULL,":");
		printf("Hostname is %s, Port is %s\n",hostname,port);
		
		/* Transfer the file to the host specified */
		transferfunction(hostname, port, filename);
		
	}	
	return 0;
}

