#include "endgame.h"

// A: No calculation of PossibleMoves, just testing if a ply flips stones. Move ordering based on pattern and parity.
// B: Calculating Possible Moves. Move ordering based on pattern and parity
// C: Using hash tables and sophisticated move ordering
// D: Iterative deepening
// E: Iterative deepening and ProbCut

bool StabilityCutoff(const unsigned long long P, const unsigned long long O, const int NumberOfEmptyStones, const int alpha)
{
	int OwnMinusOpponents = (POP_COUNT(P) << 1) + NumberOfEmptyStones - 64;
    if (OwnMinusOpponents <= STABILITY_CUTOFF_LIMIT(NumberOfEmptyStones)) //Worth checking stability
        return (64 - static_cast<int>(POP_COUNT(StableStones(P, O)) << 1) <= alpha);
    return false;
}

//int MPC(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, int beta, int depth, unsigned char * pline)
//{
//	const unsigned char Empties = NumberOfEmptyStones(P, O);
//	if (Empties == 8)
//		return AlphaBeta_Exact_B(P, O, search.NodeCounter, parity(P, O), Empties, alpha, beta);
//
//	bool SearchPV = true;
//	int value;
//	unsigned long long flipped;
//	unsigned long long BitBoardPossible = PossibleMoves(P, O);
//
//	++search.NodeCounter;
//
//	if (!BitBoardPossible)
//    {
//		BitBoardPossible = PossibleMoves(O, P);
//		if (BitBoardPossible)
//			return -PVS_Exact_C(search, O, P, -beta, -alpha, pline);
//		else{ //Game is over
//			++search.NodeCounter;
//			return BIND(EvaluateGameOver(P, NumberOfEmptyStones), alpha, beta);
//		}
//    }
//
//	if (StabilityCutoff(P, O, NumberOfEmptyStones, alpha))
//        return alpha;
//	
//	unsigned long long LocalNodeCounter = search.NodeCounter;
//	unsigned char line[60];
//	memset(line, static_cast<unsigned char>(64), 60 * sizeof(unsigned char));
//
//	if (NumberOfEmptyStones > 9)
//	{
//		std::pair<bool, HashTableValueType> HashTableReturn = search.HashTableLookUp(P, O);
//		if (HashTableReturn.first)
//		{
//			if ((HashTableReturn.second.depth >= NumberOfEmptyStones) && (HashTableReturn.second.selectivity == 0))
//			{
//				if (HashTableReturn.second.alpha >= beta)
//					return beta;
//				if (HashTableReturn.second.beta <= alpha)
//					return alpha;
//				if (HashTableReturn.second.alpha == HashTableReturn.second.beta)
//					return HashTableReturn.second.alpha;
//				alpha = MAX(alpha, static_cast<int>(HashTableReturn.second.alpha));
//			}
//
//			if (HashTableReturn.second.PV != 64)
//			{
//				if ((BitBoardPossible & (1ULL << HashTableReturn.second.PV)) == 0ULL)
//					std::cerr << "!" << (int)HashTableReturn.second.PV << " ";
//				BitBoardPossible ^= (1ULL << HashTableReturn.second.PV);
//				flipped = flip(P, O, HashTableReturn.second.PV);
//				value = -PVS_Exact_C(search, O ^ flipped, P ^ (1ULL << HashTableReturn.second.PV) ^ flipped, -beta, -alpha, line);
//				if (value >= beta)
//				{
//					search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, NumberOfEmptyStones, NumberOfEmptyStones, 0, beta, 64, HashTableReturn.second.PV, HashTableReturn.second.AV, false, true, true);
//					return beta;
//				}
//				if (value > alpha)
//				{
//					pline[0] = HashTableReturn.second.PV;
//					memcpy(pline+1, line, NumberOfEmptyStones * sizeof(unsigned char) - 1);
//					alpha = value;
//					SearchPV = false;
//				}
//			}
//
//			if (HashTableReturn.second.AV != 64)
//			{
//				if ((BitBoardPossible & (1ULL << HashTableReturn.second.AV)) == 0ULL)
//					std::cerr << "!" << (int)HashTableReturn.second.AV << " ";
//				BitBoardPossible ^= (1ULL << HashTableReturn.second.AV);
//				flipped = flip(P, O, HashTableReturn.second.AV);
//				value = -PVS_Exact_C(search, O ^ flipped, P ^ (1ULL << HashTableReturn.second.AV) ^ flipped, -beta, -alpha, line);
//				if (value >= beta)
//				{
//					search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, NumberOfEmptyStones, NumberOfEmptyStones, 0, beta, 64, HashTableReturn.second.AV, HashTableReturn.second.PV, false, true, true);
//					return beta;
//				}
//				if (value > alpha)
//				{
//					pline[0] = HashTableReturn.second.AV;
//					memcpy(pline+1, line, NumberOfEmptyStones * sizeof(unsigned char) - 1);
//					alpha = value;
//					SearchPV = false;
//				}
//			}
//		}
//	}
//
//	CMoveList mvList(P, O, BitBoardPossible);
//
//	auto end = mvList.cend();
//	for (auto it = mvList.cbegin(); it != end; ++it)
//    {
//		if (SearchPV)
//			value = -PVS_Exact_C(search, it->P, it->O, -beta, -alpha, line);
//		else
//		{
//			value = -ZWS_Exact_C(search, it->P, it->O, -alpha-1);
//			if (value > alpha)
//				value = -PVS_Exact_C(search, it->P, it->O, -beta, -alpha, line);
//		}
//        if (value >= beta)
//		{
//			search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, NumberOfEmptyStones, NumberOfEmptyStones, 0, beta, 64, it->move, 64, false, true, true);
//            return beta;
//        }
//		if (value > alpha)
//		{
//			pline[0] = it->move;
//			memcpy(pline+1, line, NumberOfEmptyStones * sizeof(unsigned char) - 1);
//			alpha = value;
//			SearchPV = false;
//		}
//    }
//	if (!SearchPV)
//		search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, NumberOfEmptyStones, NumberOfEmptyStones, 0, alpha, alpha, pline[0], 64, true, true, true);
//	else
//		search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, NumberOfEmptyStones, NumberOfEmptyStones, 0, -64, alpha, 64, 64, false, false, true);
//	return alpha;
//}

