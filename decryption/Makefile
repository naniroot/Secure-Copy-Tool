#CFLAGS variable is used to include the required libraries
CFLAGS=`libgcrypt-config --cflags --libs`
all: ncsudec.o generatekey.o receivefunction.o fileoperations.o decryptfunction.o hmac.o hashverify.o
	gcc -o ncsudec ncsudec.o generatekey.o receivefunction.o fileoperations.o decryptfunction.o hmac.o hashverify.o ${CFLAGS}

ncsudec.o: ncsudec.c decrypt.h
	gcc -c ncsudec.c ${CFLAGS}

generatekey.o: generatekey.c decrypt.h
	gcc -c generatekey.c ${CFLAGS}

receivefunction.o: receivefunction.c decrypt.h
	gcc -c receivefunction.c 

fileoperations.o: fileoperations.c decrypt.h
	gcc -c fileoperations.c 

decryptfunction.o: decryptfunction.c decrypt.h
	gcc -c decryptfunction.c ${CFLAGS}

hmac.o: hmac.c decrypt.h
	gcc -c hmac.c ${CFLAGS}

hashverify.o: hashverify.c decrypt.h
	gcc -c hashverify.c ${CFLAGS}

clean:
	rm -rf *.o ncsudec *.txt
