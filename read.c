#include <stdio.h>
#include <string.h>

// AES 128bit = 16 bytes
#define BYTES 16

int main(){
	unsigned char plainText[BYTES];	// plaintext	
	unsigned char key[BYTES];		// encryption key
	unsigned short int sizeCheck;	// temporary saving the length of plaintext and key
	
	unsigned int plainTextHEX[BYTES];	// plaintext in hexadecimal
	unsigned int keyHEX[BYTES];			// key in hexadecimal
	
	int i;	// counter for loops
	
	// read the text for encryption
	do{
		printf("Text: ");
		gets(plainText);
		//scanf("%[^\n]s", plainText);
		
		// clear the input buffer
		fflush(stdin);
		
		// save the length of plaitext
		sizeCheck = strlen(plainText);
	}while(sizeCheck != BYTES);	
	
	// print the plaintext as a string and in hexadecimal form
	printf("%s = ", plainText);
	for(i = 0; i < BYTES; i++){
		// saving hexadecimal value to new array
		plainTextHEX[i] = plainText[i];
		printf("%x  ", plainTextHEX[i]);
	}
	
	// line feed
	printf("\n\n");
	
	// read the key for encryption
	do{
		printf("Key: ");
		gets(key);
		//scanf("%[^\n]s", key);
			
		// clear the input buffer
		fflush(stdin);
		
		// save the length of key
		sizeCheck = strlen(key);
	}while(sizeCheck != BYTES);	

	// print the key as a string and in hexadecimal form
	printf("%s = ", key);
	for(i = 0; i < BYTES; i++){
		// saving hexadecimal value to new array
		keyHEX[i] = key[i];
		printf("%x  ", keyHEX[i]);
	}
	
return 0;
}
