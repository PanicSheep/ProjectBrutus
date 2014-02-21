#include "game.h"

// A: No calculation of PossibleMoves, just testing if a ply flips stones. Move ordering based on pattern and parity.
// B: Calculating Possible Moves. Move ordering based on pattern and parity
// C: Using hash tables and sophisticated move ordering
// D: Iterative deepening

namespace Endgame
{
	struct CCutOffLimits
	{
		unsigned char d, D;
		float sigma;
		CCutOffLimits(const unsigned char d, const unsigned char D, const float sigma) : d(d), D(D), sigma(sigma) {}
	};

	static const CCutOffLimits MPC_table[] = {
		//CCutOffLimits( 2,  9, 5.7f),
		CCutOffLimits( 2, 11, 5.9f),
		CCutOffLimits( 2, 13, 5.8f),
		CCutOffLimits( 3, 14, 5.7f),
		CCutOffLimits( 4, 15, 5.1f),
		CCutOffLimits( 3, 16, 5.7f),
		CCutOffLimits( 4, 17, 5.1f),
	};

	bool MPC(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, unsigned char Empties, unsigned char selectivity, int & value)
	{
		int bound;

		if (selectivity == 0)
			return false;

		for (int i = 0; i < 5; i++)
		{
			if (Empties == MPC_table[i].D)
			{
				bound = static_cast<int>(static_cast<float>(alpha) + SelectivityTable[selectivity].T * MPC_table[i].sigma + 0.5f);
				if (Midgame::ZWS(search, P, O, bound-1, MPC_table[i].d, 0) >= bound){
					value = alpha+1;
					return true;
				}

				bound = static_cast<int>(static_cast<float>(alpha) - SelectivityTable[selectivity].T * MPC_table[i].sigma + 0.5f);
				if (Midgame::ZWS(search, P, O, bound, MPC_table[i].d, 0) <= bound){
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
		int OwnMinusOpponents = (POP_COUNT(P) << 1) + NumberOfEmptyStones - 64;
		if ((StableStones_corner_and_co(O) != 0) && (OwnMinusOpponents <= stability_cutoff_limits[NumberOfEmptyStones])) //Worth checking stability
			return (64 - static_cast<int>(POP_COUNT(StableStones(P, O)) << 1) <= alpha);
		return false;
	}

	int ZWS_1(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const unsigned char x)
	{
		int Score;
		unsigned char FlipCount;
		Score = (POP_COUNT(P) << 1) - 62; // POP_COUNT(P) - POP_COUNT(O) + 1
	
		++NodeCounter;
		if (FlipCount = count_last_flip(P, x))
		{
			++NodeCounter;
			return Score + FlipCount;
		}
		else
		{
			if (Score <= alpha) // <==> POP_COUNT(P) - POP_COUNT(O) + 1 <= alpha
				return alpha;
			else
			{
				if (FlipCount = count_last_flip(O, x))
				{
					++NodeCounter;
					return Score - FlipCount - 2;
				}
				return (Score > 0) ? Score : Score - 2;
			}
			//if (Score < 0)
			//{
			//	--Score; //Count the empty field as an opponents stone. Or in the case that nobody can play it also acounts for the opponent, since the score is below zero.
			//	if (Score <= alpha) //You can't play and your score is below alpha, so you can't do better than alpha.
			//		return alpha;
			//	else
			//	{
			//		++NodeCounter;
			//		return Score - count_last_flip[x](O);
			//	}
			//}
			//else // Score > 0
			//{
			//	if (Score <= alpha) //You can't play and your score is below alpha, so you can't do better than alpha
			//		return alpha;
			//	else
			//	{
			//		++NodeCounter;
			//		if (FlipCount = count_last_flip[x](O))
			//			return Score - FlipCount - 1;
			//		else
			//			return Score + 1;
			//	}
			//}
		}
	}
	int ZWS_2(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const unsigned char x1, const unsigned char x2)
	{
		unsigned long long flipped;
		bool played = false;
		++NodeCounter;

		//Play on x1
		if (O & NEIGHBOUR(x1))
		{
			if (flipped = flip(P, O, x1))
			{
				if (-ZWS_1(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, -alpha-1, x2) > alpha)
					return alpha+1;
				else
					played = true;
			}
		}

		//Play on x2
		if (O & NEIGHBOUR(x2))
		{
			if (flipped = flip(P, O, x2))
				return -ZWS_1(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, -alpha-1, x1);
		}

		if (!played) //Player couldn't play
		{
			++NodeCounter;
			//Play on x1
			if (P & NEIGHBOUR(x1))
			{
				if (flipped = flip(O, P, x1))
				{
					if (ZWS_1(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, alpha, x2) <= alpha)
						return alpha;
					else
						played = true;
				}
			}

			//Play on x2
			if (P & NEIGHBOUR(x2))
			{
				if (flipped = flip(O, P, x2))
					return ZWS_1(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, alpha, x1);
			}

			if (played)
				return alpha+1;
			else
				return EvaluateGameOver(P, 2);
			
		}

		return alpha;
	}
	int ZWS_3(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const unsigned char x1, const unsigned char x2, const unsigned char x3)
	{
	//      if (QUADRANT_ID(x1) == QUADRANT_ID(x2)) //Play x3 first
		//	std::swap(x1,x3);
		//else if (QUADRANT_ID(x1) == QUADRANT_ID(x3)) //Play x2 first
		//	std::swap(x1,x2);

		unsigned long long flipped;
		bool played = false;
		++NodeCounter;

		//Play on x1
		if (O & NEIGHBOUR(x1))
		{
			if (flipped = flip(P, O, x1))
			{
				if (-ZWS_2(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, -alpha-1, x2, x3) > alpha)
					return alpha+1;
				else
					played = true;
			}
		}

		//Play on x2
		if (O & NEIGHBOUR(x2))
		{
			if (flipped = flip(P, O, x2))
			{
				if (-ZWS_2(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, -alpha-1, x1, x3) > alpha)
					return alpha+1;
				else
					played = true;
			}
		}

		//Play on x3
		if (O & NEIGHBOUR(x3))
		{
			if (flipped = flip(P, O, x3))
				return -ZWS_2(O ^ flipped, P ^ (1ULL << x3) ^ flipped, NodeCounter, -alpha-1, x1, x2);
		}

		if (!played) //Player couldn't play
		{
			++NodeCounter;

			//Play on x1
			if (P & NEIGHBOUR(x1))
			{
				if (flipped = flip(O, P, x1))
				{
					if (ZWS_2(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, alpha, x2, x3) <= alpha)
						return alpha;
					else
						played = true;
				}
			}

			//Play on x2
			if (P & NEIGHBOUR(x2))
			{
				if (flipped = flip(O, P, x2))
				{
					if (ZWS_2(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, alpha, x1, x3) <= alpha)
						return alpha;
					else
						played = true;
				}
			}

			//Play on x3
			if (P & NEIGHBOUR(x3))
			{
				if (flipped = flip(O, P, x3))
					return ZWS_2(P ^ flipped, O ^ (1ULL << x3) ^ flipped, NodeCounter, alpha, x1, x2);
			}

			if (played)
				return alpha+1;
			else
				return EvaluateGameOver(P, 3);
		}

		return alpha;
	}
	int ZWS_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const unsigned char x1, const unsigned char x2, const unsigned char x3, const unsigned char x4)
	{
		unsigned long long flipped;
		bool played = false;
		++NodeCounter;

		//Play on x1
		if (O & NEIGHBOUR(x1))
		{
			if (flipped = flip(P, O, x1))
			{
				if (-ZWS_3(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, -alpha-1, x2, x3, x4) > alpha)
					return alpha+1;
				else
					played = true;
			}
		}

		//Play on x2
		if (O & NEIGHBOUR(x2))
		{
			if (flipped = flip(P, O, x2))
			{
				if (-ZWS_3(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, -alpha-1, x1, x3, x4) > alpha)
					return alpha+1;
				else
					played = true;
			}
		}
		
		//Play on x3
		if (O & NEIGHBOUR(x3))
		{
			if (flipped = flip(P, O, x3))
			{
				if (-ZWS_3(O ^ flipped, P ^ (1ULL << x3) ^ flipped, NodeCounter, -alpha-1, x1, x2, x4) > alpha)
					return alpha+1;
				else
					played = true;
			}
		}

		//Play on x4
		if (O & NEIGHBOUR(x4))
		{
			if (flipped = flip(P, O, x4))
				return -ZWS_3(O ^ flipped, P ^ (1ULL << x4) ^ flipped, NodeCounter, -alpha-1, x1, x2, x3);
		}

		if (!played) //Player couldn't play
		{
			++NodeCounter;

			//Play on x1
			if (P & NEIGHBOUR(x1))
			{
				if (flipped = flip(O, P, x1))
				{
					if (ZWS_3(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, alpha, x2, x3, x4) <= alpha)
						return alpha;
					else
						played = true;
				}
			}

			//Play on x2
			if (P & NEIGHBOUR(x2))
			{
				if (flipped = flip(O, P, x2))
				{
					played = true;
					if (ZWS_3(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, alpha, x1, x3, x4) <= alpha)
						return alpha;
				}
			}

			//Play on x3
			if (P & NEIGHBOUR(x3))
			{
				if (flipped = flip(O, P, x3))
				{
					if (ZWS_3(P ^ flipped, O ^ (1ULL << x3) ^ flipped, NodeCounter, alpha, x1, x2, x4) <= alpha)
						return alpha;
					else
						played = true;
				}
			}

			//Play on x4
			if (P & NEIGHBOUR(x4))
			{
				if (flipped = flip(O, P, x4))
					return ZWS_3(P ^ flipped, O ^ (1ULL << x4) ^ flipped, NodeCounter, alpha, x1, x2, x3);
			}

			if (played)
				return alpha+1;
			else
				return EvaluateGameOver(P, 4);
		}

		return alpha;
	}
	int ZWS_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const int alpha)
	{
		unsigned long long BitBoardEmpty = ~(P | O);
		unsigned char x1 = BIT_SCAN_LS1B(BitBoardEmpty); REMOVE_LS1B(BitBoardEmpty);
		unsigned char x2 = BIT_SCAN_LS1B(BitBoardEmpty); REMOVE_LS1B(BitBoardEmpty);
		unsigned char x3 = BIT_SCAN_LS1B(BitBoardEmpty); REMOVE_LS1B(BitBoardEmpty);
		unsigned char x4 = BIT_SCAN_LS1B(BitBoardEmpty);

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
		const unsigned long long BitBoardParity = QUADRANT(parity);
		unsigned long long BitBoardTmp, flipped;
		bool played = false;
		unsigned char Move;
		++NodeCounter;

		BitBoardTmp = BitBoardEmpty & BitBoardParity & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			Move = BIT_SCAN_LS1B(BitBoardTmp);
			REMOVE_LS1B(BitBoardTmp);
			if (O & NEIGHBOUR(Move))
			{
				if (flipped = flip(P, O, Move))
				{
					played = true;
					if (-ZWS_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
						return alpha+1;
				}
			}
		}

		BitBoardTmp = BitBoardEmpty & BitBoardParity & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			Move = BIT_SCAN_LS1B(BitBoardTmp);
			REMOVE_LS1B(BitBoardTmp);
			if (O & NEIGHBOUR(Move))
			{
				if (flipped = flip(P, O, Move))
				{
					played = true;
					if (-ZWS_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
						return alpha+1;
				}
			}
		}

		BitBoardTmp = BitBoardEmpty & ~BitBoardParity & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			Move = BIT_SCAN_LS1B(BitBoardTmp);
			REMOVE_LS1B(BitBoardTmp);
			if (O & NEIGHBOUR(Move))
			{
				if (flipped = flip(P, O, Move))
				{
					played = true;
					if (-ZWS_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
						return alpha+1;
				}
			}
		}

		BitBoardTmp = BitBoardEmpty & ~BitBoardParity & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			Move = BIT_SCAN_LS1B(BitBoardTmp);
			REMOVE_LS1B(BitBoardTmp);
			if (O & NEIGHBOUR(Move))
			{
				if (flipped = flip(P, O, Move))
				{
					played = true;
					if (-ZWS_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
						return alpha+1;
				}
			}
		}

		if (!played)
		{
			++NodeCounter;

			BitBoardTmp = BitBoardEmpty & QUADRANT(parity) & PATTERN_FIRST;
			while (BitBoardTmp)
			{
				Move = BIT_SCAN_LS1B(BitBoardTmp);
				REMOVE_LS1B(BitBoardTmp);
				if (P & NEIGHBOUR(Move))
				{
					if (flipped = flip(O, P, Move))
					{
						played = true;
						if (ZWS_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, alpha) <= alpha)
							return alpha;
					}
				}
			}

			BitBoardTmp = BitBoardEmpty & QUADRANT(parity) & ~PATTERN_FIRST;
			while (BitBoardTmp)
			{
				Move = BIT_SCAN_LS1B(BitBoardTmp);
				REMOVE_LS1B(BitBoardTmp);
				if (P & NEIGHBOUR(Move))
				{
					if (flipped = flip(O, P, Move))
					{
						played = true;
						if (ZWS_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, alpha) <= alpha)
							return alpha;
					}
				}
			}

			BitBoardTmp = BitBoardEmpty & ~QUADRANT(parity) & PATTERN_FIRST;
			while (BitBoardTmp)
			{
				Move = BIT_SCAN_LS1B(BitBoardTmp);
				REMOVE_LS1B(BitBoardTmp);
				if (P & NEIGHBOUR(Move))
				{
					if (flipped = flip(O, P, Move))
					{
						played = true;
						if (ZWS_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, alpha) <= alpha)
							return alpha;
					}
				}
			}

