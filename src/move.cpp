#include "move.h"

const int w_PossibleMoves = 15;
const int w_Stable = 8;
const int w_OpponentsBoarder = 2;
const int w_PlayersExposeds = 3;
const int w_OpponentsExposeds = 7;
const int w_PlayersBoarder = 5;
const int w_Parity = 5;

void EvaluateShallow(CMoveList::Move & move, const unsigned char Parity)
{
	move.Value = -((PopCount_max15(move.PossibleMoves) + PopCount_max15(move.PossibleMoves & 0x8100000000000081UL)) << w_PossibleMoves);
	move.Value += (PopCount_max15(StableStones_corner_and_co(move.O)) << w_Stable);
	move.Value -= (PopCount_max15(OpponentsBoarder(move.P, move.O)) << w_OpponentsBoarder);
	move.Value += (PopCount(PlayersExposed(move.P, move.O)) << w_PlayersExposeds);
	move.Value -= (PopCount(OpponentsExposed(move.P, move.O)) << w_OpponentsExposeds);
	move.Value += (PopCount_max15(PlayersBoarder(move.P, move.O)) << w_PlayersBoarder);
	if (Parity & QUADRANT_ID(move.move))
		move.Value += 1 << w_Parity;
}

CMoveList::CMoveList(CSearch & search, const unsigned long long P, const unsigned long long O, unsigned long long BitBoardPossible, const int depth)
{
	unsigned long long flipped;
	unsigned long long Empties = NumberOfEmptyStones(P, O);
	if (Empties < 16)
	{
		const unsigned char Parity = parity(P, O);
		while (BitBoardPossible)
		{
			Move move;
			move.move = BitScanLSB(BitBoardPossible);
			RemoveLSB(BitBoardPossible);
			flipped = flip(P, O, move.move);
			move.P = O ^ flipped;
			move.O = P ^ (1ULL << move.move) ^ flipped;
			move.PossibleMoves = PossibleMoves(move.P, move.O);
			EvaluateShallow(move, Parity);
			m_Moves.push_back(move);
		}
		std::sort(m_Moves.begin(), m_Moves.end(), [](const Move& Move1, const Move& Move2){ return Move1.Value > Move2.Value; });
		return;
	}
	else
	{
		HashTableValueType htValue;
		static const int sort_depth[61] = {
		//   0   1   2   3   4   5   6   7   8   9
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1,  0,  0,  0,  1,  1,
			 1,  2,  2,  2,  3,  3,  3,  4,  4,  4,
			 5,  5,  5,  6,  6,  6,  6,  6,  6,  6,
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
			move.PossibleMoves = PossibleMoves(move.P, move.O);
			if (sort_depth[depth] == -1)
				EvaluateShallow(move, Parity);
			else
			{
				move.Value = -((PopCount(move.PossibleMoves) + PopCount_max15(move.PossibleMoves & 0x8100000000000081UL)) << w_PossibleMoves);
				move.Value += (PopCount(StableStones_corner_and_co(move.O)) << w_Stable);
				move.Value -= EvaluateLimitedDepth(search, move.P, move.O, -64, 64, sort_depth[depth], 0) << 14;
				//if (search.HashTableLookUp(move.P, move.O, htValue)) move.Value += 1ULL << 17;
			}
			m_Moves.push_back(move);
		}
		std::sort(m_Moves.begin(), m_Moves.end(), [](const Move& Move1, const Move& Move2){ return Move1.Value > Move2.Value; });
		return;
	}
}

//CMoveList::CMoveList(const unsigned long long P, const unsigned long long O, unsigned long long BitBoardPossible, const int depth)
//{
//	unsigned long long flipped;
//	unsigned long long Empties = NumberOfEmptyStones(P, O);
//
//	const int magnitude_InHashTable = 19;
//	const int value_InHashTable_ExactValueBonus = 1 << 29;
//	const int value_InHashTable_ZeroWindowBonus = 1 << 22;
//	const int magnitude_InHashTable_WindowSizeBonus = 18;
//
//	while (BitBoardPossible)
//	{
//		Move move;
//		move.move = BitScanLSB(BitBoardPossible);
//		RemoveLSB(BitBoardPossible);
//		flipped = flip(P, O, move.move);
//		move.P = O ^ flipped;
//		move.O = P ^ (1ULL << move.move) ^ flipped;
//		move.PossibleMoves = PossibleMoves(move.P, move.O);
//		move.Value = -((PopCount(move.PossibleMoves) + PopCount_max15(move.PossibleMoves & 0x8100000000000081UL)) << 4);
//		move.Value += (PopCount_max15(StableStones_corner_and_co(move.O)) << 5);
//		move.Value -= (PopCount(OpponentsBoarder(move.P, move.O)) << 2);
//		move.Value += (PopCount(PlayersExposed(move.P, move.O)) << 0);
//		move.Value -= (PopCount(OpponentsExposed(move.P, move.O)) << 2);
//		move.Value += (PopCount_max15(move.PossibleMoves & 0x42C300000000C342UL) << 3);
//
//		m_Moves.push_back(move);
//	}
//
//	std::sort(m_Moves.begin(), m_Moves.end(), [](const Move& Move1, const Move& Move2){ return Move1.Value > Move2.Value; });
//}