int ZWS_Exact_1(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const unsigned char x)
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
int ZWS_Exact_2(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const unsigned char x1, const unsigned char x2)
{
	unsigned long long flipped;
    bool played = false;
	++NodeCounter;

    //Play on x1
    if (O & NEIGHBOUR(x1))
    {
        if (flipped = flip(P, O, x1))
        {
			if (-ZWS_Exact_1(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, -alpha-1, x2) > alpha)
				return alpha+1;
			else
				played = true;
        }
    }

    //Play on x2
    if (O & NEIGHBOUR(x2))
    {
        if (flipped = flip(P, O, x2))
			return -ZWS_Exact_1(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, -alpha-1, x1);
    }

    if (!played) //Player couldn't play
    {
		++NodeCounter;
        //Play on x1
        if (P & NEIGHBOUR(x1))
        {
            if (flipped = flip(O, P, x1))
            {
				if (ZWS_Exact_1(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, alpha, x2) <= alpha)
					return alpha;
				else
					played = true;
            }
        }

        //Play on x2
        if (P & NEIGHBOUR(x2))
        {
            if (flipped = flip(O, P, x2))
				return ZWS_Exact_1(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, alpha, x1);
        }

        if (played)
			return alpha+1;
		else
            return EvaluateGameOver(P, 2);
			
    }

    return alpha;
}
int ZWS_Exact_3(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const unsigned char x1, const unsigned char x2, const unsigned char x3)
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
			if (-ZWS_Exact_2(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, -alpha-1, x2, x3) > alpha)
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
			if (-ZWS_Exact_2(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, -alpha-1, x1, x3) > alpha)
				return alpha+1;
			else
				played = true;
        }
    }

    //Play on x3
	if (O & NEIGHBOUR(x3))
    {
		if (flipped = flip(P, O, x3))
			return -ZWS_Exact_2(O ^ flipped, P ^ (1ULL << x3) ^ flipped, NodeCounter, -alpha-1, x1, x2);
    }

    if (!played) //Player couldn't play
    {
		++NodeCounter;

        //Play on x1
        if (P & NEIGHBOUR(x1))
        {
            if (flipped = flip(O, P, x1))
            {
				if (ZWS_Exact_2(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, alpha, x2, x3) <= alpha)
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
				if (ZWS_Exact_2(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, alpha, x1, x3) <= alpha)
					return alpha;
				else
					played = true;
            }
        }

        //Play on x3
        if (P & NEIGHBOUR(x3))
        {
            if (flipped = flip(O, P, x3))
				return ZWS_Exact_2(P ^ flipped, O ^ (1ULL << x3) ^ flipped, NodeCounter, alpha, x1, x2);
        }

        if (played)
			return alpha+1;
		else
            return EvaluateGameOver(P, 3);
    }

    return alpha;
}
int ZWS_Exact_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const unsigned char x1, const unsigned char x2, const unsigned char x3, const unsigned char x4)
{
	unsigned long long flipped;
	bool played = false;
	++NodeCounter;

    //Play on x1
    if (O & NEIGHBOUR(x1))
    {
		if (flipped = flip(P, O, x1))
        {
			if (-ZWS_Exact_3(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, -alpha-1, x2, x3, x4) > alpha)
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
			if (-ZWS_Exact_3(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, -alpha-1, x1, x3, x4) > alpha)
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
			if (-ZWS_Exact_3(O ^ flipped, P ^ (1ULL << x3) ^ flipped, NodeCounter, -alpha-1, x1, x2, x4) > alpha)
				return alpha+1;
			else
				played = true;
        }
    }

    //Play on x4
	if (O & NEIGHBOUR(x4))
    {
		if (flipped = flip(P, O, x4))
			return -ZWS_Exact_3(O ^ flipped, P ^ (1ULL << x4) ^ flipped, NodeCounter, -alpha-1, x1, x2, x3);
    }

    if (!played) //Player couldn't play
    {
		++NodeCounter;

        //Play on x1
        if (P & NEIGHBOUR(x1))
        {
            if (flipped = flip(O, P, x1))
            {
				if (ZWS_Exact_3(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, alpha, x2, x3, x4) <= alpha)
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
				if (ZWS_Exact_3(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, alpha, x1, x3, x4) <= alpha)
					return alpha;
            }
        }

        //Play on x3
        if (P & NEIGHBOUR(x3))
        {
            if (flipped = flip(O, P, x3))
            {
				if (ZWS_Exact_3(P ^ flipped, O ^ (1ULL << x3) ^ flipped, NodeCounter, alpha, x1, x2, x4) <= alpha)
					return alpha;
				else
					played = true;
            }
        }

        //Play on x4
        if (P & NEIGHBOUR(x4))
        {
            if (flipped = flip(O, P, x4))
				return ZWS_Exact_3(P ^ flipped, O ^ (1ULL << x4) ^ flipped, NodeCounter, alpha, x1, x2, x3);
        }

        if (played)
			return alpha+1;
		else
            return EvaluateGameOver(P, 4);
    }

    return alpha;
}
int ZWS_Exact_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const int alpha)
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

    return ZWS_Exact_4(P, O, NodeCounter, alpha, x1, x2, x3, x4);
}
int ZWS_Exact_A(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const unsigned char NumberOfEmptyStones, int alpha)
{
	if (NumberOfEmptyStones == 4)
		return ZWS_Exact_4(P, O, NodeCounter, parity, alpha);

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
				if (-ZWS_Exact_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
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
				if (-ZWS_Exact_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
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
				if (-ZWS_Exact_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
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
				if (-ZWS_Exact_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
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
					if (ZWS_Exact_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, alpha) <= alpha)
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
					if (ZWS_Exact_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, alpha) <= alpha)
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
					if (ZWS_Exact_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, alpha) <= alpha)
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
					if (ZWS_Exact_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, alpha) <= alpha)
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
int ZWS_Exact_B(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const unsigned char NumberOfEmptyStones, int alpha)
{
	if (NumberOfEmptyStones == 6)
		return ZWS_Exact_A(P, O, NodeCounter, parity, NumberOfEmptyStones, alpha);

	int value = -128;
	unsigned char Move;
	unsigned long long BitBoardTmp, BitBoardPossible, flipped;
	BitBoardPossible = PossibleMoves(P, O);
	++NodeCounter;
		
    if (!BitBoardPossible)
    {
		BitBoardPossible = PossibleMoves(O, P);
		if (BitBoardPossible)
			return -ZWS_Exact_B(O, P, NodeCounter, parity, NumberOfEmptyStones, -alpha-1);
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
		if (-ZWS_Exact_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
			return alpha+1;
	}

	BitBoardTmp = BitBoardPossible & BitBoardParity & ~PATTERN_FIRST;
	while (BitBoardTmp)
	{
		Move = BIT_SCAN_LS1B(BitBoardTmp);
		REMOVE_LS1B(BitBoardTmp);
		flipped = flip(P, O, Move);
		if (-ZWS_Exact_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
			return alpha+1;
	}

	BitBoardTmp = BitBoardPossible & ~BitBoardParity & PATTERN_FIRST;
	while (BitBoardTmp)
	{
		Move = BIT_SCAN_LS1B(BitBoardTmp);
		REMOVE_LS1B(BitBoardTmp);
		flipped = flip(P, O, Move);
		if (-ZWS_Exact_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
			return alpha+1;
	}

	BitBoardTmp = BitBoardPossible & ~BitBoardParity & ~PATTERN_FIRST;
	while (BitBoardTmp)
	{
		Move = BIT_SCAN_LS1B(BitBoardTmp);
		REMOVE_LS1B(BitBoardTmp);
		flipped = flip(P, O, Move);
		if (-ZWS_Exact_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -alpha-1) > alpha)
			return alpha+1;
	}

	return alpha;
}
int ZWS_Exact_C(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha)
{
	unsigned long long E = ~(P | O);
	const unsigned char NumberOfEmptyStones = POP_COUNT(E);
	if (NumberOfEmptyStones == 8)
		return ZWS_Exact_B(P, O, search.NodeCounter, parity(E), NumberOfEmptyStones, alpha);

	unsigned long long flipped;
	unsigned long long BitBoardPossible = PossibleMoves(P, O);

	++search.NodeCounter;

	if (!BitBoardPossible)
    {
		BitBoardPossible = PossibleMoves(O, P);
		if (BitBoardPossible)
			return -ZWS_Exact_C(search, O, P, -alpha-1);
		else{ //Game is over
			++search.NodeCounter;
			return BIND(EvaluateGameOver(P, NumberOfEmptyStones), alpha, alpha+1);
		}
    }

	if (StabilityCutoff(P, O, NumberOfEmptyStones, alpha))
        return alpha;
	
	unsigned long long LocalNodeCounter = search.NodeCounter;

	std::pair<bool, HashTableValueType> HashTableReturn = search.HashTableLookUp(P, O);
	if (HashTableReturn.first)
	{
		if (HashTableReturn.second.depth >= NumberOfEmptyStones)
		{
			if (HashTableReturn.second.alpha >= alpha+1)
				return alpha+1;
			if (HashTableReturn.second.beta <= alpha)
				return alpha;
			if (HashTableReturn.second.alpha == HashTableReturn.second.beta)
				return HashTableReturn.second.alpha;
			alpha = MAX(alpha, static_cast<int>(HashTableReturn.second.alpha));
		}

		if (HashTableReturn.second.PV != 64)
		{
			if ((BitBoardPossible & (1ULL << HashTableReturn.second.PV)) == 0ULL)
				std::cerr << "!" << (int)HashTableReturn.second.PV << " ";
			BitBoardPossible ^= (1ULL << HashTableReturn.second.PV);
			flipped = flip(P, O, HashTableReturn.second.PV);
			if (-ZWS_Exact_C(search, O ^ flipped, P ^ (1ULL << HashTableReturn.second.PV) ^ flipped, -alpha-1) > alpha)
			{
				search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, NumberOfEmptyStones, 0, alpha+1, 64, HashTableReturn.second.PV, HashTableReturn.second.AV);
				return alpha+1;
			}
		}

		if (HashTableReturn.second.AV != 64)
		{
			if ((BitBoardPossible & (1ULL << HashTableReturn.second.AV)) == 0ULL)
				std::cerr << "!" << (int)HashTableReturn.second.AV << " ";
			BitBoardPossible ^= (1ULL << HashTableReturn.second.AV);
			flipped = flip(P, O, HashTableReturn.second.AV);
			if (-ZWS_Exact_C(search, O ^ flipped, P ^ (1ULL << HashTableReturn.second.AV) ^ flipped, -alpha-1) > alpha)
			{
				search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, NumberOfEmptyStones, 0, alpha+1, 64, HashTableReturn.second.AV, HashTableReturn.second.PV);
				return alpha+1;
			}
		}
	}

	CMoveList mvList(P, O, BitBoardPossible);

	auto end = mvList.cend();
	for (auto it = mvList.cbegin(); it != end; ++it)
    {
        if (-ZWS_Exact_C(search, it->P, it->O, -alpha-1) > alpha)
		{
			search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, NumberOfEmptyStones, 0, alpha+1, 64, it->move, 64);
            return alpha+1;
        }
    }
	search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, NumberOfEmptyStones, 0, -64, alpha, 64, 64);
	return alpha;
}

