#include <stdio.h>
#include <stdlib.h>
#include <gcrypt.h>
#include <string.h>

#include "decrypt.h"

#define HALGO GCRY_MD_SHA512 /* Algorithm used to create HMAC */
#define HSIZE 64 /* Size of the HMAC that will be generated using the above algorithm */

/* Function which takes the ciphertext and key and returns the HMAC for it */
char * hmac(char * ciphertext, char * key, size_t filesize)
{
	
	gcry_md_hd_t handle;
	gcry_error_t err;

	char *hash = malloc(HSIZE);	
	char *hashtemp = malloc(HSIZE);
	int i;

	ciphertext[filesize]='\0'; /* FileSize reducing by one for uniformity with encryption */
	filesize=filesize-1;
	
	err= gcry_md_open(&handle, HALGO, GCRY_MD_FLAG_HMAC);
	if(err)
	{
		printf("Error while opening HMAC handle\n");
	}
	printf("HMAC handle created successfully\n");

	err=gcry_md_enable(handle, HALGO);
	if(err)
	{
		printf("Error while enabling hash algorithm\n");
	}
	printf("HMAC algorithms enabled successfully\n");

	err= gcry_md_setkey (handle, key, 32);
	if(err)
	{
		printf("Error while setting the key\n");
	}	

	gcry_md_write(handle,ciphertext, sizeof(ciphertext));
	
	hash = gcry_md_read(handle,HALGO);
	if(hash==NULL)
	{
		printf("Error reading hash function\n");
	}
	printf("HMAC created successfully\n");

	memcpy(hashtemp,hash,HSIZE);
	gcry_md_close(handle);
	return hashtemp;
	
}

