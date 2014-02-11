#pragma once
#include "count_last_flip.h"
#include "features.h"
#include "flip.h"
#include "midgame.h"
#include "move.h"
#include "position.h"
#include "search.h"
#include "utility.h"
#include <iostream>

class CSearch;

// For benchmarking purposes
int EvaluateExact(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned long long NumberOfEmptyStones);

void EvaluateExact(CSearch & search);
void EvaluateEndCut(CSearch & search);
void EvaluateLimitedDepth(CSearch & search);
void EvaluateMPC(CSearch & search);

inline int EvaluateGameOver(const unsigned long long P, const int NumberOfEmptyStones)
{
	// POP_COUNT(P) + POP_COUNT(O) == 64 - NumberOfEmptyStones
	// OwnMinusOpponents == POP_COUNT(P) - POP_COUNT(O)
	//                   == POP_COUNT(P) - ( 64 - NumberOfEmptyStones - POP_COUNT(P) )
	//                   == 2 * POP_COUNT(P) + NumberOfEmptyStones - 64
	int OwnMinusOpponents = (POP_COUNT(P) << 1) + NumberOfEmptyStones - 64;
	if (OwnMinusOpponents > 0)
		return OwnMinusOpponents + NumberOfEmptyStones;
	else if (OwnMinusOpponents < 0)
		return OwnMinusOpponents - NumberOfEmptyStones;
	else
		return OwnMinusOpponents;
	//return OwnMinusOpponents + NumberOfEmptyStones * SIGN_OR_ZERO(OwnMinusOpponents);
}