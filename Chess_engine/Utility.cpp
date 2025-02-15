#include "Utility.hpp"
#include <cstdint>
#include <iostream>
#include <chrono>

// set/get/pop bit macros
#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define pop_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))

// count bits within a bitboard (Brian Kernighan's way)
uint8_t count_bits(uint64_t bitboard)
{
	// bit counter
	uint8_t count = 0;
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
uint8_t get_ls1b_index(uint64_t bitboard) {
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

// get time in milliseconds
uint64_t get_time_ms(){
	auto now = std::chrono::high_resolution_clock::now();
	auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);

	auto value = now_ms.time_since_epoch();
	uint64_t time = value.count();
	return time;
}