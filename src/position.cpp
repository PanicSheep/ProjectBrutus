#include "position.h"

inline unsigned long long get_some_moves(const unsigned long long P, const unsigned long long mask, const int dir)
{
    // kogge-stone algorithm
    // 6 << + 6 >> + 12 & + 7 |
    // + better instruction independency
    unsigned long long flip_l, flip_r;
    unsigned long long mask_l, mask_r;
    int d;

    flip_l = flip_r = P;
    mask_l = mask_r = mask;
    d = dir;

    flip_l |= mask_l & (flip_l << d);   flip_r |= mask_r & (flip_r >> d);
    mask_l &= (mask_l << d);            mask_r &= (mask_r >> d);
    d <<= 1;
    flip_l |= mask_l & (flip_l << d);   flip_r |= mask_r & (flip_r >> d);
    mask_l &= (mask_l << d);            mask_r &= (mask_r >> d);
    d <<= 1;
    flip_l |= mask_l & (flip_l << d);   flip_r |= mask_r & (flip_r >> d);

    return ((flip_l & mask) << dir) | ((flip_r & mask) >> dir);
}

unsigned long long PossibleMoves(const unsigned long long P, const unsigned long long O)
{
	#if defined(KOGGE_STONE)
		
    return (  get_some_moves(P, O & 0x7E7E7E7E7E7E7E7EULL, 1)     // horizontal
		    | get_some_moves(P, O & 0x00FFFFFFFFFFFF00ULL, 8)   // vertical
		    | get_some_moves(P, O & 0x007E7E7E7E7E7E00ULL, 7)   // diagonal
            | get_some_moves(P, O & 0x007E7E7E7E7E7E00ULL, 9))  // codiagonal
		    & ~(P|O); // mask with empties

	#elif defined(ADVANCED_KOGGE_STONE)

	unsigned long long mask1, mask2, mask3, mask4, mask5, mask6, mask7, mask8, maskO;
	unsigned long long flip1, flip2, flip3, flip4, flip5, flip6, flip7, flip8;

	maskO = O & 0x7E7E7E7E7E7E7E7EULL;

	flip1 = P | (maskO & (P << 1));
	flip2 = P | (maskO & (P >> 1));
	flip3 = P | (    O & (P << 8));
	flip4 = P | (    O & (P >> 8));
	flip5 = P | (maskO & (P << 7));
	flip6 = P | (maskO & (P >> 7));
	flip7 = P | (maskO & (P << 9));
	flip8 = P | (maskO & (P >> 9));

	mask1 = maskO & (maskO << 1);
	mask2 = maskO & (maskO >> 1);
	mask3 =     O & (    O << 8);
	mask4 =     O & (    O >> 8);
	mask5 = maskO & (maskO << 7);
	mask6 = maskO & (maskO >> 7);
	mask7 = maskO & (maskO << 9);
	mask8 = maskO & (maskO >> 9);

	flip1 |= mask1 & (flip1 << 2);
	flip2 |= mask2 & (flip2 >> 2);
	flip3 |= mask3 & (flip3 << 16);
	flip4 |= mask4 & (flip4 >> 16);
	flip5 |= mask5 & (flip5 << 14);
	flip6 |= mask6 & (flip6 >> 14);
	flip7 |= mask7 & (flip7 << 18);
	flip8 |= mask8 & (flip8 >> 18);

	mask1 &= (mask1 << 2);
	mask2 &= (mask2 >> 2);
	mask3 &= (mask3 << 16);
	mask4 &= (mask4 >> 16);
	mask5 &= (mask5 << 14);
	mask6 &= (mask6 >> 14);
	mask7 &= (mask7 << 18);
	mask8 &= (mask8 >> 18);

	flip1 |= mask1 & (flip1 << 4);
	flip2 |= mask2 & (flip2 >> 4);
	flip3 |= mask3 & (flip3 << 32);
	flip4 |= mask4 & (flip4 >> 32);
	flip5 |= mask5 & (flip5 << 28);
	flip6 |= mask6 & (flip6 >> 28);
	flip7 |= mask7 & (flip7 << 36);
	flip8 |= mask8 & (flip8 >> 36);

	return ~(P | O) & (
						((flip1 & maskO) << 1) | 
						((flip2 & maskO) >> 1) | 
						((flip3 &     O) << 8) | 
						((flip4 &     O) >> 8) | 
						((flip5 & maskO) << 7) | 
						((flip6 & maskO) >> 7) | 
						((flip7 & maskO) << 9) | 
						((flip8 & maskO) >> 9)
						);

	#elif defined(ADVANCED_KOGGE_STONE_PARALLEL_PREFIX)

	unsigned long long mask1, mask2, mask3, mask4, mask5, mask6, mask7, mask8, maskO;
	unsigned long long flip1, flip2, flip3, flip4, flip5, flip6, flip7, flip8;
	
	maskO = O & 0x7E7E7E7E7E7E7E7EULL;

	flip1 = maskO & (P << 1);
	flip2 = maskO & (P >> 1);
	flip3 =     O & (P << 8);
	flip4 =     O & (P >> 8);
	flip5 = maskO & (P << 7);
	flip6 = maskO & (P >> 7);
	flip7 = maskO & (P << 9);
	flip8 = maskO & (P >> 9);

	flip1 |= maskO & (flip1 << 1);
	flip2 |= maskO & (flip2 >> 1);
	flip3 |=     O & (flip3 << 8);
	flip4 |=     O & (flip4 >> 8);
	flip5 |= maskO & (flip5 << 7);
	flip6 |= maskO & (flip6 >> 7);
	flip7 |= maskO & (flip7 << 9);
	flip8 |= maskO & (flip8 >> 9);

	mask1 = maskO & (maskO << 1);
	mask2 = maskO & (maskO >> 1);
	mask3 =     O & (    O << 8);
	mask4 =     O & (    O >> 8);
	mask5 = maskO & (maskO << 7);
	mask6 = maskO & (maskO >> 7);
	mask7 = maskO & (maskO << 9);
	mask8 = maskO & (maskO >> 9);

	flip1 |= mask1 & (flip1 << 2);
	flip2 |= mask2 & (flip2 >> 2);
	flip3 |= mask3 & (flip3 << 16);
	flip4 |= mask4 & (flip4 >> 16);
	flip5 |= mask5 & (flip5 << 14);
	flip6 |= mask6 & (flip6 >> 14);
	flip7 |= mask7 & (flip7 << 18);
	flip8 |= mask8 & (flip8 >> 18);
	
	flip1 |= mask1 & (flip1 << 2);
	flip2 |= mask2 & (flip2 >> 2);
	flip3 |= mask3 & (flip3 << 16);
	flip4 |= mask4 & (flip4 >> 16);
	flip5 |= mask5 & (flip5 << 14);
	flip6 |= mask6 & (flip6 >> 14);
	flip7 |= mask7 & (flip7 << 18);
	flip8 |= mask8 & (flip8 >> 18);

	return ~(P | O) & (
		((flip1 & maskO) << 1) | 
		((flip2 & maskO) >> 1) | 
		((flip3 &     O) << 8) | 
		((flip4 &     O) >> 8) | 
		((flip5 & maskO) << 7) | 
		((flip6 & maskO) >> 7) | 
		((flip7 & maskO) << 9) | 
		((flip8 & maskO) >> 9)
	);
	#endif
}

