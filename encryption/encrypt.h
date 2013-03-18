/*
** encrypt.h - header file that declares the functions in all the files
*/

/* Function that encrypts the plaintext given the key */
char * encrypt(char *textbuffer, char * aeskey, char * iv, int filesize);

/*Function that returns the size of the file */
size_t getfilesize(char *filename);

/*Funtion that reads the function and returns its content*/
char * fileread(char *filename, size_t fsize);

/*Funtion that writes the content of the file and returns the filename*/
char * filewrite(char *filename, char *ciphertext, char *hashtext, size_t fsize, char * salt, char *iv);

/* Functions that uses PBKDF2 to generate a 32byte key*/
char * generatekey(char *password, char * salt);

/*Function that sends the data to the ipaddress:port specified */
int transferfunction(char *host, char *port, char *text);

/* Function used to calculate HMAC*/
char * hmac(char * ciphertext, char * key, size_t filesize);

/* Function used to generate random salt value */
char * generatesalt();

/* Function to generate random IV value */
char * generateiv();
