#pragma once
#include "Board.hpp"
#include <map>

//##################################################################################################################
//                                                     VARIABLES
//##################################################################################################################

// set/get/pop bit macros
#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define pop_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))

// FEN dedug positions
#define empty_board "8/8/8/8/8/8/8/8 w - - "
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "

// ASCII pieces
std::string ascii_pieces = "PNBRQKpnbrqk";

// encode pieces
enum { P, N, B, R, Q, K, p, n, b, r, q, k };

// sides to move (colors)
enum { white, black, both };

// bishop and rook
enum { rook, bishop };

// convert ASCII character pieces to encoded constants
std::map<char, int> char_pieces{ 
	{'P', P}, 
	{'N', N}, 
	{'B', B}, 
	{'R', R}, 
	{'K', K}, 
	{'Q', Q}, 
	{'p', p}, 
	{'n', n}, 
	{'b', b}, 
	{'r', r}, 
	{'k', k}, 
	{'q', q}};

// convert squares to coordinates
std::vector<std::string> square_to_coordinates = {
	"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
	"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
	"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
	"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
	"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
	"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
	"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
	"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
};

/*
		  binary move bits                               hexidecimal constants

	0000 0000 0000 0000 0011 1111    source square       0x3f
	0000 0000 0000 1111 1100 0000    target square       0xfc0
	0000 0000 1111 0000 0000 0000    piece               0xf000
	0000 1111 0000 0000 0000 0000    promoted piece      0xf0000
	0001 0000 0000 0000 0000 0000    capture flag        0x100000
	0010 0000 0000 0000 0000 0000    double push flag    0x200000
	0100 0000 0000 0000 0000 0000    enpassant flag      0x400000
	1000 0000 0000 0000 0000 0000    castling flag       0x800000
*/

// encode move
#define encode_move(source, target, piece, promoted, capture, double, enpassant, castling) \
    (source) |          \
    (target << 6) |     \
    (piece << 12) |     \
    (promoted << 16) |  \
    (capture << 20) |   \
    (double << 21) |    \
    (enpassant << 22) | \
    (castling << 23)    \

// extract source square
#define get_move_source(move) (move & 0x3f)

// extract target square
#define get_move_target(move) ((move & 0xfc0) >> 6)

// extract piece
#define get_move_piece(move) ((move & 0xf000) >> 12)

// extract promoted piece
#define get_move_promoted(move) ((move & 0xf0000) >> 16)

// extract capture flag
#define get_move_capture(move) (move & 0x100000)
// ((move >> 20) & 0b1)

// extract double pawn push flag
#define get_move_double(move) (move & 0x200000)

// extract enpassant flag
#define get_move_enpassant(move) (move & 0x400000)

// extract castling flag
#define get_move_castling(move) (move & 0x800000)

// move types
enum { all_moves, only_captures };

/*
						   castling   move     in      in
							  right update     binary  decimal

 king & rooks didn't move:     1111 & 1111  =  1111    15

		white king  moved:     1111 & 1100  =  1100    12
  white king's rook moved:     1111 & 1110  =  1110    14
 white queen's rook moved:     1111 & 1101  =  1101    13

		 black king moved:     1111 & 0011  =  1011    3
  black king's rook moved:     1111 & 1011  =  1011    11
 black queen's rook moved:     1111 & 0111  =  0111    7

*/

// castling rights update constants
const int castling_rights[64] = {
	 7, 15, 15, 15,  3, 15, 15, 11,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	13, 15, 15, 15, 12, 15, 15, 14
};


//##################################################################################################################
//                                                     BOARD METHODS
//##################################################################################################################

void Board::add_piece(uint32_t piece_val, uint32_t file, uint32_t rank) {
	m_bitboards[piece_val] = set_bit(m_bitboards[piece_val], (7-rank) * 8 + file);
}

std::vector<uint64_t> Board::getOccupationBoard() {
	return m_occupancies;
}

