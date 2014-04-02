#include "game.h"

namespace Midgame
{
	struct CCutOffLimits
	{
		unsigned char d, D;
		float a, b, sigma;
		CCutOffLimits(const unsigned char d, const unsigned char D, const float b, const float a, const float sigma) : d(d), D(D), b(b), a(a), sigma(sigma) {}
	};

	static const CCutOffLimits MPC_table[] = {
		CCutOffLimits( 2,  4,  0.644710f, 0.978329f, 3.00432f),
		
		CCutOffLimits( 3,  5, -0.174416f, 0.983432f, 2.77297f),

		CCutOffLimits( 2,  6,  0.592878f, 0.984816f, 3.80884f),
		CCutOffLimits( 4,  6, -0.050528f, 1.005940f, 2.47849f),

		CCutOffLimits( 3,  7, -0.227744f, 0.973714f, 3.59683f),
		CCutOffLimits( 5,  7, -0.054585f, 0.990010f, 2.34988f),
		
		CCutOffLimits( 2,  8,  0.919680f, 0.974616f, 4.20224f),
		CCutOffLimits( 4,  8,  0.286388f, 0.995101f, 3.15356f),
		CCutOffLimits( 6,  8,  0.341149f, 0.988632f, 2.14631f),
		
		CCutOffLimits( 3,  9, -0.367928f, 0.985361f, 4.01665f),
		CCutOffLimits( 5,  9, -0.189117f, 1.001030f, 3.08096f),
		//CCutOffLimits( 7,  9, -0.130016f, 1.010210f, 2.21575f),
		
		CCutOffLimits( 2, 10,  1.063850f, 0.967414f, 4.67232f),
		CCutOffLimits( 4, 10,  0.422665f, 0.989296f, 3.53552f),
		CCutOffLimits( 6, 10,  0.479754f, 0.982539f, 2.75579f),
		//CCutOffLimits( 8, 10,  0.146744f, 0.993115f, 1.96493f),

		//CCutOffLimits( 3, 11, -0.078589f, 0.982243f, 4.55955f),
		CCutOffLimits( 5, 11,  0.092611f, 0.999483f, 3.51014f),
		CCutOffLimits( 7, 11,  0.151370f, 1.008710f, 2.77208f),
		//CCutOffLimits( 9, 11,  0.285217f, 0.997546f, 1.98103f),

		//CCutOffLimits( 2, 12,  0.636112f, 0.980799f, 4.97909f),
		CCutOffLimits( 4, 12, -0.005951f, 1.002000f, 4.03935f),
		CCutOffLimits( 6, 12,  0.041593f, 0.996420f, 3.13466f),
		CCutOffLimits( 8, 12, -0.293352f, 1.006820f, 2.52315f),
		//CCutOffLimits(10, 12, -0.435151f, 1.012970f, 1.82846f),

		//CCutOffLimits( 2,  4, 0, 0, 3.1f),
		//CCutOffLimits( 3,  5, 0, 0, 2.8f),
		//CCutOffLimits( 2,  6, 0, 0, 3.9f),
		//CCutOffLimits( 3,  7, 0, 0, 3.6f),
		//CCutOffLimits( 4,  8, 0, 0, 3.2f),
		//CCutOffLimits( 6,  8, 0, 0, 2.2f),
	};

	bool StabilityCutoff(const unsigned long long P, const unsigned long long O, const int NumberOfEmptyStones, const int alpha)
	{
		static const char stability_cutoff_limits[64] = {
			-99, -99, -99, -99, -99, -15, -16, -15,
			-16, -17, -18, -17, -18, -19, -20, -19,
			-20, -99, -99, -99, -99, -99, -99, -99,
			-99, -99, -99, -99, -99, -99, -99, -99,
			-99, -99, -99, -99, -99, -99, -99, -99,
			-99, -99, -99, -99, -99, -99, -99, -99,
			-99, -99, -99, -99, -99, -99, -99, -99,
			-99, -99, -99, -99, -99, -99, -99, -99
		};
		int OwnMinusOpponents = (POP_COUNT(P) << 1) + NumberOfEmptyStones - 64;
		if (OwnMinusOpponents <= stability_cutoff_limits[NumberOfEmptyStones]) //Worth checking stability
			return (64 - static_cast<int>(POP_COUNT(StableStones(P, O)) << 1) <= alpha);
		return false;
	}

