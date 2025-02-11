#include <cstdint>
#include <iostream>
#include "Board.hpp"
//#include "Magic_number.hpp"
#include "Moves.hpp"
#include "Utility.hpp"

//int RBits2[64] = {
//  12, 11, 11, 11, 11, 11, 11, 12,
//  11, 10, 10, 10, 10, 10, 10, 11,
//  11, 10, 10, 10, 10, 10, 10, 11,
//  11, 10, 10, 10, 10, 10, 10, 11,
//  11, 10, 10, 10, 10, 10, 10, 11,
//  11, 10, 10, 10, 10, 10, 10, 11,
//  11, 10, 10, 10, 10, 10, 10, 11,
//  12, 11, 11, 11, 11, 11, 11, 12
//};
//
//int BBits2[64] = {
//  6, 5, 5, 5, 5, 5, 5, 6,
//  5, 5, 5, 5, 5, 5, 5, 5,
//  5, 5, 7, 7, 7, 7, 5, 5,
//  5, 5, 7, 9, 9, 7, 5, 5,
//  5, 5, 7, 9, 9, 7, 5, 5,
//  5, 5, 7, 7, 7, 7, 5, 5,
//  5, 5, 5, 5, 5, 5, 5, 5,
//  6, 5, 5, 5, 5, 5, 5, 6
//};
//
//uint64_t getRookMoves(uint64_t x, uint64_t y) {
//	uint64_t rook_move = 0;
//	//rook_move << y * 8 + x;
//
//	for (uint64_t i = 1; i < 7; ++i) {
//		uint64_t moveh = 1;
//		moveh = moveh << (y * 8 + i);
//		rook_move |= moveh;
//
//		uint64_t movev = 1;
//		movev = movev << (i * 8 + x);
//		rook_move |= movev;
//
//	}
//
//	return rook_move;
//}

int main(){

	/*Board b;
	b.add_piece(0, 3, 6);
	b.add_piece(0, 3, 1);
	b.add_piece(6, 3, 0);
	b.add_piece(9, 1, 4);
	b.add_piece(5, 6, 4);
	b.plot();

	uint64_t occ = b.getOccupationBoard();
	printBitboard(occ);

	uint64_t res = get_ls1b_index(occ);
	std::cout << res << std::endl;

	MagicNumbersTest();

	moveTest();*/

	//boardTest();

	Moves m;

	//m.initAll();


	//printBitboard(i);
	//printBitboard(i & 0xFFFF);

	//uint64_t rook_move = 1;

	//std::cout << (rook_move << 3) << ";" << (rook_move >> 3) << std::endl;

	//uint64_t rook_move = maskRookAttacks(4*8+3);
	//printBitboard(rook_move);

	//uint64_t occ = b.getOccupationBoard();
	//printBitboard(occ & rook_move);

	/*uint64_t rook_pos = 4 * 8 + 3;
	uint64_t magic_num = find_magic_number(rook_pos, RBits2[rook_pos], 0);

	uint64_t attack = transform(occ, magic_num, RBits2[rook_pos]);*/

	//printBitboard(attack);

	//uint64_t index_num = (occ & rook_move) * magic_num;

	//printBitboard(index_num);

	//index_num >>= 64 - RBits2[4 * 8 + 3];

	//printBitboard(index_num);

	//uint64_t rook_attack = rook_attacks_on_the_fly(4 * 8 + 3, occ);
	//printBitboard(rook_attack);

	//uint64_t bishop_attack = bishop_attacks_on_the_fly(4 * 8 + 3, occ);
	//printBitboard(bishop_attack);

	return 0;
}