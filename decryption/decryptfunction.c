#include <stdio.h>
#include <stdlib.h>
#include <gcrypt.h>
#include <string.h>

#include "decrypt.h"

#define CIPHER GCRY_CIPHER_AES256   /*Required Algorithm */

#define MODE GCRY_CIPHER_MODE_CBC  /* using most secure CBC mode*/
#define IVSIZE 16 /* Size of the IV used */


/* Function to decrypt the ciphertext passed to it, it also takes the lenght of the file as input. function returns palintext*/
char * decryptfunction(char *textbuffer, char * aeskey, size_t textlen, char *iv)
{

	gcry_error_t err;
	gcry_cipher_hd_t handle;
	size_t keylength = gcry_cipher_get_algo_keylen(CIPHER); /* For aes256 keylength is 32 */
	size_t blklength = gcry_cipher_get_algo_blklen(CIPHER); /*For aes256 blklength is 16 */
	size_t i,result;	

	char *decbuffer = malloc(textlen); /*buffer to hold decrypted data */

	printf("Starting Decryption Process\n");
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
		printf("Error setting cipher key\n");
	}

	printf("Cipher key created successfully\n");

	err = gcry_cipher_setiv(handle, iv , blklength);

	if(err)
	{
		printf("Error setting initialization vector\n");
	}	

	printf("Initialization vector set successfully\n");

	err = gcry_cipher_decrypt( handle, decbuffer, textlen, textbuffer, textlen);

	if(err)
	{
		printf("Error while decrypting\n");
	}

	printf("Text successfully decrypted\n");

	gcry_cipher_close(handle);
	printf("Cipher handle closed successfully\n");			
	
	return decbuffer;
}

/* Function to extract IV from the file */
char * extractiv(char *filename)
{
	FILE *fp;
	char *iv = malloc(IVSIZE);

	fp= fopen(filename, "r");
	
	int i;

	i= fseek(fp,4,SEEK_SET); /* Seek the salt attached in the beginning */
	i = fread(iv,1,IVSIZE,fp);

	printf("IV extracted successfully\n");
	
	fclose(fp);
	return iv;

}

