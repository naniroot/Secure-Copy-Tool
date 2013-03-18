#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decrypt.h"

#define HSIZE 64

/* Function to read the hash in the first 64 bytes of the encrypted file and return it*/
char * extracthash(char *filename)
{
	FILE *freadp;
	char *hash;	
	size_t result;

	freadp = fopen(filename,"r");
	result= fseek(freadp,4+16,SEEK_SET); /*seek the iv and salt attached in the beginning */

	if(freadp ==NULL)
	{
		printf("File could not be opened or file does not exist\n");
	}

	hash = malloc(HSIZE);
		
	result = fread(hash, 1, HSIZE , freadp); /*reading the attached hash */

	if(result<0)
	{
		printf("Error while reading file\n");
	}
	printf("HMAC extracted successfully\n");
	
	fclose(freadp);

	return hash;
	
}

/* Function to generate HMAC for the data and compare it with the hash attached and return match or not */
int hmacverify(char *hash, char * ciphertext,char *key,size_t filesize)
{
	char *hashtext = malloc(HSIZE);
	hashtext=hmac(ciphertext, key, filesize);
	return memcmp(hash,hashtext,HSIZE); /* comparing attached and generated hash */


}
