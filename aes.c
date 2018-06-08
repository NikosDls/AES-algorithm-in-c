#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aes.h"

// substitute box
// size 16x16
const unsigned int sBox[16][16] = {
	{	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76	},
  	{	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0	},
  	{	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15	},
  	{	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75	},
  	{	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84	},
  	{	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf	},
  	{	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8	},
  	{	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2	},
  	{	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73	},
  	{	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb	},
  	{	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79	},
  	{	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08	},
  	{	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a	},
  	{	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e	},
  	{	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf	},
  	{	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16	}
	};
	
// reverse substitute box
// size 16x16
const unsigned int rsBox[16][16] = {
  	{	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb	},
  	{	0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb	},
  	{	0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e	},
  	{	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25	},
  	{	0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92	},
  	{	0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84	},
  	{	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06	},
  	{	0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b	},
  	{	0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73	},
  	{	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e	},
  	{	0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b	},
  	{	0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4	},
  	{	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f	},
  	{	0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef	},
  	{	0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61	},
  	{	0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d	} 
  	};

// round constants to find round keys
const unsigned int rCon[11] = { 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };
  	
// GF(2^8) multiplication constants to do mix columns
const unsigned int mCon[BLOCK_SIZE][BLOCK_SIZE] = {
  	{	0x02, 0x03, 0x01, 0x01	},
  	{	0x01, 0x02, 0x03, 0x01	},
  	{	0x01, 0x01, 0x02, 0x03	},
  	{	0x03, 0x01, 0x01, 0x02	}
  	};
  	
// GF(2^8) multiplication constants to reverse mix columns
const unsigned int rmCon[BLOCK_SIZE][BLOCK_SIZE] = {
  	{	0x0e, 0x0b, 0x0d, 0x09	},
  	{	0x09, 0x0e, 0x0b, 0x0d	},
  	{	0x0d, 0x09, 0x0e, 0x0b	},
  	{	0x0b, 0x0d, 0x09, 0x0e	}
  	};

// this function produces all round keys
void key_schedule(unsigned int w[], unsigned int key[][BLOCK_SIZE]){
	unsigned int row, column;	// row and column for lookup
	unsigned int temp[4];		// temporary holds the results
	int i, j, k;				// counters for the loops and used as array "pointers"
	
	// the first round key is the given key
	// we store it to the first 4 words
	// w0 w1 w2 w3 || w[0] ... w[15]
	printf("Round keys:\n");
  	for(i = 0; i < BLOCK_SIZE; i++){
  		printf("w[%d] = ", i);
  		for(j = 0; j < BLOCK_SIZE; j++){
  			w[(i * 4) + j] = key[j][i];
  			printf("%x ", w[(i * 4) + j]);
		}
		printf("\t");
  	}
  	
	// all other round keys are found from the previous round keys
	// start for 4, because we calculated the 4 words before
	// 4 is the block size and 10 is the number of rounds	
  	for(i = 4; i < (4 * (10 + 1)); i++){
		// k is "pointer" to find wi-1
		k = (i - 1) * 4;
		// temp = w-1
	    temp[0] = w[k + 0];
	    temp[1] = w[k + 1];
	    temp[2] = w[k + 2];
	    temp[3] = w[k + 3];

	    if(i % 4 == 0){
	      	// rot_word function
	        k = temp[0];
	        temp[0] = temp[1];
	        temp[1] = temp[2];
	        temp[2] = temp[3];
	        temp[3] = k;
	
			// sub_word function
			for(j = 0; j < 4; j++){
				get2Bytes(temp[j], &row, &column);
				temp[j] = sub_byte(row, column);
			}
			
			// temp = sub_word(rot_word(temp)) XOR RCi/4
	      	temp[0] = temp[0] ^ rCon[i/4];
	    }
	    
		// j is "pointer" to find wi
	    j = i * 4; 
	    // k is "pointer" to find wi-4
		k = (i - 4) * 4;
		
		// wi = wi-4 XOR temp 
    	w[j + 0] = w[k + 0] ^ temp[0];
    	w[j + 1] = w[k + 1] ^ temp[1];
    	w[j + 2] = w[k + 2] ^ temp[2];
    	w[j + 3] = w[k + 3] ^ temp[3];
	}

	// print round keys 1 - 10
	// round key 0(given key) printed before
	for(i = 4; i < 44; i++){
  		printf("%s", (i % 4 == 0) ? "\n" : "\t");
		printf("w[%d] = ", i);
  		for(j = 0; j < 4; j++){
  			printf("%x ", w[(i * 4) + j]);
		}
  	}
	printf("\n\n");

return;
}