void PossibleMoves(const unsigned long long P, const unsigned long long O, unsigned long long & PossibleMovesP, unsigned long long & PossibleMovesO)
{
	__m128i mask1, mask2, mask3, mask4, mask5, mask6, mask7, mask8, maskO1, maskO2, maskO3;
    __m128i flip1, flip2, flip3, flip4, flip5, flip6, flip7, flip8;
	__m128i A, B;

	A = _mm_set_epi64x(P, O);
	B = _mm_set_epi64x(O, P);

	maskO1 = mask1 = mask2 = _mm_and_si128(B, _mm_set1_epi64x(0x7E7E7E7E7E7E7E7EULL));
	maskO2 = mask3 = mask4 = _mm_and_si128(B, _mm_set1_epi64x(0x00FFFFFFFFFFFF00ULL));
	maskO3 = mask5 = mask6 = mask7 = mask8 = _mm_and_si128(B, _mm_set1_epi64x(0x007E7E7E7E7E7E00ULL));

	flip1 = _mm_or_si128(A, _mm_and_si128(maskO1, _mm_slli_epi64(A, 1))); // SHIFT, AND, OR
	flip2 = _mm_or_si128(A, _mm_and_si128(maskO1, _mm_srli_epi64(A, 1))); // SHIFT, AND, OR
	flip3 = _mm_or_si128(A, _mm_and_si128(maskO2, _mm_slli_epi64(A, 8))); // SHIFT, AND, OR
	flip4 = _mm_or_si128(A, _mm_and_si128(maskO2, _mm_srli_epi64(A, 8))); // SHIFT, AND, OR
	flip5 = _mm_or_si128(A, _mm_and_si128(maskO3, _mm_slli_epi64(A, 7))); // SHIFT, AND, OR
	flip6 = _mm_or_si128(A, _mm_and_si128(maskO3, _mm_srli_epi64(A, 7))); // SHIFT, AND, OR
	flip7 = _mm_or_si128(A, _mm_and_si128(maskO3, _mm_slli_epi64(A, 9))); // SHIFT, AND, OR
	flip8 = _mm_or_si128(A, _mm_and_si128(maskO3, _mm_srli_epi64(A, 9))); // SHIFT, AND, OR

    mask1 = _mm_and_si128(mask1, _mm_slli_epi64(maskO1, 1)); // SHIFT, AND
    mask2 = _mm_and_si128(mask2, _mm_srli_epi64(maskO1, 1)); // SHIFT, AND
    mask3 = _mm_and_si128(mask3, _mm_slli_epi64(maskO2, 8)); // SHIFT, AND
    mask4 = _mm_and_si128(mask4, _mm_srli_epi64(maskO2, 8)); // SHIFT, AND
    mask5 = _mm_and_si128(mask5, _mm_slli_epi64(maskO3, 7)); // SHIFT, AND
    mask6 = _mm_and_si128(mask6, _mm_srli_epi64(maskO3, 7)); // SHIFT, AND
    mask7 = _mm_and_si128(mask7, _mm_slli_epi64(maskO3, 9)); // SHIFT, AND
    mask8 = _mm_and_si128(mask8, _mm_srli_epi64(maskO3, 9)); // SHIFT, AND

	flip1 = _mm_or_si128(flip1, _mm_and_si128(mask1, _mm_slli_epi64(flip1, 2)));  // SHIFT, AND, OR
	flip2 = _mm_or_si128(flip2, _mm_and_si128(mask2, _mm_srli_epi64(flip2, 2)));  // SHIFT, AND, OR
	flip3 = _mm_or_si128(flip3, _mm_and_si128(mask3, _mm_slli_epi64(flip3, 16))); // SHIFT, AND, OR
	flip4 = _mm_or_si128(flip4, _mm_and_si128(mask4, _mm_srli_epi64(flip4, 16))); // SHIFT, AND, OR
	flip5 = _mm_or_si128(flip5, _mm_and_si128(mask5, _mm_slli_epi64(flip5, 14))); // SHIFT, AND, OR
	flip6 = _mm_or_si128(flip6, _mm_and_si128(mask6, _mm_srli_epi64(flip6, 14))); // SHIFT, AND, OR
	flip7 = _mm_or_si128(flip7, _mm_and_si128(mask7, _mm_slli_epi64(flip7, 18))); // SHIFT, AND, OR
	flip8 = _mm_or_si128(flip8, _mm_and_si128(mask8, _mm_srli_epi64(flip8, 18))); // SHIFT, AND, OR

    mask1 = _mm_and_si128(mask1, _mm_slli_epi64(mask1, 2));	 // SHIFT, AND
    mask2 = _mm_and_si128(mask2, _mm_srli_epi64(mask2, 2));	 // SHIFT, AND
    mask3 = _mm_and_si128(mask3, _mm_slli_epi64(mask3, 16)); // SHIFT, AND
    mask4 = _mm_and_si128(mask4, _mm_srli_epi64(mask4, 16)); // SHIFT, AND
    mask5 = _mm_and_si128(mask5, _mm_slli_epi64(mask5, 14)); // SHIFT, AND
    mask6 = _mm_and_si128(mask6, _mm_srli_epi64(mask6, 14)); // SHIFT, AND
    mask7 = _mm_and_si128(mask7, _mm_slli_epi64(mask7, 18)); // SHIFT, AND
    mask8 = _mm_and_si128(mask8, _mm_srli_epi64(mask8, 18)); // SHIFT, AND

	flip1 = _mm_or_si128(flip1, _mm_and_si128(mask1, _mm_slli_epi64(flip1, 4)));  // SHIFT, AND, OR
	flip2 = _mm_or_si128(flip2, _mm_and_si128(mask2, _mm_srli_epi64(flip2, 4)));  // SHIFT, AND, OR
	flip3 = _mm_or_si128(flip3, _mm_and_si128(mask3, _mm_slli_epi64(flip3, 32))); // SHIFT, AND, OR
	flip4 = _mm_or_si128(flip4, _mm_and_si128(mask4, _mm_srli_epi64(flip4, 32))); // SHIFT, AND, OR
	flip5 = _mm_or_si128(flip5, _mm_and_si128(mask5, _mm_slli_epi64(flip5, 28))); // SHIFT, AND, OR
	flip6 = _mm_or_si128(flip6, _mm_and_si128(mask6, _mm_srli_epi64(flip6, 28))); // SHIFT, AND, OR
	flip7 = _mm_or_si128(flip7, _mm_and_si128(mask7, _mm_slli_epi64(flip7, 36))); // SHIFT, AND, OR
	flip8 = _mm_or_si128(flip8, _mm_and_si128(mask8, _mm_srli_epi64(flip8, 36))); // SHIFT, AND, OR

	//ANDNOT, 8xOR, 8xSHIFT, 8xAND
	mask1 = _mm_andnot_si128(
					_mm_or_si128(A, B), //~BitBoardEmpty
					_mm_or_si128(
						_mm_or_si128(
							_mm_or_si128(
								_mm_slli_epi64(_mm_and_si128(flip1, maskO1), 1),
								_mm_srli_epi64(_mm_and_si128(flip2, maskO1), 1)
							),
							_mm_or_si128(
								_mm_slli_epi64(_mm_and_si128(flip3, maskO2), 8),
								_mm_srli_epi64(_mm_and_si128(flip4, maskO2), 8)
							)
						),
						_mm_or_si128(
							_mm_or_si128(
								_mm_slli_epi64(_mm_and_si128(flip5, maskO3), 7),
								_mm_srli_epi64(_mm_and_si128(flip6, maskO3), 7)
							),
							_mm_or_si128(
								_mm_slli_epi64(_mm_and_si128(flip7, maskO3), 9),
								_mm_srli_epi64(_mm_and_si128(flip8, maskO3), 9)
							)
						)
					)
				);
	PossibleMovesP = ((unsigned long long *)&mask1)[1];
	PossibleMovesO = ((unsigned long long *)&mask1)[0];
}

