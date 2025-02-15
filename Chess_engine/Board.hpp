#pragma once
#include <vector>
#include <cstdint>
#include <iostream>
#include "Utility.hpp"
#include "Moves.hpp"
#include <stack>
#include <array>


class Board {

	struct boardStruct {
		std::vector<uint64_t> m_bitboards;
		std::vector<uint64_t> m_occupancies;
		int16_t m_side;
		int16_t m_enpassant;
		int8_t m_castle;
	};

	//Board init
	std::vector<uint64_t> m_bitboards;
	std::vector<uint64_t> m_occupancies = std::vector<uint64_t>(3);
	int8_t m_side = -1;
	int8_t m_enpassant = -1;
	int8_t m_castle = 0;
	boardStruct m_state;
	Moves m_moves;

	boardStruct m_bs;
	std::stack<boardStruct> m_copy_stack = {};

	// Move generation variables
	// define source & target squares
	uint8_t m_source_square;
	uint8_t m_target_square;

	// define current piece's bitboard copy & it's attacks
	uint64_t m_bitboard, m_attacks;

	// makeMove variables
	uint8_t m_piece = 0;
	uint8_t m_promoted_piece = 0;
	int32_t m_capture = 0;
	int32_t m_double_push = 0;
	int32_t m_enpass = 0;
	int32_t m_castling = 0;

	uint8_t m_start_piece = 0;
	uint8_t m_end_piece = 0;


public:

	Board() {
		for (int i = 0; i < 12; ++i) {
			m_bitboards.push_back(0);
		}
		m_moves.initAll();
	}

	std::vector<uint64_t>  const& bitboards() { return m_bitboards; }
	int16_t side() { return m_side; }
	int16_t enpassant() { return m_enpassant; }
	int16_t castle() { return m_castle; }
	int16_t stackSize() { return m_copy_stack.size(); }

	void add_piece(uint32_t piece_val, uint32_t file, uint32_t rank);

	void parse_fen(std::string fen);

	std::vector<uint64_t>  getOccupationBoard();

	void plot();

	void copyBoard();

	void takeBack();

	void clearCopy();

	bool isSquareAttacked(int square, int side);

	// generate all moves
	void generateMoves(std::vector<uint64_t>* move_list);

	int makeMove(int move, int move_flag);
};

void print_move_list(std::vector<uint64_t>* move_list);

void boardTest();

void isAttackedTest();

void MoveGenerationTest();

void copyTakeBackTest();

void makeMoveTest();

static inline void perftDriver(Board *b, int depth);

void perftTest(std::string fen_str, int depth);