// this function do result = a XOR b
void add_roundkey(unsigned int result[][BLOCK_SIZE], unsigned int a[][BLOCK_SIZE], unsigned int b[][BLOCK_SIZE]){
	int i, j;	// counters for the loops

	for(i = 0; i < BLOCK_SIZE; i++)
		for(j = 0; j < BLOCK_SIZE; j++)
			result[i][j] = a[i][j] ^ b[i][j];
	
return;
}

// this function lookup in sBox table and return the new value
unsigned int sub_byte(unsigned int row, unsigned int column){
return sBox[row][column];
}

// this function shifts the rows to the left
// each row is shifted differently
void shift_rows(unsigned int state[][BLOCK_SIZE]){
	unsigned int temp;	// temporary variables to do the shifts
	
	// first row is not shifted
	
	// second row is shifted left 1 time  
	temp = state[1][0];
	state[1][0] = state[1][1]; 
	state[1][1] = state[1][2];
	state[1][2] = state[1][3];
	state[1][3] = temp;
	
	// third row is shifted left 2 times 
	temp = state[2][0];
	state[2][0] = state[2][2];
	state[2][2] = temp;
	
	temp = state[2][1];
	state[2][1] = state[2][3];
	state[2][3] = temp;
	
	// fourth row is shifted left 3 times
	temp = state[3][0];
	state[3][0] = state[3][3];
	state[3][3] = state[3][2];
	state[3][2] = state[3][1];
	state[3][1] = temp;

return;
}

// this function mix the columns
// state column multiplied with mCon row
// instead of normal multiplication here we do GF(2^8) multiply
// and instead of addition here we do XOR operation
void mix_columns(unsigned int result[][BLOCK_SIZE], unsigned int state[][BLOCK_SIZE]){
	unsigned int sum = 0;	// we hold the sum here
	int i, j, k;			// counters for the loops
	
    for(i = 0; i < BLOCK_SIZE; i++){
    	for(j = 0; j < BLOCK_SIZE; j++){
        	for(k = 0; k < BLOCK_SIZE; k++){
            	sum ^= gfMul(state[k][i], mCon[j][k]);
            }
            result[j][i] = sum;
            sum = 0;
        }
    }
		
return;
}

// this function encrypts the plaintext 
void encryption(unsigned int state[][BLOCK_SIZE], unsigned int w[]){
	unsigned int roundKey[BLOCK_SIZE][BLOCK_SIZE];	// round key
	unsigned int table[BLOCK_SIZE][BLOCK_SIZE];		// temp array to hold results
	
	unsigned int row, column;	// row and column for sub_bytes lookup
	int i, j, k;				// counters for the loops
		
	// round 0
  	// 1. add_roundkey
  	getRoundKey(w, roundKey, 0);
  	add_roundkey(state, state, roundKey);
  	
	printf("add_roundkey:\n");  
	printArray(state);
	
  	// round 1-9
  	// 1. sub_bytes
  	// 2. shift_rows
  	// 3. mix_columns
  	// 4. add_roundkey
  	for(k = 1; k < 10; k++){
		for(i = 0; i < BLOCK_SIZE; i++){
			for(j = 0; j < BLOCK_SIZE; j++){
				get2Bytes(state[i][j], &row, &column);
				state[i][j] = sub_byte(row, column);
			}
		}
		printf("sub_bytes:\n");  
		printArray(state);
		
		shift_rows(state);
		
		printf("shift_rows:\n");  
		printArray(state);
		
		mix_columns(table, state);
		
		printf("mix_columns:\n");  
		printArray(table);
		
		getRoundKey(w, roundKey, k);
    	add_roundkey(state, table, roundKey);
    	
    	printf("add_roundkey:\n");  
    	printArray(state);
	}
	
	// round 10
	// 1. sub_bytes
	// 2. shift_rows
	// 3. add_roundkey
  	for(i = 0; i < BLOCK_SIZE; i++){
		for(j = 0; j < BLOCK_SIZE; j++){
			get2Bytes(state[i][j], &row, &column);
			state[i][j] = sub_byte(row, column);
		}
	}
	
	printf("sub_bytes:\n");  
	printArray(state);
	
	shift_rows(state);
	
	printf("shift_rows:\n");  
	printArray(state);
	
	getRoundKey(w, roundKey, 10);
    add_roundkey(state, state, roundKey);
	
	printf("add_roundkey:\n");  
	printArray(state);
	
return;
}

