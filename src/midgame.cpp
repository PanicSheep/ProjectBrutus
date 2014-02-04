//#include "endgame.h"
//
//inline int LimitedDepth_0(const CActiveConfigurations & actives, const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter)
//{
//	++NodeCounter;
//	return actives.EvaluateFeatures(P, O);
//}
//int LimitedDepth_0(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter)
//{
//	++NodeCounter;
//	return EvaluateFeatures(P, O);
//}
//int LimitedDepth_1(const CActiveConfigurations & actives, const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta)
//{
//	int value = -128;
//	unsigned char Move;
//	unsigned long long flipped;
//	unsigned long long BitBoardPossible = PossibleMoves(P, O);
//
//	++NodeCounter;
//
//	if (!BitBoardPossible)
//    {
//		BitBoardPossible = PossibleMoves(O, P);
//		if (BitBoardPossible)
//			return -LimitedDepth_1(actives, O, P, NodeCounter, -beta, -alpha);
//		else{ //Game is over
//			++NodeCounter; 
//			return BIND(EvaluateGameOver(P, NumberOfEmpyStones(P, O)), alpha, beta);
//		}
//    }
//
//	while (BitBoardPossible)
//	{
//		Move = BIT_SCAN_LS1B(BitBoardPossible);
//		REMOVE_LS1B(BitBoardPossible);
//		flipped = flip(P, O, Move);
//		value = -LimitedDepth_0(actives, O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter);
//		if (value >= beta)
//            return beta;
//		if (value > alpha)
//			alpha = value;
//	}
//	return alpha;
//}
//int LimitedDepth_1(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta)
//{
//	int value = -128;
//	unsigned char Move;
//	unsigned long long flipped;
//	unsigned long long BitBoardPossible = PossibleMoves(P, O);
//
//	++NodeCounter;
//
//	if (!BitBoardPossible)
//    {
//		BitBoardPossible = PossibleMoves(O, P);
//		if (BitBoardPossible)
//			return -LimitedDepth_1(O, P, NodeCounter, -beta, -alpha);
//		else{ //Game is over
//			++NodeCounter; 
//			return BIND(EvaluateGameOver(P, NumberOfEmptyStones(P, O)), alpha, beta);
//		}
//    }
//
//	while (BitBoardPossible)
//	{
//		Move = BIT_SCAN_LS1B(BitBoardPossible);
//		REMOVE_LS1B(BitBoardPossible);
//		flipped = flip(P, O, Move);
//		value = -LimitedDepth_0(O ^ flipped, P ^ (1ULL << Move) ^ flipped, NodeCounter);
//		if (value >= beta) return beta;
//		if (value > alpha) alpha = value;
//	}
//	return alpha;
//}
//int LimitedDepth_2(CSearch & search, const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta)
//{
//	const unsigned char Empties = NumberOfEmptyStones(P, O);
//	int value = -128;
//	unsigned char BestMove = 64;
//	unsigned long long flipped;
//	unsigned long long BitBoardPossible = PossibleMoves(P, O);
//
//	++NodeCounter;
//
//	if (!BitBoardPossible)
//    {
//		BitBoardPossible = PossibleMoves(O, P);
//		if (BitBoardPossible)
//			return -LimitedDepth_2(search, O, P, NodeCounter, -beta, -alpha);
//		else{ //Game is over
//			++NodeCounter; 
//			return BIND(EvaluateGameOver(P, Empties), alpha, beta);
//		}
//    }
//	
//	CActiveConfigurations actives(P, O);
//	unsigned long long LocalNodeCounter = search.NodeCounter;
//	
//	CMoveList mvList(P, O, BitBoardPossible);
//	auto end = mvList.cend();
//	for (auto it = mvList.cbegin(); it != end; ++it)
//    {
//		value = -LimitedDepth_1(search, actives, it->P, it->O, search.NodeCounter, -beta, -alpha);
//        if (value >= beta)
//		{
//			search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, 2, 0, beta, 64, it->move, 64, false, true, true);
//            return beta;
//        }
//		if (value > alpha)
//		{
//			BestMove = it->move;
//			alpha = value;
//		}
//    }
//	if (BestMove != 64)
//		search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, 2, 0, alpha, alpha, BestMove, 64, true, true, true);
//	else
//		search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, 2, 0, -64, alpha, 64, 64, false, false, true);
//	return alpha;
//}
//
//int PVS_LimitedDepth(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, int beta, int depth, unsigned char * pline)
//{
//	//if (depth == 0)
//	//	return LimitedDepth_0(P, O, search.NodeCounter);
//	//if (depth == 1)
//	//	return LimitedDepth_1(P, O, search.NodeCounter, alpha, beta);
//	if (depth == 2)
//		return LimitedDepth_2(search, P, O, search.NodeCounter, alpha, beta);
//
//	const unsigned char Empties = NumberOfEmptyStones(P, O);
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
//			return -PVS_LimitedDepth(search, O, P, -beta, -alpha, depth, pline);
//		else{ //Game is over
//			++search.NodeCounter;
//			return BIND(EvaluateGameOver(P, Empties), alpha, beta);
//		}
//    }
//	
//	unsigned long long LocalNodeCounter = search.NodeCounter;
//	unsigned char line[60];
//
//	std::pair<bool, HashTableValueType> HashTableReturn = search.HashTableLookUp(P, O);
//	if (HashTableReturn.first)
//	{
//		if (HashTableReturn.second.depth >= NumberOfEmptyStones)
//		{
//			if (HashTableReturn.second.alpha >= beta)
//				return beta;
//			if (HashTableReturn.second.beta <= alpha)
//				return alpha;
//			if (HashTableReturn.second.alpha == HashTableReturn.second.beta)
//				return HashTableReturn.second.alpha;
//			alpha = MAX(alpha, static_cast<int>(HashTableReturn.second.alpha));
//		}
//
//		if (HashTableReturn.second.PV != 64)
//		{
//			if ((BitBoardPossible & (1ULL << HashTableReturn.second.PV)) == 0ULL)
//				std::cerr << "!" << (int)HashTableReturn.second.PV << " ";
//			BitBoardPossible ^= (1ULL << HashTableReturn.second.PV);
//			flipped = flip(P, O, HashTableReturn.second.PV);
//			value = -PVS_LimitedDepth(search, O ^ flipped, P ^ (1ULL << HashTableReturn.second.PV) ^ flipped, -beta, -alpha, depth-1, line);
//			if (value >= beta)
//			{
//				search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, depth, 0, beta, 64, HashTableReturn.second.PV, HashTableReturn.second.AV, false, true, true);
//				return beta;
//			}
//			if (value > alpha)
//			{
//				pline[0] = HashTableReturn.second.PV;
//				memcpy(pline+1, line, Empties * sizeof(unsigned char));
//				alpha = value;
//				SearchPV = false;
//			}
//		}
//
//		if (HashTableReturn.second.AV != 64)
//		{
//			if ((BitBoardPossible & (1ULL << HashTableReturn.second.AV)) == 0ULL)
//				std::cerr << "!" << (int)HashTableReturn.second.AV << " ";
//			BitBoardPossible ^= (1ULL << HashTableReturn.second.AV);
//			flipped = flip(P, O, HashTableReturn.second.AV);
//			value = -PVS_LimitedDepth(search, O ^ flipped, P ^ (1ULL << HashTableReturn.second.AV) ^ flipped, -beta, -alpha, depth-1, line);
//			if (value >= beta)
//			{
//				search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, depth, 0, beta, 64, HashTableReturn.second.AV, HashTableReturn.second.PV, false, true, true);
//				return beta;
//			}
//			if (value > alpha)
//			{
//				pline[0] = HashTableReturn.second.AV;
//				memcpy(pline+1, line, Empties * sizeof(unsigned char));
//				alpha = value;
//				SearchPV = false;
//			}
//		}
//	}
//
//	CMoveList mvList(P, O, BitBoardPossible);
//	auto end = mvList.cend();
//	for (auto it = mvList.cbegin(); it != end; ++it)
//    {
//		if (SearchPV)
//			value = -PVS_LimitedDepth(search, it->P, it->O, -beta, -alpha, depth-1, line);
//		else
//		{
//			value = -PVS_LimitedDepth(search, it->P, it->O, -alpha-1, -alpha, depth-1, line);
//			if (value > alpha)
//				value = -PVS_LimitedDepth(search, it->P, it->O, -beta, -alpha, depth-1, line);
//		}
//        if (value >= beta)
//		{
//			search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, depth, 0, beta, 64, it->move, 64, false, true, true);
//            return beta;
//        }
//		if (value > alpha)
//		{
//			pline[0] = it->move;
//			memcpy(pline+1, line, Empties * sizeof(unsigned char));
//			alpha = value;
//			SearchPV = false;
//		}
//    }
//	if (!SearchPV)
//		search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, depth, 0, alpha, alpha, pline[0], 64, true, true, true);
//	else
//		search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, depth, 0, -64, alpha, 64, 64, false, false, true);
//	return alpha;
//}
//
//int MPC(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, int beta, int depth, unsigned char * pline)
//{
//	//if (depth == 0)
//	//	return LimitedDepth_0(P, O, search.NodeCounter);
//	//if (depth == 1)
//	//	return LimitedDepth_1(P, O, search.NodeCounter, alpha, beta);
//	if (depth == 2)
//		return LimitedDepth_2(search, P, O, search.NodeCounter, alpha, beta);
//
//	const unsigned char Empties = NumberOfEmptyStones(P, O);
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
//			return -PVS_LimitedDepth(search, O, P, -beta, -alpha, depth, pline);
//		else{ //Game is over
//			++search.NodeCounter;
//			return BIND(EvaluateGameOver(P, Empties), alpha, beta);
//		}
//    }
//	
//	unsigned long long LocalNodeCounter = search.NodeCounter;
//	unsigned char line[60];
//
//	std::pair<bool, HashTableValueType> HashTableReturn = search.HashTableLookUp(P, O);
//	if (HashTableReturn.first)
//	{
//		if (HashTableReturn.second.depth >= NumberOfEmptyStones)
//		{
//			if (HashTableReturn.second.alpha >= beta)
//				return beta;
//			if (HashTableReturn.second.beta <= alpha)
//				return alpha;
//			if (HashTableReturn.second.alpha == HashTableReturn.second.beta)
//				return HashTableReturn.second.alpha;
//			alpha = MAX(alpha, static_cast<int>(HashTableReturn.second.alpha));
//		}
//
//		if (HashTableReturn.second.PV != 64)
//		{
//			if ((BitBoardPossible & (1ULL << HashTableReturn.second.PV)) == 0ULL)
//				std::cerr << "!" << (int)HashTableReturn.second.PV << " ";
//			BitBoardPossible ^= (1ULL << HashTableReturn.second.PV);
//			flipped = flip(P, O, HashTableReturn.second.PV);
//			value = -PVS_LimitedDepth(search, O ^ flipped, P ^ (1ULL << HashTableReturn.second.PV) ^ flipped, -beta, -alpha, depth-1, line);
//			if (value >= beta)
//			{
//				search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, depth, 0, beta, 64, HashTableReturn.second.PV, HashTableReturn.second.AV, false, true, true);
//				return beta;
//			}
//			if (value > alpha)
//			{
//				pline[0] = HashTableReturn.second.PV;
//				memcpy(pline+1, line, Empties * sizeof(unsigned char));
//				alpha = value;
//				SearchPV = false;
//			}
//		}
//
//		if (HashTableReturn.second.AV != 64)
//		{
//			if ((BitBoardPossible & (1ULL << HashTableReturn.second.AV)) == 0ULL)
//				std::cerr << "!" << (int)HashTableReturn.second.AV << " ";
//			BitBoardPossible ^= (1ULL << HashTableReturn.second.AV);
//			flipped = flip(P, O, HashTableReturn.second.AV);
//			value = -PVS_LimitedDepth(search, O ^ flipped, P ^ (1ULL << HashTableReturn.second.AV) ^ flipped, -beta, -alpha, depth-1, line);
//			if (value >= beta)
//			{
//				search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, depth, 0, beta, 64, HashTableReturn.second.AV, HashTableReturn.second.PV, false, true, true);
//				return beta;
//			}
//			if (value > alpha)
//			{
//				pline[0] = HashTableReturn.second.AV;
//				memcpy(pline+1, line, Empties * sizeof(unsigned char));
//				alpha = value;
//				SearchPV = false;
//			}
//		}
//	}
//
//	CMoveList mvList(P, O, BitBoardPossible);
//	auto end = mvList.cend();
//	for (auto it = mvList.cbegin(); it != end; ++it)
//    {
//		if (SearchPV)
//			value = -PVS_LimitedDepth(search, it->P, it->O, -beta, -alpha, depth-1, line);
//		else
//		{
//			value = -PVS_LimitedDepth(search, it->P, it->O, -alpha-1, -alpha, depth-1, line);
//			if (value > alpha)
//				value = -PVS_LimitedDepth(search, it->P, it->O, -beta, -alpha, depth-1, line);
//		}
//        if (value >= beta)
//		{
//			search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, depth, 0, beta, 64, it->move, 64, false, true, true);
//            return beta;
//        }
//		if (value > alpha)
//		{
//			pline[0] = it->move;
//			memcpy(pline+1, line, Empties * sizeof(unsigned char));
//			alpha = value;
//			SearchPV = false;
//		}
//    }
//	if (!SearchPV)
//		search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, depth, 0, alpha, alpha, pline[0], 64, true, true, true);
//	else
//		search.HashTableUpdate(P, O, search.NodeCounter-LocalNodeCounter, Empties, depth, 0, -64, alpha, 64, 64, false, false, true);
//	return alpha;
//}