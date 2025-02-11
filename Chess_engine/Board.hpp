#pragma once
#include <vector>
#include <cstdint>
#include <iostream>
#include "Utility.hpp"

class Board {

	std::vector<uint64_t> m_bitboards;
	std::vector<uint64_t> m_occupancies = std::vector<uint64_t>(3);

	int16_t m_side = -1;
	int16_t m_enpassant = -1;
	int8_t m_castle = 0;

public:

	Board() {
		for (int i = 0; i < 12; ++i) {
			m_bitboards.push_back(0);
		}
	}

	std::vector<uint64_t> bitboards() { return m_bitboards; }
	int16_t side() { return m_side; }
	int16_t enpassant() { return m_enpassant; }
	int16_t castle() { return m_castle; }

	void add_piece(uint32_t piece_val, uint32_t file, uint32_t rank);

	void parse_fen(std::string fen);

	std::vector<uint64_t> getOccupationBoard();

	void plot();

	bool isSquareAttacked(int square, int side);

};

void boardTest();