// this function lookup in rsBox table and return the new value
unsigned int inv_sub_byte(unsigned int row, unsigned int column){
return rsBox[row][column];
}

// this function shifts the rows to the right
// each row is shifted differently
void inv_shift_rows(unsigned int state[][BLOCK_SIZE]){
	unsigned int temp;	// temporary variable to do the shifts
	
	// first row is not shifted
	
	// second row is shifted right 1 time  
	temp = state[1][3];
	state[1][3] = state[1][2]; 
	state[1][2] = state[1][1];
	state[1][1] = state[1][0];
	state[1][0] = temp;
	
	// third row is shifted right 2 times 
	temp = state[2][0];
	state[2][0] = state[2][2];
	state[2][2] = temp;
	
	temp = state[2][1];
	state[2][1] = state[2][3];
	state[2][3] = temp;
	
	// fourth row is shifted right 3 times
	temp = state[3][0];
	state[3][0] = state[3][1];
	state[3][1] = state[3][2];
	state[3][2] = state[3][3];
	state[3][3] = temp;

return;
}

void inv_mix_columns(unsigned int result[][BLOCK_SIZE], unsigned int state[][BLOCK_SIZE]){
	unsigned int sum = 0;	// we hold the sum here
	int i, j, k;			// counters for the loops
	unsigned int temp;		// temporary variable to hold the results
	
    for(i = 0; i < BLOCK_SIZE; i++){
        for(j = 0; j < BLOCK_SIZE; j++){
            for(k = 0; k < BLOCK_SIZE; k++){
            	switch(rmCon[j][k]){
            		// x = state[k][i]
            		case 9:
            			temp = gfMul(state[k][i], 2);	// x * 2
            			temp = gfMul(temp, 2);			// (x * 2) * 2
            			temp = gfMul(temp ,2);			// ((x * 2) * 2) * 2
            			temp ^= state[k][i];			// (((x * 2) * 2) * 2) + x = x * 9
						break;	
            			
            		case 11:
            			temp = gfMul(state[k][i], 2);	// x * 2
            			temp = gfMul(temp, 2);			// (x * 2) * 2
            			temp ^= state[k][i];			// ((x * 2) * 2) + x
            			temp = gfMul(temp, 2);			// (((x * 2) * 2) + x) * 2
            			temp ^= state[k][i];			// ((((x * 2) * 2) + x) * 2) + x = x * 11
            			break;
            			
            		case 13:
            			temp = gfMul(state[k][i], 2);	// x * 2
            			temp ^= state[k][i];			// (x * 2) + x
            			temp = gfMul(temp, 2);			// ((x * 2) + x) * 2
            			temp = gfMul(temp, 2);			// (((x * 2) + x) * 2) * 2
            			temp ^= state[k][i];			// ((((x * 2) + x) * 2) * 2) + x = x * 13
            			break;
            			
            		case 14:
            			temp = gfMul(state[k][i], 2);	// x * 2
            			temp ^= state[k][i];			// (x * 2) + x
            			temp = gfMul(temp, 2);			// ((x * 2) + x) * 2
            			temp ^= state[k][i];			// (((x * 2) + x) * 2) + x
            			temp = gfMul(temp, 2);			// ((((x * 2) + x) * 2) + x) * 2 = x * 14
            			break;
				}
            	sum ^= temp;
            }
            result[j][i] = sum;
            sum = 0;
        }
    }
		
return;	
}

