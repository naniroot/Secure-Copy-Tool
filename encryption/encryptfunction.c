#include <stdio.h>
#include <stdlib.h>
#include <gcrypt.h>
#include <string.h>

#include "encrypt.h"

#define CIPHER GCRY_CIPHER_AES256   /*Required Algorithm */

#define MODE GCRY_CIPHER_MODE_CBC /* Using most secure CBC mode */
#define IVSIZE 16

/* This function takes in the plaintext that was read from a file and uses the algorithm and mode specifed 
to encrypt it and return the cipher text */

char * encrypt(char *textbuffer, char * aeskey, char *iv, int filesize)
{

	gcry_error_t err; 
	gcry_cipher_hd_t handle;
	size_t keylength = gcry_cipher_get_algo_keylen(CIPHER); /* For aes256 keylength is 32 */
	size_t blklength = gcry_cipher_get_algo_blklen(CIPHER); /*For aes256 blklength is 16 */
	size_t textlen = filesize; //strlen(textbuffer)+1; /* +1 for '\0' character*/
	size_t i,result;

		
	char *encbuffer = malloc(textlen); /* Buffer to hold encrypted data */
	

	printf("Starting Encryption Process\n");
	err = gcry_cipher_open( &handle, CIPHER, MODE, GCRY_CIPHER_CBC_CTS);
	if(err)
	{
		printf("Error creating cipher handle\n");
		exit(0);
	}
	printf("Cipher handle created successfully\n");


	err= gcry_cipher_setkey(handle, aeskey, keylength);
	if(err)
	{
		printf("error setting cipher key\n");
	}

	printf("Cipher key set successfully\n");

	err = gcry_cipher_setiv(handle, iv , blklength);

	if(err)
	{
		printf("error setting initialization vector\n");
	}	

	printf("Initialization vector set successfully\n");

	err = gcry_cipher_encrypt( handle, encbuffer, textlen, textbuffer, textlen);

	if(err)
	{
		printf("Error while encrypting\n");
	}

	printf("Text successfully encrypted\n");

	gcry_cipher_close(handle);

	printf("Cipher handle closed\n");

	printf("Encryption process completed.\n");

	return encbuffer;
}

/* Function to generate 16 bytes of random IV value */
char * generateiv()
{
	char *iv = malloc(IVSIZE);
	int i;
	iv = gcry_random_bytes (IVSIZE, GCRY_STRONG_RANDOM);
	printf("IV generated successfully\n");
	return iv;
}
