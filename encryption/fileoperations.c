#include <stdio.h>
#include <stdlib.h>
#include <gcrypt.h>
#include <string.h>

#include "encrypt.h"

/* Function to get the size of the input file and return it*/
size_t getfilesize(char *filename)
{
	FILE *freadp;	
	size_t fsize; /* to hold the size of the file	*/
	
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

/* Function to read the input file and return the plain text*/
char * fileread(char *filename, size_t fsize)
{
	FILE *freadp;
	char *textbuffer;	
	size_t result;

	freadp = fopen(filename,"r");
	
	if(freadp ==NULL)
	{
		printf("File could not be opened or file does not exist\n");
	}

	textbuffer = malloc(fsize);

	result = fread(textbuffer, 1, fsize , freadp);

	if(result<0)
	{
		printf("Error while reading file\n");
	}

	fclose(freadp);
	printf("%s read successfully\n",filename);
	return textbuffer;
	
}

/* Function to write the encrypted text to a file with .ncsu appended to it and return the filename*/
char * filewrite(char *filename, char *ciphertext, char *hashtext, size_t fsize, char *salt, char *iv)
{
	FILE *fwritep;
	char *outfile;
	int result;
	int i;
	char *testbuffer=malloc(1);	

	/* Logic to add .nscu to the end of the filename */
	outfile=(char *)malloc(strlen(filename)+strlen(".ncsu")+1);
	strcpy(outfile,filename);
	strcat(outfile,".ncsu");

	fwritep = fopen(outfile,"w");
	
	if(fwritep == NULL)
	{
		printf("File could not be opened or file does not exist\n");
	}
	
	result=fwrite(salt, 1,4, fwritep);

	result=fwrite(iv, 1,16, fwritep);
	
	result=fwrite(hashtext, 1,64, fwritep); /* Write the HMAC first */
	
	result=fwrite(ciphertext, 1,fsize+1, fwritep); /* Write the encrypted text to the file */
		
	printf("Salt, IV, HMAC and encrypted data successfully written to %s\n", outfile);

	fclose(fwritep);

	fwritep = fopen(outfile,"r");
	
	return outfile;

}
