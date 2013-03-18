#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decrypt.h"

#define KDFkeylength 32 /* Length of the key used */
#define HSIZE 64 /* HMAC Size */

int main(int argc, char * argv[])
{
	char *filename;
	char *plaintext;
	char *ciphertext;
	size_t filesize, recvtextsize;
	char password[200];
	char *key;
	char *recvtext;
	char *port;
	char *hash= malloc(HSIZE);
	char *salt;
	char *iv;
	int hcheck;

	if(argc!=3)
	{
		printf("\n Usage: ncsudec [-d <port>] [-i <input file> ]");
		exit(1);
	}
	
	filename = malloc(200);

	if(strcmp(argv[1],"-i")==0)
	{
		/* When local file is given */
		printf("\nPlease enter Password\n");
		scanf("%s",password);
		filename = argv[2];
	}	

	else if (strcmp(argv[1],"-d")==0)
	{
		/* When port number is specified to get the file */
		port = argv[2];
		receivefunction(port);

		printf("\nPlease enter Password\n");
		scanf("%s",password);
		filename = "plaintext.txt.ncsu";	
	}

	else
	{
		printf("Invalid Option\n");
		exit(1);
	}
		
	key=malloc(KDFkeylength*sizeof(char));

	/*extract salt */
	salt= extractsalt(filename);
	
	/*Generate the key using the password */
	key = generatekey(password, salt);	
	
	/*Extract iv*/
	iv = extractiv(filename);
	
	/* Get file size */
	filesize = getfilesize(filename);

	filesize-=(64+16+4); /* reduce 64 bytes of HMAC, 16 bytes of IV and 4 bytes of Salt */
	
	/* Obtain ciphertext */
	ciphertext = fileread(filename, filesize);

	/* Extract the HMAC part of the file */
	hash = extracthash(filename);
	
	/* Verify the hash*/
	hcheck = hmacverify(hash, ciphertext,key,filesize);

	if (hcheck==0)
	{
		printf("HMAC verification passed\n");
	}
	else
	{
		printf("HMAC verification failed\n Wrong password or file corrupted\n");
		exit(0);	
	}	
	
	printf("Decrypting \n");
	/* HMAC verified and beginning decryption */
	plaintext = decryptfunction(ciphertext,key, filesize, iv);

	/* Writing decrypted text to the file */
	filewrite(filename, plaintext, filesize);
		
	return 0;
}

