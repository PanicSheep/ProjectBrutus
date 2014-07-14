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
	unsigned long long PossMoves;
	unsigned long long n_P, n_O;
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
		move.flipped = flip(P, O, move.move);
		n_P = O ^ move.flipped;
		n_O = P ^ (1ULL << move.move) ^ move.flipped;
		move.Value = 0;
		PossMoves = PossibleMoves(n_P, n_O);
		if (htValue.depth > sort_depth[depth])
		{
			if (move.move == htValue.PV)
				move.Value += 1ULL << 23;
			else if (move.move == htValue.AV)
				move.Value += 1ULL << 21;
		}
		if (move.Value == 0)
		{
			move.Value += SQUARE_VALUE[move.move];
			move.Value -= (PopCount(PossMoves) + PopCount_max15(PossMoves & 0x8100000000000081UL)) << 15;
			move.Value += PopCount(StableStones_corner_and_co(n_O)) << 11;
			move.Value -= (PopCount(OpponentsBoarder(n_P, n_O))) << 5;
			//move.Value -= (PopCount(OpponentsExposed(n_P, n_O)) << 7);
			if (Parity & QUADRANT_ID(move.move))
				move.Value += 1 << 1;
			switch (sort_depth[depth])
			{
			case -1:
				break;
			case 0:
				++search.NodeCounter;
				move.Value -= (EvaluateFeatures(n_P, n_O) >> 2) << 15;
				break;
			case 1:
			case 2:
				move.Value -= (EvaluateLimitedDepth(search, n_P, n_O, -64, 64, sort_depth[depth], 0) >> 1) << 15;
				break;
			default:
				move.Value -= EvaluateLimitedDepth(search, n_P, n_O, -64, 64, sort_depth[depth], 0) << 15;
				if (search.HashTableLookUp(n_P, n_O, htValue2))
					move.Value += 1ULL << 15;
				break;
			}
		}
		m_Moves.push_back(move);
	}
	std::sort(m_Moves.begin(), m_Moves.end(), [](const Move& Move1, const Move& Move2){ return Move1.Value > Move2.Value; });
	return;
}