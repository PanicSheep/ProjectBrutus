#include "game.h"

namespace Midgame
{
	std::vector<std::pair<int, int>> MPC_aviable; // MPC_aviable(Depth, depth)
	bool MPC_actives[64][64]; // MPC_actives[Depth][depth]
	std::vector<CProbCuts> MPC_table[64][64]; // MPC_table[empties][Depth]
	float mu;

	void Initialize()
	{
		float a, b, sigma;
		bool InUse;
		std::string s_key, s_value;
		
		for (int D = 0; D < 64; D++)
			for (int d = 0; d < 64; d++)
			{
				s_key = "probcut ";
				if (D < 10) s_key += "0";
				s_key += std::to_string(D) + " ";
				if (d < 10) s_key += "0";
				s_key += std::to_string(d);

				if (ConfigFile::Configurations.count(s_key))
				{
					MPC_aviable.push_back(std::pair<int, int>(D, d));
					s_value = ConfigFile::Configurations[s_key];
					MPC_actives[D][d] = (s_value.substr(0, 1) == "t");
				}
				else
					MPC_actives[D][d] = false;
			}

		for (int empties = 0; empties < 64; empties++)
			for (int D = 0; D < 64; D++)
				for (int d = 0; d < D; d++)
				{
					s_key = "probcut ";
					if (empties < 10) s_key += "0";
					s_key += std::to_string(empties) + " ";
					if (D < 10) s_key += "0";
					s_key += std::to_string(D) + " ";
					if (d < 10) s_key += "0";
					s_key += std::to_string(d);

					if (ConfigFile::Configurations.count(s_key))
					{
						s_value = ConfigFile::Configurations[s_key];
						a = atof(s_value.substr(0, 8).c_str());
						b = atof(s_value.substr(9, 9).c_str());
						sigma = atof(s_value.substr(19, 8).c_str());
						MPC_table[empties][D].push_back(CProbCuts(d, a, b, sigma));
					}
				}

		s_value = ConfigFile::Configurations["probcut mu"];
		mu = std::stoi(s_value);
	}

	float sigma(const int D, const int d, const int E)
	{
		const double a = -0.2660;
		const double b =  0.9934;
		const double c =  0.3136;
		const double e = -0.0253;
		const double f =  2.0925;
		return (exp(a*d) + b) * (e*E + f) * pow((D - d), c);
	}

	bool MPC(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, unsigned char Depth, unsigned char selectivity, int & value)
	{
		if (selectivity == 0 || Depth < 4)
			return false;

		int empties = NumberOfEmptyStones(P, O);
		int zero_eval, small_depth;
		const int beta = alpha + 1;
		int probcut_error, eval_error;
        int depth, probcut_alpha, probcut_beta;
        int eval_beta, eval_alpha;
        int score;
		float folded_sigma;
		float t = SelectivityTable[selectivity].T;

		++search.NodeCounter;
		zero_eval = EvaluateFeatures(P, O);

		if (search.probCutLevel >= 2)
			return false;
		search.probCutLevel++;

		for (auto it : MPC_table[empties][Depth])
			if (MPC_actives[Depth][it.depth])
			{
				depth = it.depth;
				if (depth == 0)
				{
					if (zero_eval >= (beta + t * it.sigma - it.b) / it.a + 0.5f){
						value = beta;
						search.probCutLevel--;
						return true;
					}
					if (zero_eval <= (alpha - t * it.sigma - it.b) / it.a + 0.5f){
						value = alpha;
						search.probCutLevel--;
						return true;
					}
				}

				//probcut_error = t * sigma(depth, depth) + 0.5;
				//eval_error = t * sigma(depth, 0) + 0.5;
				//printf("eval_error:%d\n", eval_error);
				//eval_error = 0;

				//eval_beta = beta - eval_error;
				folded_sigma = std::sqrt(sigma(depth, 0, empties) * sigma(depth, 0, empties) + sigma(Depth, depth, empties) * sigma(Depth, depth, empties));
				probcut_beta = (beta + t * it.sigma - it.b) / it.a + 0.5f;
				probcut_alpha = probcut_beta - 1;

				if (zero_eval >= beta + mu * t * folded_sigma + 0.5f && probcut_beta <= 64)
				{
					score = ZWS(search, P, O, probcut_alpha, depth, selectivity);
					if (score >= probcut_beta){
						//printf("Upper cut at %d(%d), with alpha=%d, bound=%d, value=%d\n", depth, MPC_table[i].d, alpha, bound-1, value);
						value = beta;
						search.probCutLevel--;
						return true;
					}
				}

				//eval_alpha = alpha + eval_error;
				probcut_alpha = (alpha - t * it.sigma - it.b) / it.a + 0.5f;
				if (zero_eval <= alpha - mu * t * folded_sigma + 0.5f && probcut_alpha >= -64)
				{
					score = ZWS(search, P, O, probcut_alpha, depth, selectivity);
					if (score <= probcut_alpha){
						//printf("Lower cut at %d(%d), with alpha=%d, bound=%d, value=%d\n", depth, MPC_table[i].d, alpha, bound, value);
						value = alpha;
						search.probCutLevel--;
						return true;
					}
				}
			}

		search.probCutLevel--;
		return false;
	}
	