int AlphaBeta_Exact_1(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const int x) 
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
int AlphaBeta_Exact_2(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta, const int x1, const int x2)
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
            value = -AlphaBeta_Exact_1(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, -beta, x2);
			if (value >= beta) return beta;
            if (value > alpha) alpha = value;
        }
    }

    //Play on x2
    if (O & NEIGHBOUR(x2))
    {
		flipped = flip(P, O, x2);
        if (flipped)
			return MAX(alpha, -AlphaBeta_Exact_1(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, -beta, x1));
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
                value = AlphaBeta_Exact_1(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, alpha, x2);
				if (value <= alpha) return alpha;
				if (value < beta) beta = value;
            }
        }

        //Play on x2
        if (P & NEIGHBOUR(x2))
        {
			flipped = flip(O, P, x2);
            if (flipped)
				return MIN(beta, AlphaBeta_Exact_1(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, alpha, x1));
        }

        if (value == -128) //Nobody could play
            return EvaluateGameOver(P, 2);
		else
			return beta;
    }

    return alpha;
}
int AlphaBeta_Exact_3(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta, const int x1, const int x2, const int x3)
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
            value = -AlphaBeta_Exact_2(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, -beta, -alpha, x2, x3);
			if (value >= beta) return beta;
            if (value > alpha) alpha = value;
        }
    }

    //Play on x2
    if (O & NEIGHBOUR(x2))
    {
		if (flipped = flip(P, O, x2))
        {
            value = -AlphaBeta_Exact_2(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, -beta, -alpha, x1, x3);
			if (value >= beta) return beta;
            if (value > alpha) alpha = value;
        }
    }

    //Play on x3
	if (O & NEIGHBOUR(x3))
    {
		if (flipped = flip(P, O, x3))
			return MAX(alpha, -AlphaBeta_Exact_2(O ^ flipped, P ^ (1ULL << x3) ^ flipped, NodeCounter, -beta, -alpha, x1, x2));
    }

    if (value == -128) //Player couldn't play
    {
		++NodeCounter;

        //Play on x1
        if (P & NEIGHBOUR(x1))
        {
            if (flipped = flip(O, P, x1))
            {
                value = AlphaBeta_Exact_2(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, alpha, beta, x2, x3);
				if (value <= alpha)	return alpha;
				if (value < beta) beta = value;
            }
        }

        //Play on x2
        if (P & NEIGHBOUR(x2))
        {
            if (flipped = flip(O, P, x2))
            {
                value = AlphaBeta_Exact_2(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, alpha, beta, x1, x3);
				if (value <= alpha)	return alpha;
				if (value < beta) beta = value;
            }
        }

        //Play on x3
        if (P & NEIGHBOUR(x3))
        {
            if (flipped = flip(O, P, x3))
				return MIN(beta, AlphaBeta_Exact_2(P ^ flipped, O ^ (1ULL << x3) ^ flipped, NodeCounter, alpha, beta, x1, x2));
        }

        if (value == -128) //Nobody could play
            return EvaluateGameOver(P, 3);
		else
			return beta;
    }

    return alpha;
}
int AlphaBeta_Exact_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta, const int x1, const int x2, const int x3, const int x4)
{
	unsigned long long flipped;
	int value = -128;
	++NodeCounter;
		
    //Play on x1
    if (O & NEIGHBOUR(x1))
    {
		if (flipped = flip(P, O, x1))
        {
            value = -AlphaBeta_Exact_3(O ^ flipped, P ^ (1ULL << x1) ^ flipped, NodeCounter, -beta, -alpha, x2, x3, x4);
			if (value >= beta) return beta;
            if (value > alpha) alpha = value;
        }
    }

    //Play on x2
    if (O & NEIGHBOUR(x2))
    {
		if (flipped = flip(P, O, x2))
        {
            value = -AlphaBeta_Exact_3(O ^ flipped, P ^ (1ULL << x2) ^ flipped, NodeCounter, -beta, -alpha, x1, x3, x4);
			if (value >= beta) return beta;
            if (value > alpha) alpha = value;
        }
    }

    //Play on x3
    if (O & NEIGHBOUR(x3))
    {
		if (flipped = flip(P, O, x3))
        {
            value = -AlphaBeta_Exact_3(O ^ flipped, P ^ (1ULL << x3) ^ flipped, NodeCounter, -beta, -alpha, x1, x2, x4);
			if (value >= beta) return beta;
            if (value > alpha) alpha = value;
        }
    }

    //Play on x4
    if (O & NEIGHBOUR(x4))
    {
		if (flipped = flip(P, O, x4))
			return MAX(alpha, -AlphaBeta_Exact_3(O ^ flipped, P ^ (1ULL << x4) ^ flipped, NodeCounter, -beta, -alpha, x1, x2, x3));
    }

    if (value == -128) //Player couldn't play
    {
		++NodeCounter;

		//Play on x1
        if (P & NEIGHBOUR(x1))
        {
            if (flipped = flip(O, P, x1))
            {
                value = AlphaBeta_Exact_3(P ^ flipped, O ^ (1ULL << x1) ^ flipped, NodeCounter, alpha, beta, x2, x3, x4);
				if (value <= alpha)	return alpha;
				if (value < beta) beta = value;
            }
        }

        //Play on x2
        if (P & NEIGHBOUR(x2))
        {
            if (flipped = flip(O, P, x2))
            {
                value = AlphaBeta_Exact_3(P ^ flipped, O ^ (1ULL << x2) ^ flipped, NodeCounter, alpha, beta, x1, x3, x4);
				if (value <= alpha)	return alpha;
				if (value < beta) beta = value;
            }
        }

		//Play on x3
        if (P & NEIGHBOUR(x3))
        {
            if (flipped = flip(O, P, x3))
            {
                value = AlphaBeta_Exact_3(P ^ flipped, O ^ (1ULL << x3) ^ flipped, NodeCounter, alpha, beta, x1, x2, x4);
				if (value <= alpha)	return alpha;
				if (value < beta) beta = value;
            }
        }

		//Play on x4
        if (P & NEIGHBOUR(x4))
        {
            if (flipped = flip(O, P, x4))
				return MIN(beta, AlphaBeta_Exact_3(P ^ flipped, O ^ (1ULL << x4) ^ flipped, NodeCounter, alpha, beta, x1, x2, x3));
        }

        if (value == -128) //Nobody could play
            return EvaluateGameOver(P, 4);
		else
			return beta;
    }

    return alpha;
}
int AlphaBeta_Exact_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const int alpha, const int beta)
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

	return AlphaBeta_Exact_4(P, O, NodeCounter, alpha, beta, x1, x2, x3, x4);
}
int AlphaBeta_Exact_A(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const unsigned char NumberOfEmptyStones, int alpha, int beta)
{
	if (NumberOfEmptyStones == 4)
		return AlphaBeta_Exact_4(P, O, NodeCounter, parity, alpha, beta);

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
				value = -AlphaBeta_Exact_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -beta, -alpha);
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
				value = -AlphaBeta_Exact_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -beta, -alpha);
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
				value = -AlphaBeta_Exact_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -beta, -alpha);
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
				value = -AlphaBeta_Exact_A(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -beta, -alpha);
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
					value = AlphaBeta_Exact_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, alpha, beta);
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
					value = AlphaBeta_Exact_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, alpha, beta);
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
					value = AlphaBeta_Exact_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, alpha, beta);
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
					value = AlphaBeta_Exact_A(P ^ flipped, O ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, alpha, beta);
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
int AlphaBeta_Exact_B(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned char parity, const unsigned char NumberOfEmptyStones, int alpha, int beta)
{
	if (NumberOfEmptyStones == 6)
		return AlphaBeta_Exact_A(P, O, NodeCounter, parity, NumberOfEmptyStones, alpha, beta);

	int value = -128;
	unsigned char Move;
	unsigned long long BitBoardTmp, BitBoardPossible, flipped;
	BitBoardPossible = PossibleMoves(P, O);
	++NodeCounter;
		
    if (!BitBoardPossible)
    {
		BitBoardPossible = PossibleMoves(O, P);
		if (BitBoardPossible)
			return -AlphaBeta_Exact_B(O, P, NodeCounter, parity, NumberOfEmptyStones, -beta, -alpha);
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
		value = -AlphaBeta_Exact_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -beta, -alpha);
		if (value >= beta) return beta;
		if (value > alpha) alpha = value;
	}

	BitBoardTmp = BitBoardPossible & BitBoardParity & ~PATTERN_FIRST;
	while (BitBoardTmp)
	{
		Move = BIT_SCAN_LS1B(BitBoardTmp);
		REMOVE_LS1B(BitBoardTmp);
		flipped = flip(P, O, Move);
		value = -AlphaBeta_Exact_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -beta, -alpha);
		if (value >= beta) return beta;
		if (value > alpha) alpha = value;
	}

	BitBoardTmp = BitBoardPossible & ~BitBoardParity & PATTERN_FIRST;
	while (BitBoardTmp)
	{
		Move = BIT_SCAN_LS1B(BitBoardTmp);
		REMOVE_LS1B(BitBoardTmp);
		flipped = flip(P, O, Move);
		value = -AlphaBeta_Exact_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -beta, -alpha);
		if (value >= beta) return beta;
		if (value > alpha) alpha = value;
	}

	BitBoardTmp = BitBoardPossible & ~BitBoardParity & ~PATTERN_FIRST;
	while (BitBoardTmp)
	{
		Move = BIT_SCAN_LS1B(BitBoardTmp);
		REMOVE_LS1B(BitBoardTmp);
		flipped = flip(P, O, Move);
		value = -AlphaBeta_Exact_B(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter, parity ^ QUADRANT_ID(Move), NumberOfEmptyStones-1, -beta, -alpha);
		if (value >= beta) return beta;
		if (value > alpha) alpha = value;
	}

	return alpha;
}

