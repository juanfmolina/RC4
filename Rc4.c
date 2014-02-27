/*
 * rc4.c - written for readability
 *
 * http://blog.markloiseau.com/2012/07/rc4-tutorial/
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char u_char;

// Output level, from 0 to 3
#define OUTPUT 2

// Function prototypes
void initialize(u_char *State);
void swap(u_char *a, u_char *b);
void ksa(u_char *State, u_char *key);
u_char * prng(u_char *State, int msglength);
void rc4(u_char *key, u_char *input, u_char *output);

int main(int argc, char *argv[]) {
	u_char key[256];		// The encryption/decryption key
	u_char input[256];		// The input data
	u_char output[256];		// The output
	
	//strncpy((char *) key, argv[1], 256);
	//strncpy((char *) input, argv[2], 256);
	
	// encrypt
	key[0]=0x01;
	key[1]=0x02;
	key[2]=0x03;
	key[3]=0x04;
	key[4]=0x05;	
	
	rc4(key, input, output);
	
	#if OUTPUT >= 1
	int i = 0;
	printf("\n\n--- Encryption ---");
	printf("\nOutput:\n");
	for(i=0; i<strlen((char *) input); i++) {
		if(output[i]<16)
			printf("0%0x ", output[i]);
		else
			printf("%0x ", output[i]);
	}
	printf("\n");
	#endif
	
	// decrypt
	rc4(key, output, input); 

	#if OUTPUT >= 1
	printf("\n\n--- Decryption ---");
	printf("\nOutput:\n");
	printf("%s\n", input);
	#endif
	
	exit(0);
}

/*
 * RC4 functions
 */
 
/* Initialize State[256] to the identity permutation. */
void initialize(u_char *State) {
	int i;
	
	for(i=0; i<256; i++) {
		State[i] = i;
	}
	#if OUTPUT == 3
	printf("Initialized State[]:\n");
	for(i=0; i<16; i++)
		printf(" 0%x ", State[i]);
	printf("\n");
	for(i=1; i<16; i++) {
		for(j=0; j<16; j++)
			printf(" %x ", State[16*i+j]);
		printf("\n");
	}
	#endif
	return;
}

/* Swap array elements i=State[i] and b=State[j]. */
void swap(u_char *i, u_char *j) {
	u_char temp;
	
	temp = *i;
	*i = *j;
	*j = temp;
}

/* Key scheduling algorithm. Swap array elements based on the key. */
void ksa(u_char *State, u_char *key) {
	int byte, i, keylen, j=0;
	
	keylen = (int) strlen((char *) key);
	
	for(i=0; i<256; i++) {
		j = (j + State[i] + key[i%keylen]) % 256;
		swap(&State[i], &State[j]);
	}

	#if OUTPUT >= 2
	printf("Key scheduled State[]:\n");

	for(i=0; i<16; i++) {
		for(j=0; j<16; j++) {
			byte = State[16*i+j];
			if(byte<16)
				printf(" 0%x ", byte);
			else
				printf(" %x ", byte);
		}
		printf("\n");
	}
	#endif
	return;
}

/* Pseudo-random number generator: Generate the keystream. */
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
void rc4(u_char *key, u_char *input, u_char *output) {
	int i;
	u_char State[256];
	u_char *keystream;
	
	initialize(State);
	
	ksa(State, key);
	
	keystream = prng(State, strlen((char *) input));
	
	for(i=0; i<strlen((char *) input); i++)
		output[i] = input[i] ^ keystream[i];
	
	#if OUTPUT >= 1
	printf("\n--- Key Generation ---\nKey: %s\n", key);
	printf("Keystream:\n");
	for(i=0; i<strlen((char *) input); i++) {
		if(keystream[i]<16)
			printf("0%x ", keystream[i]);
		else
			printf("%x ", keystream[i]);
	}
	#endif
}
