#pragma once
#include "flip.h"
#include "PossibleMoves.h"
#include "utility.h"
#include <utility>

const uint64_t START_POSITION_P = 0x0000000810000000ULL;
const uint64_t START_POSITION_O = 0x0000001008000000ULL;
const uint64_t START_POSITION_ETH_P = 0x0000001800000000ULL;
const uint64_t START_POSITION_ETH_O = 0x0000000018000000ULL;

inline void ResetPosition(uint64_t & P, uint64_t & O, const bool ETH)
{
	if (ETH) { P = START_POSITION_ETH_P; O = START_POSITION_ETH_O; }
	else { P = START_POSITION_P; O = START_POSITION_O; }
}

inline uint64_t NumberOfEmptyStones(const uint64_t P, const uint64_t O) { return PopCount(~(P | O)); }
inline uint64_t parity(uint64_t E)
{
	// 6x SHIFT, 4x XOR, 2x AND, 2x OR <==> 14 OPs
	E ^= E >>  1;
	E ^= E >>  2;
	E ^= E >>  8;
	E ^= E >> 16;
	E &= 0x0000001100000011ULL;
	E |= E >>  3;
	E |= E >> 30;
	return E & 0xFULL;
}
inline uint64_t parity(const uint64_t P, const uint64_t O) { return parity(~(P | O)); }

inline void PlayStone(uint64_t & P, uint64_t & O, const int move)
{
	uint64_t flipped = flip(P, O, move);
	P ^= flipped ^ (1ULL << move);
	O ^= flipped;
	std::swap(P, O);
}

inline uint64_t StableStones_corner_and_co(const uint64_t O)
{
	// 5x AND, 4x SHIFT, 4x OR <==> 13 OPs
	return (
			((O & 0x0100000000000001ULL) << 1) |
			((O & 0x8000000000000080ULL) >> 1) |
			((O & 0x8100000000000081ULL) << 8) |
			((O & 0x8100000000000081ULL) >> 8) |
				  0x8100000000000081ULL
			) & O;
}
inline uint64_t StableStones_affectables(const uint64_t P, const uint64_t O) { return O & ~AFFECTABLE(~(P | O)); }
uint64_t StableStones_full_edges_second_order(const uint64_t P, const uint64_t O);
uint64_t StableStones_triangles(const uint64_t O);
uint64_t StableStones_skyline(const uint64_t O);

inline uint64_t StableStones(const uint64_t P, const uint64_t O)
{
	return StableStones_corner_and_co(O) | StableStones_full_edges_second_order(P, O) | StableStones_triangles(O) | StableStones_skyline(O);
}

inline uint64_t SMEAR_BITBOARD(uint64_t B)
{
	// 4x SHIFT, 4x OR, 2x AND <==> 10 OPs
	B |= (B >> 1) & 0x7F7F7F7F7F7F7F7FULL | (B << 1) & 0xFEFEFEFEFEFEFEFEULL;
	return B | (B >> 8) | (B << 8);
}

inline uint64_t PlayersBoarder(const uint64_t P, const uint64_t O) { return SMEAR_BITBOARD(P) & ~(P | O); } // 13 OPs
inline uint64_t OpponentsBoarder(const uint64_t P, const uint64_t O) { return SMEAR_BITBOARD(O) & ~(P | O); } // 13 OPs
inline uint64_t Boarders(const uint64_t P, const uint64_t O) { return SMEAR_BITBOARD(P | O) & ~(P | O); } // 14 OPs

inline uint64_t PlayersExposed(const uint64_t P, const uint64_t O) { return SMEAR_BITBOARD(~(P | O)) & P; } // 13 OPs
inline uint64_t OpponentsExposed(const uint64_t P, const uint64_t O) { return SMEAR_BITBOARD(~(P | O)) & O; } // 13 OPs
inline uint64_t Exposeds(const uint64_t P, const uint64_t O) { return SMEAR_BITBOARD(~(P | O)) & (P | O); } // 14 OPs
