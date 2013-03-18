/*
** decrypt.h - header file that declares the functions in all the files
*/

/* Function that decrypts the ciphertext given the key */
char * decryptfunction(char *textbuffer, char * aeskey, size_t textlen, char *iv);

/*Function that returns the size of the file */
size_t getfilesize(char *filename);

/*Funtion that reads the function and returns its content*/
char * fileread(char *filename, size_t fsize);

/*Funtion that writes the content of the file and returns the filename*/
void filewrite(char *filename, char *plaintext, size_t fsize);

/* Functions that uses PBKDF2 to generate a 32byte key*/
char * generatekey(char *password, char *salt);

/*Function that waits or receives data from the port specified */
void receivefunction(char *port);

/* function used to calculate HMAC*/
char * hmac(char * ciphertext, char * key, size_t filesize);

/* Function used to extract the HMAC part of the encrypted data */
char * extracthash(char *filename);

/* Function to verify the HMAC of the received textfile */
int hmacverify(char *hash, char * ciphertext,char *password, size_t filesize);

/*Function to extract salt */
char * extractsalt(char *filename);

/* Function to extract iv */
char * extractiv(char *filename);




