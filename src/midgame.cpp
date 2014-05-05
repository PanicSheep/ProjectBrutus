#include "game.h"

namespace Midgame
{
	struct CCutOffLimits
	{
		unsigned char d, D;
		float a, b, sigma;
		CCutOffLimits(const unsigned char d, const unsigned char D, const float b, const float a, const float sigma) : d(d), D(D), b(b), a(a), sigma(sigma) {}
	};

	static const int MPC_SIZE = 15;
	static const CCutOffLimits MPC_table[] = {
		CCutOffLimits( 2,  4,  0.384638f, 1.005910f, 3.10699f), // -2
		
		CCutOffLimits( 3,  5, -0.174416f, 0.983432f, 2.77297f), // -2

		CCutOffLimits( 2,  6,  0.710707f, 1.013420f, 3.80884f), // -4
		//CCutOffLimits( 4,  6,  0.321672f, 1.007570f, 2.56583f), // -2

		CCutOffLimits( 3,  7, -0.227744f, 0.973714f, 3.59683f), // -4
		//CCutOffLimits( 5,  7, -0.054585f, 0.990010f, 2.34988f), // -2
		
		CCutOffLimits( 2,  8,  0.997632f, 1.022080f, 4.60164f), // -6
		//CCutOffLimits( 4,  8,  0.605700f, 1.016160f, 3.33542f), // -4
		//CCutOffLimits( 6,  8,  0.283868f, 1.008360f, 2.14989f), // -2
		
		CCutOffLimits( 3,  9, -0.367928f, 0.985361f, 4.01665f), // -6
		//CCutOffLimits( 5,  9, -0.189117f, 1.001030f, 3.08096f), // -4
		//CCutOffLimits( 7,  9, -0.130016f, 1.010210f, 2.21575f), // -2
		
		CCutOffLimits( 2, 10,  1.206200f, 1.029760f, 5.05224f), // -8
		//CCutOffLimits( 4, 10,  0.797745f, 1.024670f, 3.78016f), // -6
		//CCutOffLimits( 6, 10,  0.475549f, 1.016660f, 2.80235f), // -4
		//CCutOffLimits( 8, 10,  0.194185f, 1.007930f, 1.87515f), // -2

		CCutOffLimits( 3, 11, -0.078589f, 0.982243f, 4.55955f), // -8
		//CCutOffLimits( 5, 11,  0.092611f, 0.999483f, 3.51014f), // -6
		//CCutOffLimits( 7, 11,  0.151370f, 1.008710f, 2.77208f), // -4
		//CCutOffLimits( 9, 11,  0.285217f, 0.997546f, 1.98103f), // -2

		//CCutOffLimits( 2, 12,  1.400410f, 1.035040f, 5.29786f), // -10
		CCutOffLimits( 4, 12,  1.000520f, 1.029230f, 4.18712f), // -8
		CCutOffLimits( 6, 12,  0.664393f, 1.021980f, 3.17664f), // -6
		//CCutOffLimits( 8, 12,  0.383466f, 1.013090f, 2.41907f), // -4
		//CCutOffLimits(10, 12,  0.194492f, 1.004740f, 1.66203f), // -2

		//CCutOffLimits( 2, 14,  1.535560f, 1.040310f, 5.44206f), // -12
		CCutOffLimits( 4, 14,  1.142750f, 1.033880f, 4.43553f), // -10
		CCutOffLimits( 6, 14,  0.811107f, 1.026210f, 3.55276f),	// -8
		//CCutOffLimits( 8, 14,  0.519522f, 1.017870f, 2.76261f),	// -6
		//CCutOffLimits(10, 14,  0.330791f, 1.009410f, 2.14435f),	// -4
		//CCutOffLimits(12, 14,  0.141703f, 1.004270f, 1.50971f),	// -2

		//CCutOffLimits( 2, 16,  1.684990f, 1.044910f, 5.61747f), // -14
		CCutOffLimits( 4, 16,  1.288540f, 1.038570f, 4.62381f), // -12
		CCutOffLimits( 6, 16,  0.960219f, 1.030560f, 3.82528f), // -10
		CCutOffLimits( 8, 16,  0.670845f, 1.021970f, 3.14029f), // -8
		//CCutOffLimits(10, 16,  0.474165f, 1.013920f, 2.50246f), // -6
		//CCutOffLimits(12, 16,  0.284867f, 1.008720f, 1.99247f), // -4
		//CCutOffLimits(14, 16,  0.149735f, 1.004000f, 1.48505f), // -2
	};