			BitBoardTmp = BitBoardEmpty & ~QUADRANT(parity) & ~PATTERN_FIRST;
			while (BitBoardTmp)
			{
				Move = BIT_SCAN_LS1B(BitBoardTmp);
				REMOVE_LS1B(BitBoardTmp);
				if (P & NEIGHBOUR(Move))
				{
					if (flipped = flip(O, P, Move))
					{
						played = true;
						if (ZWS_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, alpha) <= alpha)
							return alpha;
					}
				}
			}

			if (!played)
				return EvaluateGameOver(P, NumberOfEmptyStones);
			else
				return alpha+1;
		}
		else
			return alpha;
	}
	int ZWS_B(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const unsigned char NumberOfEmptyStones, int alpha)
	{
		if (NumberOfEmptyStones == 6)
			return ZWS_A(P, O, NodeCounter, parity, NumberOfEmptyStones, alpha);

		int value = -128;
		unsigned char Move;
		unsigned long long BitBoardTmp, BitBoardPossible, flipped;
		BitBoardPossible = PossibleMoves(P, O);
		++NodeCounter;
		
		if (!BitBoardPossible)
		{
			BitBoardPossible = PossibleMoves(O, P);
			if (BitBoardPossible)
				return -ZWS_B(O, P, NodeCounter, parity, NumberOfEmptyStones, -alpha-1);
			else //Game is over
			{
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
			Move = BIT_SCAN_LS1B(BitBoardTmp);
			REMOVE_LS1B(BitBoardTmp);
			flipped = flip(P, O, Move);
			if (-ZWS_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
				return alpha+1;
		}

		BitBoardTmp = BitBoardPossible & BitBoardParity & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			Move = BIT_SCAN_LS1B(BitBoardTmp);
			REMOVE_LS1B(BitBoardTmp);
			flipped = flip(P, O, Move);
			if (-ZWS_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
				return alpha+1;
		}

		BitBoardTmp = BitBoardPossible & ~BitBoardParity & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			Move = BIT_SCAN_LS1B(BitBoardTmp);
			REMOVE_LS1B(BitBoardTmp);
			flipped = flip(P, O, Move);
			if (-ZWS_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
				return alpha+1;
		}

		BitBoardTmp = BitBoardPossible & ~BitBoardParity & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			Move = BIT_SCAN_LS1B(BitBoardTmp);
			REMOVE_LS1B(BitBoardTmp);
			flipped = flip(P, O, Move);
			if (-ZWS_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
				return alpha+1;
		}

		return alpha;
	}
	int ZWS(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, unsigned char selectivity)
	{
		const unsigned char Empties = NumberOfEmptyStones(P, O);
		if (Empties == 8)
			return ZWS_B(P, O, search.NodeCounter, parity(P, O), Empties, alpha);

		unsigned char BestMove = 64;
		int value;
		unsigned long long flipped;
		unsigned long long BitBoardPossible = PossibleMoves(P, O);

		++search.NodeCounter;

		if (!BitBoardPossible){
			BitBoardPossible = PossibleMoves(O, P);
			if (BitBoardPossible)
				return -ZWS(search, O, P, -alpha-1, selectivity);
			else{ //Game is over
				++search.NodeCounter;
				return BIND(EvaluateGameOver(P, Empties), alpha, alpha+1);
			}
		}

		if (StabilityCutoff(P, O, Empties, alpha))
			return alpha;

		if (MPC(search, P, O, alpha, Empties, selectivity, value))
			return value;
	
		unsigned long long LocalNodeCounter = search.NodeCounter;

		HashTableValueType htValue;
		if (search.HashTableLookUp(P, O, htValue))
		{
			if (UseHashTableValue(htValue, alpha, alpha+1, Empties, 0, value))
				return value;

			if (htValue.PV != 64)
			{
				assert((BitBoardPossible & (1ULL << htValue.PV)) != 0);
				BitBoardPossible ^= (1ULL << htValue.PV);
				flipped = flip(P, O, htValue.PV);
				if (-ZWS(search, O ^ flipped, P ^ (1ULL << htValue.PV) ^ flipped, -alpha-1, selectivity) > alpha)
				{
					search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, selectivity, alpha+1, 64, htValue.PV, htValue.AV);
					return alpha+1;
				}
			}

			if (htValue.AV != 64)
			{
				assert((BitBoardPossible & (1ULL << htValue.AV)) != 0);
				BitBoardPossible ^= (1ULL << htValue.AV);
				flipped = flip(P, O, htValue.AV);
				if (-ZWS(search, O ^ flipped, P ^ (1ULL << htValue.AV) ^ flipped, -alpha-1, selectivity) > alpha)
				{
					search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, selectivity, alpha+1, 64, htValue.AV, htValue.PV);
					return alpha+1;
				}
			}
		}

		CMoveList mvList(P, O, BitBoardPossible);
		for (auto& mv : mvList)
		{
			if (-ZWS(search, mv.P, mv.O, -alpha-1, selectivity) > alpha)
			{
				search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, selectivity, alpha+1, 64, mv.move, 64);
				return alpha+1;
			}
		}
		search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, selectivity, -64, alpha, 64, 64);
		return alpha;
	}

	int AlphaBeta_1(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const int x) 
	{
		int Score, FlipCount;
		Score = (POP_COUNT(P) << 1) - 62; // POP_COUNT(P) - POP_COUNT(O) + 1
	
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
				return (Score > 0) ? Score : Score - 2;
				//if (Score > 0)
				//	return Score;
				//else
				//	return Score - 2;
			}
			//if (Score < 0)
			//{
			//	--Score; //Count the empty field as an opponents stone. Or in the case that nobody can play it also acounts for the opponent, since the score is below zero.
			//	if (Score <= alpha) //You can't play and your score is below alpha, so you can't do better than alpha.
			//		return alpha;
			//	else
			//	{
			//		++NodeCounter;
			//		return Score - count_last_flip(O, x);
			//	}
			//}
			//else // Score > 0
			//{
			//	if (Score <= alpha) //You can't play and your score is below alpha, so you can't do better than alpha
			//		return alpha;
			//	else
			//	{
			//		++NodeCounter;
			//		if (FlipCount = count_last_flip(O, x))
			//			return Score - FlipCount - 1;
			//		else
			//			return Score + 1;
			//	}
			//}
		}
	}
	int AlphaBeta_2(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta, const int x1, const int x2)
	{
		//if(EVALUATION_ORDER(x1) < EVALUATION_ORDER(x2))
		//	std::swap(x1,x2);

		unsigned long long flipped;
		int value = -128;
		++NodeCounter;

		//Play on x1
		if (O & NEIGHBOUR(x1))
		{
			flipped = flip(P, O, x1);
			if (flipped)
			{
				value = -AlphaBeta_1(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, -beta, x2);
				if (value >= beta) return beta;
				if (value > alpha) alpha = value;
			}
		}

		//Play on x2
		if (O & NEIGHBOUR(x2))
		{
			flipped = flip(P, O, x2);
			if (flipped)
				return MAX(alpha, -AlphaBeta_1(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, -beta, x1));
		}

		if (value == -128) //Player couldn't play
		{
			++NodeCounter;
			//Play on x1
			if (P & NEIGHBOUR(x1))
			{
				flipped = flip(O, P, x1);
				if (flipped)
				{
					value = AlphaBeta_1(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, alpha, x2);
					if (value <= alpha) return alpha;
					if (value < beta) beta = value;
				}
			}

			//Play on x2
			if (P & NEIGHBOUR(x2))
			{
				flipped = flip(O, P, x2);
				if (flipped)
					return MIN(beta, AlphaBeta_1(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, alpha, x1));
			}

			if (value == -128) //Nobody could play
				return EvaluateGameOver(P, 2);
			else
				return beta;
		}

		return alpha;
	}
	int AlphaBeta_3(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta, const int x1, const int x2, const int x3)
	{
		//if (QUADRANT_ID(x1) == QUADRANT_ID(x2)) //Play x3 first
		//	std::swap(x1,x3);
		//else if (QUADRANT_ID(x1) == QUADRANT_ID(x3)) //Play x2 first
		//	std::swap(x1,x2);

		unsigned long long flipped;
		int value = -128;
		++NodeCounter;

		//Play on x1
		if (O & NEIGHBOUR(x1))
		{
			if (flipped = flip(P, O, x1))
			{
				value = -AlphaBeta_2(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, -beta, -alpha, x2, x3);
				if (value >= beta) return beta;
				if (value > alpha) alpha = value;
			}
		}

		//Play on x2
		if (O & NEIGHBOUR(x2))
		{
			if (flipped = flip(P, O, x2))
			{
				value = -AlphaBeta_2(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, -beta, -alpha, x1, x3);
				if (value >= beta) return beta;
				if (value > alpha) alpha = value;
			}
		}

		//Play on x3
		if (O & NEIGHBOUR(x3))
		{
			if (flipped = flip(P, O, x3))
				return MAX(alpha, -AlphaBeta_2(O ^ flipped, P ^ (1ULL << x3) ^ flipped, NodeCounter, -beta, -alpha, x1, x2));
		}

		if (value == -128) //Player couldn't play
		{
			++NodeCounter;

			//Play on x1
			if (P & NEIGHBOUR(x1))
			{
				if (flipped = flip(O, P, x1))
				{
					value = AlphaBeta_2(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, alpha, beta, x2, x3);
					if (value <= alpha)	return alpha;
					if (value < beta) beta = value;
				}
			}

			//Play on x2
			if (P & NEIGHBOUR(x2))
			{
				if (flipped = flip(O, P, x2))
				{
					value = AlphaBeta_2(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, alpha, beta, x1, x3);
					if (value <= alpha)	return alpha;
					if (value < beta) beta = value;
				}
			}

			//Play on x3
			if (P & NEIGHBOUR(x3))
			{
				if (flipped = flip(O, P, x3))
					return MIN(beta, AlphaBeta_2(P ^ flipped, O ^ (1ULL << x3) ^ flipped, NodeCounter, alpha, beta, x1, x2));
			}

			if (value == -128) //Nobody could play
				return EvaluateGameOver(P, 3);
			else
				return beta;
		}

		return alpha;
	}
	int AlphaBeta_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta, const int x1, const int x2, const int x3, const int x4)
	{
		unsigned long long flipped;
		int value = -128;
		++NodeCounter;
		
		//Play on x1
		if (O & NEIGHBOUR(x1))
		{
			if (flipped = flip(P, O, x1))
			{
				value = -AlphaBeta_3(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, -beta, -alpha, x2, x3, x4);
				if (value >= beta) return beta;
				if (value > alpha) alpha = value;
			}
		}

		//Play on x2
		if (O & NEIGHBOUR(x2))
		{
			if (flipped = flip(P, O, x2))
			{
				value = -AlphaBeta_3(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, -beta, -alpha, x1, x3, x4);
				if (value >= beta) return beta;
				if (value > alpha) alpha = value;
			}
		}

		//Play on x3
		if (O & NEIGHBOUR(x3))
		{
			if (flipped = flip(P, O, x3))
			{
				value = -AlphaBeta_3(O ^ flipped, P ^ (1ULL << x3) ^ flipped, NodeCounter, -beta, -alpha, x1, x2, x4);
				if (value >= beta) return beta;
				if (value > alpha) alpha = value;
			}
		}

		//Play on x4
		if (O & NEIGHBOUR(x4))
		{
			if (flipped = flip(P, O, x4))
				return MAX(alpha, -AlphaBeta_3(O ^ flipped, P ^ (1ULL << x4) ^ flipped, NodeCounter, -beta, -alpha, x1, x2, x3));
		}

		if (value == -128) //Player couldn't play
		{
			++NodeCounter;

			//Play on x1
			if (P & NEIGHBOUR(x1))
			{
				if (flipped = flip(O, P, x1))
				{
					value = AlphaBeta_3(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, alpha, beta, x2, x3, x4);
					if (value <= alpha)	return alpha;
					if (value < beta) beta = value;
				}
			}

			//Play on x2
			if (P & NEIGHBOUR(x2))
			{
				if (flipped = flip(O, P, x2))
				{
					value = AlphaBeta_3(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, alpha, beta, x1, x3, x4);
					if (value <= alpha)	return alpha;
					if (value < beta) beta = value;
				}
			}

			//Play on x3
			if (P & NEIGHBOUR(x3))
			{
				if (flipped = flip(O, P, x3))
				{
					value = AlphaBeta_3(P ^ flipped, O ^ (1ULL << x3) ^ flipped, NodeCounter, alpha, beta, x1, x2, x4);
					if (value <= alpha)	return alpha;
					if (value < beta) beta = value;
				}
			}

			//Play on x4
			if (P & NEIGHBOUR(x4))
			{
				if (flipped = flip(O, P, x4))
					return MIN(beta, AlphaBeta_3(P ^ flipped, O ^ (1ULL << x4) ^ flipped, NodeCounter, alpha, beta, x1, x2, x3));
			}

			if (value == -128) //Nobody could play
				return EvaluateGameOver(P, 4);
			else
				return beta;
		}

		return alpha;
	}
	int AlphaBeta_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const int alpha, const int beta)
	{
		unsigned long long BitBoardEmpty = ~(P | O);
		int x1 = BIT_SCAN_LS1B(BitBoardEmpty); REMOVE_LS1B(BitBoardEmpty);
		int x2 = BIT_SCAN_LS1B(BitBoardEmpty); REMOVE_LS1B(BitBoardEmpty);
		int x3 = BIT_SCAN_LS1B(BitBoardEmpty); REMOVE_LS1B(BitBoardEmpty);
		int x4 = BIT_SCAN_LS1B(BitBoardEmpty);

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

		return AlphaBeta_4(P, O, NodeCounter, alpha, beta, x1, x2, x3, x4);
	}
	int AlphaBeta_A(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const unsigned char NumberOfEmptyStones, int alpha, int beta)
	{
		if (NumberOfEmptyStones == 4)
			return AlphaBeta_4(P, O, NodeCounter, parity, alpha, beta);

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
		int value = -128;
		unsigned char Move;
		++NodeCounter;

		BitBoardTmp = BitBoardEmpty & BitBoardParity & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			Move = BIT_SCAN_LS1B(BitBoardTmp);
			REMOVE_LS1B(BitBoardTmp);
			if (O & NEIGHBOUR(Move))
			{
				if (flipped = flip(P, O, Move))
				{
					value = -AlphaBeta_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -beta, -alpha);
					if (value >= beta) return beta;
					if (value > alpha) alpha = value;
				}
			}
		}

		BitBoardTmp = BitBoardEmpty & BitBoardParity & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			Move = BIT_SCAN_LS1B(BitBoardTmp);
			REMOVE_LS1B(BitBoardTmp);
			if (O & NEIGHBOUR(Move))
			{
				if (flipped = flip(P, O, Move))
				{
					value = -AlphaBeta_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -beta, -alpha);
					if (value >= beta) return beta;
					if (value > alpha) alpha = value;
				}
			}
		}

		BitBoardTmp = BitBoardEmpty & ~BitBoardParity & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			Move = BIT_SCAN_LS1B(BitBoardTmp);
			REMOVE_LS1B(BitBoardTmp);
			if (O & NEIGHBOUR(Move))
			{
				if (flipped = flip(P, O, Move))
				{
					value = -AlphaBeta_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -beta, -alpha);
					if (value >= beta) return beta;
					if (value > alpha) alpha = value;
				}
			}
		}

		BitBoardTmp = BitBoardEmpty & ~BitBoardParity & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			Move = BIT_SCAN_LS1B(BitBoardTmp);
			REMOVE_LS1B(BitBoardTmp);
			if (O & NEIGHBOUR(Move))
			{
				if (flipped = flip(P, O, Move))
				{
					value = -AlphaBeta_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -beta, -alpha);
					if (value >= beta) return beta;
					if (value > alpha) alpha = value;
				}
			}
		}

		if (value == -128)
		{
			value = 128;
			++NodeCounter;

			BitBoardTmp = BitBoardEmpty & QUADRANT(parity) & PATTERN_FIRST;
			while (BitBoardTmp)
			{
				Move = BIT_SCAN_LS1B(BitBoardTmp);
				REMOVE_LS1B(BitBoardTmp);
				if (P & NEIGHBOUR(Move))
				{
					if (flipped = flip(O, P, Move))
					{
						value = AlphaBeta_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, alpha, beta);
						if (value <= alpha) return alpha;
						if (value < beta) beta = value;
					}
				}
			}

			BitBoardTmp = BitBoardEmpty & QUADRANT(parity) & ~PATTERN_FIRST;
			while (BitBoardTmp)
			{
				Move = BIT_SCAN_LS1B(BitBoardTmp);
				REMOVE_LS1B(BitBoardTmp);
				if (P & NEIGHBOUR(Move))
				{
					if (flipped = flip(O, P, Move))
					{
						value = AlphaBeta_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, alpha, beta);
						if (value <= alpha) return alpha;
						if (value < beta) beta = value;
					}
				}
			}

			BitBoardTmp = BitBoardEmpty & ~QUADRANT(parity) & PATTERN_FIRST;
			while (BitBoardTmp)
			{
				Move = BIT_SCAN_LS1B(BitBoardTmp);
				REMOVE_LS1B(BitBoardTmp);
				if (P & NEIGHBOUR(Move))
				{
					if (flipped = flip(O, P, Move))
					{
						value = AlphaBeta_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, alpha, beta);
						if (value <= alpha) return alpha;
						if (value < beta) beta = value;
					}
				}
			}

			BitBoardTmp = BitBoardEmpty & ~QUADRANT(parity) & ~PATTERN_FIRST;
			while (BitBoardTmp)
			{
				Move = BIT_SCAN_LS1B(BitBoardTmp);
				REMOVE_LS1B(BitBoardTmp);
				if (P & NEIGHBOUR(Move))
				{
					if (flipped = flip(O, P, Move))
					{
						value = AlphaBeta_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, alpha, beta);
						if (value <= alpha) return alpha;
						if (value < beta) beta = value;
					}
				}
			}

			if (value == 128)
				return EvaluateGameOver(P, NumberOfEmptyStones);
			else
				return beta;
		}
		else
			return alpha;
	}
	int AlphaBeta_B(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const unsigned char NumberOfEmptyStones, int alpha, int beta)
	{
		if (NumberOfEmptyStones == 6)
			return AlphaBeta_A(P, O, NodeCounter, parity, NumberOfEmptyStones, alpha, beta);

		int value = -128;
		unsigned char Move;
		unsigned long long BitBoardTmp, BitBoardPossible, flipped;
		BitBoardPossible = PossibleMoves(P, O);
		++NodeCounter;
		
		if (!BitBoardPossible)
		{
			BitBoardPossible = PossibleMoves(O, P);
			if (BitBoardPossible)
				return -AlphaBeta_B(O, P, NodeCounter, parity, NumberOfEmptyStones, -beta, -alpha);
			else //Game is over
			{
				++NodeCounter;
				return BIND(EvaluateGameOver(P, NumberOfEmptyStones), alpha, beta);
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
			Move = BIT_SCAN_LS1B(BitBoardTmp);
			REMOVE_LS1B(BitBoardTmp);
			flipped = flip(P, O, Move);
			value = -AlphaBeta_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -beta, -alpha);
			if (value >= beta) return beta;
			if (value > alpha) alpha = value;
		}

		BitBoardTmp = BitBoardPossible & BitBoardParity & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			Move = BIT_SCAN_LS1B(BitBoardTmp);
			REMOVE_LS1B(BitBoardTmp);
			flipped = flip(P, O, Move);
			value = -AlphaBeta_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -beta, -alpha);
			if (value >= beta) return beta;
			if (value > alpha) alpha = value;
		}

		BitBoardTmp = BitBoardPossible & ~BitBoardParity & PATTERN_FIRST;
		while (BitBoardTmp)
		{
			Move = BIT_SCAN_LS1B(BitBoardTmp);
			REMOVE_LS1B(BitBoardTmp);
			flipped = flip(P, O, Move);
			value = -AlphaBeta_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -beta, -alpha);
			if (value >= beta) return beta;
			if (value > alpha) alpha = value;
		}

		BitBoardTmp = BitBoardPossible & ~BitBoardParity & ~PATTERN_FIRST;
		while (BitBoardTmp)
		{
			Move = BIT_SCAN_LS1B(BitBoardTmp);
			REMOVE_LS1B(BitBoardTmp);
			flipped = flip(P, O, Move);
			value = -AlphaBeta_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -beta, -alpha);
			if (value >= beta) return beta;
			if (value > alpha) alpha = value;
		}

		return alpha;
	}

	int PVS(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, int beta, unsigned char selectivity, unsigned char * pline)
	{
		const unsigned char Empties = NumberOfEmptyStones(P, O);
		if (Empties == 8)
			return AlphaBeta_B(P, O, search.NodeCounter, parity(P, O), Empties, alpha, beta);

		bool SearchPV = true;
		int value;
		unsigned long long flipped;
		unsigned long long BitBoardPossible = PossibleMoves(P, O);

		++search.NodeCounter;

		if (!BitBoardPossible)
		{
			BitBoardPossible = PossibleMoves(O, P);
			if (BitBoardPossible)
				return -PVS(search, O, P, -beta, -alpha, selectivity, pline);
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
			if (UseHashTableValue(htValue, alpha, beta, Empties, selectivity, value))
				return value;

			if (htValue.PV != 64)
			{
				assert((BitBoardPossible & (1ULL << htValue.PV)) != 0);
				BitBoardPossible ^= (1ULL << htValue.PV);
				flipped = flip(P, O, htValue.PV);
				value = -PVS(search, O ^ flipped, P ^ (1ULL << htValue.PV) ^ flipped, -beta, -alpha, selectivity, line);
				if (value >= beta)
				{
					search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, selectivity, beta, 64, htValue.PV, htValue.AV);
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
				value = -PVS(search, O ^ flipped, P ^ (1ULL << htValue.AV) ^ flipped, -beta, -alpha, selectivity, line);
				if (value >= beta)
				{
					search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, selectivity, beta, 64, htValue.AV, htValue.PV);
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
				value = -PVS(search, mv.P, mv.O, -beta, -alpha, selectivity, line);
			else
			{
				value = -ZWS(search, mv.P, mv.O, -alpha-1, selectivity);
				if (value > alpha)
					value = -PVS(search, mv.P, mv.O, -beta, -alpha, selectivity, line);
			}
			if (value >= beta)
			{
				search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, selectivity, beta, 64, mv.move, 64);
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
			search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, selectivity, -64, alpha, 64, 64);
		else
			search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, selectivity, alpha, alpha, pline[0], 64);
		return alpha;
	}


	inline int Exact_0(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter)
	{
		++NodeCounter;
		return EvaluateGameOver(P, 0);
	}
	inline int Exact_1(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha)
	{
		const unsigned long long E = ~(P | O);
		const int x1 = BIT_SCAN_LS1B(E);
		return AlphaBeta_1(P, O, NodeCounter, alpha, x1);
	}
	inline int Exact_2(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const int beta)
	{
		unsigned long long E = ~(P | O);
		const int x1 = BIT_SCAN_LS1B(E); REMOVE_LS1B(E);
		const int x2 = BIT_SCAN_LS1B(E);
		return AlphaBeta_2(P, O, NodeCounter, alpha, beta, x1, x2);
	}
	inline int Exact_3(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const int beta)
	{
		unsigned long long E = ~(P | O);
		const int x1 = BIT_SCAN_LS1B(E); REMOVE_LS1B(E);
		const int x2 = BIT_SCAN_LS1B(E); REMOVE_LS1B(E);
		const int x3 = BIT_SCAN_LS1B(E);
		return AlphaBeta_3(P, O, NodeCounter, alpha, beta, x1, x2, x3);
	}
	inline int Exact_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const int beta)
	{
		return AlphaBeta_4(P, O, NodeCounter, parity(P, O), alpha, beta);
	}

	inline void Exact_0(CSearch & search) { search.score = Exact_0    (search.P, search.O, search.NodeCounter); }
	inline void Exact_1(CSearch & search) { search.score = Exact_1    (search.P, search.O, search.NodeCounter, search.alpha); }
	inline void Exact_2(CSearch & search) { search.score = Exact_2    (search.P, search.O, search.NodeCounter, search.alpha, search.beta); }
	inline void Exact_3(CSearch & search) { search.score = Exact_3    (search.P, search.O, search.NodeCounter, search.alpha, search.beta); }
	inline void Exact_4(CSearch & search) { search.score = AlphaBeta_4(search.P, search.O, search.NodeCounter, parity(search.P, search.O), search.alpha, search.beta); }
	inline void Exact_A(CSearch & search) { search.score = AlphaBeta_A(search.P, search.O, search.NodeCounter, parity(search.P, search.O), NumberOfEmptyStones(search.P, search.O), search.alpha, search.beta); }
	inline void Exact_B(CSearch & search) { search.score = AlphaBeta_B(search.P, search.O, search.NodeCounter, parity(search.P, search.O), NumberOfEmptyStones(search.P, search.O), search.alpha, search.beta); }
	inline void Exact_C(CSearch & search) { search.score = PVS(search, search.P, search.O, search.alpha, search.beta, search.selectivity, search.PV); }
	inline void Exact_D(CSearch & search)
	{
		const int Empties = NumberOfEmptyStones(search.P, search.O);
		
		for (int d = 4; d < Empties-11; d++)
			Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, d, 3, search.PV);
		Midgame::PVS(search, search.P, search.O, search.alpha, search.beta,Empties-11, 3, search.PV);
		Midgame::PVS(search, search.P, search.O, search.alpha, search.beta,Empties-10, 4, search.PV);
		Midgame::PVS(search, search.P, search.O, search.alpha, search.beta,Empties- 9, 5, search.PV);
		Midgame::PVS(search, search.P, search.O, search.alpha, search.beta,Empties- 8, 6, search.PV);
		Exact_C(search);
	}
}

