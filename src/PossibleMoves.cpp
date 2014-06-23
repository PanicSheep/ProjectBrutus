#include "PossibleMoves.h"

inline unsigned long long get_some_moves(const unsigned long long P, const unsigned long long mask, const int dir)
{
	// kogge-stone parallel prefix
	// 12 x SHIFT, 10 x AND, 7 x OR, 1 x PLUS
	unsigned long long flip_l, flip_r;
	unsigned long long mask_l, mask_r;
	const int dir2 = dir + dir;

	flip_l  = mask & (P << dir);
	flip_r  = mask & (P >> dir);
	
	flip_l |= mask & (flip_l << dir);
	flip_r |= mask & (flip_r >> dir);
	
	mask_l  = mask & (mask << dir);
	mask_r  = mask & (mask >> dir);
	
	flip_l |= mask_l & (flip_l << dir2);
	flip_r |= mask_r & (flip_r >> dir2);
	
	flip_l |= mask_l & (flip_l << dir2);
	flip_r |= mask_r & (flip_r >> dir2);

	return (flip_l << dir) | (flip_r >> dir);
}

bool HasMoves(const unsigned long long P, const unsigned long long O)
{
	const unsigned long long empties = ~(P | O);

	if (get_some_moves(P, O & 0x7E7E7E7E7E7E7E7EULL, 1) & empties) return true;
	if (get_some_moves(P, O & 0x00FFFFFFFFFFFF00ULL, 8) & empties) return true;
	if (get_some_moves(P, O & 0x007E7E7E7E7E7E00ULL, 7) & empties) return true;
	if (get_some_moves(P, O & 0x007E7E7E7E7E7E00ULL, 9) & empties) return true;
	return false;
}

unsigned long long PossibleMoves(const unsigned long long P, const unsigned long long O)
{
	// Advanced Kogge-Stone Parallel Prefix
	// 50 x AND, 48 x SHIFT, 32 x OR, 1 x NOT
	// = 131 OPs
	unsigned long long mask1, mask2, mask3, mask4, mask5, mask6, mask7, mask8;
	unsigned long long flip1, flip2, flip3, flip4, flip5, flip6, flip7, flip8;

	// 1 x AND
	const unsigned long long maskO = O & 0x7E7E7E7E7E7E7E7EULL;

	// 8 x SHIFT, 8 x AND
	flip1 = maskO & (P << 1);
	flip2 = maskO & (P >> 1);
	flip3 =     O & (P << 8);
	flip4 =     O & (P >> 8);
	flip5 = maskO & (P << 7);
	flip6 = maskO & (P >> 7);
	flip7 = maskO & (P << 9);
	flip8 = maskO & (P >> 9);

	// 8 x SHIFT, 8 x AND, 8 x OR
	flip1 |= maskO & (flip1 << 1);
	flip2 |= maskO & (flip2 >> 1);
	flip3 |=     O & (flip3 << 8);
	flip4 |=     O & (flip4 >> 8);
	flip5 |= maskO & (flip5 << 7);
	flip6 |= maskO & (flip6 >> 7);
	flip7 |= maskO & (flip7 << 9);
	flip8 |= maskO & (flip8 >> 9);

	// 8 x SHIFT, 8 x AND
	mask1 = maskO & (maskO << 1);
	mask2 = maskO & (maskO >> 1);
	mask3 =     O & (    O << 8);
	mask4 =     O & (    O >> 8);
	mask5 = maskO & (maskO << 7);
	mask6 = maskO & (maskO >> 7);
	mask7 = maskO & (maskO << 9);
	mask8 = maskO & (maskO >> 9);

	// 8 x SHIFT, 8 x AND, 8 x OR
	flip1 |= mask1 & (flip1 << 2);
	flip2 |= mask2 & (flip2 >> 2);
	flip3 |= mask3 & (flip3 << 16);
	flip4 |= mask4 & (flip4 >> 16);
	flip5 |= mask5 & (flip5 << 14);
	flip6 |= mask6 & (flip6 >> 14);
	flip7 |= mask7 & (flip7 << 18);
	flip8 |= mask8 & (flip8 >> 18);

	// 8 x SHIFT, 8 x AND, 8 x OR
	flip1 |= mask1 & (flip1 << 2);
	flip2 |= mask2 & (flip2 >> 2);
	flip3 |= mask3 & (flip3 << 16);
	flip4 |= mask4 & (flip4 >> 16);
	flip5 |= mask5 & (flip5 << 14);
	flip6 |= mask6 & (flip6 >> 14);
	flip7 |= mask7 & (flip7 << 18);
	flip8 |= mask8 & (flip8 >> 18);

	// 8 x SHIFT, 9 x AND, 8 x OR, 1 x NOT
	return ~(P | O) & (
		(flip1 << 1) |
		(flip2 >> 1) |
		(flip3 << 8) |
		(flip4 >> 8) |
		(flip5 << 7) |
		(flip6 >> 7) |
		(flip7 << 9) |
		(flip8 >> 9)
		);
}