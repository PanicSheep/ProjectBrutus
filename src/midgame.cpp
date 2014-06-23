#include "game.h"

namespace Midgame
{
	std::vector<CCutOffLimits> MPC_table;

	void Initialize()
	{
		float a, b, sigma;
		bool InUse;
		std::string s;
		for (int D = 0; D < 64; D++)
			for (int d = 0; d < D; d++)
			{
				s = "probcut ";
				if (D < 10) s += "0";
				s += std::to_string(D) + " ";
				if (d < 10) s += "0";
				s += std::to_string(d);

				if (ConfigFile::Configurations.count(s))
				{
					s = ConfigFile::Configurations[s];
					b = atof(s.substr(0, 9).c_str());
					a = atof(s.substr(10, 8).c_str());
					sigma = atof(s.substr(19, 8).c_str());
					InUse = (s.substr(28, 1) == "t");
					MPC_table.push_back(CCutOffLimits(D, d, b, a, sigma, InUse));
				}
			}
	}

	void Change_MPC_table(CCutOffLimits * pair)
	{
		pair->InUse = !pair->InUse;
	}
	void Change_MPC_table(int D, int d)
	{
		for (auto& it : MPC_table)
			if ((it.D == D) && (it.d == D))
				it.InUse = !it.InUse;
	}
	void Change_MPC_table(int D, int d, bool InUse)
	{
		for (auto& it : MPC_table)
			if ((it.D == D) && (it.d == D))
				it.InUse = InUse;
	}

	double sigma(int D, int d)
	{
		const double a = -0.8251;
		const double b = -5.328;
		const double c = 0.06022;
		const double e = 8.25;
		return (a * d + b) * exp(-c * D) + e;
	}