	bool MPC(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, signed char depth, unsigned char selectivity, int & value)
	{
		int bound;

		if (selectivity == 0)
			return false;

		for (int i = 0; i < MPC_SIZE; i++)
		{
			if (depth == MPC_table[i].D)
			{
				bound = static_cast<int>((static_cast<float>(alpha + 1) + SelectivityTable[selectivity].T * MPC_table[i].sigma - MPC_table[i].b) / MPC_table[i].a + 0.5f);
				if (ZWS(search, P, O, bound-1, MPC_table[i].d, selectivity) >= bound){
					value = alpha+1;
					return true;
				}

				bound = static_cast<int>((static_cast<float>(alpha) - SelectivityTable[selectivity].T * MPC_table[i].sigma - MPC_table[i].b) / MPC_table[i].a + 0.5f);
				if (ZWS(search, P, O, bound, MPC_table[i].d, selectivity) <= bound){
					value = alpha;
					return true;
				}
			}
		}
		return false;
	}

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
		int OwnMinusOpponents = (PopCount(P) << 1) + NumberOfEmptyStones - 64;
		if ((OwnMinusOpponents <= stability_cutoff_limits[NumberOfEmptyStones]) && StableStones_corner_and_co(O)) //Worth checking stability
			return (64 - static_cast<int>(PopCount(StableStones(P, O)) << 1) <= alpha);
		return false;
	}

	inline int ZWS_0(const CActiveConfigurations & actives, const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha)
	{
		++NodeCounter;
		return (actives.EvaluateFeatures(P, O) > alpha) ? alpha+1 : alpha;
	}
	inline int ZWS_0(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha)
	{
		++NodeCounter;
		return (EvaluateFeatures(P, O) > alpha) ? alpha+1 : alpha;
	}
	int ZWS_1(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha)
	{
		unsigned char Move;
		unsigned long long flipped;
		unsigned long long BitBoardPossible = PossibleMoves(P, O);

		++NodeCounter;

		if (!BitBoardPossible)
		{
			BitBoardPossible = PossibleMoves(O, P);
			if (BitBoardPossible)
				return -ZWS_1(O, P, NodeCounter, -alpha-1);
			else{ //Game is over
				++NodeCounter; 
				return (EvaluateGameOver(P, NumberOfEmptyStones(P, O)) > alpha) ? alpha+1 : alpha;
			}
		}

		CActiveConfigurations actives(P, O);

		while (BitBoardPossible)
		{
			Move = BitScanLSB(BitBoardPossible);
			RemoveLSB(BitBoardPossible);
			flipped = flip(P, O, Move);
			if (-ZWS_0(actives, O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, -alpha-1) > alpha)
				return alpha+1;
		}

		return alpha;
	}
	int ZWS_2(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha)
	{
		unsigned char Move;
		unsigned long long flipped;
		unsigned long long BitBoardPossible = PossibleMoves(P, O);

		++NodeCounter;

		if (!BitBoardPossible)
		{
			BitBoardPossible = PossibleMoves(O, P);
			if (BitBoardPossible)
				return -ZWS_2(O, P, NodeCounter, -alpha-1);
			else{ //Game is over
				++NodeCounter; 
				return (EvaluateGameOver(P, NumberOfEmptyStones(P, O)) > alpha) ? alpha+1 : alpha;
			}
		}

		while (BitBoardPossible)
		{
			Move = BitScanLSB(BitBoardPossible);
			RemoveLSB(BitBoardPossible);
			flipped = flip(P, O, Move);
			if (-ZWS_1(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, -alpha-1) > alpha)
				return alpha+1;
		}
		return alpha;
	}
	int ZWS(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, signed char depth, unsigned char selectivity)
	{
		if (depth == 2)
			return ZWS_2(P, O, search.NodeCounter, alpha);

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
				return (EvaluateGameOver(P, Empties) > alpha) ? alpha+1 : alpha;
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

		CMoveList mvList(search, P, O, BitBoardPossible, depth);
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

	inline int PVS_0(const CActiveConfigurations & actives, const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta)
	{
		++NodeCounter;
		return BIND(actives.EvaluateFeatures(P, O), alpha, beta);
	}
	inline int PVS_0(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta)
	{
		++NodeCounter;
		return BIND(EvaluateFeatures(P, O), alpha, beta);
	}
	int PVS_1(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta)
	{
		int value = -128;
		unsigned long Move;
		unsigned long long flipped;
		unsigned long long BitBoardPossible = PossibleMoves(P, O);

		++NodeCounter;

		if (!BitBoardPossible)
		{
			if (HasMoves(O, P))
				return -PVS_1(O, P, NodeCounter, -beta, -alpha);
			else{ //Game is over
				++NodeCounter; 
				return BIND(EvaluateGameOver(P, NumberOfEmptyStones(P, O)), alpha, beta);
			}
		}

		CActiveConfigurations actives(P, O);

		while (BitBoardPossible)
		{
			BitScanLSB(&Move, BitBoardPossible);
			RemoveLSB(BitBoardPossible);
			flipped = flip(P, O, Move);
			value = -PVS_0(actives, O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, -beta, -alpha);
			if (value >= beta) return beta;
			if (value > alpha) alpha = value;
		}

		return alpha;
	}
	int PVS_1(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta, CLine & pline)
	{
		if (pline.size == 0) return PVS_1(P, O, NodeCounter, alpha, beta);

		int value = -128;
		unsigned long Move;
		unsigned long long flipped;
		unsigned long long BitBoardPossible = PossibleMoves(P, O);

		++NodeCounter;

		if (!BitBoardPossible)
		{
			if (HasMoves(O, P))
				return -PVS_1(O, P, NodeCounter, -beta, -alpha, pline);
			else{ //Game is over
				++NodeCounter; 
				return BIND(EvaluateGameOver(P, NumberOfEmptyStones(P, O)), alpha, beta);
			}
		}

		CActiveConfigurations actives(P, O);

		while (BitBoardPossible)
		{
			BitScanLSB(&Move, BitBoardPossible);
			RemoveLSB(BitBoardPossible);
			flipped = flip(P, O, Move);
			value = -PVS_0(actives, O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, -beta, -alpha);
			if (value >= beta) return beta;
			if (value > alpha)
			{
				pline.line[0] = Move;
				alpha = value;
			}
		}

		return alpha;
	}
	int PVS_2(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta)
	{
		int value = -128;
		unsigned long Move;
		unsigned long long flipped;
		unsigned long long BitBoardPossible = PossibleMoves(P, O);

		++NodeCounter;

		if (!BitBoardPossible)
		{
			if (HasMoves(O, P))
				return -PVS_2(O, P, NodeCounter, -beta, -alpha);
			else{ //Game is over
				++NodeCounter; 
				return BIND(EvaluateGameOver(P, NumberOfEmptyStones(P, O)), alpha, beta);
			}
		}

		unsigned char line = 64;

		while (BitBoardPossible)
		{
			BitScanLSB(&Move, BitBoardPossible);
			RemoveLSB(BitBoardPossible);
			flipped = flip(P, O, Move);
			value = -PVS_1(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, -beta, -alpha);
			if (value >= beta) return beta;
			if (value > alpha) alpha = value;
		}
		return alpha;
	}
	int PVS_2(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta, CLine & pline)
	{
		if (pline.size == 0) return PVS_2(P, O, NodeCounter, alpha, beta);

		int value = -128;
		unsigned long Move;
		unsigned long long flipped;
		unsigned long long BitBoardPossible = PossibleMoves(P, O);
		
		++NodeCounter;

		if (!BitBoardPossible)
		{
			if (HasMoves(O, P))
				return -PVS_2(O, P, NodeCounter, -beta, -alpha, pline);
			else{ //Game is over
				++NodeCounter; 
				return BIND(EvaluateGameOver(P, NumberOfEmptyStones(P, O)), alpha, beta);
			}
		}

		CLine line(pline.size-1);
		
		while (BitBoardPossible)
		{
			BitScanLSB(&Move, BitBoardPossible);
			RemoveLSB(BitBoardPossible);
			flipped = flip(P, O, Move);
			value = -PVS_1(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, -beta, -alpha, line);
			if (value >= beta) return beta;
			if (value > alpha)
			{
				pline.NewPV(Move, line);
				alpha = value;
			}
		}
		return alpha;
	}
	int PVS(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, int beta, signed char depth, unsigned char selectivity)
	{
		if (depth == 2) 
			return PVS_2(P, O, search.NodeCounter, alpha, beta);

		const unsigned char Empties = NumberOfEmptyStones(P, O);

		bool SearchPV = true;
		int value;
		unsigned char BestMove = 64;
		unsigned long long flipped;
		unsigned long long BitBoardPossible = PossibleMoves(P, O);

		++search.NodeCounter;

		if (!BitBoardPossible)
		{
			if (HasMoves(O, P))
				return -PVS(search, O, P, -beta, -alpha, depth, selectivity);
			else{ //Game is over
				++search.NodeCounter;
				return BIND(EvaluateGameOver(P, Empties), alpha, beta);
			}
		}

		if (StabilityCutoff(P, O, Empties, alpha))
			return alpha;
	
		unsigned long long LocalNodeCounter = search.NodeCounter;

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
				value = -PVS(search, O ^ flipped, P ^ (1ULL << htValue.PV) ^ flipped, -beta, -alpha, depth-1, selectivity);
				if (value >= beta)
				{
					search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, beta, 64, htValue.PV, htValue.AV);
					return beta;
				}
				if (value > alpha)
				{
					alpha = value;
					SearchPV = false;
				}
			}

			if (htValue.AV != 64)
			{
				assert((BitBoardPossible & (1ULL << htValue.AV)) != 0);
				BitBoardPossible ^= (1ULL << htValue.AV);
				flipped = flip(P, O, htValue.AV);
				value = -PVS(search, O ^ flipped, P ^ (1ULL << htValue.AV) ^ flipped, -beta, -alpha, depth-1, selectivity);
				if (value >= beta)
				{
					search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, beta, 64, htValue.AV, htValue.PV);
					return beta;
				}
				if (value > alpha)
				{
					alpha = value;
					SearchPV = false;
				}
			}
		}

		CMoveList mvList(search, P, O, BitBoardPossible, depth);
		for (auto& mv : mvList)
		{
			if (SearchPV)
				value = -PVS(search, mv.P, mv.O, -beta, -alpha, depth-1, selectivity);
			else
			{
				value = -ZWS(search, mv.P, mv.O, -alpha-1, depth-1, selectivity);
				if (value > alpha)
					value = -PVS(search, mv.P, mv.O, -beta, -alpha, depth-1, selectivity);
			}
			if (value >= beta)
			{
				search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, beta, 64, mv.move, 64);
				return beta;
			}
			if (value > alpha)
			{
				BestMove = mv.move;
				alpha = value;
				SearchPV = false;
			}
		}
		if (SearchPV)
			search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, -64, alpha, 64, 64);
		else
			search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, alpha, alpha, BestMove, 64);
		return alpha;
	}
	int PVS(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, int beta, signed char depth, unsigned char selectivity, CLine & pline)
	{
		if (pline.size == 0) return PVS(search, P, O, alpha, beta, depth, selectivity);

		if (depth == 2)
			return PVS_2(P, O, search.NodeCounter, alpha, beta, pline);

		const unsigned char Empties = NumberOfEmptyStones(P, O);

		bool SearchPV = true;
		int value;
		unsigned long long flipped;
		unsigned long long BitBoardPossible = PossibleMoves(P, O);

		++search.NodeCounter;

		if (!BitBoardPossible)
		{
			if (HasMoves(O, P))
				return -PVS(search, O, P, -beta, -alpha, depth, selectivity, pline);
			else{ //Game is over
				++search.NodeCounter;
				return BIND(EvaluateGameOver(P, Empties), alpha, beta);
			}
		}

		if (StabilityCutoff(P, O, Empties, alpha))
			return alpha;
	
		unsigned long long LocalNodeCounter = search.NodeCounter;
		CLine line(pline.size-1);

		HashTableValueType htValue;
		if (search.HashTableLookUp(P, O, htValue))
		{
			if (UseHashTableValue(htValue, alpha, beta, Empties, selectivity, value))
				if ((value < alpha) || (value > beta))
					return value;

			if (htValue.PV != 64)
			{
				assert((BitBoardPossible & (1ULL << htValue.PV)) != 0);
				BitBoardPossible ^= (1ULL << htValue.PV);
				flipped = flip(P, O, htValue.PV);
				value = -PVS(search, O ^ flipped, P ^ (1ULL << htValue.PV) ^ flipped, -beta, -alpha, depth-1, selectivity, line);
				if (value >= beta)
				{
					pline.NewPV(htValue.PV, line);
					search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, beta, 64, htValue.PV, htValue.AV);
					return beta;
				}
				if (value > alpha)
				{
					pline.NewPV(htValue.PV, line);
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
					pline.NewPV(htValue.AV, line);
					search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, beta, 64, htValue.AV, htValue.PV);
					return beta;
				}
				if (value > alpha)
				{
					pline.NewPV(htValue.AV, line);
					alpha = value;
					SearchPV = false;
				}
			}
		}

		CMoveList mvList(search, P, O, BitBoardPossible, depth);
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
				pline.NewPV(mv.move, line);
				search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, beta, 64, mv.move, 64);
				return beta;
			}
			if (value > alpha)
			{
				pline.NewPV(mv.move, line);
				alpha = value;
				SearchPV = false;
			}
		}
		if (SearchPV)
			search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, -64, alpha, 64, 64);
		else
			search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, alpha, alpha, pline.BestMove(), 64);
		return alpha;
	}
}

int EvaluateLimitedDepth(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const signed char depth, const unsigned char selectivity)
{
	switch (depth)
	{
	case 0:  return Midgame::PVS_0(P, O, search.NodeCounter, alpha, beta);
	case 1:  return Midgame::PVS_1(P, O, search.NodeCounter, alpha, beta);
	case 2:  return Midgame::PVS_2(P, O, search.NodeCounter, alpha, beta);
	default: return Midgame::PVS(search, P, O, alpha, beta, depth, selectivity);
	}
}
int EvaluateLimitedDepth(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const signed char depth, const unsigned char selectivity, CLine & pline)
{
	if (pline.size == 0) return EvaluateLimitedDepth(search, P, O, alpha, beta, depth, selectivity);

	switch (depth)
	{
	case 0:  return Midgame::PVS_0(P, O, search.NodeCounter, alpha, beta);
	case 1:  return Midgame::PVS_1(P, O, search.NodeCounter, alpha, beta, pline);
	case 2:  return Midgame::PVS_2(P, O, search.NodeCounter, alpha, beta, pline);
	default: return Midgame::PVS(search, P, O, alpha, beta, depth, selectivity, pline);
	}
}