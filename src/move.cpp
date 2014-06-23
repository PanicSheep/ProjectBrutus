#include "move.h"

const int w_PossibleMoves = 15;
const int w_Stable = 8;
const int w_OpponentsBoarder = 2;
const int w_PlayersExposeds = 3;
const int w_OpponentsExposeds = 7;
const int w_PlayersBoarder = 5;
const int w_Parity = 5;


CMoveList::CMoveList(CSearch & search, const unsigned long long P, const unsigned long long O, unsigned long long BitBoardPossible, const int depth, const HashTableValueType & htValue)
{
	unsigned long long flipped;
	unsigned long long Empties = NumberOfEmptyStones(P, O);
	
	//if (depth == Empties)
	//{
	//	if (Empties < 16)
	//	{
	//		const unsigned char Parity = parity(P, O);
	//		while (BitBoardPossible)
	//		{
	//			Move move;
	//			move.move = BitScanLSB(BitBoardPossible);
	//			RemoveLSB(BitBoardPossible);
	//			flipped = flip(P, O, move.move);
	//			move.P = O ^ flipped;
	//			move.O = P ^ (1ULL << move.move) ^ flipped;
	//			move.PossibleMoves = PossibleMoves(move.P, move.O);
	//			move.Value = ((PopCount_max15(move.PossibleMoves) + PopCount_max15(move.PossibleMoves & 0x8100000000000081UL)) << w_PossibleMoves);
	//			move.Value -= (PopCount_max15(StableStones_corner_and_co(move.O)) << w_Stable);
	//			move.Value += (PopCount_max15(OpponentsBoarder(move.P, move.O)) << w_OpponentsBoarder);
	//			move.Value -= (PopCount(PlayersExposed(move.P, move.O)) << w_PlayersExposeds);
	//			move.Value += (PopCount(OpponentsExposed(move.P, move.O)) << w_OpponentsExposeds);
	//			move.Value -= (PopCount_max15(PlayersBoarder(move.P, move.O)) << w_PlayersBoarder);
	//			if (Parity & QUADRANT_ID(move.move))
	//				move.Value -= 1 << w_Parity;

	//			m_Moves.push_back(move);
	//			//std::push_heap(m_Moves.begin(), m_Moves.end(), [](const Move& Move1, const Move& Move2){ return Move1.m_Value > Move2.m_Value; });
	//		}
	//	}
	//	else
	//	{
	//		const int magnitude_InHashTable = 19;
	//		const int value_InHashTable_ExactValueBonus = 1 << 29;
	//		const int value_InHashTable_ZeroWindowBonus = 1 << 22;
	//		const int magnitude_InHashTable_WindowSizeBonus = 18;

	//		while (BitBoardPossible)
	//		{
	//			Move move;
	//			move.move = BitScanLSB(BitBoardPossible);
	//			RemoveLSB(BitBoardPossible);
	//			flipped = flip(P, O, move.move);
	//			move.P = O ^ flipped;
	//			move.O = P ^ (1ULL << move.move) ^ flipped;
	//			move.PossibleMoves = PossibleMoves(move.P, move.O);
	//			move.Value = ((PopCount(move.PossibleMoves) + PopCount_max15(move.PossibleMoves & 0x8100000000000081UL)) << 4);
	//			move.Value -= (PopCount_max15(StableStones_corner_and_co(move.O)) << 5);
	//			move.Value += (PopCount(OpponentsBoarder(move.P, move.O)) << 2);
	//			move.Value -= (PopCount(PlayersExposed(move.P, move.O)) << 0);
	//			move.Value += (PopCount(OpponentsExposed(move.P, move.O)) << 2);
	//			move.Value -= (PopCount_max15(move.PossibleMoves & 0x42C300000000C342UL) << 3);

	//			//if (HashTableReturn_p.first)
	//			//{
	//			//	if (move.move == HashTableReturn_p.second.PV)
	//			//		move.Value -= (1 << 4);
	//			//}

	//			//std::pair<bool, ReversiValueType> HashTableReturn_n = search.CheckHashTable(move.m_Position);

	//			//if (HashTableReturn_n.first){
	//			//	move.m_Value -= HashTableReturn_n.second.m_depth << magnitude_InHashTable;
	//			//	if (HashTableReturn_n.second.m_alpha == HashTableReturn_n.second.m_beta){
	//			//		if (HashTableReturn_n.second.m_depth == move.m_Position.NumberOfEmptyStones())
	//			//			move.m_Value -= value_InHashTable_ExactValueBonus;
	//			//		else
	//			//			move.m_Value -= value_InHashTable_ZeroWindowBonus;
	//			//	}
	//			//	else
	//			//		move.m_Value -= ((128 - (HashTableReturn_n.second.m_beta - HashTableReturn_n.second.m_alpha))/32) << magnitude_InHashTable_WindowSizeBonus;
	//			//}

	//			m_Moves.push_back(move);
	//			//std::push_heap(m_Moves.begin(), m_Moves.end(), [](const Move& Move1, const Move& Move2){ return Move1.m_Value > Move2.m_Value; });
	//		}
	//	}
	//	std::sort(m_Moves.begin(), m_Moves.end(), [](const Move& Move1, const Move& Move2){ return Move1.Value < Move2.Value; });
	//	return;
	//}

	//if (Empties < 17)
	//{
	//	const unsigned char Parity = parity(P, O);
	//	while (BitBoardPossible)
	//	{
	//		Move move;
	//		move.move = BitScanLSB(BitBoardPossible);
	//		RemoveLSB(BitBoardPossible);
	//		flipped = flip(P, O, move.move);
	//		move.P = O ^ flipped;
	//		move.O = P ^ (1ULL << move.move) ^ flipped;
	//		move.PossibleMoves = PossibleMoves(move.P, move.O);
	//		move.Value = -((PopCount_max15(move.PossibleMoves) + PopCount_max15(move.PossibleMoves & 0x8100000000000081UL)) << w_PossibleMoves);
	//		move.Value += (PopCount_max15(StableStones_corner_and_co(move.O)) << w_Stable);
	//		move.Value -= (PopCount_max15(OpponentsBoarder(move.P, move.O)) << w_OpponentsBoarder);
	//		move.Value += (PopCount(PlayersExposed(move.P, move.O)) << w_PlayersExposeds);
	//		move.Value -= (PopCount(OpponentsExposed(move.P, move.O)) << w_OpponentsExposeds);
	//		move.Value += (PopCount_max15(PlayersBoarder(move.P, move.O)) << w_PlayersBoarder);
	//		if (Parity & QUADRANT_ID(move.move))
	//			move.Value += 1 << w_Parity;
	//		m_Moves.push_back(move);
	//	}
	//	std::sort(m_Moves.begin(), m_Moves.end(), [](const Move& Move1, const Move& Move2){ return Move1.Value > Move2.Value; });
	//	return;
	//}
	//else
	//{
		HashTableValueType htValue2;
		static const int SQUARE_VALUE[] = {
			18,  4,  16, 12, 12, 16,  4, 18,
			 4,  2,   6,  8,  8,  6,  2,  4,
			16,  6,  14, 10, 10, 14,  6, 16,
			12,  8,  10,  0,  0, 10,  8, 12,
			12,  8,  10,  0,  0, 10,  8, 12,
			16,  6,  14, 10, 10, 14,  6, 16,
			 4,  2,   6,  8,  8,  6,  2,  4,
			18,  4,  16, 12, 12, 16,  4, 18
		};
		static const int sort_depth[61] = {
		//   0   1   2   3   4   5   6   7   8   9

			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1,  0,  0,  0,  1,  1,	 1,  2,  2, 
			 2,  3,  3,  3,  4,  4,  4,	 5,  5,  5,
			 6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
			 6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
			 6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
			 6
		};
		const unsigned char Parity = parity(P, O);
		while (BitBoardPossible)
		{
			Move move;
			move.move = BitScanLSB(BitBoardPossible);
			RemoveLSB(BitBoardPossible);
			flipped = flip(P, O, move.move);
			move.P = O ^ flipped;
			move.O = P ^ (1ULL << move.move) ^ flipped;
			move.Value = 0;
			move.PossibleMoves = PossibleMoves(move.P, move.O);
			if (htValue.depth >= sort_depth[depth])
			{
				if (move.move == htValue.PV)
					move.Value += 1ULL << 23;
				else if (move.move == htValue.AV)
					move.Value += 1ULL << 21;
			}
			if (move.Value == 0)
			{
				move.Value += SQUARE_VALUE[move.move];
				move.Value -= (PopCount(move.PossibleMoves) + PopCount_max15(move.PossibleMoves & 0x8100000000000081UL)) << 15;
				move.Value += PopCount(StableStones_corner_and_co(move.O)) << 11;
				move.Value -= (PopCount(OpponentsBoarder(move.P, move.O))) << 5;
				//move.Value -= (PopCount(OpponentsExposed(move.P, move.O)) << 7);
				if (Parity & QUADRANT_ID(move.move))
					move.Value += 1 << 1;
				switch (sort_depth[depth])
				{
				case -1:
					break;
				case 0:
					move.Value -= (EvaluateLimitedDepth(search, move.P, move.O, -64, 64, 0, 0) >> 2) << 15;
					break;
				case 1:
				case 2:
					move.Value -= (EvaluateLimitedDepth(search, move.P, move.O, -64, 64, sort_depth[depth], 0) >> 1) << 15;
					break;
				default:
					move.Value -= EvaluateLimitedDepth(search, move.P, move.O, -64, 64, sort_depth[depth], 0) << 15;
					if (search.HashTableLookUp(move.P, move.O, htValue2))
						move.Value += 1ULL << 15;
					break;
				}
			}
			m_Moves.push_back(move);
		}
		std::sort(m_Moves.begin(), m_Moves.end(), [](const Move& Move1, const Move& Move2){ return Move1.Value > Move2.Value; });
		return;
	//}
}