	bool MPC(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, signed char depth, unsigned char selectivity, int & value)
	{
		if (selectivity == 0 || depth < 3)
			return false;

		int zero_eval, small_depth;
		const int beta = alpha + 1;
		int probcut_error, eval_error;
        int probcut_depth, probcut_alpha, probcut_beta;
        int eval_beta, eval_alpha;
        int score;
		double t = SelectivityTable[selectivity].T;
		double mu = -1.5;

		++search.NodeCounter;
		zero_eval = EvaluateFeatures(P, O);

		if (depth > 16)
		{
			probcut_depth = (depth / 4) * 2 + (depth & 1) - 4;
			while (probcut_depth <= (depth / 4) * 2 + (depth & 1))
			{
				probcut_error = t * sigma(depth, probcut_depth) + 0.5;
				eval_error = t * sigma(probcut_depth, 0) + 0.5;

				eval_beta = beta - eval_error;
				probcut_beta = beta + probcut_error;
				probcut_alpha = probcut_beta - 1;

				if (zero_eval - mu * sigma(probcut_depth, 0) >= beta + t * sigma(depth, probcut_depth) && probcut_beta <= 64)
				{
					score = ZWS(search, P, O, probcut_alpha, probcut_depth, selectivity);
					if (score >= probcut_beta){
						//printf("Upper cut at %d/%d, with alpha=%d, bound=%d, value=%d\n", depth, small_depth, alpha, bound - 1, value);
						value = beta;
						return true;
					}
				}

				eval_alpha = alpha + eval_error;
				probcut_alpha = alpha - probcut_error;
				if (zero_eval + mu * sigma(probcut_depth, 0) < alpha - t * sigma(depth, probcut_depth) && probcut_alpha >= -64)
				{
					score = ZWS(search, P, O, probcut_alpha, probcut_depth, selectivity);
					if (score <= probcut_alpha){
						//printf("Lower cut at %d/%d, with alpha=%d, bound=%d, value=%d\n", depth, small_depth, alpha, bound, value);
						value = alpha;
						return true;
					}
				}

				probcut_depth += 2;
			}
		}
		else
		{
			for (int i = 0; i < MPC_table.size(); i++)
				if (MPC_table[i].InUse && (depth == MPC_table[i].D))
				{
					if (MPC_table[i].d == 0)
					{
						if (zero_eval >= (beta + t * MPC_table[i].sigma - MPC_table[i].b) / MPC_table[i].a + 0.5f){
							value = beta;
							return true;
						}
						if (zero_eval <= (alpha - t * MPC_table[i].sigma - MPC_table[i].b) / MPC_table[i].a + 0.5f){
							value = alpha;
							return true;
						}
					}

					probcut_depth = MPC_table[i].d;
					//probcut_error = t * sigma(depth, probcut_depth) + 0.5;
					eval_error = t * sigma(probcut_depth, 0) + 0.5;
					//printf("eval_error:%d\n", eval_error);
					//eval_error = 0;

					eval_beta = beta - eval_error;
					probcut_beta = (beta + t * MPC_table[i].sigma - MPC_table[i].b) / MPC_table[i].a + 0.5f;
					probcut_alpha = probcut_beta - 1;

					if (zero_eval >= eval_beta && probcut_beta <= 64)
					{
						score = ZWS(search, P, O, probcut_alpha, probcut_depth, 0);
						if (score >= probcut_beta){
							//printf("Upper cut at %d(%d), with alpha=%d, bound=%d, value=%d\n", depth, MPC_table[i].d, alpha, bound-1, value);
							value = beta;
							return true;
						}
					}

					eval_alpha = alpha + eval_error;
					probcut_alpha = (alpha - t * MPC_table[i].sigma - MPC_table[i].b) / MPC_table[i].a + 0.5f;
					if (zero_eval < eval_alpha && probcut_alpha >= -64)
					{
						score = ZWS(search, P, O, probcut_alpha, probcut_depth, 0);
						if (score <= probcut_alpha){
							//printf("Lower cut at %d(%d), with alpha=%d, bound=%d, value=%d\n", depth, MPC_table[i].d, alpha, bound, value);
							value = alpha;
							return true;
						}
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

	int ZWS_0(const CActiveConfigurations & actives, const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha)
	{
		++NodeCounter;
		return (actives.EvaluateFeatures(P, O) > alpha) ? alpha+1 : alpha;
	}
	int ZWS_0(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha)
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
		if (search.TestTimeOut())
			return alpha;

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
				//if (ETC(search, O ^ flipped, P ^ (1ULL << htValue.PV) ^ flipped, alpha, depth, selectivity))
				//	return alpha + 1;
				if (ESC(search, O ^ flipped, P ^ (1ULL << htValue.PV) ^ flipped, -alpha-1))
					return alpha + 1;
			}
			if (htValue.AV != 64)
			{
				flipped = flip(P, O, htValue.AV);
				//if (ETC(search, O ^ flipped, P ^ (1ULL << htValue.AV) ^ flipped, alpha, depth, selectivity))
				//	return alpha + 1;
				if (ESC(search, O ^ flipped, P ^ (1ULL << htValue.AV) ^ flipped, -alpha-1))
					return alpha + 1;
			}

			if (htValue.PV != 64)
			{
				assert((BitBoardPossible & (1ULL << htValue.PV)) != 0);
				BitBoardPossible ^= (1ULL << htValue.PV);
				flipped = flip(P, O, htValue.PV);
				if (-ZWS(search, O ^ flipped, P ^ (1ULL << htValue.PV) ^ flipped, -alpha-1, depth-1, selectivity) > alpha)
				{
					if (!search.TestTimeOut())
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
					if (!search.TestTimeOut())
						search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, alpha+1, 64, htValue.AV, htValue.PV);
					return alpha+1;
				}
			}
		}

		CMoveList mvList(search, P, O, BitBoardPossible, depth, htValue);

		for (auto& mv : mvList)
		{
			//if (ETC(search, mv.P, mv.O, alpha, depth, selectivity))
			//	return alpha+1;
			if (ESC(search, mv.P, mv.O, -alpha-1))
				return alpha + 1;
		}

		for (auto& mv : mvList)
		{
			if (-ZWS(search, mv.P, mv.O, -alpha-1, depth-1, selectivity) > alpha)
			{
				if (!search.TestTimeOut())
					search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, alpha+1, 64, mv.move, 64);
				return alpha+1;
			}
		}
		if (!search.TestTimeOut())
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
		if (search.TestTimeOut())
			return alpha;
						
