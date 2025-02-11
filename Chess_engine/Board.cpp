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

//##################################################################################################################
//                                                     BOARD METHODS
//##################################################################################################################

void Board::add_piece(uint32_t piece_val, uint32_t file, uint32_t rank) {
	m_bitboards[piece_val] = set_bit(m_bitboards[piece_val], (7-rank) * 8 + file);
}

std::vector<uint64_t> Board::getOccupationBoard() {
	//uint64_t occ = m_bitboards[0];
	//for (uint32_t i = 1; i < m_bitboards.size(); ++i) {
	//	occ |= m_bitboards[i];
	//}
	//return occ;

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

bool Board::isSquareAttacked(int square, int side) {



	// by default return false
	return false;
}

//##################################################################################################################
//                                                     FUNCTIONS
//##################################################################################################################



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