int PVS_Exact_C(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, int beta, unsigned char * pline)
{
	const unsigned long long E = ~(P | O);
	const unsigned char NumberOfEmptyStones = POP_COUNT(E);
	if (NumberOfEmptyStones == 8)
		return AlphaBeta_Exact_B(P, O, search.NodeCounter, parity(E), NumberOfEmptyStones, alpha, beta);

	bool SearchPV = true;
	int value;
	unsigned long long flipped;
	unsigned long long BitBoardPossible = PossibleMoves(P, O);

	++search.NodeCounter;

	if (!BitBoardPossible)
    {
		BitBoardPossible = PossibleMoves(O, P);
		if (BitBoardPossible)
			return -PVS_Exact_C(search, O, P, -beta, -alpha, pline);
		else{ //Game is over
			++search.NodeCounter;
			return BIND(EvaluateGameOver(P, NumberOfEmptyStones), alpha, beta);
		}
    }

	if (StabilityCutoff(P, O, NumberOfEmptyStones, alpha))
        return alpha;
	
	unsigned long long LocalNodeCounter = search.NodeCounter;
	unsigned char line[60];
	memset(line, static_cast<unsigned char>(64), 60 * sizeof(unsigned char));

	//if (NumberOfEmptyStones > 9)
	{
		std::pair<bool, HashTableValueType> HashTableReturn = search.HashTableLookUp(P, O);
		if (HashTableReturn.first)
		{
			if ((HashTableReturn.second.depth >= NumberOfEmptyStones) && (HashTableReturn.second.selectivity == 0))
			{
				if (HashTableReturn.second.alpha >= beta)
					return beta;
				if (HashTableReturn.second.beta <= alpha)
					return alpha;
				if (HashTableReturn.second.alpha == HashTableReturn.second.beta)
					return HashTableReturn.second.alpha;
				alpha = MAX(alpha, static_cast<int>(HashTableReturn.second.alpha));
			}

			if (HashTableReturn.second.PV != 64)
			{
				if ((BitBoardPossible & (1ULL << HashTableReturn.second.PV)) == 0ULL)
					std::cerr << "!" << (int)HashTableReturn.second.PV << " ";
				BitBoardPossible ^= (1ULL << HashTableReturn.second.PV);
				flipped = flip(P, O, HashTableReturn.second.PV);
				value = -PVS_Exact_C(search, O ^ flipped, P ^ (1ULL << HashTableReturn.second.PV) ^ flipped, -beta, -alpha, line);
				if (value >= beta)
				{
					search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, NumberOfEmptyStones, 0, beta, 64, HashTableReturn.second.PV, HashTableReturn.second.AV);
					return beta;
				}
				if (value > alpha)
				{
					pline[0] = HashTableReturn.second.PV;
					alpha = value;
					SearchPV = false;
				}
			}

			if (HashTableReturn.second.AV != 64)
			{
				if ((BitBoardPossible & (1ULL << HashTableReturn.second.AV)) == 0ULL)
					std::cerr << "!" << (int)HashTableReturn.second.AV << " ";
				BitBoardPossible ^= (1ULL << HashTableReturn.second.AV);
				flipped = flip(P, O, HashTableReturn.second.AV);
				value = -PVS_Exact_C(search, O ^ flipped, P ^ (1ULL << HashTableReturn.second.AV) ^ flipped, -beta, -alpha, line);
				if (value >= beta)
				{
					search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, NumberOfEmptyStones, 0, beta, 64, HashTableReturn.second.AV, HashTableReturn.second.PV);
					return beta;
				}
				if (value > alpha)
				{
					pline[0] = HashTableReturn.second.AV;
					alpha = value;
					SearchPV = false;
				}
			}
		}
	}

	CMoveList mvList(P, O, BitBoardPossible);

	auto end = mvList.cend();
	for (auto it = mvList.cbegin(); it != end; ++it)
    {
		if (SearchPV)
			value = -PVS_Exact_C(search, it->P, it->O, -beta, -alpha, line);
		else
		{
			value = -ZWS_Exact_C(search, it->P, it->O, -alpha-1);
			if (value > alpha)
				value = -PVS_Exact_C(search, it->P, it->O, -beta, -alpha, line);
		}
        if (value >= beta)
		{
			search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, NumberOfEmptyStones, 0, beta, 64, it->move, 64);
            return beta;
        }
		if (value > alpha)
		{
			pline[0] = it->move;
			alpha = value;
			SearchPV = false;
		}
    }
	if (!SearchPV){
		search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, NumberOfEmptyStones, 0, alpha, alpha, pline[0], 64);
		memcpy(pline+1, line, NumberOfEmptyStones * sizeof(unsigned char) - 1);
	}
	else
		search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, NumberOfEmptyStones, 0, -64, alpha, 64, 64);
	return alpha;
}

