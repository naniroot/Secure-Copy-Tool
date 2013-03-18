#include <stdio.h>
#include <stdlib.h>
#include <gcrypt.h>
#include <string.h>

#include "decrypt.h"


/* Function to get the size of the file */
size_t getfilesize(char *filename)
{
	FILE *freadp;	
	size_t fsize; /* To hold the size of the file	*/
	
	freadp = fopen(filename,"r");
	
	if(freadp ==NULL)
	{
		printf("File could not be opened or file does not exist\n");
	}
	
	/*obtain file size */
	fseek(freadp,0, SEEK_END);
	fsize=ftell(freadp);
	rewind(freadp);
	fclose(freadp);

	printf("The file size is %d\n", fsize);
	return fsize;

}

/* Function to read the contents of the encrypted file and return its content */
char * fileread(char *filename, size_t fsize)
{
	FILE *freadp;
	char *textbuffer;	
	size_t result;
	int i;

	freadp = fopen(filename,"r");
	
	if(freadp ==NULL)
	{
		printf("File could not be opened or file does not exist\n");
	}

	fseek(freadp,64+16+4, SEEK_SET); /*skip the HMAC, salt, iv attached at the beginning */

	textbuffer = malloc(fsize);
		
	result = fread(textbuffer, 1, fsize , freadp); /*reading the ciphertext */

	if(result<0)
	{
		printf("Error while reading file\n");
	}
	
	printf("%s read successfully\n",filename);
	printf("The extracted cipher text\n");
	
	return textbuffer;
	
}

/* Function to write the decrypted text to the file after removing ".ncsu" suffix to the filename */
void filewrite(char *filename, char *plaintext, size_t fsize)
{
	FILE *fwritep;
	char *outfile;
	int result;
	int i,k;
	
	/*Logic to remove ".ncsu" from the end of the filename */
	k=strlen(filename);
	outfile= (char *)malloc((k-4)*sizeof(char));
	for(i=0;i<k-5;i++)
	{
		outfile[i]=filename[i];
	}
		outfile[i]='\0';

	fwritep = fopen(outfile,"w");
	
	if(fwritep == NULL)
	{
		printf("File could not be opened or file does not exist\n");
	}

	printf("The output file is %s\n", outfile);
	result=fwrite(plaintext, 1,fsize-1, fwritep);
	
	printf("The Plaintext text written to file successfully\n");	

	fclose(fwritep);

}
