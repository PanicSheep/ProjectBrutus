#include "position.h"

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
	unsigned long long flipped = flip(P, O, coordinate);
	P ^= flipped ^ (1ULL << coordinate);
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

unsigned long long StableStones_skyline(unsigned long long O)
{
    int StablesOld, StablesNew;
    unsigned long long tmpBitBoard, StableStones;
	StableStones = 0x0000000000000000ULL;
		
	for (int directions = 0; directions < 8; ++directions)
	{
		StablesOld = BitScanLSB(~(O & 0x00000000000000FFULL));
		StableStones |= 0x00000000000000FFULL >> 8-StablesOld;
		for (int counter = 0; (StablesOld > 1) && (counter < 64); counter += 8)
		{
			StablesNew = BitScanLSB(~((O >> counter) & 0x00000000000000FFULL));
			if ((StablesOld != 8) || (StablesNew != 8))
				StablesOld = MIN(StablesOld-1, StablesNew);
			StableStones |= (0x00000000000000FFULL >> 8-StablesOld) << counter;
		}

		switch (directions)
		{
		case 0:
		case 2:
		case 5:
			O = vertical_flip(O);
			StableStones = vertical_flip(StableStones);
			break;
		case 1:
		case 4:
		case 6:
			O = horizontal_flip(O);
			StableStones = horizontal_flip(StableStones);
			break;
		case 3:
			O = diagonal_flip(O);
			StableStones = diagonal_flip(StableStones);
			break;
		}
	}

    StableStones = diagonal_flip(StableStones); //Fliping back

    return StableStones;
}