	inline int LimitedDepth_0(const CActiveConfigurations & actives, const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta)
	{
		++NodeCounter;
		return BIND(actives.EvaluateFeatures(P, O), alpha, beta);
	}
	inline int LimitedDepth_0(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta)
	{
		++NodeCounter;
		return BIND(EvaluateFeatures(P, O), alpha, beta);
	}
	int LimitedDepth_1(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta)
	{
		int value = -128;
		unsigned char Move;
		unsigned long long flipped;
		unsigned long long BitBoardPossible = PossibleMoves(P, O);

		++NodeCounter;

		if (!BitBoardPossible)
		{
			BitBoardPossible = PossibleMoves(O, P);
			if (BitBoardPossible)
				return -LimitedDepth_1(O, P, NodeCounter, -beta, -alpha);
			else{ //Game is over
				++NodeCounter; 
				return BIND(EvaluateGameOver(P, NumberOfEmptyStones(P, O)), alpha, beta);
			}
		}

		CActiveConfigurations actives(P, O);

		while (BitBoardPossible)
		{
			Move = BIT_SCAN_LS1B(BitBoardPossible);
			REMOVE_LS1B(BitBoardPossible);
			flipped = flip(P, O, Move);
			value = -LimitedDepth_0(actives, O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, -beta, -alpha);
			if (value >= beta) return beta;
			if (value > alpha) alpha = value;
		}

		return alpha;
	}
	int LimitedDepth_2(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta)
	{
		int value = -128;
		unsigned char Move;
		unsigned long long flipped;
		unsigned long long BitBoardPossible = PossibleMoves(P, O);

		++NodeCounter;

		if (!BitBoardPossible)
		{
			BitBoardPossible = PossibleMoves(O, P);
			if (BitBoardPossible)
				return -LimitedDepth_2(O, P, NodeCounter, -beta, -alpha);
			else{ //Game is over
				++NodeCounter; 
				return BIND(EvaluateGameOver(P, NumberOfEmptyStones(P, O)), alpha, beta);
			}
		}

		while (BitBoardPossible)
		{
			Move = BIT_SCAN_LS1B(BitBoardPossible);
			REMOVE_LS1B(BitBoardPossible);
			flipped = flip(P, O, Move);
			value = -LimitedDepth_1(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, -beta, -alpha);
			if (value >= beta) return beta;
			if (value > alpha) alpha = value;
		}
		return alpha;
	}

	bool MPC(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, signed char depth, unsigned char selectivity, int & value)
	{
		int bound;

		if (selectivity == 0)
			return false;

		//for (int i = 0; i < 6; i++)
		//{
		//	if (depth == MPC_table[i].D)
		//	{
		//		bound = static_cast<int>(static_cast<float>(alpha) + SelectivityTable[selectivity].T * MPC_table[i].sigma + 0.5f);
		//		if (ZWS(search, P, O, bound-1, MPC_table[i].d, 0) >= bound){
		//			value = alpha+1;
		//			return true;
		//		}

		//		bound = static_cast<int>(static_cast<float>(alpha) - SelectivityTable[selectivity].T * MPC_table[i].sigma + 0.5f);
		//		if (ZWS(search, P, O, bound, MPC_table[i].d, 0) <= bound){
		//			value = alpha;
		//			return true;
		//		}
		//	}
		//}
		for (int i = 0; i < 19; i++)
		{
			if (depth == MPC_table[i].D)
			{
				bound = static_cast<int>((static_cast<float>(alpha+1) + SelectivityTable[selectivity].T * MPC_table[i].sigma -  MPC_table[i].b) /  MPC_table[i].a + 0.5f);
				if (ZWS(search, P, O, bound-1, MPC_table[i].d, selectivity) >= bound){
					value = alpha+1;
					return true;
				}

				bound = static_cast<int>((static_cast<float>(alpha) - SelectivityTable[selectivity].T * MPC_table[i].sigma -  MPC_table[i].b) /  MPC_table[i].a + 0.5f);
				if (ZWS(search, P, O, bound, MPC_table[i].d, selectivity) <= bound){
					value = alpha;
					return true;
				}
			}
		}
		return false;
	}

