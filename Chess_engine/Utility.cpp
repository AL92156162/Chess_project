#include "Utility.hpp"
#include <cstdint>
#include <iostream>

uint64_t set_bit(uint64_t b, uint64_t i) {
	return (b) |= (1ULL << i);
}

uint64_t get_bit(uint64_t b, uint64_t i) {
	return (b) & (1ULL << i);
}

#define pop_bit(bitboard, square) (get_bit(bitboard, square) ? bitboard ^= (1ULL << square) : 0)

// count bits within a bitboard (Brian Kernighan's way)
int count_bits(uint64_t bitboard)
{
	// bit counter
	int count = 0;
	// consecutively reset least significant 1st bit
	while (bitboard)
	{
		// increment count
		count++;
		// reset least significant 1st bit
		bitboard &= bitboard - 1;
	}
	// return bit count
	return count;
}

//// get least significant 1st bit index
uint64_t get_ls1b_index(uint64_t bitboard) {
	return _tzcnt_u64(bitboard);
}

uint64_t random_uint64() {
	uint64_t u1, u2, u3, u4;
	u1 = (uint64_t)(rand()) & 0xFFFF; u2 = (uint64_t)(rand()) & 0xFFFF;
	u3 = (uint64_t)(rand()) & 0xFFFF; u4 = (uint64_t)(rand()) & 0xFFFF;
	return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

uint64_t random_uint64_fewbits() {
	return random_uint64() & random_uint64() & random_uint64();
}

void printBitboard(uint64_t bitboard) {
	// print offset
	printf("\n");

	// loop over board ranks
	for (int rank = 0; rank < 8; rank++)
	{
		// loop over board files
		for (int file = 0; file < 8; file++)
		{
			// convert file & rank into square index
			int square = rank * 8 + file;

			// print ranks
			if (!file)
				printf("  %d ", 8 - rank);

			// print bit state (either 1 or 0)
			printf(" %d", get_bit(bitboard, square) ? 1 : 0);

		}

		// print new line every rank
		printf("\n");
	}

	// print board files
	printf("\n     a b c d e f g h\n\n");

	// print bitboard as unsigned decimal number
	printf("     Bitboard: %llud\n\n", bitboard);
}