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

struct CCutOffLimits
{
	unsigned char D, d;
	float a, b, sigma;
	bool InUse;
	CCutOffLimits(const unsigned char D, const unsigned char d, const float b, const float a, const float sigma, const bool InUse) : D(D), d(d), b(b), a(a), sigma(sigma), InUse(InUse) {}
};

struct CProbCuts
{
	unsigned char depth;
	float a, b, sigma;
	CProbCuts(const unsigned char depth, const float a, const float b, const float sigma) : depth(depth), a(a), b(b), sigma(sigma) {}
};

namespace Endgame
{
	extern std::vector<std::pair<int, int>> MPC_aviable;
	extern std::vector<CCutOffLimits> MPC_table;
	extern float mu;

	void Change_MPC_table(CCutOffLimits * pair);
	void Change_MPC_table(int D, int d);
	void Change_MPC_table(int D, int d, bool InUse);
	int NegaMax(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter);
//	int ZWS(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, unsigned char selectivity, bool & GotProbCut);
//	int PVS(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, int beta, unsigned char selectivity, unsigned char * pline, bool & GotProbCut);
//
//	int       ZWS_Exact_B(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const unsigned char NumberOfEmptyStones, int alpha);
//	int AlphaBeta_Exact_B(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const unsigned char NumberOfEmptyStones, int alpha, int beta);
//	int       ZWS_Exact_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const int alpha);
//	int AlphaBeta_Exact_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const int alpha, const int beta);
}

namespace Midgame
{
	extern std::vector<std::pair<int, int>> MPC_aviable; // MPC_aviable(Depth, depth)
	extern bool MPC_actives[64][64]; // MPC_actives[Depth][depth]
	extern std::vector<CProbCuts> MPC_table[64][64]; // MPC_table[empties][Depth]
	void Initialize();
	float sigma(const int D, const int d, const int E);
	inline void Change_MPC_actives(const int D, const int d) { MPC_actives[D][d] = !MPC_actives[D][d]; }
	inline void Change_MPC_actives(const int D, const int d, const bool InUse) { MPC_actives[D][d] = InUse; }

	int ZWS_0(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha);
	int ZWS_1(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha);
	int ZWS_2(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha);
	//int LimitedDepth_0(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter);
	int ZWS(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, signed char depth, unsigned char selectivity);
	//int PVS(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, int beta, signed char depth, unsigned char selectivity, unsigned char * pline);
}