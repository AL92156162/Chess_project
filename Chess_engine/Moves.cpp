#include "Moves.hpp"
#include "Board.hpp"

// set/get/pop bit macros
#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define pop_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))

//==================================================================================================================
//Variables
//==================================================================================================================

// board squares
enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1
};

// not A file constant
const uint64_t not_a_file = 18374403900871474942ULL;

// not H file constant
const uint64_t not_h_file = 9187201950435737471ULL;

// not HG file constant
const uint64_t not_hg_file = 4557430888798830399ULL;

// not AB file constant
const uint64_t not_ab_file = 18229723555195321596ULL;

//==================================================================================================================
//MOVES METHODS
//==================================================================================================================

// init leaper pieces attacks
void Moves::initLeapersAttacks()
{
    //loop over 64 board squares
    for (int square = 0; square < 64; square++)
    {
        // init pawn attacks
        m_pawn_attacks[0][square] = maskPawnAttacks(0, square);
        m_pawn_attacks[1][square] = maskPawnAttacks(1, square);
        // init knight attacks
        m_knight_attacks[square] = maskKnightAttacks(square);
        // init king attacks
        m_king_attacks[square] = maskKingAttacks(square);
    }
}

// init slider piece's attack tables
void Moves::initSlidersAttacks(int bishop)
{
     //loop over 64 board squares
    for (int square = 0; square < 64; square++)
    {
        // init bishop & rook masks
        m_bishop_masks[square] = maskBishopAttacks(square);
        m_rook_masks[square] = maskRookAttacks(square);
        // init current mask
        uint64_t attack_mask = bishop ? m_bishop_masks[square] : m_rook_masks[square];
        // init relevant occupancy bit count
        uint8_t relevant_bits_count = count_bits(attack_mask);
        // init occupancy indicies
        int occupancy_indicies = (1 << relevant_bits_count);
        // loop over occupancy indicies
        for (int index = 0; index < occupancy_indicies; index++)
        {
            // bishop
            if (bishop)
            {
                // init current occupancy variation
                uint64_t occupancy = set_occupancy(index, relevant_bits_count, attack_mask);
                // init magic index
                int magic_index = (occupancy * m_bishop_magic_numbers[square]) >> (64 - m_bishop_relevant_bits[square]);
                // init bishop attacks
                m_bishop_attacks_ptr[square * 512 + magic_index] = bishopAttacksOnTheFly(square, occupancy);
            }
            // rook
            else
            {
                // init current occupancy variation
                uint64_t occupancy = set_occupancy(index, relevant_bits_count, attack_mask);
                // init magic index
                int magic_index = (occupancy * m_rook_magic_numbers[square]) >> (64 - m_rook_relevant_bits[square]);
                // init bishop attacks
                m_rook_attacks_ptr[square * 4096 + magic_index] = rookAttacksOnTheFly(square, occupancy);
            }
        }
    }
}

 //init magic numbers
void Moves::initMagicNumbers()
{
    // loop over 64 board squares
    for (int square = 0; square < 64; square++)
        // init rook magic numbers
        m_rook_magic_numbers[square] = findMagicNumber(square, m_rook_relevant_bits[square], 0);

    // loop over 64 board squares
    for (int square = 0; square < 64; square++)
        // init bishop magic numbers
        m_bishop_magic_numbers[square] = findMagicNumber(square, m_bishop_relevant_bits[square], 1);
}
 
// init all variables
void Moves::initAll()
{
    // init leaper pieces attacks
    initLeapersAttacks();

    // init slider pieces attacks
    initSlidersAttacks(0);
    initSlidersAttacks(1);

    // init magic numbers
    //init_magic_numbers();
}

uint64_t Moves::getPawnAttacks(int side, uint8_t square) {
    return m_pawn_attacks[side][square];
}

uint64_t Moves::getKnightAttacks(uint8_t square) {
    return m_knight_attacks[square];
}

uint64_t Moves::getKingAttacks(uint8_t square) {
    return m_king_attacks[square];
}

uint64_t Moves::getBishopAttacks(uint8_t square, uint64_t occupancy)
{
    // get bishop attacks assuming current board occupancy
    occupancy &= m_bishop_masks[square];
    occupancy *= m_bishop_magic_numbers[square];
    occupancy >>= 64 - m_bishop_relevant_bits[square];

    // return bishop attacks
    return m_bishop_attacks_ptr[square * 512 + occupancy];
}

uint64_t Moves::getRookAttacks(uint8_t square, uint64_t occupancy)
{
    // get bishop attacks assuming current board occupancy
    occupancy &= m_rook_masks[square];
    occupancy *= m_rook_magic_numbers[square];
    occupancy >>= 64 - m_rook_relevant_bits[square];

    // return rook attacks
    return m_rook_attacks_ptr[square * 4096 + occupancy];
}

