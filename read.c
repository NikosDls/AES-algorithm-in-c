#include <stdio.h>
#include <string.h>

#include "aes.h"

void readInput(char []);
void convertStringTo4x4Block(char [], unsigned int [][BLOCK_SIZE]);
void convert4x4BlockToString(unsigned int [][BLOCK_SIZE], char []);

// this function reads the user input
void readInput(char input[BYTES+1]){
	unsigned short int sizeCheck;	// temporary saving the length of given input
	
	// read the input
	do{
		gets(input);
		//scanf("%[^\n]s", plainText);
		
		// clear the input buffer
		fflush(stdin);
		
		// save the length of plaitext
		sizeCheck = strlen(input);
	}while(sizeCheck != BYTES);
	
return;
}

// this function converts string to unsigned int array 4x4
void convertStringTo4x4Block(char string[BYTES+1], unsigned int block[][BLOCK_SIZE]){
	int i, j;	// counter for loops
	
	for(i = 0; i < BLOCK_SIZE; i++){
		for(j = 0; j < BLOCK_SIZE; j++){
			block[j][i] = string[BLOCK_SIZE * i + j]; 
		}
	}
	
return;
}

// this function converts unsigned int array 4x4 to string
void convert4x4BlockToString(unsigned int block[][BLOCK_SIZE], char string[BYTES+1]){
	int i, j;	// counter for loops
	
	for(i = 0; i < BLOCK_SIZE; i++){
		for(j = 0; j < BLOCK_SIZE; j++){
			string[BLOCK_SIZE * i + j] = block[j][i]; 
		}
	}
	
return;
}
