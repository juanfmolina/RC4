#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char u_char;


// Function prototypes
void initialize(u_char *State);
void swap(u_char *a, u_char *b);
void ksa(u_char *State, u_char *key, int keylen) ;
u_char * prng(u_char *State, int msglength);
void rc4(u_char *key, u_char *input, u_char *output, int keylen, int msglen);

int main(int argc, char *argv[]) {
	
	u_char input[4096];		// The input data
	u_char output[4096];	// The output
	int msglength = strlen(argv[1]);
	strncpy((char *) input, argv[1], 4096);
	u_char key[]={0x01,0x02,0x03,0x04,0x05,0x06,0x07, 0x08};		// The encryption/decryption key 
	// se puede cambiar la Key a cualquiera que esté en los vectores de prueba de 
	int keylength = sizeof(key)/sizeof(key[0]);
	
	// encrypt

	rc4(key, input, output, keylength, msglength);
	

	int i = 0;
	printf("\n\n--- Encryption ---");
	printf("\nOutput:\n");
	for(i=0; i<msglength; i++) {
		if(i%16==0){
			printf("\n");
			printf("%d:\t", i);
		}
		if(output[i]<16)
			printf("0%0x ", output[i]);
		else
			printf("%0x ", output[i]);
	}
	printf("\n");

	
	// decrypt
	rc4(key, output, input, keylength,msglength); 


	printf("\n\n--- Decryption ---");
	printf("\nOutput:\n");
	printf("%s\n", input);

	exit(0);
}

/*
 * RC4 functions
 */
 
/* Initializa el array state de 0 a 255*/
void initialize(u_char *State) {
	int i;
	for(i=0; i<256; i++) {
		State[i] = i;
	}
	return;
}

/* Intercambia los valores de i y j */
void swap(u_char *i, u_char *j) {
	u_char temp;
	
	temp = *i;
	*i = *j;
	*j = temp;
}

/* Key scheduling algorithm. */
void ksa(u_char *State, u_char *key, int keylen) {
	int i, j=0;
	
	
	printf("\n\n Keylenght %d \n\n", keylen);
	
	
	for(i=0; i<256; i++) {
		j = (j + State[i] + key[i%keylen]) % 256;
		swap(&State[i], &State[j]);
	}
	return;
}

/* Pseudo-random number generator: Generate the keystream. Este algoritmo genera el keyStream*/
u_char * prng(u_char *State, int msglength) {
	int i=0, j=0, k;
	
	u_char *keystream;
	
	keystream = (u_char *)malloc(sizeof(u_char)*msglength);
	
	for(k=0; k<msglength; k++) {
		i = (i+1) % 256;
		j = (j+State[i]) % 256;
		
		swap(&State[i], &State[j]);
		
		keystream[k] = State[(State[i]+State[j]) % 256];
	}
	return keystream;
}

/* Encrypt or Decrypt */
void rc4(u_char *key, u_char *input, u_char *output, int keylen, int msglen) {
	int i;
	u_char State[256];
	u_char *keystream;
	
	initialize(State);
	
	ksa(State, key, keylen);
	printf("\n--- Tamaño del mensaje %d\n", msglen);
	keystream = prng(State, msglen);
	
	for(i=0; i<msglen; i++)
		output[i] = input[i] ^ keystream[i];
	
	printf("\n--- Key Generation ---\nKey: %s\n\n", key);
	
	printf("Keystream:\n");
	for(i=0; i<msglen; i++) {
		if(i%16==0){
			printf("\n");
			printf("%d:\t", i);
		}
		if(keystream[i]<16)
			printf("0%x ", keystream[i]);
		else
			printf("%x ", keystream[i]);
	}
}
