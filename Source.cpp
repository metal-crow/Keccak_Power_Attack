#include <stdio.h>
#include <stdint.h>
#include <cstring>

#include "Keccak_Ops.h"

static uint64_t* ReadKey(FILE* fp) {
	uint64_t* key = new uint64_t[5];

	//make sure we start at the beginnning of the file since the key must be there
	fseek(fp, 0, SEEK_SET);

	//Read the 40 key bytes into 5 64 bit chunks
	for (int i = 0; i < 5; i++) {
		uint64_t row = 0;
		for (int j = 0; j < 8; j++) {
			uint8_t c;
			fscanf(fp, "%2hhx", &c); //TODO error handle
			row <<= 8;
			row |= c;
		}
		key[i] = row;
	};

	//don't mess up the FP seek state, reset it to prevent confusion for later functions
	fseek(fp, 0, SEEK_SET);

	return key;
}

static uint64_t* ReadMessage(FILE* fp) {
	uint64_t* msg = new uint64_t[11];

	//Message begins 0x5C bytes from the start of a new entry (Single space in a row)
	fseek(fp, 0x5C, SEEK_CUR);

	//Read in the 88 message bytes into 11 64 bit chunks
	for (int i = 0; i < 11; i++) {
		uint64_t row = 0;
		for (int j = 0; j < 8; j++) {
			uint8_t c;
			fscanf(fp, "%2hhx", &c);
			row <<= 8;
			row |= c;
		}
		msg[i] = row;
	};

	//At this point file ptr offset should be at the start of the next message
	//So this function can just be rerun

	return msg;
}


//Hamming value count for every possible byte
static const uint8_t HammingWeightValues[256] = {
	0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,
	3,3,4,3,4,4,5,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,
	3,4,3,4,4,5,3,4,4,5,4,5,5,6,1,2,2,3,2,3,3,4,2,3,3,
	4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,
	3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,
	6,6,7,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,
	4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,
	6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,2,3,3,4,3,4,4,5,
	3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,
	4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,
	6,7,6,7,7,8
};

int main() {
	//TODO do we want to optimize reading via Memmap?
	FILE* input = fopen("C:\\Users\\Magnesium\\Desktop\\HWSW\\03_2R_t500000_Avg_RandKey_N\\InputText.txt", "r");

	//read in key
	uint64_t* key = ReadKey(input);

	//Run through Keccak Theta operation
	Theta_Second_XOR_Operation(key);

	uint64_t keccak_state[25];
	uint64_t key_guess[5];

	//for every message (Todo Parallelize)
	while (true) {
		uint64_t* msg = ReadMessage(input);

		for (int key_byte_guess = 0; key_byte_guess < 256; key_byte_guess++) {
			//expand the byte guess to a full key
			memset(key_guess, key_byte_guess, 5 * 8);

			//Run Keccak Theta op with the key byte guess and message
			sha3_permutation_Theta_with_Key(keccak_state, msg, key_guess);

			//Parse the 200 byte result from Theta out
			uint8_t hamming_weight_results[40] = { 0 };

			//go through each 40 byte plane
			for (int plane = 1; plane < 5; plane++) {
				//since we storing the state in chunks of 64 bits
				for (int y = 0; y < 5; y++) {
					//have to traverse each 64 bit chunk byte by byte
					for (int z = 0; z < 8; z++) {
						uint8_t val = ((uint8_t*)keccak_state)[plane*40 + y*8 + z];
						hamming_weight_results[y * 8 + z] += HammingWeightValues[val];
					}
				}
			}


		}

		delete msg;
	}

	return 0;
}