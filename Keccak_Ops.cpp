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