// For benchmarking purposes
int EvaluateExact(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned long long NumberOfEmptyStones)
{
	switch (NumberOfEmptyStones)
	{
	case 0: return Endgame::Exact_0(P, O, NodeCounter);
	case 1: return Endgame::Exact_1(P, O, NodeCounter, -64);
	case 2: return Endgame::Exact_2(P, O, NodeCounter, -64, 64);
	case 3: return Endgame::Exact_3(P, O, NodeCounter, -64, 64);
	case 4: return Endgame::Exact_4(P, O, NodeCounter, -64, 64);
	default: throw std::logic_error("Wrong number of empty stones!");
	}
}

void EvaluateExact(CSearch & search)
{
	const unsigned long long Empties = NumberOfEmptyStones(search.P, search.O);
	switch (Empties)
	{
	case  0: Endgame::Exact_0(search); break;
	case  1: Endgame::Exact_1(search); break;
	case  2: Endgame::Exact_2(search); break;
	case  3: Endgame::Exact_3(search); break;
	case  4: Endgame::Exact_4(search); break;
	case  5: Endgame::Exact_A(search); break;
	case  6: Endgame::Exact_A(search); break;
	case  7: Endgame::Exact_B(search); break;
	case  8: Endgame::Exact_B(search); break;
	case  9: Endgame::Exact_C(search); break;
	case 10: Endgame::Exact_C(search); break;
	case 11:
		Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, 3, 0, search.PV);
		Endgame::Exact_C(search); 
		break;
	case 12:
	case 13:
		Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, 4, 0, search.PV);
		Endgame::Exact_C(search);
		break;
	case 14:
	case 15:
		Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, 6, 6, search.PV);
		Endgame::Exact_C(search);
		break;
	case 16:
		Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, 6, 2, search.PV);
		Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, 7, 4, search.PV);
		Midgame::PVS(search, search.P, search.O, search.alpha, search.beta, 8, 6, search.PV);
		Endgame::Exact_C(search);
		break;
	default: Endgame::Exact_D(search); break;
	}
}