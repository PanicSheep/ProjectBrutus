#include "game.h"

// A: No calculation of PossibleMoves, just testing if a ply flips stones. Move ordering based on pattern and parity.
// B: Calculating Possible Moves. Move ordering based on pattern and parity
// C: Using hash tables and sophisticated move ordering

namespace Endgame
{
	std::vector<CCutOffLimits> MPC_table({
		CCutOffLimits(10,  0, -0.955690f, 0.982075f, 7.229145f, true),
		CCutOffLimits(10,  1,  0.608850f, 1.044230f, 6.540396f, true),
		CCutOffLimits(10,  2, -1.790080f, 1.035370f, 5.835613f, true),
		CCutOffLimits(10,  3,  0.726926f, 1.017016f, 5.481348f, true),
		CCutOffLimits(11,  0,  0.836652f, 0.994059f, 7.082141f, true),
		CCutOffLimits(11,  1,  2.652704f, 0.980076f, 6.404368f, false),
		CCutOffLimits(11,  2,  0.060596f, 1.040618f, 5.798437f, false),
		CCutOffLimits(11,  3,  2.803727f, 1.023115f, 5.376995f, true),
		CCutOffLimits(11,  4, -0.017008f, 1.007512f, 4.919953f, true),
		CCutOffLimits(12,  0, -1.404029f, 1.037020f, 7.301017f, true),
		CCutOffLimits(12,  1,  0.861866f, 1.021294f, 6.560548f, false),
		CCutOffLimits(12,  2, -2.119963f, 1.013227f, 5.966220f, true),
		CCutOffLimits(12,  3,  0.915875f, 1.060153f, 5.651452f, true),
		CCutOffLimits(12,  4, -2.216285f, 1.045512f, 5.106089f, false),
		CCutOffLimits(12,  5,  0.705978f, 1.021842f, 4.836680f, true),
		CCutOffLimits(13,  0,  1.413220f, 0.964083f, 7.142576f, true),
		CCutOffLimits(13,  1,  3.153194f, 1.029896f, 6.457468f, false),
		CCutOffLimits(13,  2, -0.251685f, 1.022943f, 5.792326f, false),
		CCutOffLimits(13,  3,  3.176289f, 0.998355f, 5.524462f, true),
		CCutOffLimits(13,  4, -0.224287f, 1.050109f, 5.046984f, true),
		CCutOffLimits(13,  5,  2.920321f, 1.026998f, 4.753281f, false),
		CCutOffLimits(13,  6, -0.117578f, 1.012825f, 4.342136f, false),
		CCutOffLimits(14,  0, -0.858567f, 1.008158f, 7.282141f, true),
		CCutOffLimits(14,  1,  0.192599f, 0.993065f, 6.610465f, false),
		CCutOffLimits(14,  2, -2.691474f, 1.064405f, 6.015619f, true),
		CCutOffLimits(14,  3,  1.266951f, 1.039642f, 5.652557f, false),
		CCutOffLimits(14,  4, -2.623739f, 1.025726f, 5.227389f, true),
		CCutOffLimits(14,  5,  0.903164f, 1.064284f, 4.992097f, true),
		CCutOffLimits(14,  6, -2.454023f, 1.050153f, 4.535461f, false),
		CCutOffLimits(15,  0,  1.179192f, 1.018373f, 7.163458f, true),
		CCutOffLimits(15,  1,  2.538466f, 1.002338f, 6.421089f, false),
		CCutOffLimits(15,  2,  0.431105f, 0.990603f, 5.836637f, true),
		CCutOffLimits(15,  3,  3.690411f, 1.045506f, 5.581817f, true),
		CCutOffLimits(15,  4, -0.615447f, 1.033543f, 5.042296f, false),
		CCutOffLimits(15,  5,  3.379743f, 1.004002f, 4.908287f, false),
		CCutOffLimits(15,  6, -0.335630f, 1.053034f, 4.473601f, false),
		CCutOffLimits(16,  0, -1.126149f, 0.984720f, 7.193613f, true),
		CCutOffLimits(16,  1,  0.496513f, 1.045424f, 6.623466f, false),
		CCutOffLimits(16,  2, -2.046250f, 1.034087f, 5.960103f, false),
		CCutOffLimits(16,  3,  0.578069f, 1.011154f, 5.708272f, false),
		CCutOffLimits(16,  4, -3.204021f, 1.075319f, 5.280633f, false),
		CCutOffLimits(16,  5,  1.357198f, 1.046045f, 5.004946f, true),
		CCutOffLimits(16,  6, -2.936041f, 1.032093f, 4.667277f, false),
		CCutOffLimits(17,  0,  0.920282f, 0.995588f, 7.021305f, true),
		CCutOffLimits(17,  1,  2.715802f, 0.973366f, 6.542318f, true),
		CCutOffLimits(17,  2,  0.083205f, 1.042347f, 5.844496f, false),
		CCutOffLimits(17,  3,  3.052708f, 1.019415f, 5.547676f, true),
		CCutOffLimits(17,  4,  0.067480f, 1.002611f, 5.091725f, true),
		CCutOffLimits(17,  5,  3.909372f, 1.050908f, 4.975393f, false),
		CCutOffLimits(17,  6, -0.819175f, 1.039098f, 4.470674f, false),
		CCutOffLimits(18,  0, -1.592779f, 1.038661f, 7.181811f, false),
		CCutOffLimits(18,  1,  0.605193f, 1.016764f, 6.662402f, true),
		CCutOffLimits(18,  2, -2.234622f, 1.007983f, 5.986608f, false),
		CCutOffLimits(18,  3,  0.898225f, 1.062776f, 5.728421f, false),
		CCutOffLimits(18,  4, -2.540610f, 1.045602f, 5.226578f, false),
		CCutOffLimits(18,  5,  0.694262f, 1.019182f, 5.072979f, false),
		CCutOffLimits(18,  6, -3.524015f, 1.080564f, 4.741089f, false),
		CCutOffLimits(19,  0,  1.586007f, 0.965885f, 7.028058f, false),
		CCutOffLimits(19,  1,  3.260213f, 1.024397f, 6.553807f, false),
		CCutOffLimits(19,  2, -0.063446f, 1.017345f, 5.811272f, true),
		CCutOffLimits(19,  3,  3.260832f, 0.990744f, 5.640768f, false),
		CCutOffLimits(19,  4, -0.288293f, 1.053177f, 5.113163f, false),
		CCutOffLimits(19,  5,  3.291963f, 1.026315f, 4.946041f, false),
		CCutOffLimits(19,  6, -0.139905f, 1.008808f, 4.541028f, false),
		CCutOffLimits(20,  0, -0.951239f, 1.009906f, 7.132139f, false),
		CCutOffLimits(20,  1, -0.148935f, 0.989733f, 6.680770f, false),
		CCutOffLimits(20,  2, -2.833894f, 1.062117f, 6.046139f, false),
		CCutOffLimits(20,  3,  1.050450f, 1.035706f, 5.757993f, false),
		CCutOffLimits(20,  4, -2.765163f, 1.020927f, 5.308957f, false),
		CCutOffLimits(20,  5,  1.035966f, 1.070758f, 5.120746f, false),
		CCutOffLimits(20,  6, -2.859929f, 1.052501f, 4.693923f, false),
		CCutOffLimits(21,  0,  1.312580f, 1.017781f, 7.019736f, false),
		CCutOffLimits(21,  1,  2.519873f, 0.997891f, 6.506688f, false),
		CCutOffLimits(21,  2,  0.696631f, 0.986368f, 5.829326f, false),
		CCutOffLimits(21,  3,  3.809805f, 1.042751f, 5.668088f, false),
		CCutOffLimits(21,  4, -0.462419f, 1.029706f, 5.121321f, false),
		CCutOffLimits(21,  5,  3.536810f, 0.999587f, 5.045815f, false),
		CCutOffLimits(21,  6, -0.504488f, 1.059326f, 4.581100f, false),
		CCutOffLimits(22,  0, -0.478459f, 1.010294f, 7.132730f, false),
		CCutOffLimits(22,  1,  0.222635f, 1.042224f, 6.683277f, false),
		CCutOffLimits(22,  2, -2.040013f, 1.030831f, 5.942886f, false),
		CCutOffLimits(22,  3,  0.257676f, 1.007646f, 5.781188f, false),
		CCutOffLimits(22,  4, -3.340425f, 1.075136f, 5.313757f, false),
		CCutOffLimits(22,  5,  1.253716f, 1.045036f, 5.141610f, true),
		CCutOffLimits(22,  6, -3.105332f, 1.028820f, 4.777626f, false),
	});

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

	double sigma(int Empties, int d)
	{
		const double a = 3.65;
		const double b = 0.003298;
		const double c = -0.1764;
		const double e = 3.294;
		return a * exp(b * Empties) * exp(c * d) + e;
	}