	bool StabilityCutoff(const unsigned long long P, const unsigned long long O, const int NumberOfEmptyStones, const int alpha)
	{
		static const char stability_cutoff_limits[64] = {
			 99, 99, 99, 99,  6,  8, 10, 12,
			 14, 16, 20, 22, 24, 26, 28, 30,
			 32, 34, 36, 38, 40, 42, 44, 46,
			 48, 48, 50, 50, 52, 52, 54, 54,
			 56, 56, 58, 58, 60, 60, 62, 62,
			 64, 64, 64, 64, 64, 64, 64, 64,
			 99, 99, 99, 99, 99, 99, 99, 99
		};
		int OwnMinusOpponents = PopCount(P) - PopCount(O);
		if ((OwnMinusOpponents <= stability_cutoff_limits[NumberOfEmptyStones]) && StableStones_corner_and_co(O)) //Worth checking stability
			return (64 - static_cast<int>(PopCount(StableStones(P, O)) << 1) <= alpha);
		return false;
	}

	bool ESC(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha)
	{
		return StabilityCutoff(P, O, NumberOfEmptyStones(P, O), alpha);
	}

	bool ETC(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, signed char depth, unsigned char selectivity)
	{
		HashTableValueType htValue;
		if (search.HashTableLookUp(P, O, htValue))
			if ((htValue.depth >= depth) && (htValue.selectivity <= selectivity) && (-htValue.beta > alpha))
				return true;
		return false;
	}

