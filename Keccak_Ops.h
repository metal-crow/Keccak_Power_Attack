#ifndef KECCAK_OPS_H
#define KECCAK_OPS_H

#include <stdint.h>

void Theta_Second_XOR_Operation(uint64_t *sum_sheet);
void sha3_permutation_Theta_with_Key(uint64_t* keccak_state, uint64_t* message, uint64_t* key);

#endif // !KECCAK_OPS_H