void Board::parse_fen(std::string fen)
{
    // reset board position (bitboards)
    std::fill(m_bitboards.begin(), m_bitboards.end(), 0);

    // reset occupancies (bitboards)
    std::fill(m_occupancies.begin(), m_occupancies.end(), 0);

    // reset game state variables
    m_side = 0;
    m_enpassant = -1;
    m_castle = 0;

	uint16_t index=0;
    // loop over board ranks
    for (int rank = 0; rank < 8; rank++)
    {
		for (int file = 0; file < 8; file++)
		{
			// init current square
			int square = rank * 8 + file;

			// match ascii pieces within FEN string
			if ((fen[index] >= 'a' && fen[index] <= 'z') || (fen[index] >= 'A' && fen[index] <= 'Z'))
			{
				// init piece type
				int piece = char_pieces[fen[index]];

				// set piece on corresponding bitboard
				set_bit(m_bitboards[piece], square);

				// increment index to FEN string
				index++;
			}

			// match empty square numbers within FEN string
			if (fen[index] >= '0' && fen[index] <= '9')
			{
				// init offset (convert char 0 to int 0)
				int offset = fen[index] - '0';

				// define piece variable
				int piece = -1;

				// loop over all piece bitboards
				for (int bb_piece = P; bb_piece <= k; bb_piece++)
				{
					// if there is a piece on current square
					if (get_bit(m_bitboards[bb_piece], square))
						// get piece code
						piece = bb_piece;
				}

				// on empty current square
				if (piece == -1)
					// decrement file
					file--;

				// adjust file counter
				file += offset;

				// increment pointer to FEN string
				index++;
			}
			// match rank separator
			if (fen[index] == '/')
				// increment pointer to FEN string
				index++;
		}
    }

	// got to parsing side to move (increment pointer to FEN string)
	index++;

	// parse side to move
	(fen[index] == 'w') ? (m_side = white) : (m_side = black);

	// go to parsing castling rights
	index += 2;

	// parse castling rights
	while (fen[index] != ' ')
	{
		switch (fen[index])
		{
		case 'K': m_castle |= 1; break;
		case 'Q': m_castle |= 2; break;
		case 'k': m_castle |= 4; break;
		case 'q': m_castle |= 8; break;
		case '-': break;
		}

		// increment pointer to FEN string
		index++;
	}

	index++;

	// parse enpassant square
	if (fen[index] != '-')
	{
		//std::cout << fen[index] << ";" << fen[index+1] << std::endl;
		// parse enpassant file & rank
		int file = fen[index] - 'a';
		int rank = 8 - (fen[index+1] - '0');

		//std::cout << file << ";" << rank << std::endl;

		// init enpassant square
		m_enpassant = rank * 8 + file;
	}
	// no enpassant square
	else
		m_enpassant = -1;

	// loop over white pieces bitboards
	for (int piece = P; piece <= K; piece++)
		// populate white occupancy bitboard
		m_occupancies[white] |= m_bitboards[piece];

	// loop over black pieces bitboards
	for (int piece = p; piece <= k; piece++)
		// populate white occupancy bitboard
		m_occupancies[black] |= m_bitboards[piece];

	// init all occupancies
	m_occupancies[both] |= m_occupancies[white];
	m_occupancies[both] |= m_occupancies[black];

}

void Board::plot() {
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
			int32_t val=-1;
			for (uint32_t k = 0; k < 12; ++k) {
				//std::cout << get_bit(m_bitboards[k], square) << std::endl;
				if (get_bit(m_bitboards[k], square) >= 1) {
					val = k;
					break;
				}
			}
			if (val == -1) {
				printf(" .");
			}
			else{
				printf(" %c", ascii_pieces[val]);
			}
		}
		// print new line every rank
		printf("\n");
	}
	// print board files
	printf("\n     a b c d e f g h\n\n");

	// print side to move
	printf("     Side:     %s\n", !m_side ? "white" : "black");

	// print enpassant square
	std::cout << "     Enpassant:   " << ((m_enpassant != -1) ? square_to_coordinates[m_enpassant] : "no") << std::endl;

	// print castling rights
	printf("     Castling:  %c%c%c%c\n\n", (m_castle & 1) ? 'K' : '-',
		(m_castle & 2) ? 'Q' : '-',
		(m_castle & 4) ? 'k' : '-',
		(m_castle & 8) ? 'q' : '-');
	
}

void Board::copyBoard() {
	m_bs.m_bitboards = m_bitboards;
	m_bs.m_occupancies = m_occupancies;
	m_bs.m_side = m_side;
	m_bs.m_enpassant = m_enpassant;
	m_bs.m_castle = m_castle;
	m_copy_stack.push(m_bs);
}

