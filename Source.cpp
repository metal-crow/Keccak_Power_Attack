#include <stdio.h>
#include <stdint.h>

static uint64_t* ReadKey(FILE* fp) {
	uint64_t* key = new uint64_t[5];

	fseek(fp, 0, SEEK_SET);

	for (int i = 0; i < 5; i++) {
		uint64_t row = 0;
		for (int j = 0; j < 8; j++) {
			uint8_t c;
			fscanf(fp, "%2hhx", &c);
			row <<= 8;
			row |= c;
		}
		key[i] = row;
	};

	return key;
}


int main() {
	//TODO do we want to optimize reading via Memmap?
	FILE* input = fopen("C:\\Users\\Magnesium\\Desktop\\HWSW\\03_2R_t500000_Avg_RandKey_N\\InputText.txt", "r");

	uint64_t* key = ReadKey(input);



	return 0;
}