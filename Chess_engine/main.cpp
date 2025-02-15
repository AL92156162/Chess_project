#include <cstdint>
#include <iostream>
#include "Board.hpp"
//#include "Magic_number.hpp"
#include "Moves.hpp"
#include "Utility.hpp"
#include <chrono>
#include <thread>

// FEN dedug positions
#define empty_board "8/8/8/8/8/8/8/8 w - - "
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "
#define kiwipete_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - "

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

	*/

	//moveTest();

	//boardTest();
	
	//isAttackedTest();

	//MoveGenerationTest();

	//copyTakeBackTest();

	//makeMoveTest();
	
	perftTest(start_position, 4);

	//perftTest(kiwipete_position, 5);

	//perftTest("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/1R2K2R b K-kq - ", 3);

	//perftTest("r3k2r/p2pqpb1/bn2pnp1/2pPN3/1p2P3/2N2Q1p/PPPBBPPP/1R2K2R w K-kq c6 ", 2);

	//perftTest("r3k2r/p1ppqpb1/bnN1pnp1/3P4/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - ", 1);
	

	//perftTest("r3k2r/p1ppqpb1/bn2pnp1/3PN3/Pp2P3/2N2Q1p/1PPBBPPP/R3K2R b KQkq a3 ", 2);

	//perftTest("r3k2r/p1ppqpb1/bn2pnp1/3PN3/4P3/p1N2Q1p/1PPBBPPP/R3K2R w KQkq - ", 1);
	

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