void Board::takeBack() {
	m_bs = m_copy_stack.top();
	m_copy_stack.pop();
	m_bitboards = m_bs.m_bitboards;
	m_occupancies = m_bs.m_occupancies;
	m_side = m_bs.m_side;
	m_enpassant = m_bs.m_enpassant;
	m_castle = m_bs.m_castle;
}

void Board::clearCopy() {
	m_copy_stack.pop();
}

bool Board::isSquareAttacked(int square, int side) {

	// attacked by white pawns
	if ((side == white) && (m_moves.getPawnAttacks(black,square) & m_bitboards[P])) return 1;

	// attacked by black pawns
	if ((side == black) && (m_moves.getPawnAttacks(white, square) & m_bitboards[p])) return 1;

	// attacked by knights
	if (m_moves.getKnightAttacks(square) & ((side == white) ? m_bitboards[N] : m_bitboards[n])) return 1;

	// attacked by bishops
	if (m_moves.getBishopAttacks(square, m_occupancies[both]) & ((side == white) ? m_bitboards[B] : m_bitboards[b])) return 1;

	// attacked by rooks
	if (m_moves.getRookAttacks(square, m_occupancies[both]) & ((side == white) ? m_bitboards[R] : m_bitboards[r])) return 1;

	// attacked by queens
	if (m_moves.getQueenAttacks(square, m_occupancies[both]) & ((side == white) ? m_bitboards[Q] : m_bitboards[q])) return 1;

	// attacked by kings
	if (m_moves.getKingAttacks(square) & ((side == white) ? m_bitboards[K] : m_bitboards[k])) return 1;

	// by default return false
	return false;
}

//##################################################################################################################
//                                                     MOVES GENERATION
//##################################################################################################################