	int ZWS_0(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha)
	{
		++NodeCounter;
		return (EvaluateFeatures(P, O) > alpha) ? alpha+1 : alpha;
	}
	int ZWS_0(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, const Features::CIndexArray& Index_P, const Features::CIndexArray& Index_O)
	{
		++NodeCounter;
		return (EvaluateFeatures(P, O, Index_P, Index_O) > alpha) ? alpha+1 : alpha;
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
		
		while (BitBoardPossible)
		{
			Move = BitScanLSB(BitBoardPossible);
			RemoveLSB(BitBoardPossible);
			flipped = flip(P, O, Move);
			if (-ZWS_0(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, -alpha-1) > alpha)
				return alpha+1;
		}

		return alpha;
	}
	int ZWS_1(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, const Features::CIndexArray& Index_P, const Features::CIndexArray& Index_O)
	{
		unsigned char Move;
		unsigned long long flipped;
		unsigned long long BitBoardPossible = PossibleMoves(P, O);

		++NodeCounter;

		if (!BitBoardPossible)
		{
			BitBoardPossible = PossibleMoves(O, P);
			if (BitBoardPossible)
				return -ZWS_1(O, P, NodeCounter, -alpha-1, Index_O, Index_P);
			else{ //Game is over
				++NodeCounter; 
				return (EvaluateGameOver(P, NumberOfEmptyStones(P, O)) > alpha) ? alpha+1 : alpha;
			}
		}

		//Features::CIndexArray newIndex_P(Index_P);
		//Features::CIndexArray newIndex_O(Index_O);
		//UpdateIndexVec(P, O, newIndex_P, newIndex_O);
		
		while (BitBoardPossible)
		{
			Move = BitScanLSB(BitBoardPossible);
			RemoveLSB(BitBoardPossible);
			flipped = flip(P, O, Move);
			if (-ZWS_0(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, -alpha-1, Index_O, Index_P) > alpha)
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

		Features::CIndexArray Index_P(P);
		Features::CIndexArray Index_O(O);

		while (BitBoardPossible)
		{
			Move = BitScanLSB(BitBoardPossible);
			RemoveLSB(BitBoardPossible);
			flipped = flip(P, O, Move);
			if (-ZWS_1(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, -alpha-1, Index_O, Index_P) > alpha)
				return alpha+1;
		}
		return alpha;
	}
	int ZWS(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, signed char depth, unsigned char selectivity)
	{
		if (search.TestTimeOut()) return alpha;
		if (depth == 2) return ZWS_2(P, O, search.NodeCounter, alpha);
		if (depth == 1) return ZWS_1(P, O, search.NodeCounter, alpha);
		if (depth == 0) return ZWS_0(P, O, search.NodeCounter, alpha);

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
				flipped = flip(P, O, htValue.PV);
				if (ETC(search, O ^ flipped, P ^ (1ULL << htValue.PV) ^ flipped, alpha, depth, selectivity))
					return alpha + 1;
				if (ESC(search, O ^ flipped, P ^ (1ULL << htValue.PV) ^ flipped, -alpha-1))
					return alpha + 1;
			}
			if (htValue.AV != 64)
			{
				flipped = flip(P, O, htValue.AV);
				if (ETC(search, O ^ flipped, P ^ (1ULL << htValue.AV) ^ flipped, alpha, depth, selectivity))
					return alpha + 1;
				if (ESC(search, O ^ flipped, P ^ (1ULL << htValue.AV) ^ flipped, -alpha-1))
					return alpha + 1;
			}

			//if (htValue.PV != 64)
			//{
			//	assert((BitBoardPossible & (1ULL << htValue.PV)) != 0);
			//	BitBoardPossible ^= (1ULL << htValue.PV);
			//	flipped = flip(P, O, htValue.PV);
			//	if (-ZWS(search, O ^ flipped, P ^ (1ULL << htValue.PV) ^ flipped, -alpha-1, depth-1, selectivity) > alpha)
			//	{
			//		if (!search.stop)
			//			search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, alpha+1, 64, htValue.PV, htValue.AV);
			//		return alpha+1;
			//	}
			//}

			//if (htValue.AV != 64)
			//{
			//	assert((BitBoardPossible & (1ULL << htValue.AV)) != 0);
			//	BitBoardPossible ^= (1ULL << htValue.AV);
			//	flipped = flip(P, O, htValue.AV);
			//	if (-ZWS(search, O ^ flipped, P ^ (1ULL << htValue.AV) ^ flipped, -alpha-1, depth-1, selectivity) > alpha)
			//	{
			//		if (!search.stop)
			//			search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, alpha+1, 64, htValue.AV, htValue.PV);
			//		return alpha+1;
			//	}
			//}
		}

		CMoveList mvList(search, P, O, BitBoardPossible, depth, htValue);

		//for (auto& mv : mvList)
		//{
		//	//if (ETC(search, O ^ mv.flipped, P ^ (1ULL << mv.move) ^ mv.flipped, alpha, depth, selectivity))
		//	//	return alpha+1;
		//	if (ESC(search, O ^ mv.flipped, P ^ (1ULL << mv.move) ^ mv.flipped, -alpha-1))
		//		return alpha + 1;
		//}