	bool MPC(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, unsigned char Empties, unsigned char selectivity, int & value)
	{
		if (selectivity == 0 || Empties > 22)
			return false;

		int zero_eval, small_depth;
		const int beta = alpha + 1;
		int probcut_error, eval_error;
        int probcut_depth, probcut_alpha, probcut_beta;
        int eval_beta, eval_alpha;
        int score;
		double t = SelectivityTable[selectivity].T;
		
		++search.NodeCounter;
		zero_eval = EvaluateFeatures(P, O);

		for (int i = 0; i < MPC_table.size(); i++)
			if (MPC_table[i].InUse && (Empties == MPC_table[i].D))
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
					score = Midgame::ZWS(search, P, O, probcut_alpha, probcut_depth, 0);
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
					score =  Midgame::ZWS(search, P, O, probcut_alpha, probcut_depth, 0);
					if (score <= probcut_alpha){
						//printf("Lower cut at %d(%d), with alpha=%d, bound=%d, value=%d\n", depth, MPC_table[i].d, alpha, bound, value);
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

	int NegaMax_1(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char x)
	{
		int Score, FlipCount, Max;
		Score = (PopCount(P) << 1) - 62; // PopCount(P) - PopCount(O) + 1

		++NodeCounter;
		if (FlipCount = count_last_flip(P, x))
		{
			++NodeCounter;
			return Score + FlipCount;
		}
		else if (FlipCount = count_last_flip(O, x))
		{
			NodeCounter += 2;
			return Score - FlipCount - 2;
		}
		else
			return (Score > 0) ? Score : Score - 2;
	}
	int NegaMax_2(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char x1, const unsigned char x2)
	{
		unsigned long long flipped;
		int Score = -64;
		bool played = false;
		++NodeCounter;

		//Play on x1
		if ((O & NEIGHBOUR(x1)) && (flipped = flip(P, O, x1)))
		{
			Score = MAX(Score, -NegaMax_1(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, x2));
			played = true;
		}

		//Play on x2
		if ((O & NEIGHBOUR(x2)) && (flipped = flip(P, O, x2)))
		{
			Score = MAX(Score, -NegaMax_1(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, x1));
			played = true;
		}

		if (played) return Score;

		++NodeCounter;
		Score = 64;

		//Play on x1
		if ((P & NEIGHBOUR(x1)) && (flipped = flip(O, P, x1)))
		{
			Score = MIN(Score, NegaMax_1(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, x2));
			played = true;
		}

		//Play on x2
		if ((P & NEIGHBOUR(x2)) && (flipped = flip(O, P, x2)))
		{
			Score = MIN(Score, NegaMax_1(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, x1));
			played = true;
		}

		if (played)
			return Score;
		else
			return EvaluateGameOver(P, 2);
	}
	int NegaMax_3(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char x1, const unsigned char x2, const unsigned char x3)
	{
		unsigned long long flipped;
		int Score = -64;
		bool played = false;
		++NodeCounter;

		//Play on x1
		if ((O & NEIGHBOUR(x1)) && (flipped = flip(P, O, x1)))
		{
			Score = MAX(Score, -NegaMax_2(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, x2, x3));
			played = true;
		}

		//Play on x2
		if ((O & NEIGHBOUR(x2)) && (flipped = flip(P, O, x2)))
		{
			Score = MAX(Score, -NegaMax_2(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, x1, x3));
			played = true;
		}

		//Play on x3
		if ((O & NEIGHBOUR(x3)) && (flipped = flip(P, O, x3)))
		{
			Score = MAX(Score, -NegaMax_2(O ^ flipped, P ^ (1ULL << x3) ^ flipped, NodeCounter, x1, x2));
			played = true;
		}

		if (played) return Score;

		++NodeCounter;
		Score = 64;

		//Play on x1
		if ((P & NEIGHBOUR(x1)) && (flipped = flip(O, P, x1)))
		{
			Score = MIN(Score, NegaMax_2(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, x2, x3));
			played = true;
		}

		//Play on x2
		if ((P & NEIGHBOUR(x2)) && (flipped = flip(O, P, x2)))
		{
			Score = MIN(Score, NegaMax_2(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, x1, x3));
			played = true;
		}

		//Play on x3
		if ((P & NEIGHBOUR(x3)) && (flipped = flip(O, P, x3)))
		{
			Score = MIN(Score, NegaMax_2(P ^ flipped, O ^ (1ULL << x3) ^ flipped, NodeCounter, x1, x2));
			played = true;
		}

		if (played)
			return Score;
		else
			return EvaluateGameOver(P, 3);
	}
	int NegaMax_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char x1, const unsigned char x2, const unsigned char x3, const unsigned char x4)
	{
		unsigned long long flipped;
		int Score = -64;
		bool played = false;
		++NodeCounter;

		//Play on x1
		if ((O & NEIGHBOUR(x1)) && (flipped = flip(P, O, x1)))
		{
			Score = MAX(Score, -NegaMax_3(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, x2, x3, x4));
			played = true;
		}

		//Play on x2
		if ((O & NEIGHBOUR(x2)) && (flipped = flip(P, O, x2)))
		{
			Score = MAX(Score, -NegaMax_3(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, x1, x3, x4));
			played = true;
		}

		//Play on x3
		if ((O & NEIGHBOUR(x3)) && (flipped = flip(P, O, x3)))
		{
			Score = MAX(Score, -NegaMax_3(O ^ flipped, P ^ (1ULL << x3) ^ flipped, NodeCounter, x1, x2, x4));
			played = true;
		}

		//Play on x4
		if ((O & NEIGHBOUR(x4)) && (flipped = flip(P, O, x4)))
		{
			Score = MAX(Score, -NegaMax_3(O ^ flipped, P ^ (1ULL << x4) ^ flipped, NodeCounter, x1, x2, x3));
			played = true;
		}

		if (played) return Score;

		++NodeCounter;
		Score = 64;

		//Play on x1
		if ((P & NEIGHBOUR(x1)) && (flipped = flip(O, P, x1)))
		{
			Score = MIN(Score, NegaMax_3(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, x2, x3, x4));
			played = true;
		}

		//Play on x2
		if ((P & NEIGHBOUR(x2)) && (flipped = flip(O, P, x2)))
		{
			Score = MIN(Score, NegaMax_3(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, x1, x3, x4));
			played = true;
		}

		//Play on x3
		if ((P & NEIGHBOUR(x3)) && (flipped = flip(O, P, x3)))
		{
			Score = MIN(Score, NegaMax_3(P ^ flipped, O ^ (1ULL << x3) ^ flipped, NodeCounter, x1, x2, x4));
			played = true;
		}

		//Play on x4
		if ((P & NEIGHBOUR(x4)) && (flipped = flip(O, P, x4)))
		{
			Score = MIN(Score, NegaMax_3(P ^ flipped, O ^ (1ULL << x4) ^ flipped, NodeCounter, x1, x2, x3));
			played = true;
		}

		if (played)
			return Score;
		else
			return EvaluateGameOver(P, 4);
	}
	int NegaMax_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter)
	{
		unsigned long long BitBoardEmpty = ~(P | O);
		const unsigned char x1 = BitScanLSB(BitBoardEmpty); RemoveLSB(BitBoardEmpty);
		const unsigned char x2 = BitScanLSB(BitBoardEmpty); RemoveLSB(BitBoardEmpty);
		const unsigned char x3 = BitScanLSB(BitBoardEmpty); RemoveLSB(BitBoardEmpty);
		const unsigned char x4 = BitScanLSB(BitBoardEmpty);

		return NegaMax_4(P, O, NodeCounter, x1, x2, x3, x4);
	}
	int NegaMax_A(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter)
	{
		if (NumberOfEmptyStones(P, O) == 4)
			return NegaMax_4(P, O, NodeCounter);

		unsigned long long BitBoardEmpty = ~(P | O);
		unsigned long long flipped;
		unsigned long Move;
		int Score = -64;
		bool played = false;
		++NodeCounter;

		while (BitBoardEmpty)
		{
			BitScanLSB(&Move, BitBoardEmpty);
			RemoveLSB(BitBoardEmpty);
			if ((O & neighbour[Move]) && (flipped = flip(P, O, Move)))
			{
				Score = MAX(Score, -NegaMax_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter));
				played = true;
			}
		}

		if (played) return Score;

		++NodeCounter;
		Score = 64;
		BitBoardEmpty = ~(P | O);