void PlayStone(unsigned long long & P, unsigned long long & O, const unsigned char coordinate)
{
	unsigned long long flipped;

    flipped = flip(P, O, coordinate);
	P ^= (1ULL << coordinate) ^ flipped;
    O ^= flipped;
	std::swap(P, O);
}

unsigned long long StableStones_full_edges_second_order(const unsigned long long P, const unsigned long long O)
{
    unsigned long long StableStones, tmpBitBoard;
    unsigned long long BitBoardOccupied = P | O;
    StableStones = 0;

    if ((BitBoardOccupied & 0x00000000000000FFULL) == 0x00000000000000FFULL)
    {
        StableStones |= O & 0x00000000000000FFULL;
        if ((BitBoardOccupied & 0x000000000000FFFFULL) == 0x000000000000FFFFULL)
        {
            tmpBitBoard = O & 0x00000000000000FFULL;
            StableStones |= O & (((tmpBitBoard << 1) & tmpBitBoard & (tmpBitBoard >> 1)) << 8);
        }
    }
    if ((BitBoardOccupied & 0x0101010101010101ULL) == 0x0101010101010101ULL)
    {
        StableStones |= O & 0x0101010101010101ULL;
        if ((BitBoardOccupied & 0x0303030303030303ULL) == 0x0303030303030303ULL)
        {
            tmpBitBoard = O & 0x0101010101010101ULL;
            StableStones |= O & (((tmpBitBoard << 8) & tmpBitBoard & (tmpBitBoard >> 8)) << 1);
        }
    }
    if ((BitBoardOccupied & 0xFF00000000000000ULL) == 0xFF00000000000000ULL)
    {
        StableStones |= O & 0xFF00000000000000ULL;
        if ((BitBoardOccupied & 0xFFFF000000000000ULL) == 0xFFFF000000000000ULL)
        {
            tmpBitBoard = O & 0xFF00000000000000ULL;
            StableStones |= O & (((tmpBitBoard << 1) & tmpBitBoard & (tmpBitBoard >> 1)) >> 8);
        }
    }
    if ((BitBoardOccupied & 0x8080808080808080ULL) == 0x8080808080808080ULL)
    {
        StableStones |= O & 0x8080808080808080ULL;
        if ((BitBoardOccupied & 0xC0C0C0C0C0C0C0C0ULL) == 0xC0C0C0C0C0C0C0C0ULL)
        {
            tmpBitBoard = O & 0x8080808080808080ULL;
            StableStones |= O & (((tmpBitBoard << 8) & tmpBitBoard & (tmpBitBoard >> 8)) >> 1);
        }
    }

    return StableStones;
}

