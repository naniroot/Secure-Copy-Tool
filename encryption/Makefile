#CFLAGS variable is used to include the required libraries
CFLAGS=`libgcrypt-config --cflags --libs`
all: ncsuenc.o generatekey.o transferfunction.o fileoperations.o encryptfunction.o hmac.o
	gcc -o ncsuenc ncsuenc.o generatekey.o transferfunction.o fileoperations.o encryptfunction.o hmac.o ${CFLAGS}

ncsuenc.o: ncsuenc.c encrypt.h
	gcc -c ncsuenc.c ${CFLAGS}

generatekey.o: generatekey.c encrypt.h
	gcc -c generatekey.c ${CFLAGS}

transferfunction.o: transferfunction.c encrypt.h
	gcc -c transferfunction.c 

fileoperations.o: fileoperations.c encrypt.h
	gcc -c fileoperations.c 

hmac.o: hmac.c encrypt.h
	gcc -c hmac.c ${CFLAGS}

encryptfunction.o: encryptfunction.c encrypt.h
	gcc -c encryptfunction.c ${CFLAGS}

clean:
	rm -rf *.o ncsuenc *.ncsu