uint64_t Moves::getQueenAttacks(uint8_t square, uint64_t occupancy) {
    uint64_t queen_attacks = 0ULL;

    queen_attacks |= getBishopAttacks(square, occupancy);
    queen_attacks |= getRookAttacks(square, occupancy);

    return queen_attacks;
}

//==================================================================================================================
//Functions
//==================================================================================================================

uint64_t maskPawnAttacks(bool side, uint8_t square) {
	uint64_t attacks = 0ULL;
	uint64_t bitboard = 0ULL;

    // set piece on board
    bitboard = set_bit(bitboard, square);

    // white pawns
    if (!side)
    {
        // generate pawn attacks
        if ((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
        if ((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);
    }

    // black pawns
    else
    {
        // generate pawn attacks
        if ((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
        if ((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);
    }
    
    return attacks;
}

uint64_t maskKnightAttacks(uint8_t square){
    // result attacks bitboard
    uint64_t attacks = 0ULL;

    // piece bitboard
    uint64_t bitboard = 0ULL;

    // set piece on board
    bitboard = set_bit(bitboard, square);

    // generate knight attacks
    if ((bitboard >> 17) & not_h_file) attacks |= (bitboard >> 17);
    if ((bitboard >> 15) & not_a_file) attacks |= (bitboard >> 15);
    if ((bitboard >> 10) & not_hg_file) attacks |= (bitboard >> 10);
    if ((bitboard >> 6) & not_ab_file) attacks |= (bitboard >> 6);
    if ((bitboard << 17) & not_a_file) attacks |= (bitboard << 17);
    if ((bitboard << 15) & not_h_file) attacks |= (bitboard << 15);
    if ((bitboard << 10) & not_ab_file) attacks |= (bitboard << 10);
    if ((bitboard << 6) & not_hg_file) attacks |= (bitboard << 6);

    // return attack map
    return attacks;
}

uint64_t maskKingAttacks(uint8_t square){
    // result attacks bitboard
    uint64_t attacks = 0ULL;

    // piece bitboard
    uint64_t bitboard = 0ULL;

    // set piece on board
    bitboard = set_bit(bitboard, square);

    // generate king attacks
    if (bitboard >> 8) attacks |= (bitboard >> 8);
    if ((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);
    if ((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
    if ((bitboard >> 1) & not_h_file) attacks |= (bitboard >> 1);
    if (bitboard << 8) attacks |= (bitboard << 8);
    if ((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);
    if ((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
    if ((bitboard << 1) & not_a_file) attacks |= (bitboard << 1);

    // return attack map
    return attacks;
}

uint64_t maskBishopAttacks(uint8_t square) {
    // result attacks bitboard
    uint64_t attacks = 0ULL;

    // init ranks & files
    int r, f;

    // init target rank & files
    int tr = square / 8;
    int tf = square % 8;

    // mask relevant bishop occupancy bits
    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++) attacks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++) attacks |= (1ULL << (r * 8 + f));
    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--) attacks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--) attacks |= (1ULL << (r * 8 + f));

    // return attack map
    return attacks;
}

uint64_t maskRookAttacks(uint8_t square) {
    // result attacks bitboard
    uint64_t attacks = 0ULL;

    // init ranks & files
    int r, f;

    // init target rank & files
    int tr = square / 8;
    int tf = square % 8;

    // mask relevant rook occupancy bits
    for (r = tr + 1; r <= 6; r++) attacks |= (1ULL << (r * 8 + tf));
    for (r = tr - 1; r >= 1; r--) attacks |= (1ULL << (r * 8 + tf));
    for (f = tf + 1; f <= 6; f++) attacks |= (1ULL << (tr * 8 + f));
    for (f = tf - 1; f >= 1; f--) attacks |= (1ULL << (tr * 8 + f));

    // return attack map
    return attacks;
}

// generate rook attacks on the fly
uint64_t rookAttacksOnTheFly(uint8_t square, uint64_t block)
{
    // result attacks bitboard
    uint64_t attacks = 0ULL;

    // init ranks & files
    int r, f;

    // init target rank & files
    int tr = square / 8;
    int tf = square % 8;

    // generate rook attacks
    for (r = tr + 1; r <= 7; r++)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & block) break;
    }

    for (r = tr - 1; r >= 0; r--)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & block) break;
    }

    for (f = tf + 1; f <= 7; f++)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & block) break;
    }

    for (f = tf - 1; f >= 0; f--)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & block) break;
    }

    // return attack map
    return attacks;
}

// generate bishop attacks on the fly
uint64_t bishopAttacksOnTheFly(uint8_t square, uint64_t block)
{
    // result attacks bitboard
    uint64_t attacks = 0ULL;

    // init ranks & files
    int r, f;

    // init target rank & files
    int tr = square / 8;
    int tf = square % 8;

    // generate bishop atacks
    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }

    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }

    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }

    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }

    // return attack map
    return attacks;
}