		if (depth == 2) 
			return PVS_2(P, O, search.NodeCounter, alpha, beta);

		const unsigned char Empties = NumberOfEmptyStones(P, O);

		bool SearchPV = true;
		int value;
		unsigned char BestMove = 64;
		unsigned long long flipped;
		unsigned long long BitBoardPossible = PossibleMoves(P, O);
		unsigned char BestMove1, BestMove2;

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
					if (!search.TestTimeOut())
						search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, beta, 64, htValue.PV, htValue.AV);
					return beta;
				}
				if (value > alpha)
				{
					BestMove = htValue.PV;
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
					if (!search.TestTimeOut())
						search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, depth, selectivity, beta, 64, htValue.AV, htValue.PV);
					return beta;
				}
				if (value > alpha)
				{
					BestMove = htValue.AV;
					alpha = value;
					SearchPV = false;
				}
			}
		}

		CMoveList mvList(search, P, O, BitBoardPossible, depth, htValue);
		BestMove1 = mvList.BestMove();
		BestMove2 = mvList.NextBestMove();
		for (auto& mv : mvList)
		{
			if (search.TestTimeOut())
				return alpha;
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
				if (!search.TestTimeOut()){
					//if (mv.move == BestMove1) BestMove1 = BestMove2;
					search.HashTableUpdate(P, O, search.NodeCounter - LocalNodeCounter, depth, selectivity, beta, 64, mv.move, 64);
				}
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
		if (!search.TestTimeOut())
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
		if (search.TestTimeOut())
			return alpha;

		if (pline.size == 0) return PVS(search, P, O, alpha, beta, depth, selectivity);

		if (depth == 2)
			return PVS_2(P, O, search.NodeCounter, alpha, beta, pline);

		const unsigned char Empties = NumberOfEmptyStones(P, O);

		bool SearchPV = true;
		int value;
		unsigned long long flipped;
		unsigned long long BitBoardPossible = PossibleMoves(P, O);
		unsigned char BestMove1, BestMove2;

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

			if (htValue.PV != 64)
			{
				assert((BitBoardPossible & (1ULL << htValue.PV)) != 0);
				BitBoardPossible ^= (1ULL << htValue.PV);
				flipped = flip(P, O, htValue.PV);
				value = -PVS(search, O ^ flipped, P ^ (1ULL << htValue.PV) ^ flipped, -beta, -alpha, depth-1, selectivity, line);
				if (value >= beta)
				{
					if (!search.TestTimeOut()){
						pline.NewPV(htValue.PV, line);
						search.HashTableUpdate(P, O, search.NodeCounter - LocalNodeCounter, depth, selectivity, beta, 64, htValue.PV, htValue.AV);
					}
					return beta;
				}
				if (value > alpha)
				{
					if (!search.TestTimeOut())
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
					if (!search.TestTimeOut()){
						pline.NewPV(htValue.AV, line);
						search.HashTableUpdate(P, O, search.NodeCounter - LocalNodeCounter, depth, selectivity, beta, 64, htValue.AV, htValue.PV);
					}
					return beta;
				}
				if (value > alpha)
				{
					if (!search.TestTimeOut())
						pline.NewPV(htValue.AV, line);
					alpha = value;
					SearchPV = false;
				}
			}
		}

		CMoveList mvList(search, P, O, BitBoardPossible, depth, htValue);
		BestMove1 = mvList.BestMove();
		BestMove2 = mvList.NextBestMove();
		for (auto& mv : mvList)
		{
			if (search.TestTimeOut())
				return alpha;
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
				if (!search.TestTimeOut()){
					pline.NewPV(mv.move, line);
					//if (mv.move == BestMove1) BestMove1 = BestMove2;
					search.HashTableUpdate(P, O, search.NodeCounter - LocalNodeCounter, depth, selectivity, beta, 64, mv.move, 64);
				}
				return beta;
			}
			if (value > alpha)
			{
				if (!search.TestTimeOut())
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
		if (!search.TestTimeOut())
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