		for (auto& mv : mvList)
		{
			if (-ZWS(search, O ^ mv.flipped, P ^ (1ULL << mv.move) ^ mv.flipped, -alpha-1, depth-1, selectivity) > alpha)
			{
				if (!search.stop)
					search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, alpha+1, 64, mv.move, 64);
				return alpha+1;
			}
		}
		if (!search.stop)
			search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, -64, alpha, 64, 64);
		return alpha;
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

		while (BitBoardPossible)
		{
			BitScanLSB(&Move, BitBoardPossible);
			RemoveLSB(BitBoardPossible);
			flipped = flip(P, O, Move);
			value = -PVS_0(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, -beta, -alpha);
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

		while (BitBoardPossible)
		{
			BitScanLSB(&Move, BitBoardPossible);
			RemoveLSB(BitBoardPossible);
			flipped = flip(P, O, Move);
			value = -PVS_0(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, -beta, -alpha);
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
		if (search.TestTimeOut()) return alpha;
		if (depth == 2) return PVS_2(P, O, search.NodeCounter, alpha, beta);

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

			//if (htValue.PV != 64)
			//{
			//	assert((BitBoardPossible & (1ULL << htValue.PV)) != 0);
			//	BitBoardPossible ^= (1ULL << htValue.PV);
			//	flipped = flip(P, O, htValue.PV);
			//	value = -PVS(search, O ^ flipped, P ^ (1ULL << htValue.PV) ^ flipped, -beta, -alpha, depth-1, selectivity);
			//	if (value >= beta)
			//	{
			//		if (!search.stop)
			//			search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, beta, 64, htValue.PV, htValue.AV);
			//		return beta;
			//	}
			//	if (value > alpha)
			//	{
			//		BestMove = htValue.PV;
			//		alpha = value;
			//		SearchPV = false;
			//	}
			//}

			//if (htValue.AV != 64)
			//{
			//	assert((BitBoardPossible & (1ULL << htValue.AV)) != 0);
			//	BitBoardPossible ^= (1ULL << htValue.AV);
			//	flipped = flip(P, O, htValue.AV);
			//	value = -PVS(search, O ^ flipped, P ^ (1ULL << htValue.AV) ^ flipped, -beta, -alpha, depth-1, selectivity);
			//	if (value >= beta)
			//	{
			//		if (!search.stop)
			//			search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, beta, 64, htValue.AV, htValue.PV);
			//		return beta;
			//	}
			//	if (value > alpha)
			//	{
			//		BestMove = htValue.AV;
			//		alpha = value;
			//		SearchPV = false;
			//	}
			//}
		}

		if (htValue.PV == 64 && depth >= 4 && PopCount(BitBoardPossible) > 1)
		{
			PVS(search, P, O, -64, 64, depth-2, 6);
			search.HashTableLookUp(P, O, htValue);
		}

		CMoveList mvList(search, P, O, BitBoardPossible, depth, htValue);
		for (auto& mv : mvList)
		{
			if (SearchPV)
				value = -PVS(search, O ^ mv.flipped, P ^ (1ULL << mv.move) ^ mv.flipped, -beta, -alpha, depth-1, selectivity);
			else
			{
				value = -ZWS(search, O ^ mv.flipped, P ^ (1ULL << mv.move) ^ mv.flipped, -alpha-1, depth-1, selectivity);
				if (value > alpha)
					value = -PVS(search, O ^ mv.flipped, P ^ (1ULL << mv.move) ^ mv.flipped, -beta, -alpha, depth-1, selectivity);
			}
			if (value >= beta)
			{
				if (!search.stop)
					search.HashTableUpdate(P, O, search.NodeCounter - LocalNodeCounter, depth, selectivity, beta, 64, mv.move, 64);
				return beta;
			}
			if (value > alpha)
			{
				//if (mv.move == BestMove1);
				//else if (mv.move == BestMove2)
				//	std::swap(BestMove1, BestMove2);
				//else{
				//	BestMove2 = BestMove1;
				//	BestMove1 = mv.move;
				//}
				BestMove = mv.move;
				alpha = value;
				SearchPV = false;
			}
		}
		if (!search.stop)
		{
			if (SearchPV)
				search.HashTableUpdate(P, O, search.NodeCounter - LocalNodeCounter, depth, selectivity, -64, alpha, 64, 64);
			else
				search.HashTableUpdate(P, O, search.NodeCounter - LocalNodeCounter, depth, selectivity, alpha, alpha, BestMove, 64);
		}
		return alpha;
	}
	int PVS(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, int beta, signed char depth, unsigned char selectivity, CLine & pline)
	{
		if (search.TestTimeOut()) return alpha;
		if (pline.size == 0) return PVS(search, P, O, alpha, beta, depth, selectivity);
		if (depth == 2) return PVS_2(P, O, search.NodeCounter, alpha, beta, pline);

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
				pline.NoMoves();
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
			if (UseHashTableValue(htValue, alpha, beta, depth, selectivity, value))
				if ((value < alpha) || (value > beta))
					return value;

			//if (htValue.PV != 64)
			//{
			//	assert((BitBoardPossible & (1ULL << htValue.PV)) != 0);
			//	BitBoardPossible ^= (1ULL << htValue.PV);
			//	flipped = flip(P, O, htValue.PV);
			//	value = -PVS(search, O ^ flipped, P ^ (1ULL << htValue.PV) ^ flipped, -beta, -alpha, depth-1, selectivity, line);
			//	if (value >= beta)
			//	{
			//		if (!search.stop){
			//			pline.NewPV(htValue.PV, line);
			//			search.HashTableUpdate(P, O, search.NodeCounter - LocalNodeCounter, depth, selectivity, beta, 64, htValue.PV, htValue.AV);
			//		}
			//		return beta;
			//	}
			//	if (value > alpha)
			//	{
			//		if (!search.stop)
			//			pline.NewPV(htValue.PV, line);
			//		alpha = value;
			//		SearchPV = false;
			//	}
			//}

			//if (htValue.AV != 64)
			//{
			//	assert((BitBoardPossible & (1ULL << htValue.AV)) != 0);
			//	BitBoardPossible ^= (1ULL << htValue.AV);
			//	flipped = flip(P, O, htValue.AV);
			//	value = -PVS(search, O ^ flipped, P ^ (1ULL << htValue.AV) ^ flipped, -beta, -alpha, depth-1, selectivity, line);
			//	if (value >= beta)
			//	{
			//		if (!search.stop){
			//			pline.NewPV(htValue.AV, line);
			//			search.HashTableUpdate(P, O, search.NodeCounter - LocalNodeCounter, depth, selectivity, beta, 64, htValue.AV, htValue.PV);
			//		}
			//		return beta;
			//	}
			//	if (value > alpha)
			//	{
			//		if (!search.stop)
			//			pline.NewPV(htValue.AV, line);
			//		alpha = value;
			//		SearchPV = false;
			//	}
			//}
		}

		if (htValue.PV == 64 && depth >= 4 && PopCount(BitBoardPossible) > 1)
		{
			PVS(search, P, O, -64, 64, depth-2, 6);
			search.HashTableLookUp(P, O, htValue);
		}

		CMoveList mvList(search, P, O, BitBoardPossible, depth, htValue);
		for (auto& mv : mvList)
		{
			if (SearchPV)
				value = -PVS(search, O ^ mv.flipped, P ^ (1ULL << mv.move) ^ mv.flipped, -beta, -alpha, depth-1, selectivity, line);
			else
			{
				value = -ZWS(search, O ^ mv.flipped, P ^ (1ULL << mv.move) ^ mv.flipped, -alpha-1, depth-1, selectivity);
				if (value > alpha)
					value = -PVS(search, O ^ mv.flipped, P ^ (1ULL << mv.move) ^ mv.flipped, -beta, -alpha, depth-1, selectivity, line);
			}
			if (value >= beta)
			{
				if (!search.stop){
					pline.NewPV(mv.move, line);
					//if (mv.move == BestMove1) BestMove1 = BestMove2;
					search.HashTableUpdate(P, O, search.NodeCounter - LocalNodeCounter, depth, selectivity, beta, 64, mv.move, 64);
				}
				return beta;
			}
			if (value > alpha)
			{
				if (!search.stop)
					pline.NewPV(mv.move, line);
				//if (mv.move == BestMove1);
				//else if (mv.move == BestMove2)
				//	std::swap(BestMove1, BestMove2);
				//else{
				//	BestMove2 = BestMove1;
				//	BestMove1 = mv.move;
				//}
				alpha = value;
				SearchPV = false;
			}
		}
		if (!search.stop)
		{
			if (SearchPV)
				search.HashTableUpdate(P, O, search.NodeCounter - LocalNodeCounter, depth, selectivity, -64, alpha, 64, 64);
			else
				search.HashTableUpdate(P, O, search.NodeCounter - LocalNodeCounter, depth, selectivity, alpha, alpha, pline.BestMove(), 64);
		}
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