		while (BitBoardEmpty)
		{
			BitScanLSB(&Move, BitBoardEmpty);
			RemoveLSB(BitBoardEmpty);
			if ((P & neighbour[Move]) && (flipped = flip(O, P, Move)))
			{
				Score = MIN(Score, NegaMax_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter));
				played = true;
			}
		}

		if (played)
			return Score;
		else
			return EvaluateGameOver(P, 2);
	}
	int NegaMax(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter)
	{
		const unsigned char Empties = NumberOfEmptyStones(P, O);
		if (Empties == 6)
			return NegaMax_A(P, O, NodeCounter);

		unsigned long long flipped;
		unsigned long long BitBoardPossible = PossibleMoves(P, O);
		unsigned long Move;
		int Score = -64;
		++NodeCounter;

		if (!BitBoardPossible){
			if (HasMoves(O, P))
				return -NegaMax(O, P, NodeCounter);
			else //Game is over
				return EvaluateGameOver(P, Empties);
		}

		while (BitBoardPossible)
		{
			BitScanLSB(&Move, BitBoardPossible);
			RemoveLSB(BitBoardPossible);
			flipped = flip(P, O, Move);
			Score = MAX(Score, -NegaMax_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter));
		}

		return Score;
	}

	int ZWS_1(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const unsigned char x)
	{
		int Score;
		unsigned char FlipCount;
		Score = (PopCount(P) << 1) - 62; // PopCount(P) - PopCount(O) + 1
	
		++NodeCounter;
		if (FlipCount = count_last_flip(P, x))
		{
			++NodeCounter;
			return Score + FlipCount;
		}
		else
		{
			if (Score <= alpha) // <==> PopCount(P) - PopCount(O) + 1 <= alpha
				return alpha;
			else
			{
				if (FlipCount = count_last_flip(O, x))
				{
					++NodeCounter;
					return Score - FlipCount - 2;
				}
				else
					return (Score > 0) ? Score : Score - 2;
			}
		}
	}
	int ZWS_2(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const unsigned char x1, const unsigned char x2)
	{
		unsigned long long flipped;
		bool played = false;
		++NodeCounter;

		//Play on x1
		if ((O & NEIGHBOUR(x1)) && (flipped = flip(P, O, x1)))
		{
			if (-ZWS_1(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, -alpha-1, x2) > alpha)
				return alpha+1;
			played = true;
		}

		//Play on x2
		if ((O & NEIGHBOUR(x2)) && (flipped = flip(P, O, x2)))
			return -ZWS_1(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, -alpha-1, x1);

		if (played)
			return alpha;

		++NodeCounter;

		//Play on x1
		if ((P & NEIGHBOUR(x1)) && (flipped = flip(O, P, x1)))
		{
			if (ZWS_1(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, alpha, x2) <= alpha)
				return alpha;
			played = true;
		}

		//Play on x2
		if ((P & NEIGHBOUR(x2)) && (flipped = flip(O, P, x2)))
			return ZWS_1(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, alpha, x1);

		if (played)
			return alpha+1;
		else
			return EvaluateGameOver(P, 2);
	}
	int ZWS_3(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const unsigned char x1, const unsigned char x2, const unsigned char x3)
	{
		//if (QUADRANT_ID(x1) == QUADRANT_ID(x2)) //Play x3 first
		//	std::swap(x1,x3);
		//else if (QUADRANT_ID(x1) == QUADRANT_ID(x3)) //Play x2 first
		//	std::swap(x1,x2);

		unsigned long long flipped;
		bool played = false;
		++NodeCounter;

		//Play on x1
		if ((O & NEIGHBOUR(x1)) && (flipped = flip(P, O, x1)))
		{
			if (-ZWS_2(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, -alpha-1, x2, x3) > alpha)
				return alpha+1;
			played = true;
		}

		//Play on x2
		if ((O & NEIGHBOUR(x2)) && (flipped = flip(P, O, x2)))
		{
			if (-ZWS_2(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, -alpha-1, x1, x3) > alpha)
				return alpha+1;
			played = true;
		}

		//Play on x3
		if ((O & NEIGHBOUR(x3)) && (flipped = flip(P, O, x3)))
			return -ZWS_2(O ^ flipped, P ^ (1ULL << x3) ^ flipped, NodeCounter, -alpha-1, x1, x2);

		if (played)
			return alpha;

		++NodeCounter;

		//Play on x1
		if ((P & NEIGHBOUR(x1)) && (flipped = flip(O, P, x1)))
		{
			if (ZWS_2(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, alpha, x2, x3) <= alpha)
				return alpha;
			played = true;
		}

		//Play on x2
		if ((P & NEIGHBOUR(x2)) && (flipped = flip(O, P, x2)))
		{
			if (ZWS_2(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, alpha, x1, x3) <= alpha)
				return alpha;
			played = true;
		}

		//Play on x3
		if ((P & NEIGHBOUR(x3)) && (flipped = flip(O, P, x3)))
			return ZWS_2(P ^ flipped, O ^ (1ULL << x3) ^ flipped, NodeCounter, alpha, x1, x2);

		if (played)
			return alpha+1;
		else
			return EvaluateGameOver(P, 3);
	}
	int ZWS_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const unsigned char x1, const unsigned char x2, const unsigned char x3, const unsigned char x4)
	{
		unsigned long long flipped;
		bool played = false;
		++NodeCounter;

		//Play on x1
		if ((O & NEIGHBOUR(x1)) && (flipped = flip(P, O, x1)))
		{
			if (-ZWS_3(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, -alpha-1, x2, x3, x4) > alpha)
				return alpha+1;
			played = true;
		}

		//Play on x2
		if ((O & NEIGHBOUR(x2)) && (flipped = flip(P, O, x2)))
		{
			if (-ZWS_3(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, -alpha-1, x1, x3, x4) > alpha)
				return alpha+1;
			played = true;
		}
		
		//Play on x3
		if ((O & NEIGHBOUR(x3)) && (flipped = flip(P, O, x3)))
		{
			if (-ZWS_3(O ^ flipped, P ^ (1ULL << x3) ^ flipped, NodeCounter, -alpha-1, x1, x2, x4) > alpha)
				return alpha+1;
			played = true;
		}

		//Play on x4
		if ((O & NEIGHBOUR(x4)) && (flipped = flip(P, O, x4)))
			return -ZWS_3(O ^ flipped, P ^ (1ULL << x4) ^ flipped, NodeCounter, -alpha-1, x1, x2, x3);

		if (played)
			return alpha;

		++NodeCounter;

		//Play on x1
		if ((P & NEIGHBOUR(x1)) && (flipped = flip(O, P, x1)))
		{
			if (ZWS_3(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, alpha, x2, x3, x4) <= alpha)
				return alpha;
			played = true;
		}

		//Play on x2
		if ((P & NEIGHBOUR(x2)) && (flipped = flip(O, P, x2)))
		{
			if (ZWS_3(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, alpha, x1, x3, x4) <= alpha)
				return alpha;
			played = true;
		}

		//Play on x3
		if ((P & NEIGHBOUR(x3)) && (flipped = flip(O, P, x3)))
		{
			if (ZWS_3(P ^ flipped, O ^ (1ULL << x3) ^ flipped, NodeCounter, alpha, x1, x2, x4) <= alpha)
				return alpha;
			played = true;
		}

		//Play on x4
		if ((P & NEIGHBOUR(x4)) && (flipped = flip(O, P, x4)))
			return ZWS_3(P ^ flipped, O ^ (1ULL << x4) ^ flipped, NodeCounter, alpha, x1, x2, x3);

		if (played)
			return alpha+1;
		else
			return EvaluateGameOver(P, 4);
	}
	int ZWS_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const int alpha)
	{
		unsigned long long BitBoardEmpty = ~(P | O);
		unsigned char x1 = BitScanLSB(BitBoardEmpty); RemoveLSB(BitBoardEmpty);
		unsigned char x2 = BitScanLSB(BitBoardEmpty); RemoveLSB(BitBoardEmpty);
		unsigned char x3 = BitScanLSB(BitBoardEmpty); RemoveLSB(BitBoardEmpty);
		unsigned char x4 = BitScanLSB(BitBoardEmpty);

		if (parity & QUADRANT_ID(x1)){ // Either 1 or 3 empties in x1's quadrant
			if (parity & QUADRANT_ID(x2)){ // Either 1 or 3 empties in x2's quadrant
				if (parity & QUADRANT_ID(x3)){ // Either 1 or 3 empties in x3's quadrant
					if ((QUADRANT_ID(x1) == QUADRANT_ID(x3)) && (QUADRANT_ID(x1) == QUADRANT_ID(x4)))
						std::swap(x1,x2);
					else if ((QUADRANT_ID(x1) == QUADRANT_ID(x2)) && (QUADRANT_ID(x1) == QUADRANT_ID(x4)))
						std::swap(x1,x3);
					else if ((QUADRANT_ID(x1) == QUADRANT_ID(x2)) && (QUADRANT_ID(x1) == QUADRANT_ID(x3)))
						std::swap(x1,x4);
				}
			}
			else{ // (x1) (..) (x2 ..)
				if (parity & QUADRANT_ID(x3)) // (x1) (x3) (x2 x4)
					std::swap(x2,x3); // Now playing x1, x3, x2, x4
				else // (x1) (x4) (x2 x3)
					std::swap(x2,x4); // Now playing x1, x4, x3, x2
			}
		}
		else { // Either 2 or 4 in x1's quadrant 
			if (parity & QUADRANT_ID(x2)){ // (x2) (..) (x1 ..)
				if (parity & QUADRANT_ID(x3)) // (x2) (x3) (x1 x4)
					std::swap(x1,x3); // Now playing x3, x2, x1, x4
				else // (x2) (x4) (x1 x3)
					std::swap(x1,x4); // Now playing x4, x2, x3, x1
			}
			else if (parity & QUADRANT_ID(x3)){ // (x3) (x4) (x1 x2)
				std::swap(x1,x3);
				std::swap(x2,x4); // Now playing x3, x4, x1, x2
			}
		}

		return ZWS_4(P, O, NodeCounter, alpha, x1, x2, x3, x4);
	}
	int ZWS_A(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const unsigned char NumberOfEmptyStones, int alpha)
	{
		if (NumberOfEmptyStones == 4)
			return ZWS_4(P, O, NodeCounter, parity, alpha);

		// #.####.#
		// ........
		// #.####.#
		// #.#..#.#
		// #.#..#.#
		// #.####.#
		// ........
		// #.####.#
		const unsigned long long PATTERN_FIRST = 0xBD00BDA5A5BD00BDULL;
		const unsigned long long BitBoardEmpty = ~(P | O);
		const unsigned long long BitBoardParity = quadrant[parity];
		unsigned long long BitBoardTmp, flipped;
		unsigned long Move;
		bool played = false;
		++NodeCounter;

		BitBoardTmp = BitBoardEmpty & BitBoardParity & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((O & neighbour[Move]) && (flipped = flip(P, O, Move)))
			{
				if (-ZWS_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
					return alpha+1;
				played = true;
			}
		}

		BitBoardTmp = BitBoardEmpty & BitBoardParity & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((O & neighbour[Move]) && (flipped = flip(P, O, Move)))
			{
				if (-ZWS_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
					return alpha+1;
				played = true;
			}
		}

		BitBoardTmp = BitBoardEmpty & ~BitBoardParity & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((O & neighbour[Move]) && (flipped = flip(P, O, Move)))
			{
				if (-ZWS_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
					return alpha+1;
				played = true;
			}
		}

		BitBoardTmp = BitBoardEmpty & ~BitBoardParity & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((O & neighbour[Move]) && (flipped = flip(P, O, Move)))
			{
				if (-ZWS_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
					return alpha+1;
				played = true;
			}
		}

		if (played)
			return alpha;
		
		++NodeCounter;

		BitBoardTmp = BitBoardEmpty & QUADRANT(parity) & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((P & neighbour[Move]) && (flipped = flip(O, P, Move)))
			{
				if (ZWS_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, alpha) <= alpha)
					return alpha;
				played = true;
			}
		}

		BitBoardTmp = BitBoardEmpty & QUADRANT(parity) & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((P & neighbour[Move]) && (flipped = flip(O, P, Move)))
			{
				if (ZWS_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, alpha) <= alpha)
					return alpha;
				played = true;
			}
		}

		BitBoardTmp = BitBoardEmpty & ~QUADRANT(parity) & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((P & neighbour[Move]) && (flipped = flip(O, P, Move)))
			{
				if (ZWS_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, alpha) <= alpha)
					return alpha;
				played = true;
			}
		}

		BitBoardTmp = BitBoardEmpty & ~QUADRANT(parity) & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((P & neighbour[Move]) && (flipped = flip(O, P, Move)))
			{
				if (ZWS_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, alpha) <= alpha)
					return alpha;
				played = true;
			}
		}

		if (played)
			return alpha+1;
		else
			return EvaluateGameOver(P, NumberOfEmptyStones);
	}
	int ZWS_B(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const unsigned char NumberOfEmptyStones, int alpha)
	{
		if (NumberOfEmptyStones == 6)
			return ZWS_A(P, O, NodeCounter, parity, NumberOfEmptyStones, alpha);

		int value = -128;
		unsigned long Move;
		unsigned long long BitBoardTmp, BitBoardPossible, flipped;
		BitBoardPossible = PossibleMoves(P, O);
		++NodeCounter;
		
		if (!BitBoardPossible)
		{
			if (HasMoves(O, P))
				return -ZWS_B(O, P, NodeCounter, parity, NumberOfEmptyStones, -alpha-1);
			else{ //Game is over
				++NodeCounter;
				return BIND(EvaluateGameOver(P, NumberOfEmptyStones), alpha, alpha+1);
			}
		}

		// #......#
		// ........
		// ..####..
		// ..#..#..
		// ..#..#..
		// ..####..
		// ........
		// #......#
		const unsigned long long PATTERN_FIRST = 0x81003C24243C0081ULL;
		const unsigned long long BitBoardParity = QUADRANT(parity);

		BitBoardTmp = BitBoardPossible & BitBoardParity & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			flipped = flip(P, O, Move);
			if (-ZWS_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
				return alpha+1;
		}

		BitBoardTmp = BitBoardPossible & BitBoardParity & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			flipped = flip(P, O, Move);
			if (-ZWS_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
				return alpha+1;
		}

		BitBoardTmp = BitBoardPossible & ~BitBoardParity & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			flipped = flip(P, O, Move);
			if (-ZWS_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
				return alpha+1;
		}

		BitBoardTmp = BitBoardPossible & ~BitBoardParity & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			flipped = flip(P, O, Move);
			if (-ZWS_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
				return alpha+1;
		}

		return alpha;
	}
	int ZWS(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, unsigned char selectivity)
	{
		if (search.TestTimeOut())
			return alpha;

		const unsigned char Empties = NumberOfEmptyStones(P, O);
		if (Empties == 8)
			return ZWS_B(P, O, search.NodeCounter, parity(P, O), Empties, alpha);

		unsigned char BestMove = 64;
		int value;
		unsigned long long flipped;
		unsigned long long BitBoardPossible = PossibleMoves(P, O);

		++search.NodeCounter;

		if (!BitBoardPossible){
			if (HasMoves(O, P))
				return -ZWS(search, O, P, -alpha-1, selectivity);
			else{ //Game is over
				++search.NodeCounter;
				return BIND(EvaluateGameOver(P, Empties), alpha, alpha+1);
			}
		}

		if (StabilityCutoff(P, O, Empties, alpha))
			return alpha;

		HashTableValueType htValue;
		if (search.HashTableLookUp(P, O, htValue))
		{
			if (UseHashTableValue(htValue, alpha, alpha + 1, Empties, selectivity, value))
				return value;
		}

		if (MPC(search, P, O, alpha, Empties, selectivity, value))
			return value;
	
		unsigned long long LocalNodeCounter = search.NodeCounter;

		//HashTableValueType htValue;
		if (search.HashTableLookUp(P, O, htValue))
		{
			if (UseHashTableValue(htValue, alpha, alpha + 1, Empties, selectivity, value))
				return value;

			if (htValue.PV != 64)
			{
				assert((BitBoardPossible & (1ULL << htValue.PV)) != 0);
				BitBoardPossible ^= (1ULL << htValue.PV);
				flipped = flip(P, O, htValue.PV);
				value = -ZWS(search, O ^ flipped, P ^ (1ULL << htValue.PV) ^ flipped, -alpha-1, selectivity);
				if (value > alpha)
				{
					if (!search.TestTimeOut())
						search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, selectivity, alpha+1, 64, htValue.PV, htValue.AV);
					return alpha+1;
				}
			}

			if (htValue.AV != 64)
			{
				assert((BitBoardPossible & (1ULL << htValue.AV)) != 0);
				BitBoardPossible ^= (1ULL << htValue.AV);
				flipped = flip(P, O, htValue.AV);
				value = -ZWS(search, O ^ flipped, P ^ (1ULL << htValue.AV) ^ flipped, -alpha-1, selectivity);
				if (value > alpha)
				{
					if (!search.TestTimeOut())
						search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, selectivity, alpha+1, 64, htValue.AV, htValue.PV);
					return alpha+1;
				}
			}
		}

		CMoveList mvList(search, P, O, BitBoardPossible, Empties, htValue);
		for (auto& mv : mvList)
		{
			if (search.TestTimeOut())
				return alpha;
			value = -ZWS(search, mv.P, mv.O, -alpha-1, selectivity);
			if (value > alpha)
			{
				if (!search.TestTimeOut())
					search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, selectivity, alpha+1, 64, mv.move, 64);
				return alpha+1;
			}
		}
		if (!search.TestTimeOut())
			search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, selectivity, -64, alpha, 64, 64);
		return alpha;
	}
	
	int PVS_1(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const int x) 
	{
		int Score, FlipCount;
		Score = (PopCount(P) << 1) - 62; // PopCount(P) - PopCount(O) + 1
	
		++NodeCounter;
		if (FlipCount = count_last_flip(P, x))
		{
			++NodeCounter;
			return Score + FlipCount;
		}
		else
		{
			if (Score <= alpha)
				return alpha;
			else
			{
				if (FlipCount = count_last_flip(O, x))
				{
					++NodeCounter;
					return Score - FlipCount - 2;
				}
				else
					return (Score > 0) ? Score : Score - 2;
			}
		}
	}
	int PVS_1(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const int x, CLine & pline) 
	{
		if (pline.size == 0) return PVS_1(P, O, NodeCounter, alpha, x);

		int Score, FlipCount;
		Score = (PopCount(P) << 1) - 62; // PopCount(P) - PopCount(O) + 1
	
		++NodeCounter;
		if (FlipCount = count_last_flip(P, x))
		{
			++NodeCounter;
			pline.line[0] = x;
			return Score + FlipCount;
		}
		else
		{
			if (FlipCount = count_last_flip(O, x))
			{
				++NodeCounter;
				pline.line[0] = x;
				return Score - FlipCount - 2;
			}
			else
			{
				pline.line[0] = 64;
				return (Score > 0) ? Score : Score - 2;
			}
		}
	}
	int PVS_2(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta, const int x1, const int x2)
	{
		//if(EVALUATION_ORDER(x1) < EVALUATION_ORDER(x2))
		//	std::swap(x1,x2);
		
		int value = -128;
		unsigned long long flipped;
		++NodeCounter;

		//Play on x1
		if ((O & NEIGHBOUR(x1)) && (flipped = flip(P, O, x1)))
		{
			value = -PVS_1(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, -beta, x2);
			if (value >= beta) return beta;
			if (value > alpha) alpha = value;
		}

		//Play on x2
		if ((O & NEIGHBOUR(x2)) && (flipped = flip(P, O, x2)))
		{
			value = -PVS_1(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, -beta, x1);
			if (value >= beta) return beta;
			if (value > alpha) alpha = value;
			return alpha;
		}

		if (value != -128)
			return alpha;

		++NodeCounter;
		//Play on x1
		if ((P & NEIGHBOUR(x1)) && (flipped = flip(O, P, x1)))
		{
			value = PVS_1(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, alpha, x2);
			if (value <= alpha) return alpha;
			if (value < beta) beta = value;
		}

		//Play on x2
		if ((P & NEIGHBOUR(x2)) && (flipped = flip(O, P, x2)))
		{
			value = PVS_1(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, alpha, x1);
			if (value <= alpha) return alpha;
			if (value < beta) beta = value;
			return beta;
		}

		if (value == -128) //Nobody could play
			return EvaluateGameOver(P, 2);
		else
			return beta;
	}
	int PVS_2(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta, const int x1, const int x2, CLine & pline)
	{
		if (pline.size == 0) return PVS_2(P, O, NodeCounter, alpha, beta, x1, x2);

		//if(EVALUATION_ORDER(x1) < EVALUATION_ORDER(x2))
		//	std::swap(x1,x2);
		
		int value = -128;
		unsigned long long flipped;
		CLine line(pline.size-1);
		++NodeCounter;

		//Play on x1
		if ((O & NEIGHBOUR(x1)) && (flipped = flip(P, O, x1)))
		{
			value = -PVS_1(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, -beta, x2, line);
			if (value >= beta) return beta;
			if (value > alpha){
				pline.NewPV(x1, line);
				alpha = value;
			}
		}

		//Play on x2
		if ((O & NEIGHBOUR(x2)) && (flipped = flip(P, O, x2)))
		{
			value = -PVS_1(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, -beta, x1, line);
			if (value >= beta) return beta;
			if (value > alpha){
				pline.NewPV(x2, line);
				alpha = value;
			}
			return alpha;
		}

		if (value != -128)
			return alpha;

		++NodeCounter;
		//Play on x1
		if ((P & NEIGHBOUR(x1)) && (flipped = flip(O, P, x1)))
		{
			value = PVS_1(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, alpha, x2, line);
			if (value <= alpha) return alpha;
			if (value < beta){
				pline.NewPV(x1, line);
				beta = value;
			}
		}

		//Play on x2
		if ((P & NEIGHBOUR(x2)) && (flipped = flip(O, P, x2)))
		{
			value = PVS_1(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, alpha, x1, line);
			if (value <= alpha) return alpha;
			if (value < beta){
				pline.NewPV(x2, line);
				beta = value;
			}
			return beta;
		}

		if (value == -128){ //Nobody could play
			pline.NoMoves();
			return EvaluateGameOver(P, 2);
		}
		else
			return beta;
	}
	int PVS_3(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta, const int x1, const int x2, const int x3)
	{
		//if (QUADRANT_ID(x1) == QUADRANT_ID(x2)) //Play x3 first
		//	std::swap(x1,x3);
		//else if (QUADRANT_ID(x1) == QUADRANT_ID(x3)) //Play x2 first
		//	std::swap(x1,x2);
		
		int value = -128;
		unsigned long long flipped;
		++NodeCounter;

		//Play on x1
		if ((O & NEIGHBOUR(x1)) && (flipped = flip(P, O, x1)))
		{
			value = -PVS_2(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, -beta, -alpha, x2, x3);
			if (value >= beta) return beta;
			if (value > alpha) alpha = value;
		}

		//Play on x2
		if ((O & NEIGHBOUR(x2)) && (flipped = flip(P, O, x2)))
		{
			value = -PVS_2(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, -beta, -alpha, x1, x3);
			if (value >= beta) return beta;
			if (value > alpha) alpha = value;
		}

		//Play on x3
		if ((O & NEIGHBOUR(x3)) && (flipped = flip(P, O, x3)))
		{
			value = -PVS_2(O ^ flipped, P ^ (1ULL << x3) ^ flipped, NodeCounter, -beta, -alpha, x1, x2);
			if (value >= beta) return beta;
			if (value > alpha) alpha = value;
			return alpha;
		}

		if (value != -128)
			return alpha;
		
		++NodeCounter;

		//Play on x1
		if ((P & NEIGHBOUR(x1)) && (flipped = flip(O, P, x1)))
		{
			value = PVS_2(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, alpha, beta, x2, x3);
			if (value <= alpha)	return alpha;
			if (value < beta) beta = value;
		}

		//Play on x2
		if ((P & NEIGHBOUR(x2)) && (flipped = flip(O, P, x2)))
		{
			value = PVS_2(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, alpha, beta, x1, x3);
			if (value <= alpha)	return alpha;
			if (value < beta) beta = value;
		}

		//Play on x3
		if ((P & NEIGHBOUR(x3)) && (flipped = flip(O, P, x3)))
		{
			value = PVS_2(P ^ flipped, O ^ (1ULL << x3) ^ flipped, NodeCounter, alpha, beta, x1, x2);
			if (value <= alpha)	return alpha;
			if (value < beta) beta = value;
			return beta;
		}

		if (value == -128) //Nobody could play
			return EvaluateGameOver(P, 3);
		else
			return beta;
	}
	int PVS_3(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta, const int x1, const int x2, const int x3, CLine & pline)
	{
		if (pline.size == 0) return PVS_3(P, O, NodeCounter, alpha, beta, x1, x2, x3);

		//if (QUADRANT_ID(x1) == QUADRANT_ID(x2)) //Play x3 first
		//	std::swap(x1,x3);
		//else if (QUADRANT_ID(x1) == QUADRANT_ID(x3)) //Play x2 first
		//	std::swap(x1,x2);
		
		int value = -128;
		unsigned long long flipped;
		CLine line(pline.size-1);
		++NodeCounter;

		//Play on x1
		if ((O & NEIGHBOUR(x1)) && (flipped = flip(P, O, x1)))
		{
			value = -PVS_2(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, -beta, -alpha, x2, x3, line);
			if (value >= beta) return beta;
			if (value > alpha){
				pline.NewPV(x1, line);
				alpha = value;
			}
		}

		//Play on x2
		if ((O & NEIGHBOUR(x2)) && (flipped = flip(P, O, x2)))
		{
			value = -PVS_2(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, -beta, -alpha, x1, x3, line);
			if (value >= beta) return beta;
			if (value > alpha){
				pline.NewPV(x2, line);
				alpha = value;
			}
		}

		//Play on x3
		if ((O & NEIGHBOUR(x3)) && (flipped = flip(P, O, x3)))
		{
			value = -PVS_2(O ^ flipped, P ^ (1ULL << x3) ^ flipped, NodeCounter, -beta, -alpha, x1, x2, line);
			if (value >= beta) return beta;
			if (value > alpha){
				pline.NewPV(x3, line);
				alpha = value;
			}
			return alpha;
		}

		if (value != -128)
			return alpha;
		
		++NodeCounter;

		//Play on x1
		if ((P & NEIGHBOUR(x1)) && (flipped = flip(O, P, x1)))
		{
			value = PVS_2(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, alpha, beta, x2, x3, line);
			if (value <= alpha)	return alpha;
			if (value < beta){
				pline.NewPV(x1, line);
				beta = value;
			}
		}

		//Play on x2
		if ((P & NEIGHBOUR(x2)) && (flipped = flip(O, P, x2)))
		{
			value = PVS_2(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, alpha, beta, x1, x3, line);
			if (value <= alpha)	return alpha;
			if (value < beta){
				pline.NewPV(x2, line);
				beta = value;
			}
		}

		//Play on x3
		if ((P & NEIGHBOUR(x3)) && (flipped = flip(O, P, x3)))
		{
			value = PVS_2(P ^ flipped, O ^ (1ULL << x3) ^ flipped, NodeCounter, alpha, beta, x1, x2, line);
			if (value <= alpha)	return alpha;
			if (value < beta){
				pline.NewPV(x3, line);
				beta = value;
			}
			return beta;
		}

		if (value == -128){ //Nobody could play
			pline.NoMoves();
			return EvaluateGameOver(P, 3);
		}
		else
			return beta;
	}
	int PVS_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta, const int x1, const int x2, const int x3, const int x4)
	{
		int value = -128;
		unsigned long long flipped;
		++NodeCounter;
		
		//Play on x1
		if ((O & NEIGHBOUR(x1)) && (flipped = flip(P, O, x1)))
		{
			value = -PVS_3(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, -beta, -alpha, x2, x3, x4);
			if (value >= beta) return beta;
			if (value > alpha) alpha = value;
		}

		//Play on x2
		if ((O & NEIGHBOUR(x2)) && (flipped = flip(P, O, x2)))
		{
			value = -PVS_3(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, -beta, -alpha, x1, x3, x4);
			if (value >= beta) return beta;
			if (value > alpha) alpha = value;
		}

		//Play on x3
		if ((O & NEIGHBOUR(x3)) && (flipped = flip(P, O, x3)))
		{
			value = -PVS_3(O ^ flipped, P ^ (1ULL << x3) ^ flipped, NodeCounter, -beta, -alpha, x1, x2, x4);
			if (value >= beta) return beta;
			if (value > alpha) alpha = value;
		}

		//Play on x4
		if ((O & NEIGHBOUR(x4)) && (flipped = flip(P, O, x4)))
		{
			value = -PVS_3(O ^ flipped, P ^ (1ULL << x4) ^ flipped, NodeCounter, -beta, -alpha, x1, x2, x3);
			if (value >= beta) return beta;
			if (value > alpha) alpha = value;
			return alpha;
		}

		if (value != -128)
			return alpha;

		++NodeCounter;

		//Play on x1
		if ((P & NEIGHBOUR(x1)) && (flipped = flip(O, P, x1)))
		{
			value = PVS_3(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, alpha, beta, x2, x3, x4);
			if (value <= alpha)	return alpha;
			if (value < beta) beta = value;
		}

		//Play on x2
		if ((P & NEIGHBOUR(x2)) && (flipped = flip(O, P, x2)))
		{
			value = PVS_3(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, alpha, beta, x1, x3, x4);
			if (value <= alpha)	return alpha;
			if (value < beta) beta = value;
		}

		//Play on x3
		if ((P & NEIGHBOUR(x3)) && (flipped = flip(O, P, x3)))
		{
			value = PVS_3(P ^ flipped, O ^ (1ULL << x3) ^ flipped, NodeCounter, alpha, beta, x1, x2, x4);
			if (value <= alpha)	return alpha;
			if (value < beta) beta = value;
		}

		//Play on x4
		if ((P & NEIGHBOUR(x4)) && (flipped = flip(O, P, x4)))
		{
			value = PVS_3(P ^ flipped, O ^ (1ULL << x4) ^ flipped, NodeCounter, alpha, beta, x1, x2, x3);
			if (value <= alpha)	return alpha;
			if (value < beta) beta = value;
			return beta;
		}

		if (value == -128) //Nobody could play
			return EvaluateGameOver(P, 4);
		else
			return beta;
	}
	int PVS_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta, const int x1, const int x2, const int x3, const int x4, CLine & pline)
	{
		if (pline.size == 0) return PVS_4(P, O, NodeCounter, alpha, beta, x1, x2, x3, x4);

		int value = -128;
		unsigned long long flipped;
		CLine line(pline.size-1);
		++NodeCounter;
		
		//Play on x1
		if ((O & NEIGHBOUR(x1)) && (flipped = flip(P, O, x1)))
		{
			value = -PVS_3(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, -beta, -alpha, x2, x3, x4, line);
			if (value >= beta) return beta;
			if (value > alpha){
				pline.NewPV(x1, line);
				alpha = value;
			}
		}

		//Play on x2
		if ((O & NEIGHBOUR(x2)) && (flipped = flip(P, O, x2)))
		{
			value = -PVS_3(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, -beta, -alpha, x1, x3, x4, line);
			if (value >= beta) return beta;
			if (value > alpha){
				pline.NewPV(x2, line);
				alpha = value;
			}
		}

		//Play on x3
		if ((O & NEIGHBOUR(x3)) && (flipped = flip(P, O, x3)))
		{
			value = -PVS_3(O ^ flipped, P ^ (1ULL << x3) ^ flipped, NodeCounter, -beta, -alpha, x1, x2, x4, line);
			if (value >= beta) return beta;
			if (value > alpha){
				pline.NewPV(x3, line);
				alpha = value;
			}
		}

		//Play on x4
		if ((O & NEIGHBOUR(x4)) && (flipped = flip(P, O, x4)))
		{
			value = -PVS_3(O ^ flipped, P ^ (1ULL << x4) ^ flipped, NodeCounter, -beta, -alpha, x1, x2, x3, line);
			if (value >= beta) return beta;
			if (value > alpha){
				pline.NewPV(x4, line);
				alpha = value;
			}
			return alpha;
		}

		if (value != -128)
			return alpha;

		++NodeCounter;

		//Play on x1
		if ((P & NEIGHBOUR(x1)) && (flipped = flip(O, P, x1)))
		{
			value = PVS_3(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, alpha, beta, x2, x3, x4, line);
			if (value <= alpha)	return alpha;
			if (value < beta){
				pline.NewPV(x1, line);
				beta = value;
			}
		}

		//Play on x2
		if ((P & NEIGHBOUR(x2)) && (flipped = flip(O, P, x2)))
		{
			value = PVS_3(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, alpha, beta, x1, x3, x4, line);
			if (value <= alpha)	return alpha;
			if (value < beta){
				pline.NewPV(x2, line);
				beta = value;
			}
		}

		//Play on x3
		if ((P & NEIGHBOUR(x3)) && (flipped = flip(O, P, x3)))
		{
			value = PVS_3(P ^ flipped, O ^ (1ULL << x3) ^ flipped, NodeCounter, alpha, beta, x1, x2, x4, line);
			if (value <= alpha)	return alpha;
			if (value < beta){
				pline.NewPV(x3, line);
				beta = value;
			}
		}

		//Play on x4
		if ((P & NEIGHBOUR(x4)) && (flipped = flip(O, P, x4)))
		{
			value = PVS_3(P ^ flipped, O ^ (1ULL << x4) ^ flipped, NodeCounter, alpha, beta, x1, x2, x3, line);
			if (value <= alpha)	return alpha;
			if (value < beta){
				pline.NewPV(x4, line);
				beta = value;
			}
			return beta;
		}

		if (value == -128){ //Nobody could play
			pline.NoMoves();
			return EvaluateGameOver(P, 4);
		}
		else
			return beta;
	}
	int PVS_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const int alpha, const int beta)
	{
		unsigned long long BitBoardEmpty = ~(P | O);
		int x1 = BitScanLSB(BitBoardEmpty); RemoveLSB(BitBoardEmpty);
		int x2 = BitScanLSB(BitBoardEmpty); RemoveLSB(BitBoardEmpty);
		int x3 = BitScanLSB(BitBoardEmpty); RemoveLSB(BitBoardEmpty);
		int x4 = BitScanLSB(BitBoardEmpty);

		if (parity & QUADRANT_ID(x1)){ // Either 1 or 3 in x1's quadrant
			if (parity & QUADRANT_ID(x2)){ // Either 1 or 3 in x2's quadrant
				if ((parity & QUADRANT_ID(x3)) && (parity != 15)){ // (..) (.. .. ..)
					if ((QUADRANT_ID(x1) == QUADRANT_ID(x3)) && (QUADRANT_ID(x1) == QUADRANT_ID(x4)))
						std::swap(x1,x2);
					else if ((QUADRANT_ID(x1) == QUADRANT_ID(x2)) && (QUADRANT_ID(x1) == QUADRANT_ID(x4)))
						std::swap(x1,x3);
					else if ((QUADRANT_ID(x1) == QUADRANT_ID(x2)) && (QUADRANT_ID(x1) == QUADRANT_ID(x3)))
						std::swap(x1,x4);
				}
			}
			else{ // (x1) (..) (x2 ..)
				if (parity & QUADRANT_ID(x3)) // (x1) (x3) (x2 x4)
					std::swap(x2,x3); // Now playing x1, x3, x2, x4
				else // (x1) (x4) (x2 x3)
					std::swap(x2,x4); // Now playing x1, x4, x3, x2
			}
		}
		else { // Either 2 or 4 in x1's quadrant 
			if (parity & QUADRANT_ID(x2)){ // (x2) (..) (x1 ..)
				if (parity & QUADRANT_ID(x3)) // (x2) (x3) (x1 x4)
					std::swap(x1,x3); // Now playing x3, x2, x1, x4
				else // (x2) (x4) (x1 x3)
					std::swap(x1,x4); // Now playing x4, x2, x3, x1
			}
			else if (parity & QUADRANT_ID(x3)){ // (x3) (x4) (x1 x2)
				std::swap(x1,x3);
				std::swap(x2,x3); // Now playing x3, x4, x1, x2
			}
		}

		return PVS_4(P, O, NodeCounter, alpha, beta, x1, x2, x3, x4);
	}
	int PVS_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const int alpha, const int beta, CLine & pline)
	{
		unsigned long long BitBoardEmpty = ~(P | O);
		int x1 = BitScanLSB(BitBoardEmpty); RemoveLSB(BitBoardEmpty);
		int x2 = BitScanLSB(BitBoardEmpty); RemoveLSB(BitBoardEmpty);
		int x3 = BitScanLSB(BitBoardEmpty); RemoveLSB(BitBoardEmpty);
		int x4 = BitScanLSB(BitBoardEmpty);

		if (parity & QUADRANT_ID(x1)){ // Either 1 or 3 in x1's quadrant
			if (parity & QUADRANT_ID(x2)){ // Either 1 or 3 in x2's quadrant
				if ((parity & QUADRANT_ID(x3)) && (parity != 15)){ // (..) (.. .. ..)
					if ((QUADRANT_ID(x1) == QUADRANT_ID(x3)) && (QUADRANT_ID(x1) == QUADRANT_ID(x4)))
						std::swap(x1,x2);
					else if ((QUADRANT_ID(x1) == QUADRANT_ID(x2)) && (QUADRANT_ID(x1) == QUADRANT_ID(x4)))
						std::swap(x1,x3);
					else if ((QUADRANT_ID(x1) == QUADRANT_ID(x2)) && (QUADRANT_ID(x1) == QUADRANT_ID(x3)))
						std::swap(x1,x4);
				}
			}
			else{ // (x1) (..) (x2 ..)
				if (parity & QUADRANT_ID(x3)) // (x1) (x3) (x2 x4)
					std::swap(x2,x3); // Now playing x1, x3, x2, x4
				else // (x1) (x4) (x2 x3)
					std::swap(x2,x4); // Now playing x1, x4, x3, x2
			}
		}
		else { // Either 2 or 4 in x1's quadrant 
			if (parity & QUADRANT_ID(x2)){ // (x2) (..) (x1 ..)
				if (parity & QUADRANT_ID(x3)) // (x2) (x3) (x1 x4)
					std::swap(x1,x3); // Now playing x3, x2, x1, x4
				else // (x2) (x4) (x1 x3)
					std::swap(x1,x4); // Now playing x4, x2, x3, x1
			}
			else if (parity & QUADRANT_ID(x3)){ // (x3) (x4) (x1 x2)
				std::swap(x1,x3);
				std::swap(x2,x3); // Now playing x3, x4, x1, x2
			}
		}

		return PVS_4(P, O, NodeCounter, alpha, beta, x1, x2, x3, x4, pline);
	}
	int PVS_A(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const unsigned char Empties, int alpha, int beta)
	{
		if (Empties == 4)
			return PVS_4(P, O, NodeCounter, parity, alpha, beta);

		// #.####.#
		// ........
		// #.####.#
		// #.#..#.#
		// #.#..#.#
		// #.####.#
		// ........
		// #.####.#
		const unsigned long long PATTERN_FIRST = 0xBD00BDA5A5BD00BDULL;
		const unsigned long long BitBoardEmpty = ~(P | O);
		const unsigned long long BitBoardParity = QUADRANT(parity);
		unsigned long long BitBoardTmp, flipped;
		unsigned long Move;
		int value = -128;
		++NodeCounter;

		BitBoardTmp = BitBoardEmpty & BitBoardParity & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((O & neighbour[Move]) && (flipped = flip(P, O, Move)))
			{
				value = -PVS_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, -beta, -alpha);
				if (value >= beta) return beta;
				if (value > alpha) alpha = value;
			}
		}

		BitBoardTmp = BitBoardEmpty & BitBoardParity & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((O & neighbour[Move]) && (flipped = flip(P, O, Move)))
			{
				value = -PVS_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, -beta, -alpha);
				if (value >= beta) return beta;
				if (value > alpha) alpha = value;
			}
		}

		BitBoardTmp = BitBoardEmpty & ~BitBoardParity & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((O & neighbour[Move]) && (flipped = flip(P, O, Move)))
			{
				value = -PVS_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, -beta, -alpha);
				if (value >= beta) return beta;
				if (value > alpha) alpha = value;
			}
		}

		BitBoardTmp = BitBoardEmpty & ~BitBoardParity & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((O & neighbour[Move]) && (flipped = flip(P, O, Move)))
			{
				value = -PVS_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, -beta, -alpha);
				if (value >= beta) return beta;
				if (value > alpha) alpha = value;
			}
		}

		if (value != -128)
			return alpha;
		
		value = 128;
		++NodeCounter;

		BitBoardTmp = BitBoardEmpty & QUADRANT(parity) & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((P & neighbour[Move]) && (flipped = flip(O, P, Move)))
			{
				value = PVS_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, alpha, beta);
				if (value <= alpha) return alpha;
				if (value < beta) beta = value;
			}
		}

		BitBoardTmp = BitBoardEmpty & QUADRANT(parity) & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((P & neighbour[Move]) && (flipped = flip(O, P, Move)))
			{
				value = PVS_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, alpha, beta);
				if (value <= alpha) return alpha;
				if (value < beta) beta = value;
			}
		}

		BitBoardTmp = BitBoardEmpty & ~QUADRANT(parity) & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((P & neighbour[Move]) && (flipped = flip(O, P, Move)))
			{
				value = PVS_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, alpha, beta);
				if (value <= alpha) return alpha;
				if (value < beta) beta = value;
			}
		}

		BitBoardTmp = BitBoardEmpty & ~QUADRANT(parity) & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((P & neighbour[Move]) && (flipped = flip(O, P, Move)))
			{
				value = PVS_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, alpha, beta);
				if (value <= alpha) return alpha;
				if (value < beta) beta = value;
			}
		}

		if (value == 128)
			return EvaluateGameOver(P, Empties);
		else
			return beta;
	}
	int PVS_A(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const unsigned char Empties, int alpha, int beta, CLine & pline)
	{
		if (pline.size == 0) return PVS_A(P, O, NodeCounter, parity, Empties, alpha, beta);

		if (Empties == 4)
			return PVS_4(P, O, NodeCounter, parity, alpha, beta, pline);

		// #.####.#
		// ........
		// #.####.#
		// #.#..#.#
		// #.#..#.#
		// #.####.#
		// ........
		// #.####.#
		const unsigned long long PATTERN_FIRST = 0xBD00BDA5A5BD00BDULL;
		const unsigned long long BitBoardEmpty = ~(P | O);
		const unsigned long long BitBoardParity = QUADRANT(parity);
		unsigned long long BitBoardTmp, flipped;
		unsigned long Move;
		int value = -128;
		CLine line(pline.size-1);
		++NodeCounter;

		BitBoardTmp = BitBoardEmpty & BitBoardParity & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((O & neighbour[Move]) && (flipped = flip(P, O, Move)))
			{
				value = -PVS_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, -beta, -alpha, line);
				if (value >= beta) return beta;
				if (value > alpha){
					pline.NewPV(Move, line);
					alpha = value;
				}
			}
		}

		BitBoardTmp = BitBoardEmpty & BitBoardParity & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((O & neighbour[Move]) && (flipped = flip(P, O, Move)))
			{
				value = -PVS_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, -beta, -alpha, line);
				if (value >= beta) return beta;
				if (value > alpha){
					pline.NewPV(Move, line);
					alpha = value;
				}
			}
		}

		BitBoardTmp = BitBoardEmpty & ~BitBoardParity & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((O & neighbour[Move]) && (flipped = flip(P, O, Move)))
			{
				value = -PVS_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, -beta, -alpha, line);
				if (value >= beta) return beta;
				if (value > alpha){
					pline.NewPV(Move, line);
					alpha = value;
				}
			}
		}

		BitBoardTmp = BitBoardEmpty & ~BitBoardParity & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((O & neighbour[Move]) && (flipped = flip(P, O, Move)))
			{
				value = -PVS_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, -beta, -alpha, line);
				if (value >= beta) return beta;
				if (value > alpha){
					pline.NewPV(Move, line);
					alpha = value;
				}
			}
		}

		if (value != -128)
			return alpha;
		
		value = 128;
		++NodeCounter;

		BitBoardTmp = BitBoardEmpty & QUADRANT(parity) & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((P & neighbour[Move]) && (flipped = flip(O, P, Move)))
			{
				value = PVS_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, alpha, beta, line);
				if (value <= alpha) return alpha;
				if (value < beta){
					pline.NewPV(Move, line);
					beta = value;
				}
			}
		}

		BitBoardTmp = BitBoardEmpty & QUADRANT(parity) & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((P & neighbour[Move]) && (flipped = flip(O, P, Move)))
			{
				value = PVS_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, alpha, beta, line);
				if (value <= alpha) return alpha;
				if (value < beta){
					pline.NewPV(Move, line);
					beta = value;
				}
			}
		}

		BitBoardTmp = BitBoardEmpty & ~QUADRANT(parity) & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((P & neighbour[Move]) && (flipped = flip(O, P, Move)))
			{
				value = PVS_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, alpha, beta, line);
				if (value <= alpha) return alpha;
				if (value < beta){
					pline.NewPV(Move, line);
					beta = value;
				}
			}
		}

		BitBoardTmp = BitBoardEmpty & ~QUADRANT(parity) & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			if ((P & neighbour[Move]) && (flipped = flip(O, P, Move)))
			{
				value = PVS_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, alpha, beta, line);
				if (value <= alpha) return alpha;
				if (value < beta){
					pline.NewPV(Move, line);
					beta = value;
				}
			}
		}

		if (value == 128){
			pline.NoMoves();
			return EvaluateGameOver(P, Empties);
		}
		else
			return beta;
	}
	int PVS_B(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const unsigned char Empties, int alpha, int beta)
	{
		if (Empties == 6)
			return PVS_A(P, O, NodeCounter, parity, Empties, alpha, beta);

		int value = -128;
		unsigned long Move;
		unsigned long long BitBoardTmp, BitBoardPossible, flipped;
		BitBoardPossible = PossibleMoves(P, O);
		++NodeCounter;
		
		if (!BitBoardPossible)
		{
			if (HasMoves(O, P))
				return -PVS_B(O, P, NodeCounter, parity, Empties, -beta, -alpha);
			else //Game is over
			{
				++NodeCounter;
				return BIND(EvaluateGameOver(P, Empties), alpha, beta);
			}
		}
		
		// #......#
		// ........
		// ..####..
		// ..#..#..
		// ..#..#..
		// ..####..
		// ........
		// #......#
		const unsigned long long PATTERN_FIRST = 0x81003C24243C0081ULL;
		const unsigned long long BitBoardParity = QUADRANT(parity);

		BitBoardTmp = BitBoardPossible & BitBoardParity & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			flipped = flip(P, O, Move);
			value = -PVS_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, -beta, -alpha);
			if (value >= beta) return beta;
			if (value > alpha) alpha = value;
		}

		BitBoardTmp = BitBoardPossible & BitBoardParity & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			flipped = flip(P, O, Move);
			value = -PVS_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, -beta, -alpha);
			if (value >= beta) return beta;
			if (value > alpha) alpha = value;
		}

		BitBoardTmp = BitBoardPossible & ~BitBoardParity & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			flipped = flip(P, O, Move);
			value = -PVS_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, -beta, -alpha);
			if (value >= beta) return beta;
			if (value > alpha) alpha = value;
		}

		BitBoardTmp = BitBoardPossible & ~BitBoardParity & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			flipped = flip(P, O, Move);
			value = -PVS_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, -beta, -alpha);
			if (value >= beta) return beta;
			if (value > alpha) alpha = value;
		}

		return alpha;
	}
	int PVS_B(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const unsigned char Empties, int alpha, int beta, CLine & pline)
	{
		if (pline.size == 0) return PVS_B(P, O, NodeCounter, parity, Empties, alpha, beta);

		if (Empties == 6)
			return PVS_A(P, O, NodeCounter, parity, Empties, alpha, beta, pline);

		int value = -128;
		unsigned long Move;
		unsigned long long BitBoardTmp, BitBoardPossible, flipped;
		BitBoardPossible = PossibleMoves(P, O);
		++NodeCounter;
		
		if (!BitBoardPossible)
		{
			if (HasMoves(O, P))
				return -PVS_B(O, P, NodeCounter, parity, Empties, -beta, -alpha, pline);
			else //Game is over
			{
				++NodeCounter;
				pline.NoMoves();
				return BIND(EvaluateGameOver(P, Empties), alpha, beta);
			}
		}
		
		CLine line(pline.size-1);

		// #......#
		// ........
		// ..####..
		// ..#..#..
		// ..#..#..
		// ..####..
		// ........
		// #......#
		const unsigned long long PATTERN_FIRST = 0x81003C24243C0081ULL;
		const unsigned long long BitBoardParity = QUADRANT(parity);

		BitBoardTmp = BitBoardPossible & BitBoardParity & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			flipped = flip(P, O, Move);
			value = -PVS_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, -beta, -alpha, line);
			if (value >= beta) return beta;
			if (value > alpha){
				pline.NewPV(Move, line);
				alpha = value;
			}
		}

		BitBoardTmp = BitBoardPossible & BitBoardParity & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			flipped = flip(P, O, Move);
			value = -PVS_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, -beta, -alpha, line);
			if (value >= beta) return beta;
			if (value > alpha){
				pline.NewPV(Move, line);
				alpha = value;
			}
		}

		BitBoardTmp = BitBoardPossible & ~BitBoardParity & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			flipped = flip(P, O, Move);
			value = -PVS_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, -beta, -alpha, line);
			if (value >= beta) return beta;
			if (value > alpha){
				pline.NewPV(Move, line);
				alpha = value;
			}
		}

		BitBoardTmp = BitBoardPossible & ~BitBoardParity & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			BitScanLSB(&Move, BitBoardTmp);
			RemoveLSB(BitBoardTmp);
			flipped = flip(P, O, Move);
			value = -PVS_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), Empties-1, -beta, -alpha, line);
			if (value >= beta) return beta;
			if (value > alpha){
				pline.NewPV(Move, line);
				alpha = value;
			}
		}

		return alpha;
	}

	int PVS(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, int beta, unsigned char selectivity)
	{
		if (search.TestTimeOut())
			return alpha;

		const unsigned char Empties = NumberOfEmptyStones(P, O);
		if (Empties == 8)
			return PVS_B(P, O, search.NodeCounter, parity(P, O), Empties, alpha, beta);

		bool SearchPV = true;
		int value;
		unsigned char BestMove = 64;
		unsigned long long flipped;
		unsigned long long BitBoardPossible = PossibleMoves(P, O);

		++search.NodeCounter;

		if (!BitBoardPossible)
		{
			if (HasMoves(O, P))
				return -PVS(search, O, P, -beta, -alpha, selectivity);
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
			if (UseHashTableValue(htValue, alpha, beta, Empties, selectivity, value))
				return value;

			if (htValue.PV != 64)
			{
				assert((BitBoardPossible & (1ULL << htValue.PV)) != 0);
				BitBoardPossible ^= (1ULL << htValue.PV);
				flipped = flip(P, O, htValue.PV);
				value = -PVS(search, O ^ flipped, P ^ (1ULL << htValue.PV) ^ flipped, -beta, -alpha, selectivity);
				if (value >= beta)
				{
					if (!search.TestTimeOut())
						search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, selectivity, beta, 64, htValue.PV, htValue.AV);
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
				value = -PVS(search, O ^ flipped, P ^ (1ULL << htValue.AV) ^ flipped, -beta, -alpha, selectivity);
				if (value >= beta)
				{
					if (!search.TestTimeOut())
						search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, selectivity, beta, 64, htValue.AV, htValue.PV);
					return beta;
				}
				if (value > alpha)
				{
					alpha = value;
					SearchPV = false;
				}
			}
		}

		CMoveList mvList(search, P, O, BitBoardPossible, Empties, htValue);
		for (auto& mv : mvList)
		{
			if (search.TestTimeOut())
				return alpha;
			if (SearchPV)
				value = -PVS(search, mv.P, mv.O, -beta, -alpha, selectivity);
			else
			{
				value = -ZWS(search, mv.P, mv.O, -alpha-1, selectivity);
				if (value > alpha)
					value = -PVS(search, mv.P, mv.O, -beta, -alpha, selectivity);
			}
			if (value >= beta)
			{
				if (!search.TestTimeOut())
					search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, selectivity, beta, 64, mv.move, 64);
				return beta;
			}
			if (value > alpha)
			{
				BestMove = mv.move;
				alpha = value;
				SearchPV = false;
			}
		}
		if (!search.TestTimeOut())
		{
			if (SearchPV)
				search.HashTableUpdate(P, O, search.NodeCounter - LocalNodeCounter, Empties, selectivity, -64, alpha, 64, 64);
			else
				search.HashTableUpdate(P, O, search.NodeCounter - LocalNodeCounter, Empties, selectivity, alpha, alpha, BestMove, 64);
		}
		return alpha;
	}
	int PVS(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, int beta, unsigned char selectivity, CLine & pline)
	{
		if (search.TestTimeOut())
			return alpha;

		if (pline.size == 0) return PVS(search, P, O, alpha, beta, selectivity);

		const unsigned char Empties = NumberOfEmptyStones(P, O);
		if (Empties == 8)
			return PVS_B(P, O, search.NodeCounter, parity(P, O), Empties, alpha, beta, pline);

		bool SearchPV = true;
		int value;
		bool GetsProbCut;
		unsigned long long flipped;
		unsigned long long BitBoardPossible = PossibleMoves(P, O);

		++search.NodeCounter;

		if (!BitBoardPossible)
		{
			if (HasMoves(O, P))
				return -PVS(search, O, P, -beta, -alpha, selectivity, pline);
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
			if (UseHashTableValue(htValue, alpha, beta, Empties, selectivity, value))
				if ((value < alpha) || (value > beta))
					return value;

			if (htValue.PV != 64)
			{
				assert((BitBoardPossible & (1ULL << htValue.PV)) != 0);
				BitBoardPossible ^= (1ULL << htValue.PV);
				flipped = flip(P, O, htValue.PV);
				GetsProbCut = false;
				value = -PVS(search, O ^ flipped, P ^ (1ULL << htValue.PV) ^ flipped, -beta, -alpha, selectivity, line);
				if (value >= beta)
				{
					if (!search.TestTimeOut()){
						pline.NewPV(htValue.PV, line);
						search.HashTableUpdate(P, O, search.NodeCounter - LocalNodeCounter, Empties, selectivity, beta, 64, htValue.PV, htValue.AV);
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
				GetsProbCut = false;
				value = -PVS(search, O ^ flipped, P ^ (1ULL << htValue.AV) ^ flipped, -beta, -alpha, selectivity, line);
				if (value >= beta)
				{
					if (!search.TestTimeOut()){
						pline.NewPV(htValue.AV, line);
						search.HashTableUpdate(P, O, search.NodeCounter - LocalNodeCounter, Empties, selectivity, beta, 64, htValue.AV, htValue.PV);
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

		CMoveList mvList(search, P, O, BitBoardPossible, Empties, htValue);
		for (auto& mv : mvList)
		{
			if (search.TestTimeOut())
				return alpha;
			if (SearchPV)
				value = -PVS(search, mv.P, mv.O, -beta, -alpha, selectivity, line);
			else
			{
				value = -ZWS(search, mv.P, mv.O, -alpha-1, selectivity);
				if (value > alpha)
					value = -PVS(search, mv.P, mv.O, -beta, -alpha, selectivity, line);
			}
			if (value >= beta)
			{
				if (!search.TestTimeOut()){
					pline.NewPV(mv.move, line);
					search.HashTableUpdate(P, O, search.NodeCounter - LocalNodeCounter, Empties, selectivity, beta, 64, mv.move, 64);
				}
				return beta;
			}
			if (value > alpha)
			{
				if (!search.TestTimeOut())
					pline.NewPV(mv.move, line);
				alpha = value;
				SearchPV = false;
			}
		}
		if (!search.TestTimeOut())
		{
			if (SearchPV)
				search.HashTableUpdate(P, O, search.NodeCounter - LocalNodeCounter, Empties, selectivity, -64, alpha, 64, 64);
			else
				search.HashTableUpdate(P, O, search.NodeCounter - LocalNodeCounter, Empties, selectivity, alpha, alpha, pline.BestMove(), 64);
		}
		return alpha;
	}

	inline int PVS_0(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter)
	{
		++NodeCounter;
		return EvaluateGameOver(P, 0);
	}
	inline int PVS_1(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha)
	{
		const unsigned long long E = ~(P | O);
		const int x1 = BitScanLSB(E);
		return PVS_1(P, O, NodeCounter, alpha, x1);
	}
	inline int PVS_1(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, CLine & pline)
	{
		const unsigned long long E = ~(P | O);
		const int x1 = BitScanLSB(E);
		return PVS_1(P, O, NodeCounter, alpha, x1, pline);
	}
	inline int PVS_2(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const int beta)
	{
		unsigned long long E = ~(P | O);
		const int x1 = BitScanLSB(E); RemoveLSB(E);
		const int x2 = BitScanLSB(E);
		return PVS_2(P, O, NodeCounter, alpha, beta, x1, x2);
	}
	inline int PVS_2(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const int beta, CLine & pline)
	{
		unsigned long long E = ~(P | O);
		const int x1 = BitScanLSB(E); RemoveLSB(E);
		const int x2 = BitScanLSB(E);
		return PVS_2(P, O, NodeCounter, alpha, beta, x1, x2, pline);
	}
	inline int PVS_3(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const int beta)
	{
		unsigned long long E = ~(P | O);
		const int x1 = BitScanLSB(E); RemoveLSB(E);
		const int x2 = BitScanLSB(E); RemoveLSB(E);
		const int x3 = BitScanLSB(E);
		return PVS_3(P, O, NodeCounter, alpha, beta, x1, x2, x3);
	}
	inline int PVS_3(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const int beta, CLine & pline)
	{
		unsigned long long E = ~(P | O);
		const int x1 = BitScanLSB(E); RemoveLSB(E);
		const int x2 = BitScanLSB(E); RemoveLSB(E);
		const int x3 = BitScanLSB(E);
		return PVS_3(P, O, NodeCounter, alpha, beta, x1, x2, x3, pline);
	}
	inline int PVS_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const int beta)
	{
		return PVS_4(P, O, NodeCounter, parity(P, O), alpha, beta);
	}
	inline int PVS_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const int beta, CLine & pline)
	{
		return PVS_4(P, O, NodeCounter, parity(P, O), alpha, beta, pline);
	}
	inline int PVS_A(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const int beta)
	{
		return PVS_A(P, O, NodeCounter, parity(P, O), NumberOfEmptyStones(P, O), alpha, beta);
	}
	inline int PVS_A(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const int beta, CLine & pline)
	{
		return PVS_A(P, O, NodeCounter, parity(P, O), NumberOfEmptyStones(P, O), alpha, beta, pline);
	}
	inline int PVS_B(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const int beta)
	{
		return PVS_B(P, O, NodeCounter, parity(P, O), NumberOfEmptyStones(P, O), alpha, beta);
	}
	inline int PVS_B(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const int beta, CLine & pline)
	{
		return PVS_B(P, O, NodeCounter, parity(P, O), NumberOfEmptyStones(P, O), alpha, beta, pline);
	}
	inline int PVS_C(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, int beta, unsigned char selectivity)
	{
		return PVS(search, P, O, alpha, beta, selectivity);
	}
	inline int PVS_C(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, int beta, unsigned char selectivity, CLine & pline)
	{
		return PVS(search, P, O, alpha, beta, selectivity, pline);
	}
}

// For benchmarking purposes
int EvaluateEnd(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned long long Empties)
{
	switch (Empties)
	{
	case 0: return Endgame::PVS_0(P, O, NodeCounter);
	case 1: return Endgame::PVS_1(P, O, NodeCounter, -64);
	case 2: return Endgame::PVS_2(P, O, NodeCounter, -64, 64);
	case 3: return Endgame::PVS_3(P, O, NodeCounter, -64, 64);
	case 4: return Endgame::PVS_4(P, O, NodeCounter, -64, 64);
	default: throw std::logic_error("Wrong number of empty stones!");
	}
}

int EvaluateEnd(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const unsigned char selectivity)
{
	const unsigned long long Empties = NumberOfEmptyStones(search.P, search.O);
	switch (Empties)
	{
	case  0: return Endgame::PVS_0(P, O, search.NodeCounter);
	case  1: return Endgame::PVS_1(P, O, search.NodeCounter, alpha);
	case  2: return Endgame::PVS_2(P, O, search.NodeCounter, alpha, beta);
	case  3: return Endgame::PVS_3(P, O, search.NodeCounter, alpha, beta);
	case  4: return Endgame::PVS_4(P, O, search.NodeCounter, alpha, beta);
	case  5: return Endgame::PVS_A(P, O, search.NodeCounter, alpha, beta);
	case  6: return Endgame::PVS_A(P, O, search.NodeCounter, alpha, beta);
	case  7: return Endgame::PVS_B(P, O, search.NodeCounter, alpha, beta);
	case  8: return Endgame::PVS_B(P, O, search.NodeCounter, alpha, beta);
	default: return Endgame::PVS_C(search, P, O, alpha, beta, selectivity);
	}
}
int EvaluateEnd(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const unsigned char selectivity, CLine & pline)
{
	if (pline.size == 0) return EvaluateEnd(search, P, O, alpha, beta, selectivity);

	const unsigned long long Empties = NumberOfEmptyStones(search.P, search.O);
	switch (Empties)
	{
	case  0: return Endgame::PVS_0(P, O, search.NodeCounter);
	case  1: return Endgame::PVS_1(P, O, search.NodeCounter, alpha, pline);
	case  2: return Endgame::PVS_2(P, O, search.NodeCounter, alpha, beta, pline);
	case  3: return Endgame::PVS_3(P, O, search.NodeCounter, alpha, beta, pline);
	case  4: return Endgame::PVS_4(P, O, search.NodeCounter, alpha, beta, pline);
	case  5: return Endgame::PVS_A(P, O, search.NodeCounter, alpha, beta, pline);
	case  6: return Endgame::PVS_A(P, O, search.NodeCounter, alpha, beta, pline);
	case  7: return Endgame::PVS_B(P, O, search.NodeCounter, alpha, beta, pline);
	case  8: return Endgame::PVS_B(P, O, search.NodeCounter, alpha, beta, pline);
	default: return Endgame::PVS_C(search, P, O, alpha, beta, selectivity, pline);
	}
}