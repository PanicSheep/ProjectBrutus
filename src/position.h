#pragma once
#include "flip.h"
#include "utility.h"
#include <emmintrin.h>
#include <intrin.h>
#include <utility>

//Variations of "PossibleMoves()"
//#define KOGGE_STONE
#define ADVANCED_KOGGE_STONE
//#define ADVANCED_KOGGE_STONE_PARALLEL_PREFIX

static const unsigned long long START_POSITION_P = 0x0000000810000000ULL; // Original
static const unsigned long long START_POSITION_O = 0x0000001008000000ULL; // Original
static const unsigned long long START_POSITION_ETH_P = 0x0000001800000000ULL; // ETH
static const unsigned long long START_POSITION_ETH_O = 0x0000000018000000ULL; // ETH

inline void ResetPosition(unsigned long long & P, unsigned long long & O, const bool ETH)
{
	if (ETH){
		P = START_POSITION_ETH_P;
		O = START_POSITION_ETH_O;
	}
	else {
		P = START_POSITION_P;
		O = START_POSITION_O;
	}
}

inline unsigned long long NumberOfEmptyStones(const unsigned long long P, const unsigned long long O) { return POP_COUNT(~(P | O)); }
inline unsigned char parity(const unsigned long long E)
{
	unsigned long long p = E;
	p ^= p >>  1;
	p ^= p >>  2;
	p ^= p >>  8;
	p ^= p >> 16;
	p &= 0x0000001100000011ULL;
	p |= p >>  3;
	p |= p >> 30;
	return p & 0xFULL;
}
inline unsigned char parity(const unsigned long long P, const unsigned long long O) { return parity(~(P | O)); }
unsigned long long PossibleMoves(const unsigned long long P, const unsigned long long O);
void PossibleMoves(const unsigned long long P, const unsigned long long O, unsigned long long & PossibleMovesP, unsigned long long & PossibleMovesO);
void PlayStone(unsigned long long & P, unsigned long long & O, const unsigned char coordinate);

inline unsigned long long StableStones_corner_and_co(const unsigned long long O)
{
	return (
			((O & 0x0100000000000001ULL) << 1) |
			((O & 0x8000000000000080ULL) >> 1) |
			((O & 0x8100000000000081ULL) << 8) |
			((O & 0x8100000000000081ULL) >> 8) |
				  0x8100000000000081ULL
			) & O;
}
inline unsigned long long StableStones_affectables(const unsigned long long P, const unsigned long long O) { return O & ~AFFECTABLE(~(P | O)); }
unsigned long long StableStones_full_edges_second_order(const unsigned long long P, const unsigned long long O);
unsigned long long StableStones_triangles(const unsigned long long O);
unsigned long long StableStones_skyline(const unsigned long long O);

inline unsigned long long StableStones(const unsigned long long P, const unsigned long long O)
{
	return StableStones_corner_and_co(O) | StableStones_full_edges_second_order(P, O) | StableStones_triangles(O) | StableStones_skyline(O);
}

inline unsigned long long SMEAR_BITBOARD(unsigned long long B)
{
	B |= (B >> 1) & 0x7F7F7F7F7F7F7F7FULL | (B << 1) & 0xFEFEFEFEFEFEFEFEULL;
	return B | (B >> 8) | (B << 8);
}

inline unsigned long long PlayersBoarder(const unsigned long long P, const unsigned long long O) { return SMEAR_BITBOARD(P) & ~(P | O); }
inline unsigned long long OpponentsBoarder(const unsigned long long P, const unsigned long long O) { return SMEAR_BITBOARD(O) & ~(P | O); }
inline unsigned long long Boarders(const unsigned long long P, const unsigned long long O) { return SMEAR_BITBOARD(P | O) & ~(P | O); }

inline unsigned long long PlayersExposed(const unsigned long long P, const unsigned long long O) { return SMEAR_BITBOARD(~(P | O)) & P; }
inline unsigned long long OpponentsExposed(const unsigned long long P, const unsigned long long O) { return SMEAR_BITBOARD(~(P | O)) & O; }
inline unsigned long long Exposeds(const unsigned long long P, const unsigned long long O) { return SMEAR_BITBOARD(~(P | O)) & (P | O); }