int PVS_EndCut(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, int beta, unsigned char * pline)
{
	const unsigned long long E = ~(P | O);
	const unsigned char NumberOfEmptyStones = POP_COUNT(E);
	if (NumberOfEmptyStones == 11)
	{
		float sigma = 1.8f * 7.24f;
		int bound;
		
		//bound = static_cast<int>(static_cast<float>(beta) + sigma + 0.5f);
		//if (LimitedDepth_1(P, O, search.NodeCounter, bound-1, bound) >= bound)
		//	return beta;

		//bound = static_cast<int>(static_cast<float>(alpha) - sigma + 0.5f);
		//if (LimitedDepth_1(P, O, search.NodeCounter, bound, bound+1) <= bound)
		//	return alpha;

		bound = static_cast<int>(static_cast<float>(beta) + sigma + 0.5f);
		if (PVS_LimitedDepth(search, P, O, bound-1, bound, 3, pline) >= bound)
			return beta;

		bound = static_cast<int>(static_cast<float>(alpha) - sigma + 0.5f);
		if (PVS_LimitedDepth(search, P, O, bound, bound+1, 3, pline) <= bound)
			return alpha;

		return PVS_Exact_C(search, P, O, alpha, beta, pline);
	}

	bool SearchPV = true;
	int value;
	unsigned long long flipped;
	unsigned long long BitBoardPossible = PossibleMoves(P, O);

	++search.NodeCounter;

	if (!BitBoardPossible)
    {
		BitBoardPossible = PossibleMoves(O, P);
		if (BitBoardPossible)
			return -PVS_EndCut(search, O, P, -beta, -alpha, pline);
		else{ //Game is over
			++search.NodeCounter;
			return BIND(EvaluateGameOver(P, NumberOfEmptyStones), alpha, beta);
		}
    }

	if (StabilityCutoff(P, O, NumberOfEmptyStones, alpha))
        return alpha;
	
	unsigned long long LocalNodeCounter = search.NodeCounter;
	unsigned char line[60];
	memset(line, static_cast<unsigned char>(64), 60 * sizeof(unsigned char));

	if (NumberOfEmptyStones > 9)
	{
		std::pair<bool, HashTableValueType> HashTableReturn = search.HashTableLookUp(P, O);
		if (HashTableReturn.first)
		{
			if (HashTableReturn.second.depth >= NumberOfEmptyStones)
			{
				if (HashTableReturn.second.alpha >= beta)
					return beta;
				if (HashTableReturn.second.beta <= alpha)
					return alpha;
				if (HashTableReturn.second.alpha == HashTableReturn.second.beta)
					return HashTableReturn.second.alpha;
				alpha = MAX(alpha, static_cast<int>(HashTableReturn.second.alpha));
			}

			if (HashTableReturn.second.PV != 64)
			{
				if ((BitBoardPossible & (1ULL << HashTableReturn.second.PV)) == 0ULL)
					std::cerr << "!" << (int)HashTableReturn.second.PV << " ";
				BitBoardPossible ^= (1ULL << HashTableReturn.second.PV);
				flipped = flip(P, O, HashTableReturn.second.PV);
				value = -PVS_EndCut(search, O ^ flipped, P ^ (1ULL << HashTableReturn.second.PV) ^ flipped, -beta, -alpha, line);
				if (value >= beta)
				{
					search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, NumberOfEmptyStones, 1, beta, 64, HashTableReturn.second.PV, HashTableReturn.second.AV);
					return beta;
				}
				if (value > alpha)
				{
					pline[0] = HashTableReturn.second.PV;
					memcpy(pline+1, line, NumberOfEmptyStones * sizeof(unsigned char) - 1);
					alpha = value;
					SearchPV = false;
				}
			}

			if (HashTableReturn.second.AV != 64)
			{
				if ((BitBoardPossible & (1ULL << HashTableReturn.second.AV)) == 0ULL)
					std::cerr << "!" << (int)HashTableReturn.second.AV << " ";
				BitBoardPossible ^= (1ULL << HashTableReturn.second.AV);
				flipped = flip(P, O, HashTableReturn.second.AV);
				value = -PVS_EndCut(search, O ^ flipped, P ^ (1ULL << HashTableReturn.second.AV) ^ flipped, -beta, -alpha, line);
				if (value >= beta)
				{
					search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, NumberOfEmptyStones, 1, beta, 64, HashTableReturn.second.AV, HashTableReturn.second.PV);
					return beta;
				}
				if (value > alpha)
				{
					pline[0] = HashTableReturn.second.AV;
					memcpy(pline+1, line, NumberOfEmptyStones * sizeof(unsigned char) - 1);
					alpha = value;
					SearchPV = false;
				}
			}
		}
	}

	CMoveList mvList(P, O, BitBoardPossible);

	auto end = mvList.cend();
	for (auto it = mvList.cbegin(); it != end; ++it)
    {
		if (SearchPV)
			value = -PVS_EndCut(search, it->P, it->O, -beta, -alpha, line);
		else
		{
			value = -ZWS_Exact_C(search, it->P, it->O, -alpha-1);
			if (value > alpha)
				value = -PVS_EndCut(search, it->P, it->O, -beta, -alpha, line);
		}
        if (value >= beta)
		{
			search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, NumberOfEmptyStones, 1, beta, 64, it->move, 64);
            return beta;
        }
		if (value > alpha)
		{
			pline[0] = it->move;
			memcpy(pline+1, line, NumberOfEmptyStones * sizeof(unsigned char) - 1);
			alpha = value;
			SearchPV = false;
		}
    }
	if (!SearchPV)
		search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, NumberOfEmptyStones, 1, alpha, alpha, pline[0], 64);
	else
		search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, NumberOfEmptyStones, 1, -64, alpha, 64, 64);
	return alpha;
}