	int ZWS(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, signed char depth, unsigned char selectivity)
	{
		if (depth == 2)
			return LimitedDepth_2(P, O, search.NodeCounter, alpha, alpha+1);

		const unsigned char Empties = NumberOfEmptyStones(P, O);
		int value;
		unsigned long long flipped;
		unsigned long long BitBoardPossible = PossibleMoves(P, O);

		++search.NodeCounter;

		if (!BitBoardPossible)
		{
			BitBoardPossible = PossibleMoves(O, P);
			if (BitBoardPossible)
				return -ZWS(search, O, P, -alpha-1, depth, selectivity);
			else{ //Game is over
				++search.NodeCounter;
				return BIND(EvaluateGameOver(P, Empties), alpha, alpha+1);
			}
		}

		if (StabilityCutoff(P, O, Empties, alpha))
			return alpha;

		if (MPC(search, P, O, alpha, depth, selectivity, value))
			return value;
	
		unsigned long long LocalNodeCounter = search.NodeCounter;

		HashTableValueType htValue;
		if (search.HashTableLookUp(P, O, htValue))
		{
			if (UseHashTableValue(htValue, alpha, alpha+1, depth, selectivity, value))
				return value;

			if (htValue.PV != 64)
			{
				assert((BitBoardPossible & (1ULL << htValue.PV)) != 0);
				BitBoardPossible ^= (1ULL << htValue.PV);
				flipped = flip(P, O, htValue.PV);
				if (-ZWS(search, O ^ flipped, P ^ (1ULL << htValue.PV) ^ flipped, -alpha-1, depth-1, selectivity) > alpha)
				{
					search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, alpha+1, 64, htValue.PV, htValue.AV);
					return alpha+1;
				}
			}

			if (htValue.AV != 64)
			{
				assert((BitBoardPossible & (1ULL << htValue.AV)) != 0);
				BitBoardPossible ^= (1ULL << htValue.AV);
				flipped = flip(P, O, htValue.AV);
				if (-ZWS(search, O ^ flipped, P ^ (1ULL << htValue.AV) ^ flipped, -alpha-1, depth-1, selectivity) > alpha)
				{
					search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, alpha+1, 64, htValue.AV, htValue.PV);
					return alpha+1;
				}
			}
		}

		CMoveList mvList(P, O, BitBoardPossible);
		for (auto& mv : mvList)
		{
			if (-ZWS(search, mv.P, mv.O, -alpha-1, depth-1, selectivity) > alpha)
			{
				search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, alpha+1, 64, mv.move, 64);
				return alpha+1;
			}
		}
		search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, -64, alpha, 64, 64);
		return alpha;
	}

	int PVS(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, int beta, signed char depth, unsigned char selectivity, unsigned char * pline)
	{
		if (depth == 2)
			return LimitedDepth_2(P, O, search.NodeCounter, alpha, beta);

		const unsigned char Empties = NumberOfEmptyStones(P, O);

		bool SearchPV = true;
		int value;
		unsigned long long flipped;
		unsigned long long BitBoardPossible = PossibleMoves(P, O);

		++search.NodeCounter;

		if (!BitBoardPossible)
		{
			BitBoardPossible = PossibleMoves(O, P);
			if (BitBoardPossible)
				return -PVS(search, O, P, -beta, -alpha, depth, selectivity, pline);
			else{ //Game is over
				++search.NodeCounter;
				return BIND(EvaluateGameOver(P, Empties), alpha, beta);
			}
		}

		if (StabilityCutoff(P, O, Empties, alpha))
			return alpha;
	
		unsigned long long LocalNodeCounter = search.NodeCounter;
		unsigned char line[60];
		memset(line, static_cast<unsigned char>(64), 60 * sizeof(unsigned char));

		HashTableValueType htValue;
		if (search.HashTableLookUp(P, O, htValue))
		{
			if (UseHashTableValue(htValue, alpha, beta, depth, selectivity, value))
				return value;

			if (htValue.PV != 64)
			{
				assert((BitBoardPossible & (1ULL << htValue.PV)) != 0);
				BitBoardPossible ^= (1ULL << htValue.PV);
				flipped = flip(P, O, htValue.PV);
				value = -PVS(search, O ^ flipped, P ^ (1ULL << htValue.PV) ^ flipped, -beta, -alpha, depth-1, selectivity, line);
				if (value >= beta)
				{
					search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, beta, 64, htValue.PV, htValue.AV);
					return beta;
				}
				if (value > alpha)
				{
					pline[0] = htValue.PV;
					memcpy(pline+1, line, Empties * sizeof(unsigned char) - 1);
					alpha = value;
					SearchPV = false;
				}
			}

			if (htValue.AV != 64)
			{
				assert((BitBoardPossible & (1ULL << htValue.AV)) != 0);
				BitBoardPossible ^= (1ULL << htValue.AV);
				flipped = flip(P, O, htValue.AV);
				value = -PVS(search, O ^ flipped, P ^ (1ULL << htValue.AV) ^ flipped, -beta, -alpha, depth-1, selectivity, line);
				if (value >= beta)
				{
					search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, beta, 64, htValue.AV, htValue.PV);
					return beta;
				}
				if (value > alpha)
				{
					pline[0] = htValue.AV;
					memcpy(pline+1, line, Empties * sizeof(unsigned char) - 1);
					alpha = value;
					SearchPV = false;
				}
			}
		}

		CMoveList mvList(P, O, BitBoardPossible);
		for (auto& mv : mvList)
		{
			if (SearchPV)
				value = -PVS(search, mv.P, mv.O, -beta, -alpha, depth-1, selectivity, line);
			else
			{
				value = -ZWS(search, mv.P, mv.O, -alpha-1, depth-1, selectivity);
				if (value > alpha)
					value = -PVS(search, mv.P, mv.O, -beta, -alpha, depth-1, selectivity, line);
			}
			if (value >= beta)
			{
				search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, beta, 64, mv.move, 64);
				return beta;
			}
			if (value > alpha)
			{
				pline[0] = mv.move;
				memcpy(pline+1, line, Empties * sizeof(unsigned char) - 1);
				alpha = value;
				SearchPV = false;
			}
		}
		if (SearchPV)
			search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, -64, alpha, 64, 64);
		else
			search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, alpha, alpha, pline[0], 64);
		return alpha;
	}
}

