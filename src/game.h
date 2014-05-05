#pragma once
#include "count_last_flip.h"
#include "features.h"
#include "flip.h"
#include "hashtable.h"
#include "line.h"
#include "move.h"
#include "position.h"
#include "search.h"
#include "utility.h"
#include <iostream>
#include <cassert>

class CSearch;

// For benchmarking purposes
int EvaluateEnd(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned long long NumberOfEmptyStones);

int EvaluateEnd(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const unsigned char selectivity);
int EvaluateEnd(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const unsigned char selectivity, CLine & pline);
int EvaluateLimitedDepth(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const signed char depth, const unsigned char selectivity);
int EvaluateLimitedDepth(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const signed char depth, const unsigned char selectivity, CLine & pline);


inline int EvaluateGameOver(const unsigned long long P, const int NumberOfEmptyStones)
{
	// PopCount(P) + PopCount(O) == 64 - NumberOfEmptyStones
	// OwnMinusOpponents == PopCount(P) - PopCount(O)
	//                   == PopCount(P) - ( 64 - NumberOfEmptyStones - PopCount(P) )
	//                   == 2 * PopCount(P) + NumberOfEmptyStones - 64
	int OwnMinusOpponents = (PopCount(P) << 1) + NumberOfEmptyStones - 64;
	if (OwnMinusOpponents > 0)
		return OwnMinusOpponents + NumberOfEmptyStones;
	else if (OwnMinusOpponents < 0)
		return OwnMinusOpponents - NumberOfEmptyStones;
	else
		return OwnMinusOpponents;
}

bool UseHashTableValue(HashTableValueType htValue, int alpha, int beta, signed char depth, unsigned char selectivity, int & value);

//namespace Endgame
//{
//	int ZWS(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, unsigned char selectivity, bool & GotProbCut);
//	int PVS(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, int beta, unsigned char selectivity, unsigned char * pline, bool & GotProbCut);
//
//	int       ZWS_Exact_B(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const unsigned char NumberOfEmptyStones, int alpha);
//	int AlphaBeta_Exact_B(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const unsigned char NumberOfEmptyStones, int alpha, int beta);
//	int       ZWS_Exact_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const int alpha);
//	int AlphaBeta_Exact_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const int alpha, const int beta);
//}

namespace Midgame
{
	//int LimitedDepth_0(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter);
	int ZWS(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, signed char depth, unsigned char selectivity);
	//int PVS(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, int beta, signed char depth, unsigned char selectivity, unsigned char * pline);
}