// this function encrypts the plaintext 
void decryption(unsigned int state[][BLOCK_SIZE], unsigned int w[]){
	unsigned int roundKey[BLOCK_SIZE][BLOCK_SIZE];	// round key
	unsigned int table[BLOCK_SIZE][BLOCK_SIZE];		// temp array to hold results
	
	unsigned int row, column;	// row and column for sub_bytes lookup
	int i, j, k, i1, i2;		// counters for the loops
	
	// round 0
	// 1. add_roundkey
	// 2. inv_shift_rows
	// 3. inv_sub_bytes
	getRoundKey(w, roundKey, 10);
    add_roundkey(state, state, roundKey);
	
	printf("\nadd_roundkey:\n");  
	printArray(state);
	
	inv_shift_rows(state);
	
	printf("inv_shift_rows:\n");  
	printArray(state);
	
  	for(i = 0; i < BLOCK_SIZE; i++){
		for(j = 0; j < BLOCK_SIZE; j++){
			get2Bytes(state[i][j], &row, &column);
			state[i][j] = inv_sub_byte(row, column);
		}
	}
	
	printf("inv_sub_bytes:\n");  
	printArray(state);
	
	// round 1-9
  	// 1. add_roundkey
  	// 2. inv_mix_columns
  	// 3. inv_shift_rows
  	// 4. inv_sub_bytes
  	for(k = 9; k > 0; k--){
  		getRoundKey(w, roundKey, k);
    	add_roundkey(state, state, roundKey);
    	
    	printf("add_roundkey:\n");  
    	printArray(state);
    	
    	inv_mix_columns(table, state);
		
		printf("inv_mix_columns:\n");  
		printArray(table);
    	
    	for(i1 = 0; i1 < BLOCK_SIZE; i1++){
    		for(i2 = 0; i2 < BLOCK_SIZE; i2++){
    			state[i1][i2] = table[i1][i2];
			}
		}
    	
    	inv_shift_rows(state);
		
		printf("inv_shift_rows:\n");  
		printArray(state);
    	
		for(i = 0; i < BLOCK_SIZE; i++){
			for(j = 0; j < BLOCK_SIZE; j++){
				get2Bytes(state[i][j], &row, &column);
				state[i][j] = inv_sub_byte(row, column);
			}
		}
		
		printf("inv_sub_bytes:\n");  
		printArray(state);	
	}
	
	// round 10
  	// 1. add_roundkey
  	getRoundKey(w, roundKey, 0);
  	add_roundkey(state, state, roundKey);
  	
	printf("add_roundkey:\n");  
	printArray(state);

return;
}

// this function converts hexadecimal character to integer
int hexCharToDec(char hex){
	if(hex >= 48 && hex <= 57){	// ascii code for character 1-9
		return (hex - '0');		// as it happens, the ascii value of the characters 1-9 is greater than the value of '0'
	}else{
		switch(hex){
			case 'a':			// a hexadecimal is a number 10 to decimal. Similar for the rest
				return 10;			
				
			case 'b':
				return 11;
				
			case 'c':
				return 12;
				
			case 'd':
				return 13;
				
			case 'e':
				return 14;
	
			case 'f':
				return 15;
		}
	}
}

// this function returns the integer value of the 2 bytes hex input
// input: xy || row = x	and column = y 
void get2Bytes(unsigned int a, unsigned int *row, unsigned int *column){
	// we need 2 bytes for the hexadecimal value and 1 more for '\0' character
	unsigned char temp[3];
	
	// convert the number to string
	sprintf(temp, "%x", a);
	
	if(strlen(temp) == 1){	// if number is smaller than 15, c saving 1 digit instead 2 digits
							// e.g. (14)dec = (0x0e)hex | C saving only e instead of 0e 
		// add the '\0' character to 2nd position, because we need 1 slot for the hexadecimal number
		temp[1] = '\0';
		
		// find the row for the lookup
		*row = 0;	// row will be 0, because number will have form like this: 0x0..
		//printf("(%c)hex = (%d)dec\n", temp[0], *row);
		
		// find the column for the lookup
		*column = hexCharToDec(temp[0]);
		//printf("(%c)hex = (%d)dec\n", temp[1], *column);
	}else{
		// add the '\0' character to 3rd position, because we need 2 slots for the hexadecimal number
		temp[2] = '\0';
		
		// find the row for the lookup
		*row = hexCharToDec(temp[0]);
		//printf("(%c)hex = (%d)dec\n", temp[0], *row);
		
		// find the column for the lookup
		*column = hexCharToDec(temp[1]);
		//printf("(%c)hex = (%d)dec\n", temp[1], *column);
	}

return;
}