int LimitedDepth(CSearch & search, const int depth)
{
	switch (depth)
	{
	case 0: return LimitedDepth_0(search.P, search.O, search.NodeCounter);
	case 1: return LimitedDepth_1(search.P, search.O, search.NodeCounter, search.alpha, search.beta);
	case 2: return LimitedDepth_2(search, search.P, search.O, search.alpha, search.beta);
	default: return PVS_LimitedDepth(search, search.P, search.O, search.alpha, search.beta, depth, search.PV);
	}
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
	return AlphaBeta_Exact_1(P, O, NodeCounter, alpha, x1);
}
inline int Exact_2(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const int beta)
{
	unsigned long long E = ~(P | O);
	const int x1 = BIT_SCAN_LS1B(E); REMOVE_LS1B(E);
	const int x2 = BIT_SCAN_LS1B(E);
	return AlphaBeta_Exact_2(P, O, NodeCounter, alpha, beta, x1, x2);
}
inline int Exact_3(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const int beta)
{
	unsigned long long E = ~(P | O);
	const int x1 = BIT_SCAN_LS1B(E); REMOVE_LS1B(E);
	const int x2 = BIT_SCAN_LS1B(E); REMOVE_LS1B(E);
	const int x3 = BIT_SCAN_LS1B(E);
	return AlphaBeta_Exact_3(P, O, NodeCounter, alpha, beta, x1, x2, x3);
}
inline int Exact_4(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const int alpha, const int beta)
{
	return AlphaBeta_Exact_4(P, O, NodeCounter, parity(P, O), alpha, beta);
}

