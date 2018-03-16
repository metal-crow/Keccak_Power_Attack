#include <cstring>

#include "Keccak_Ops.h"

inline uint64_t ROTL64(uint64_t x, int8_t r)
{
	return (x << r) | (x >> (64 - r));
}

void Theta_Second_XOR_Operation(uint64_t *sum_sheet)
{
	uint64_t D[5];
	// SumSheet (x+1, z-1) XOR  SumSheet (x-1, z) 
	D[0] = ROTL64(sum_sheet[1], 1) ^ sum_sheet[4];
	D[1] = ROTL64(sum_sheet[2], 1) ^ sum_sheet[0];
	D[2] = ROTL64(sum_sheet[3], 1) ^ sum_sheet[1];
	D[3] = ROTL64(sum_sheet[4], 1) ^ sum_sheet[2];
	D[4] = ROTL64(sum_sheet[0], 1) ^ sum_sheet[3];

	sum_sheet[0] = D[0];
	sum_sheet[1] = D[1];
	sum_sheet[2] = D[2];
	sum_sheet[3] = D[3];
	sum_sheet[4] = D[4];
}

void sha3_permutation_Theta_with_Key(uint64_t* keccak_state, uint64_t* message, uint64_t* key) {
	//reset state
	memset(keccak_state, 0, 25 * 8);
	//5-16 is the message
	memcpy(&keccak_state[5], message, 11 * 8);

	uint64_t C[5], D[5];

	// Theta Plane Generation (XOR Column)
	for (int x = 0; x < 5; x++)
	{
		C[x] = keccak_state[x] ^ keccak_state[x + 5] ^ keccak_state[x + 10] ^ keccak_state[x + 15] ^ keccak_state[x + 20];
	}

	// SumSheet (x+1, z-1) XOR  SumSheet (x-1, z) 
	D[0] = ROTL64(C[1], 1) ^ C[4] ^ key[0];
	D[1] = ROTL64(C[2], 1) ^ C[0] ^ key[1];
	D[2] = ROTL64(C[3], 1) ^ C[1] ^ key[2];
	D[3] = ROTL64(C[4], 1) ^ C[2] ^ key[3];
	D[4] = ROTL64(C[0], 1) ^ C[3] ^ key[4];

	// XOR Sheet Result with Original State - S(x,y,z) 
	for (int x = 0; x < 5; x++)
	{
		keccak_state[x] ^= D[x];
		keccak_state[x + 5] ^= D[x];
		keccak_state[x + 10] ^= D[x];
		keccak_state[x + 15] ^= D[x];
		keccak_state[x + 20] ^= D[x];
	}
}