// this function return the 16bytes round key
void getRoundKey(unsigned int w[], unsigned int roundKey[][BLOCK_SIZE], int round){
	int i, j;	// counters for the loops

	for(i = (round * 4); i < ((round * 4) + 4); i++){
  		for(j = 0; j < 4; j++){
			roundKey[j][i - (round * 4)] = w[(i * 4) + j];	
		}
  	}
  	
return;
}

// multiply two numbers in the GF(2^8)
// polynomial: x^8 + x^4 + x^3 + x + 1
// binary: 00011011  || hex: 0x1b
unsigned int gfMul(unsigned int a, unsigned int b){
	unsigned int r = 0;			// result
	unsigned int hi_bit_set;	// high bit (leftmost)
	int i;						// counter for the loop
	
	for(i = 0; i < 8; i++) {
    	if(b & 1) 
        	r ^= a;
    	hi_bit_set = (a & 0x80);
    	a <<= 1;
        
		if(hi_bit_set) 
        	a ^= 0x1b;	// x^8 + x^4 + x^3 + x + 1
    	b >>= 1;
	}
    
	// if result legnth is more than 8 bits
	if(r > 255 && r < 512)
		return r - 256;
		
	if(r > 511)
		return r - 512;
	
	// if result is max 8 bits
	return r;
}

// this function converts string to unsigned int array 4x4
void convertStringToBlock(char string[BYTES+1], unsigned int block[][BLOCK_SIZE]){
	int i, j;	// counters for the loops
	
	for(i = 0; i < BLOCK_SIZE; i++){
		for(j = 0; j < BLOCK_SIZE; j++){
			block[j][i] = string[BLOCK_SIZE * i + j]; 
		}
	}
	
return;
}

// this function converts unsigned int array 4x4 to string
void convertBlockToString(unsigned int block[][BLOCK_SIZE], char string[BYTES+1]){
	int i, j;	// counters for the loops
	
	for(i = 0; i < BLOCK_SIZE; i++){
		for(j = 0; j < BLOCK_SIZE; j++){
			string[BLOCK_SIZE * i + j] = block[j][i]; 
		}
	}
	
return;
}

// this function prints the array
void printArray(unsigned int array[][BLOCK_SIZE]){
	int i, j;	// counters for the loops
	
	for(i = 0; i < BLOCK_SIZE; i++){
		for(j = 0; j < BLOCK_SIZE; j++){
			printf("%x\t", array[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	
return;
}

/* Run the encryption and decryption
 * Plaintext: 	Two One Nine Two = (54 77 6F 20 4F 6E 65 20 4E 69 6E 65 20 54 77 6F)hex
 * Key: 		Thats my Kung Fu = (54 68 61 74 73 20 6D 79 20 4B 75 6E 67 20 46 75)hex
 * we decrypting the output of encryption
 */
void test(){
	unsigned int state[BLOCK_SIZE][BLOCK_SIZE] = {	// input
		{	0x54, 0x4f,	0x4e, 0x20	},
		{	0x77, 0x6e, 0x69, 0x54	},
		{	0x6f, 0x65, 0x6e, 0x77	},
		{	0x20, 0x20, 0x65, 0x6f	}
		};
	
	unsigned int key[BLOCK_SIZE][BLOCK_SIZE] = {	// encryption key
		{	0x54, 0x73,	0x20, 0x67	},
		{	0x68, 0x20, 0x4b, 0x20	},
		{	0x61, 0x6d, 0x75, 0x46	},
		{	0x74, 0x79, 0x6e, 0x75	}
		};	
	
	// we have 10 round, so we need 40 words in array plus 4 for the given key
	// each word have 4 bytes, so we need 44 * 4 = 176
	unsigned int w[176];		// all round keys	
	
	/*****************************/
	
	// print the plaintext and key
	printf("Plaintext:\n");
	printArray(state);
	
	printf("Key:\n");
	printArray(key);

	// we calculate all round keys 1-10
	key_schedule(w, key);
	
	encryption(state, w);
	
	// print the result (ciphertext)
	printf("\nCiphertext:\n");
	printArray(state);
	
	printf("\n\n--------------------\n\n\n");
	
	decryption(state, w);
	
	// print the result (plaintext)
	printf("\nPlaintext:\n");
	printArray(state);
	
return;
}