// set occupancies
uint64_t set_occupancy(int index, int bits_in_mask, uint64_t attack_mask)
{
    // occupancy map
    uint64_t occupancy = 0ULL;

    // loop over the range of bits within attack mask
    for (int count = 0; count < bits_in_mask; count++)
    {
        // get LS1B index of attacks mask
        int square = get_ls1b_index(attack_mask);

        // pop LS1B in attack map
        pop_bit(attack_mask, square);

        // make sure occupancy is on board
        if (index & (1 << count))
            // populate occupancy map
            occupancy |= (1ULL << square);
    }

    // return occupancy map
    return occupancy;
}

// find appropriate magic number
uint64_t findMagicNumber(int square, int relevant_bits, int bishop)
{
    // init occupancies
    uint64_t occupancies[4096];

    // init attack tables
    uint64_t attacks[4096];

    // init used attacks
    uint64_t used_attacks[4096];

    // init attack mask for a current piece
    uint64_t attack_mask = bishop ? maskBishopAttacks(square) : maskRookAttacks(square);

    // init occupancy indicies
    int occupancy_indicies = 1 << relevant_bits;

    // loop over occupancy indicies
    for (int index = 0; index < occupancy_indicies; index++)
    {
        // init occupancies
        occupancies[index] = set_occupancy(index, relevant_bits, attack_mask);

        // init attacks
        attacks[index] = bishop ? bishopAttacksOnTheFly(square, occupancies[index]) :
            rookAttacksOnTheFly(square, occupancies[index]);
    }

    // test magic numbers loop
    for (int random_count = 0; random_count < 100000000; random_count++)
    {
        // generate magic number candidate
        uint64_t magic_number = random_uint64_fewbits();

        // skip inappropriate magic numbers
        if (count_bits((attack_mask * magic_number) & 0xFF00000000000000) < 6) continue;

        // init used attacks
        memset(used_attacks, 0ULL, sizeof(used_attacks));

        // init index & fail flag
        int index, fail;

        // test magic index loop
        for (index = 0, fail = 0; !fail && index < occupancy_indicies; index++)
        {
            // init magic index
            int magic_index = (int)((occupancies[index] * magic_number) >> (64 - relevant_bits));

            // if magic index works
            if (used_attacks[magic_index] == 0ULL)
                // init used attacks
                used_attacks[magic_index] = attacks[index];

            // otherwise
            else if (used_attacks[magic_index] != attacks[index])
                // magic index doesn't work
                fail = 1;
        }

        // if magic number works
        if (!fail)
            // return it
            return magic_number;
    }

    // if magic number doesn't work
    printf("  Magic number fails!\n");
    return 0ULL;
}

//==================================================================================================================
//Initialization functions
//==================================================================================================================

//==================================================================================================================
//Test functions
//==================================================================================================================

void MagicNumbersTest() {
    //init_magic_numbers();

    //std::cout << "ROOK MAGIC NUMBERS" << std::endl;
    //for (int i = 0; i < 64; ++i) {
    //    std::cout << rook_magic_numbers[i] << std::endl;
    //}

    //std::cout << "BISHOP MAGIC NUMBERS" << std::endl;
    //for (int i = 0; i < 64; ++i) {
    //    std::cout << std::hex << bishop_magic_numbers[i] << std::endl;
    //}
}

void maskTest() {
    uint64_t pawn_attack = maskPawnAttacks(false, 4 * 8 + 4);
    std::cout << pawn_attack << std::endl;
    printBitboard(pawn_attack);

    uint64_t knight_attack = maskKnightAttacks(4 * 8 + 4);
    std::cout << knight_attack << std::endl;
    printBitboard(knight_attack);

    uint64_t bishop_attack = maskBishopAttacks(4 * 8 + 4);
    std::cout << bishop_attack << std::endl;
    printBitboard(bishop_attack);

    uint64_t rook_attack = maskRookAttacks(4 * 8 + 4);
    std::cout << rook_attack << std::endl;
    printBitboard(rook_attack);

    uint64_t king_attack = maskKingAttacks(4 * 8 + 4);
    std::cout << king_attack << std::endl;
    printBitboard(king_attack);
}

void moveTest() {
    //initAll();

    // define test bitboard
    uint64_t occupancy = 0ULL;

    // set blocker pieces on board
    set_bit(occupancy, c5);
    set_bit(occupancy, f2);
    set_bit(occupancy, g7);
    set_bit(occupancy, b2);
    set_bit(occupancy, g5);
    set_bit(occupancy, e2);
    set_bit(occupancy, e7);

    // print occupancies
    printBitboard(occupancy);

    Moves m;
    m.initAll();

    printBitboard(m.getRookAttacks(e5, occupancy));

    printBitboard(m.getBishopAttacks(d4, occupancy));

    printBitboard(m.getQueenAttacks(e5, occupancy));

}