unsigned long long StableStones_triangles(const unsigned long long O)
{
    unsigned long long StableStones;

    if ((O & 0x0000000000000001ULL) == 0x0000000000000001ULL) // Triangle of Size 1
    {
        if ((O & 0x0000000000000103ULL) == 0x0000000000000103ULL) // Triangle of Size 2
        {
            if ((O & 0x0000000000010307ULL) == 0x0000000000010307ULL) // Triangle of Size 3
            {
                if ((O & 0x000000000103070FULL) == 0x000000000103070FULL) // Triangle of Size 4
                {
                    if ((O & 0x0000000103070F1FULL) == 0x0000000103070F1FULL) // Triangle of Size 5
                    {
                        if ((O & 0x00000103070F1F3FULL) == 0x00000103070F1F3FULL) // Triangle of Size 6
                        {
                            if ((O & 0x000103070F1F3F7FULL) == 0x000103070F1F3F7FULL) // Triangle of Size 7
                            {
                                if ((O & 0x0103070F1F3F7FFFULL) == 0x0103070F1F3F7FFFULL) // Triangle of Size 8
                                    StableStones = 0x0103070F1F3F7FFFULL; // BTW: You have more then half of the board
                                else
                                    StableStones = 0x000103070F1F3F7FULL;
                            }
                            else
                                StableStones = 0x00000103070F1F3FULL;
                        }
                        else
                            StableStones = 0x0000000103070F1FULL;
                    }
                    else
                        StableStones = 0x000000000103070FULL;
                }
                else
                    StableStones = 0x0000000000010307ULL;
            }
            else
                StableStones = 0x0000000000000103ULL;
        }
        else
            StableStones = 0x0000000000000001ULL;
    }
    else
        StableStones = 0x0000000000000000ULL;

    if ((O & 0x0000000000000080ULL) == 0x0000000000000080ULL) // Triangle of Size 1
    {
        if ((O & 0x00000000000080C0ULL) == 0x00000000000080C0ULL) // Triangle of Size 2
        {
            if ((O & 0x000000000080C0E0ULL) == 0x000000000080C0E0ULL) // Triangle of Size 3
            {
                if ((O & 0x0000000080C0E0F0ULL) == 0x0000000080C0E0F0ULL) // Triangle of Size 4
                {
                    if ((O & 0x00000080C0E0F0F8ULL) == 0x00000080C0E0F0F8ULL) // Triangle of Size 5
                    {
                        if ((O & 0x000080C0E0F0F8FCULL) == 0x000080C0E0F0F8FCULL) // Triangle of Size 6
                        {
                            if ((O & 0x0080C0E0F0F8FCFEULL) == 0x0080C0E0F0F8FCFEULL) // Triangle of Size 7
                            {
                                if ((O & 0x80C0E0F0F8FCFEFFULL) == 0x80C0E0F0F8FCFEFFULL) // Triangle of Size 8
                                    StableStones |= 0x80C0E0F0F8FCFEFFULL; // BTW: You have more then half of the board
                                else
                                    StableStones |= 0x0080C0E0F0F8FCFEULL;
                            }
                            else
                                StableStones |= 0x000080C0E0F0F8FCULL;
                        }
                        else
                            StableStones |= 0x00000080C0E0F0F8ULL;
                    }
                    else
                        StableStones |= 0x0000000080C0E0F0ULL;
                }
                else
                    StableStones |= 0x000000000080C0E0ULL;
            }
            else
                StableStones |= 0x00000000000080C0ULL;
        }
        else
            StableStones |= 0x0000000000000080ULL;
    }

    if ((O & 0x8000000000000000ULL) == 0x8000000000000000ULL) // Triangle of Size 1
    {
        if ((O & 0xC080000000000000ULL) == 0xC080000000000000ULL) // Triangle of Size 2
        {
            if ((O & 0xE0C0800000000000ULL) == 0xE0C0800000000000ULL) // Triangle of Size 3
            {
                if ((O & 0xF0E0C08000000000ULL) == 0xF0E0C08000000000ULL) // Triangle of Size 4
                {
                    if ((O & 0xF8F0E0C080000000ULL) == 0xF8F0E0C080000000ULL) // Triangle of Size 5
                    {
                        if ((O & 0xFCF8F0E0C0800000ULL) == 0xFCF8F0E0C0800000ULL) // Triangle of Size 6
                        {
                            if ((O & 0xFEFCF8F0E0C08000ULL) == 0xFEFCF8F0E0C08000ULL) // Triangle of Size 7
                            {
                                if ((O & 0xFFFEFCF8F0E0C080ULL) == 0xFFFEFCF8F0E0C080ULL) // Triangle of Size 8
                                    StableStones |= 0xFFFEFCF8F0E0C080ULL; // BTW: You have more then half of the board
                                else
                                    StableStones |= 0xFEFCF8F0E0C08000ULL;
                            }
                            else
                                StableStones |= 0xFCF8F0E0C0800000ULL;
                        }
                        else
                            StableStones |= 0xF8F0E0C080000000ULL;
                    }
                    else
                        StableStones |= 0xF0E0C08000000000ULL;
                }
                else
                    StableStones |= 0xE0C0800000000000ULL;
            }
            else
                StableStones |= 0xC080000000000000ULL;
        }
        else
            StableStones |= 0x8000000000000000ULL;
    }

    if ((O & 0x0100000000000000ULL) == 0x0100000000000000ULL) // Triangle of Size 1
    {
        if ((O & 0x0301000000000000ULL) == 0x0301000000000000ULL) // Triangle of Size 2
        {
            if ((O & 0x0703010000000000ULL) == 0x0703010000000000ULL) // Triangle of Size 3
            {
                if ((O & 0x0F07030100000000ULL) == 0x0F07030100000000ULL) // Triangle of Size 4
                {
                    if ((O & 0x1F0F070301000000ULL) == 0x1F0F070301000000ULL) // Triangle of Size 5
                    {
                        if ((O & 0x3F1F0F0703010000ULL) == 0x3F1F0F0703010000ULL) // Triangle of Size 6
                        {
                            if ((O & 0x7F3F1F0F07030100ULL) == 0x7F3F1F0F07030100ULL) // Triangle of Size 7
                            {
                                if ((O & 0xFF7F3F1F0F070301ULL) == 0xFF7F3F1F0F070301ULL) // Triangle of Size 8
                                    StableStones |= 0xFF7F3F1F0F070301ULL; // BTW: You have more then half of the board
                                else
                                    StableStones |= 0x7F3F1F0F07030100ULL;
                            }
                            else
                                StableStones |= 0x3F1F0F0703010000ULL;
                        }
                        else
                            StableStones |= 0x1F0F070301000000ULL;
                    }
                    else
                        StableStones |= 0x0F07030100000000ULL;
                }
                else
                    StableStones |= 0x0703010000000000ULL;
            }
            else
                StableStones |= 0x0301000000000000ULL;
        }
        else
            StableStones |= 0x0100000000000000ULL;
    }

    return StableStones;
}