inline void Exact_0(CSearch & search) { search.score = Exact_0(search.P, search.O, search.NodeCounter); }
inline void Exact_1(CSearch & search) { search.score = Exact_1(search.P, search.O, search.NodeCounter, search.alpha); }
inline void Exact_2(CSearch & search) { search.score = Exact_2(search.P, search.O, search.NodeCounter, search.alpha, search.beta); }
inline void Exact_3(CSearch & search) { search.score = Exact_3(search.P, search.O, search.NodeCounter, search.alpha, search.beta); }
inline void Exact_4(CSearch & search) { search.score = AlphaBeta_Exact_4(search.P, search.O, search.NodeCounter, parity(search.P, search.O), search.alpha, search.beta); }
inline void Exact_A(CSearch & search) { search.score = AlphaBeta_Exact_A(search.P, search.O, search.NodeCounter, parity(search.P, search.O), NumberOfEmptyStones(search.P, search.O), search.alpha, search.beta); }
inline void Exact_B(CSearch & search) { search.score = AlphaBeta_Exact_B(search.P, search.O, search.NodeCounter, parity(search.P, search.O), NumberOfEmptyStones(search.P, search.O), search.alpha, search.beta); }
inline void Exact_C(CSearch & search) { search.score = PVS_Exact_C(search, search.P, search.O, search.alpha, search.beta, search.PV); }
void Exact_D(CSearch & search)
{
	const int Empties = NumberOfEmptyStones(search.P, search.O);
	for (int d = 1; Empties - d > 10; d++)
		LimitedDepth(search, d);
	//LimitedDepth(search, Empties - 8);
	//PVS_EndCut(search, search.P, search.O, search.alpha, search.beta, search.PV);
	Exact_C(search);
}
//int Exact_D(CSearch & search)
//{
//	unsigned long long E = ~(search.P | search.O);
//	const int NumberOfEmptyStones = POP_COUNT(E);
//	for (int d = 1; NumberOfEmptyStones - d > 10; d++)
//		LimitedDepth(search, d);
//	return Exact_C(search);
//}