void Board::generateMoves(std::vector<uint64_t>* move_list)
{
	// loop over all the bitboards
	for (uint8_t piece = P; piece <= k; piece++)
	{
		// init piece bitboard copy
		m_bitboard = m_bitboards[piece];

		// generate white pawns & white king castling moves
		if (m_side == white)
		{
			// Loop over all white pawn
			if (piece == P) {
				while (m_bitboard) {
					// init source square
					m_source_square = get_ls1b_index(m_bitboard);
					//init target square
					m_target_square = m_source_square - 8;

					//Check if m_target_square is inside the board and empty
					if (!(m_target_square < 0) && !get_bit(m_occupancies[both], m_target_square)) {
						//pawn promotion if the pawn is in rank 7
						if (m_source_square >= 8 && m_source_square<=15) {
							// add moves to move_list
							move_list->push_back(encode_move(m_source_square, m_target_square, piece, Q, 0, 0, 0, 0));
							move_list->push_back(encode_move(m_source_square, m_target_square, piece, R, 0, 0, 0, 0));
							move_list->push_back(encode_move(m_source_square, m_target_square, piece, B, 0, 0, 0, 0));
							move_list->push_back(encode_move(m_source_square, m_target_square, piece, N, 0, 0, 0, 0));
						}
						else {
							move_list->push_back(encode_move(m_source_square, m_target_square, piece, 0, 0, 0, 0, 0));
							
							if (m_source_square >= 48 && m_source_square <= 55 && !get_bit(m_occupancies[both], m_target_square - 8)) {
								move_list->push_back(encode_move(m_source_square, m_target_square-8, piece, 0, 0, 1, 0, 0));
							}
						}
					}

					//Get pawn attacks
					m_attacks = m_moves.getPawnAttacks(white, m_source_square) & m_occupancies[black];

					//Capture moves
					while (m_attacks) {
						//Get target square
						m_target_square = get_ls1b_index(m_attacks);

						//pawn promotion if the pawn is in rank 7
						if (m_source_square >= 8 && m_source_square <= 15) {
							// add moves to move_list
							move_list->push_back(encode_move(m_source_square, m_target_square, piece, Q, 1, 0, 0, 0));
							move_list->push_back(encode_move(m_source_square, m_target_square, piece, R, 1, 0, 0, 0));
							move_list->push_back(encode_move(m_source_square, m_target_square, piece, B, 1, 0, 0, 0));
							move_list->push_back(encode_move(m_source_square, m_target_square, piece, N, 1, 0, 0, 0));
						}
						else {
							move_list->push_back(encode_move(m_source_square, m_target_square, piece, 0, 1, 0, 0, 0));
						}

						// pop ls1b index
						pop_bit(m_attacks, m_target_square);
					}

					//Generate enpassant captures
					if (m_enpassant != -1) {
						uint64_t enpassant_attacks = m_moves.getPawnAttacks(white, m_source_square) & (1ULL << m_enpassant);

						if (enpassant_attacks) {
							uint64_t target_enpassant = get_ls1b_index(enpassant_attacks);
							move_list->push_back(encode_move(m_source_square, target_enpassant, piece, 0, 0, 0, 1, 0));
						}
					}

					// pop source square
					pop_bit(m_bitboard, m_source_square);
				}
			}
		
			if (piece == K) {
				// King side caslting is available
				if (m_castle & 1) {
					//Check squares between king and rook
					if (!get_bit(m_occupancies[both], 62) && !get_bit(m_occupancies[both], 61)) {
						//Check if king and f1 square are not attacked
						if (!isSquareAttacked(60, black) && !isSquareAttacked(61, black)) {
							move_list->push_back(encode_move(60, 62, piece, 0, 0, 0, 0, 1));
							//std::cout << "White king side castle" << std::endl;
						}
					}
				}

				// Queen side caslting is available
				if (m_castle & 2) {
					//Check squares between king and rook
					if (!get_bit(m_occupancies[both], 59) && !get_bit(m_occupancies[both], 58) && !get_bit(m_occupancies[both], 57)) {
						//Check if king and d1 square are not attacked
						if (!isSquareAttacked(60, black) && !isSquareAttacked(59, black)) {
							move_list->push_back(encode_move(60, 58, piece, 0, 0, 0, 0, 1));
							//std::cout << "White queen side castle" << std::endl;
						}
					}
				}
			}
		}

		// generate black pawns & black king castling moves
		else
		{
			// Loop over all white pawn
			if (piece == p) {
				while (m_bitboard) {
					// init source square
					m_source_square = get_ls1b_index(m_bitboard);
					//init target square
					m_target_square = m_source_square + 8;

					//Check if m_target_square is inside the board and empty
					if (!(m_target_square > 63) && !get_bit(m_occupancies[both], m_target_square)) {
						//pawn promotion if the pawn is in rank 7
						if (m_source_square >= 48 && m_source_square <= 55) {
							// add moves to move_list
							move_list->push_back(encode_move(m_source_square, m_target_square, piece, q, 0, 0, 0, 0));
							move_list->push_back(encode_move(m_source_square, m_target_square, piece, r, 0, 0, 0, 0));
							move_list->push_back(encode_move(m_source_square, m_target_square, piece, b, 0, 0, 0, 0));
							move_list->push_back(encode_move(m_source_square, m_target_square, piece, n, 0, 0, 0, 0));
						}
						else {
							move_list->push_back(encode_move(m_source_square, m_target_square, piece, 0, 0, 0, 0, 0));

							if (m_source_square >= 8 && m_source_square <= 15 && !get_bit(m_occupancies[both], m_target_square + 8)) {
								move_list->push_back(encode_move(m_source_square, m_target_square+8, piece, 0, 0, 1, 0, 0));
							}
						}
					}

					//Get pawn attacks
					m_attacks = m_moves.getPawnAttacks(black, m_source_square) & m_occupancies[white];

					//Capture moves
					while (m_attacks) {
						//Get target square
						m_target_square = get_ls1b_index(m_attacks);

						//pawn promotion if the pawn is in rank 7
						if (m_source_square >= 48 && m_source_square <= 55) {
							// add moves to move_list
							move_list->push_back(encode_move(m_source_square, m_target_square, piece, q, 1, 0, 0, 0));
							move_list->push_back(encode_move(m_source_square, m_target_square, piece, r, 1, 0, 0, 0));
							move_list->push_back(encode_move(m_source_square, m_target_square, piece, b, 1, 0, 0, 0));
							move_list->push_back(encode_move(m_source_square, m_target_square, piece, n, 1, 0, 0, 0));
						}
						else {
							move_list->push_back(encode_move(m_source_square, m_target_square, piece, 0, 1, 0, 0, 0));
						}

						// pop ls1b index
						pop_bit(m_attacks, m_target_square);
					}

					//Generate enpassant captures
					if (m_enpassant != -1) {
						uint64_t enpassant_attacks = m_moves.getPawnAttacks(black, m_source_square) & (1ULL << m_enpassant);

						if (enpassant_attacks) {
							uint64_t target_enpassant = get_ls1b_index(enpassant_attacks);
							move_list->push_back(encode_move(m_source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
						}
					}

					// pop source square
					pop_bit(m_bitboard, m_source_square);
				}
			}

			if (piece == k) {
				// King side caslting is available
				if (m_castle & 4) {
					//Check squares between king and rook
					if (!get_bit(m_occupancies[both], 5) && !get_bit(m_occupancies[both], 6)) {
						//Check if king and f1 square are not attacked
						if (!isSquareAttacked(4, white) && !isSquareAttacked(5, white)) {
							move_list->push_back(encode_move(4, 6, piece, 0, 0, 0, 0, 1));
							//std::cout << "Black king side castle" << std::endl;
						}
					}
				}

				// Queen side caslting is available
				if (m_castle & 8) {
					//Check squares between king and rook
					if (!get_bit(m_occupancies[both], 1) && !get_bit(m_occupancies[both], 2) && !get_bit(m_occupancies[both], 3)) {
						//Check if king and d1 square are not attacked
						if (!isSquareAttacked(3, white) && !isSquareAttacked(4, white)) {
							move_list->push_back(encode_move(4, 2, piece, 0, 0, 0, 0, 1));
							//std::cout << "Black queen side castle" << std::endl;
						}
					}
				}
			}
		}

		// genarate knight moves
		if ((m_side == white) ? piece == N : piece == n) {
			while (m_bitboard) {
				// init source square
				m_source_square = get_ls1b_index(m_bitboard);

				m_attacks = m_moves.getKnightAttacks(m_source_square) & ((m_side==white) ? ~m_occupancies[white] : ~m_occupancies[black]);

				while (m_attacks) {
					//Get target square
					m_target_square = get_ls1b_index(m_attacks);

					if (get_bit(m_occupancies[!m_side], m_target_square)) {
						move_list->push_back(encode_move(m_source_square, m_target_square, piece, 0, 1, 0, 0, 0));
					}
					else {
						move_list->push_back(encode_move(m_source_square, m_target_square, piece, 0, 0, 0, 0, 0));
					}

					// pop ls1b index
					pop_bit(m_attacks, m_target_square);
				}

				// pop source square
				pop_bit(m_bitboard, m_source_square);
			}
		}

		// generate bishop moves
		if ((m_side == white) ? piece == B : piece == b) {
			while (m_bitboard) {
				// init source square
				m_source_square = get_ls1b_index(m_bitboard);

				//init attacks
				m_attacks = m_moves.getBishopAttacks(m_source_square, m_occupancies[both]) & ((m_side == white) ? ~m_occupancies[white] : ~m_occupancies[black]);

				while (m_attacks) {
					//Get target square
					m_target_square = get_ls1b_index(m_attacks);

					if (get_bit(m_occupancies[!m_side], m_target_square)) {
						move_list->push_back(encode_move(m_source_square, m_target_square, piece, 0, 1, 0, 0, 0));
					}
					else {
						move_list->push_back(encode_move(m_source_square, m_target_square, piece, 0, 0, 0, 0, 0));
					}

					// pop ls1b index
					pop_bit(m_attacks, m_target_square);
				}

				// pop source square
				pop_bit(m_bitboard, m_source_square);
			}
		}

		// generate rook moves
		if ((m_side == white) ? piece == R : piece == r) {
			while (m_bitboard) {
				// init source square
				m_source_square = get_ls1b_index(m_bitboard);

				//init attacks
				m_attacks = m_moves.getRookAttacks(m_source_square, m_occupancies[both]) & ((m_side == white) ? ~m_occupancies[white] : ~m_occupancies[black]);

				while (m_attacks) {
					//Get target square
					m_target_square = get_ls1b_index(m_attacks);

					if (get_bit(m_occupancies[!m_side], m_target_square)) {
						move_list->push_back(encode_move(m_source_square, m_target_square, piece, 0, 1, 0, 0, 0));
					}
					else {
						move_list->push_back(encode_move(m_source_square, m_target_square, piece, 0, 0, 0, 0, 0));
					}

					// pop ls1b index
					pop_bit(m_attacks, m_target_square);
				}

				// pop source square
				pop_bit(m_bitboard, m_source_square);
			}
		}

		// generate queen moves
		if ((m_side == white) ? piece == Q : piece == q) {
			while (m_bitboard) {
				// init source square
				m_source_square = get_ls1b_index(m_bitboard);

				//init attacks
				m_attacks = m_moves.getQueenAttacks(m_source_square, m_occupancies[both]) & ((m_side == white) ? ~m_occupancies[white] : ~m_occupancies[black]);

				while (m_attacks) {
					//Get target square
					m_target_square = get_ls1b_index(m_attacks);

					if (get_bit(m_occupancies[!m_side], m_target_square)) {
						move_list->push_back(encode_move(m_source_square, m_target_square, piece, 0, 1, 0, 0, 0));
					}
					else {
						move_list->push_back(encode_move(m_source_square, m_target_square, piece, 0, 0, 0, 0, 0));
					}

					// pop ls1b index
					pop_bit(m_attacks, m_target_square);
				}

				// pop source square
				pop_bit(m_bitboard, m_source_square);
			}
		}

		// generate king moves
		if ((m_side == white) ? piece == K : piece == k) {
			while (m_bitboard) {
				// init source square
				m_source_square = get_ls1b_index(m_bitboard);

				//init attacks
				m_attacks = m_moves.getKingAttacks(m_source_square) & ((m_side == white) ? ~m_occupancies[white] : ~m_occupancies[black]);

				while (m_attacks) {
					//Get target square
					m_target_square = get_ls1b_index(m_attacks);

					if (get_bit(m_occupancies[!m_side], m_target_square)) {
						move_list->push_back(encode_move(m_source_square, m_target_square, piece, 0, 1, 0, 0, 0));
					}
					else {
						move_list->push_back(encode_move(m_source_square, m_target_square, piece, 0, 0, 0, 0, 0));
					}
					// pop ls1b index
					pop_bit(m_attacks, m_target_square);
				}
				// pop source square
				pop_bit(m_bitboard, m_source_square);
			}
		}
	}
}

int Board::makeMove(int move, int move_flag){
	// quite moves
	if (move_flag == all_moves)
	{
		// preserve board state
		copyBoard();

		m_source_square = get_move_source(move);
		m_target_square = get_move_target(move);
		m_piece = get_move_piece(move);
		m_promoted_piece = get_move_promoted(move);
		m_capture = get_move_capture(move);
		m_double_push = get_move_double(move);
		m_enpass = get_move_enpassant(move);
		m_castling = get_move_castling(move);
		

		// move piece
		pop_bit(m_bitboards[m_piece], m_source_square);
		set_bit(m_bitboards[m_piece], m_target_square);

		//Get capture moves
		if (m_capture) {
			m_start_piece = ((m_side == white) ? p : P);
			m_end_piece = ((m_side == white) ? k : K);

			for (int bb_piece = m_start_piece; bb_piece <= m_end_piece; bb_piece++) {
				// If a piece is found on target square
				if (get_bit(m_bitboards[bb_piece], m_target_square)) {
					pop_bit(m_bitboards[bb_piece], m_target_square);
					break;
				}
			}
		}

		// handle pawn promotions
		if (m_promoted_piece){
			// erase the pawn from the target square
			pop_bit(m_bitboards[(m_side == white) ? P : p], m_target_square);

			// set up promoted piece on chess board
			set_bit(m_bitboards[m_promoted_piece], m_target_square);
		}

		// handle enpassant captures
		if (m_enpass)
		{
			// erase the pawn depending on side to move
			(m_side == white) ? pop_bit(m_bitboards[p], m_target_square + 8) : pop_bit(m_bitboards[P], m_target_square - 8);
		}
		// reset enpassant square
		m_enpassant = -1;

		// handle double pawn push
		if (m_double_push)
		{
			// set enpassant aquare depending on side to move
			(m_side == white) ? (m_enpassant = m_target_square + 8) : (m_enpassant = m_target_square - 8);
		}

		// handle castling moves
		if (m_castling)
		{
			// switch target square
			switch (m_target_square)
			{
				// white castles king side
			case (62):
				// move H rook
				pop_bit(m_bitboards[R], 63);
				set_bit(m_bitboards[R], 61);
				break;

				// white castles queen side
			case (58):
				// move A rook
				pop_bit(m_bitboards[R], 56);
				set_bit(m_bitboards[R], 59);
				break;

				// black castles king side
			case (6):
				// move H rook
				pop_bit(m_bitboards[r], 7);
				set_bit(m_bitboards[r], 5);
				break;

				// black castles queen side
			case (2):
				// move A rook
				pop_bit(m_bitboards[r], 0);
				set_bit(m_bitboards[r], 3);
				break;
			}
		}

		// update castling rights
		m_castle &= castling_rights[m_source_square];
		m_castle &= castling_rights[m_target_square];

		// reset occupancies
		std::fill(m_occupancies.begin(), m_occupancies.end(), 0);

		// loop over white pieces bitboards
		for (int bb_piece = P; bb_piece <= K; bb_piece++)
			// update white occupancies
			m_occupancies[white] |= m_bitboards[bb_piece];

		// loop over black pieces bitboards
		for (int bb_piece = p; bb_piece <= k; bb_piece++)
			// update black occupancies
			m_occupancies[black] |= m_bitboards[bb_piece];

		// update both sides occupancies
		m_occupancies[both] |= m_occupancies[white];
		m_occupancies[both] |= m_occupancies[black];

		// change side
		m_side ^= 1;

		// make sure that king has not been exposed into a check
		if (isSquareAttacked((m_side == white) ? get_ls1b_index(m_bitboards[k]) : get_ls1b_index(m_bitboards[K]), m_side))
		{
			// take move back
			takeBack();
			// return illegal move
			return 0;
		}
		else
			// return legal move
			clearCopy();
			return 1;
	}

	// capture moves
	else
	{
		// make sure move is the capture
		if (get_move_capture(move))
			makeMove(move, all_moves);

		// otherwise the move is not a capture
		else
			// don't make it
			return 0;
	}
}

//##################################################################################################################
//                                                     FUNCTIONS
//##################################################################################################################

// print move list
void print_move_list(std::vector<uint64_t>* move_list)
{
	printf("\n    move    piece   capture   double    enpassant    castling\n\n");

	// loop over moves within a move list
	for (int move_count = 0; move_count < move_list->size(); move_count++)
	{
		// init move
		int move = move_list->at(move_count);

		// print move
		std::cout << "    ";
		std::cout << square_to_coordinates[get_move_source(move)];
		std::cout << square_to_coordinates[get_move_target(move)];
		std::cout << (get_move_promoted(move) ? static_cast<char>(get_move_promoted(move)) : ' ');
		std::cout << "   ";
		std::cout << ascii_pieces[get_move_piece(move)];
		std::cout << "       ";
		std::cout << (get_move_capture(move) ? 1 : 0);
		std::cout << "         ";
		std::cout << (get_move_double(move) ? 1 : 0);
		std::cout << "         ";
		std::cout << (get_move_enpassant(move) ? 1 : 0);
		std::cout << "         ";
		std::cout << (get_move_castling(move) ? 1 : 0);
		std::cout << std::endl;

	}
	// print total number of moves
	std::cout << "\n\n    Total number of moves: " << move_list->size() << "\n\n";
}

//##################################################################################################################
//                                                     TESTS
//##################################################################################################################

void boardTest() {
    Board b;
    b.plot();

    b.parse_fen("r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 w q a3 0 9 ");
    b.plot();

	b.parse_fen(start_position);
	b.plot();
	

	b.parse_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b Kk e6 0 1 ");
	b.plot();

	// sides to move (colors)
	enum { white, black, both };
	// print black occupancies
	printBitboard(b.getOccupationBoard()[black]);
	// print white occupancies
	printBitboard(b.getOccupationBoard()[white]);
	// print all occupancies
	printBitboard(b.getOccupationBoard()[both]);
}

void isAttackedTest() {

	Board b;

	// parse custom FEN string
	b.parse_fen(tricky_position);
	b.plot();

	int side = black;

	printf("\n");
	// loop over board ranks
	for (int rank = 0; rank < 8; rank++)
	{
		// loop over board files
		for (int file = 0; file < 8; file++)
		{
			// init square
			int square = rank * 8 + file;
			// print ranks
			if (!file)
				printf("  %d ", 8 - rank);
			// check whether current square is attacked or not
			printf(" %d", b.isSquareAttacked(square, side) ? 1 : 0);
		}
		// print new line every rank
		printf("\n");
	}

	// print files
	printf("\n     a b c d e f g h\n\n");
}

void MoveGenerationTest() {
	Board b;

	// parse custom FEN string
	b.parse_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ");
	b.plot();

	//std::vector<uint64_t> move_list;
	std::vector<uint64_t>* move_list_ptr = new std::vector<uint64_t>();

	// generate moves
	b.generateMoves(move_list_ptr);

	print_move_list(move_list_ptr);

	uint64_t t0 = get_time_ms();
	// loop over generated moves
	for (int move_count = 0; move_count < move_list_ptr->size(); move_count++)
	{
		// init move
		int move = move_list_ptr->at(move_count);

		// make move
		b.makeMove(move, all_moves);
		//b.plot();

		// take back
		b.takeBack();
		//b.plot();
	}

	uint64_t t1 = get_time_ms();
	std::cout << "Time to run : " << t1 - t0 << " microseconds" << std::endl;

}

void copyTakeBackTest() {
	Board* b_ptr = new Board();
	// parse custom FEN string
	b_ptr->parse_fen(start_position);
	b_ptr->plot();

	for (uint16_t i = 0; i < 5; ++i) {
		b_ptr->copyBoard();
		std::vector<uint64_t>* move_list_ptr = new std::vector<uint64_t>();
		b_ptr->generateMoves(move_list_ptr);
		b_ptr->makeMove(move_list_ptr->at(7), all_moves);
		b_ptr->plot();
		std::cout << b_ptr->stackSize() << "\n";
	}
	for (uint16_t i = 0; i < 5; ++i) {
		b_ptr->takeBack();
		b_ptr->plot();
		std::cout << b_ptr->stackSize() << "\n";
	}
}

void makeMoveTest() {
	Board* b_ptr = new Board();
	// parse custom FEN string
	b_ptr->parse_fen(start_position);
	b_ptr->plot();

	std::vector<uint64_t>* move_list_ptr = new std::vector<uint64_t>();
	// generate moves
	b_ptr->generateMoves(move_list_ptr);

	b_ptr->makeMove(move_list_ptr->at(7), all_moves);

	b_ptr->plot();


}

// leaf nodes (number of positions reached during the test of the move generator at a given depth)
long nodes;

// perft driver
static inline void perftDriver(Board *b, int depth)
{
	// reccursion escape condition
	if (depth == 0)
	{
		// increment nodes count (count reached positions)
		nodes++;
		return;
	}

	std::vector<uint64_t>* move_list_ptr = new std::vector<uint64_t>();

	// generate moves
	b->generateMoves(move_list_ptr);

	for (auto& move: (*move_list_ptr)){
		// preserve board state
		b->copyBoard();

		// make move
		if (!b->makeMove(move, all_moves)) {
			// skip to the next move
			b->clearCopy();
			continue;
		}

		// call perft driver recursively
		perftDriver(b, depth - 1);

		// take back
		b->takeBack();
	}
}

void perftTest(std::string fen_str, int depth)
{
	printf("\n     Performance test\n\n");

	Board *b_ptr = new Board();
	// parse custom FEN string
	b_ptr->parse_fen(fen_str);

	std::vector<uint64_t>* move_list_ptr = new std::vector<uint64_t>();
	// generate moves
	b_ptr->generateMoves(move_list_ptr);
	
	print_move_list(move_list_ptr);

	// init start time
	long start = get_time_ms();

	// loop over generated moves
	for (auto& move : (*move_list_ptr))
	{
		// preserve board state
		b_ptr->copyBoard();

		// make move
		if (!b_ptr->makeMove(move, all_moves)) {
			// skip to the next move
			b_ptr->clearCopy();
			continue;
		}

		// cummulative nodes
		long cummulative_nodes = nodes;

		// call perft driver recursively
		perftDriver(b_ptr, depth - 1);

		// old nodes
		long old_nodes = nodes - cummulative_nodes;

		// take back
		b_ptr->takeBack();
		
		// print move
		std::cout << square_to_coordinates[get_move_source(move)];
		std::cout << square_to_coordinates[get_move_target(move)];
		std::cout << (get_move_promoted(move) ? static_cast<char>(get_move_promoted(move)) : ' ');
		std::cout << ":  ";
		std::cout << old_nodes << std::endl;
	}

	// print results
	printf("\n    Depth: %d\n", depth);
	printf("    Nodes: %ld\n", nodes);
	printf("     Time: %ld\n\n", get_time_ms() - start);

	std::cout << "Stack size : " << b_ptr->stackSize() << std::endl;
}