#pragma once
#include <cstdint>

uint8_t count_bits(uint64_t bitboard);

uint8_t get_ls1b_index(uint64_t bitboard);

uint64_t random_uint64();

uint64_t random_uint64_fewbits();

void printBitboard(uint64_t bitboard);

uint64_t get_time_ms();