// For benchmarking purposes
int EvaluateExact(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned long long NumberOfEmptyStones)
{
	switch (NumberOfEmptyStones)
	{
	case 0: return Exact_0(P, O, NodeCounter);
	case 1: return Exact_1(P, O, NodeCounter, -64);
	case 2: return Exact_2(P, O, NodeCounter, -64, 64);
	case 3: return Exact_3(P, O, NodeCounter, -64, 64);
	case 4: return Exact_4(P, O, NodeCounter, -64, 64);
	default: throw std::logic_error("Wrong number of empty stones!");
	}
}

void EvaluateExact(CSearch & search)
{
	const unsigned long long Empties = NumberOfEmptyStones(search.P, search.O);
	switch (Empties)
	{
	case  0: Exact_0(search); break;
	case  1: Exact_1(search); break;
	case  2: Exact_2(search); break;
	case  3: Exact_3(search); break;
	case  4: Exact_4(search); break;
	case  5: Exact_A(search); break;
	case  6: Exact_A(search); break;
	case  7: Exact_B(search); break;
	case  8: Exact_B(search); break;
	case  9: Exact_C(search); break;
	case 10: Exact_C(search); break;
	case 11: Exact_C(search); break;
	default:
		Exact_D(search); break;
	}
}

void EvaluateEndCut(CSearch & search)
{
	const unsigned long long Empties = NumberOfEmptyStones(search.P, search.O);
}

void EvaluateLimitedDepth(CSearch & search)
{
	search.score = LimitedDepth(search, search.depth);
}

void EvaluateMPC(CSearch & search)
{
	const unsigned long long Empties = NumberOfEmptyStones(search.P, search.O);
}