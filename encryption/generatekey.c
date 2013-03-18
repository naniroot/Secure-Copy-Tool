#include <stdio.h>
#include <stdlib.h>
#include <gcrypt.h>
#include <string.h>

#include "encrypt.h"

#define KDFALGO GCRY_KDF_PBKDF2 /* Algorithm used to derive the key */
#define SUBALGO GCRY_MD_MD5 /* Hash function used to create the hash for the key */
#define SALTSIZE 4 /* Salt used to prevent rainbow table attacks on hashed keys */
#define KDFkeylength 32 /*length of the key required for encryption*/


/* This function is used to generate the key based on the password given. It takes the password and gives the key*/
char * generatekey(char *password, char * salt)
{
	gpg_error_t err;
	size_t passlen = strlen(password);
	char *keybuffer=malloc(KDFkeylength);
	int i;
	
	err = gcry_kdf_derive(password, passlen , KDFALGO, SUBALGO, salt,SALTSIZE, 10, KDFkeylength,keybuffer);

	if(err)
	{
		printf("Error while generating the key\n");
		exit(-1);
	}

	printf("Key generated successfully\n"); 
	
	return keybuffer;

}

/*This function returns randomly generated 4bytes of data that can be used as salt value for encryption */
char * generatesalt()
{
	char *salt = malloc(SALTSIZE);
	int i;
	salt = gcry_random_bytes (SALTSIZE, GCRY_STRONG_RANDOM);
	printf("Salt value generated successfully\n");
	
	return salt;
}


