#pragma once
#include <stdint.h>

uint64_t HasMoves(const uint64_t P, const uint64_t O);
uint64_t PossibleMoves(const uint64_t P, const uint64_t O);

template <const int dir>
inline uint64_t get_some_moves(const uint64_t P, const uint64_t mask)
{
	// kogge-stone parallel prefix
	// 12 x SHIFT, 10 x AND, 7 x OR
	// = 29 OPs
	uint64_t flip_l, flip_r;
	uint64_t mask_l, mask_r;

	flip_l  = mask & (P << dir);
	flip_r  = mask & (P >> dir);

	flip_l |= mask & (flip_l << dir);
	flip_r |= mask & (flip_r >> dir);

	mask_l  = mask & (mask << dir);
	mask_r  = mask & (mask >> dir);

	flip_l |= mask_l & (flip_l << (dir * 2));
	flip_r |= mask_r & (flip_r >> (dir * 2));

	flip_l |= mask_l & (flip_l << (dir * 2));
	flip_r |= mask_r & (flip_r >> (dir * 2));

	return (flip_l << dir) | (flip_r >> dir);
}
