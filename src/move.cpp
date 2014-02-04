#include "move.h"

const int w_PossibleMoves = 15;
const int w_Stable = 8;
const int w_OpponentsBoarder = 2;
const int w_PlayersExposeds = 3;
const int w_OpponentsExposeds = 7;
const int w_PlayersBoarder = 5;
const int w_Parity = 5;

CMoveList::CMoveList(const unsigned long long P, const unsigned long long O, unsigned long long BitBoardPossible)
{
	unsigned long long flipped;
	unsigned long long E = ~(P | O);
	const unsigned char NumberOfEmptyStones = POP_COUNT(E);
	const unsigned char parity = (POP_COUNT(E & 0xF0F0F0F000000000ULL) & 1) << 3 
							   | (POP_COUNT(E & 0x0F0F0F0F00000000ULL) & 1) << 2 
							   | (POP_COUNT(E & 0x00000000F0F0F0F0ULL) & 1) << 1 
							   |  POP_COUNT(E & 0x000000000F0F0F0FULL) & 1;
	if (NumberOfEmptyStones < 15)
	{
		while (BitBoardPossible)
		{
			Move move;
			move.move = BIT_SCAN_LS1B(BitBoardPossible);
			REMOVE_LS1B(BitBoardPossible);
			flipped = flip(P, O, move.move);
			move.P = O ^ flipped;
			move.O = P ^ (1ULL << move.move) ^ flipped;
			move.PossibleMoves = PossibleMoves(move.P, move.O);
			move.Value = ((POP_COUNT(move.PossibleMoves) + POP_COUNT(move.PossibleMoves & 0x8100000000000081UL)) << w_PossibleMoves);
			move.Value -= (POP_COUNT(StableStones_corner_and_co(move.O)) << w_Stable);
			move.Value += (POP_COUNT(OpponentsBoarder(move.P, move.O)) << w_OpponentsBoarder);
			move.Value -= (POP_COUNT(PlayersExposed(move.P, move.O)) << w_PlayersExposeds);
			move.Value += (POP_COUNT(OpponentsExposed(move.P, move.O)) << w_OpponentsExposeds);
			move.Value -= (POP_COUNT(PlayersBoarder(move.P, move.O)) << w_PlayersBoarder);
			if (parity & QUADRANT_ID(move.move))
				move.Value -= 1 << w_Parity;

			m_Moves.push_back(move);
			//std::push_heap(m_Moves.begin(), m_Moves.end(), [](const Move& Move1, const Move& Move2){ return Move1.m_Value > Move2.m_Value; });
		}
	}
	else
	{
		const int magnitude_InHashTable = 19;
		const int value_InHashTable_ExactValueBonus = 1 << 29;
		const int value_InHashTable_ZeroWindowBonus = 1 << 22;
		const int magnitude_InHashTable_WindowSizeBonus = 18;

		while (BitBoardPossible)
		{
			Move move;
			move.move = BIT_SCAN_LS1B(BitBoardPossible);
			REMOVE_LS1B(BitBoardPossible);
			flipped = flip(P, O, move.move);
			move.P = O ^ flipped;
			move.O = P ^ (1ULL << move.move) ^ flipped;
			move.PossibleMoves = PossibleMoves(move.P, move.O);
			move.Value = ((POP_COUNT(move.PossibleMoves) + POP_COUNT(move.PossibleMoves & 0x8100000000000081UL)) << 4);
			move.Value -= (POP_COUNT(StableStones_corner_and_co(move.O)) << 5);
			move.Value += (POP_COUNT(OpponentsBoarder(move.P, move.O)) << 2);
			move.Value -= (POP_COUNT(PlayersExposed(move.P, move.O)) << 0);
			move.Value += (POP_COUNT(OpponentsExposed(move.P, move.O)) << 2);
			move.Value -= (POP_COUNT(move.PossibleMoves & 0x42C300000000C342UL) << 3);

			//if (HashTableReturn_p.first)
			//{
			//	if (move.move == HashTableReturn_p.second.PV)
			//		move.Value -= (1 << 4);
			//}

			//std::pair<bool, ReversiValueType> HashTableReturn_n = search.CheckHashTable(move.m_Position);

			//if (HashTableReturn_n.first){
			//	move.m_Value -= HashTableReturn_n.second.m_depth << magnitude_InHashTable;
			//	if (HashTableReturn_n.second.m_alpha == HashTableReturn_n.second.m_beta){
			//		if (HashTableReturn_n.second.m_depth == move.m_Position.NumberOfEmptyStones())
			//			move.m_Value -= value_InHashTable_ExactValueBonus;
			//		else
			//			move.m_Value -= value_InHashTable_ZeroWindowBonus;
			//	}
			//	else
			//		move.m_Value -= ((128 - (HashTableReturn_n.second.m_beta - HashTableReturn_n.second.m_alpha))/32) << magnitude_InHashTable_WindowSizeBonus;
			//}

			m_Moves.push_back(move);
			//std::push_heap(m_Moves.begin(), m_Moves.end(), [](const Move& Move1, const Move& Move2){ return Move1.m_Value > Move2.m_Value; });
		}
	}
	std::sort(m_Moves.begin(), m_Moves.end(), [](const Move& Move1, const Move& Move2){ return Move1.Value < Move2.Value; });
}