#include "position.h"

uint64_t StableStones_full_edges_second_order(const uint64_t P, const uint64_t O)
{
    uint64_t StableStones, tmpBitBoard;
    uint64_t BitBoardOccupied = P | O;
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

uint64_t StableStones_triangles(const uint64_t O)
{
    uint64_t StableStones;

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

uint64_t StableStones_skyline(uint64_t O)
{
    int StablesOld, StablesNew;
    uint64_t tmpBitBoard, StableStones;
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