void EvaluateLimitedDepth(CSearch & search)
{
	switch (search.depth)
	{
	case 0: search.score = Midgame::LimitedDepth_0(search.P, search.O, search.NodeCounter, search.alpha, search.beta); break;
	case 1: search.score = Midgame::LimitedDepth_1(search.P, search.O, search.NodeCounter, search.alpha, search.beta); break;
	case 2: search.score = Midgame::LimitedDepth_2(search.P, search.O, search.NodeCounter, search.alpha, search.beta); break;
	case 3:
	case 4:
	case 5:
		search.score = Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, search.depth, search.selectivity, search.PV); break;
	case 6:
	case 7:
		Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, 5, 6, search.PV);
		search.score = Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, search.depth, search.selectivity, search.PV);
		break;
	case 8:
		Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, 5, 4, search.PV);
		Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, 6, 6, search.PV);
		search.score = Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, search.depth, search.selectivity, search.PV);
		break;
	case 9:
		Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, 5, 4, search.PV);
		Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, 7, 6, search.PV);
		search.score = Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, search.depth, search.selectivity, search.PV);
		break;
	case 10:
		Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, 5, 2, search.PV);
		Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, 6, 4, search.PV);
		Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, 8, 6, search.PV);
		search.score = Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, search.depth, search.selectivity, search.PV);
		break;
	case 11:
		Midgame::PVS(search, search.P, search.O, search.alpha, search.beta,  5, 2, search.PV);
		Midgame::PVS(search, search.P, search.O, search.alpha, search.beta,  7, 4, search.PV);
		Midgame::PVS(search, search.P, search.O, search.alpha, search.beta,  8, 6, search.PV);
		Midgame::PVS(search, search.P, search.O, search.alpha, search.beta,  9, 6, search.PV);
		search.score = Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, search.depth, search.selectivity, search.PV);
		break;
	default:
		if (search.depth % 2 == 0)
		{
			for (int d = 4; d <= search.depth; d+=2)
				Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, d, 6, search.PV);
		}
		else
		{
			for (int d = 3; d <= search.depth; d+=2)
				Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, d, 6, search.PV);
		}
		for (int s = 6; s > search.selectivity; s-=2)
			Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, search.depth, s, search.PV);
		search.score = Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, search.depth, search.selectivity, search.PV);
	}
}