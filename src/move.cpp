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
	unsigned long long Empties = NumberOfEmptyStones(P, O);
	if (Empties < 16)
	{
		const unsigned char Parity = parity(P, O);
		while (BitBoardPossible)
		{
			Move move;
			move.move = BIT_SCAN_LS1B(BitBoardPossible);
			REMOVE_LS1B(BitBoardPossible);
			flipped = flip(P, O, move.move);
			move.P = O ^ flipped;
			move.O = P ^ (1ULL << move.move) ^ flipped;
			move.PossibleMoves = PossibleMoves(move.P, move.O);
			move.Value = ((POP_COUNT_MAX15(move.PossibleMoves) + POP_COUNT_MAX15(move.PossibleMoves & 0x8100000000000081UL)) << w_PossibleMoves);
			move.Value -= (POP_COUNT_MAX15(StableStones_corner_and_co(move.O)) << w_Stable);
			move.Value += (POP_COUNT_MAX15(OpponentsBoarder(move.P, move.O)) << w_OpponentsBoarder);
			move.Value -= (POP_COUNT(PlayersExposed(move.P, move.O)) << w_PlayersExposeds);
			move.Value += (POP_COUNT(OpponentsExposed(move.P, move.O)) << w_OpponentsExposeds);
			move.Value -= (POP_COUNT_MAX15(PlayersBoarder(move.P, move.O)) << w_PlayersBoarder);
			if (Parity & QUADRANT_ID(move.move))
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
			move.Value = ((POP_COUNT(move.PossibleMoves) + POP_COUNT_MAX15(move.PossibleMoves & 0x8100000000000081UL)) << 4);
			move.Value -= (POP_COUNT_MAX15(StableStones_corner_and_co(move.O)) << 5);
			move.Value += (POP_COUNT(OpponentsBoarder(move.P, move.O)) << 2);
			move.Value -= (POP_COUNT(PlayersExposed(move.P, move.O)) << 0);
			move.Value += (POP_COUNT(OpponentsExposed(move.P, move.O)) << 2);
			move.Value -= (POP_COUNT_MAX15(move.PossibleMoves & 0x42C300000000C342UL) << 3);

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

CMoveList::CMoveList(const unsigned long long P, const unsigned long long O, unsigned long long BitBoardPossible, const int depth)
{
	unsigned long long flipped;
	unsigned long long Empties = NumberOfEmptyStones(P, O);

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
		move.Value = ((POP_COUNT(move.PossibleMoves) + POP_COUNT_MAX15(move.PossibleMoves & 0x8100000000000081UL)) << 4);
		move.Value -= (POP_COUNT_MAX15(StableStones_corner_and_co(move.O)) << 5);
		move.Value += (POP_COUNT(OpponentsBoarder(move.P, move.O)) << 2);
		move.Value -= (POP_COUNT(PlayersExposed(move.P, move.O)) << 0);
		move.Value += (POP_COUNT(OpponentsExposed(move.P, move.O)) << 2);
		move.Value -= (POP_COUNT_MAX15(move.PossibleMoves & 0x42C300000000C342UL) << 3);

		m_Moves.push_back(move);
	}

	std::sort(m_Moves.begin(), m_Moves.end(), [](const Move& Move1, const Move& Move2){ return Move1.Value < Move2.Value; });
}