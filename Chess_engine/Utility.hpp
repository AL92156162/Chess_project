#pragma once
#include <cstdint>

uint64_t set_bit(uint64_t b, uint64_t i);

uint64_t get_bit(uint64_t b, uint64_t i);

int count_bits(uint64_t bitboard);

uint64_t get_ls1b_index(uint64_t bitboard);

uint64_t random_uint64();

uint64_t random_uint64_fewbits();

void printBitboard(uint64_t bitboard);