unsigned long long StableStones_skyline(const unsigned long long O)
{
    int StablesOld, StablesNew;
    unsigned long long tmpBitBoard, StableStones;
	StableStones = 0x0000000000000000ULL;
		
	for (int directions = 0; directions < 8; ++directions)
	{
		StablesOld = BIT_SCAN_LS1B(~(O & 0x00000000000000FFULL));
		StableStones |= 0x00000000000000FFULL >> 8-StablesOld;
		for (int counter = 0; (StablesOld > 1) && (counter < 64); counter += 8)
		{
			StablesNew = BIT_SCAN_LS1B(~((O >> counter) & 0x00000000000000FFULL));
			if ((StablesOld != 8) || (StablesNew != 8))
				StablesOld = MIN(StablesOld-1, StablesNew);
			StableStones |= (0x00000000000000FFULL >> 8-StablesOld) << counter;
		}

		switch (directions)
		{
		case 0:
		case 2:
		case 5:
			vertical_flip(O);
			vertical_flip(StableStones);
			break;
		case 1:
		case 4:
		case 6:
			horizontal_flip(O);
			horizontal_flip(StableStones);
			break;
		case 3:
			diagonal_flip(O);
			diagonal_flip(StableStones);
			break;
		}
	}

    diagonal_flip(StableStones); //Fliping back

    return StableStones;
}