#pragma once
#include "search.h"
#include "position.h"
#include "hashtable.h"
#include "utility.h"
#include <algorithm>
#include <vector>

class CSearch;

class CMoveList
{
public:
	struct Move
	{
		unsigned long long P;
		unsigned long long O;
		unsigned long long PossibleMoves;
		int Value;
		unsigned char move;
	};	
		
	static const int w_PossibleMoves = 15;
	static const int w_Stable = 8;
	static const int w_OpponentsBoarder = 2;
	static const int w_PlayersExposeds = 3;
	static const int w_OpponentsExposeds = 7;
	static const int w_PlayersBoarder = 5;
	static const int w_Parity = 5;
	
	//CMoveList(const unsigned long long P, const unsigned long long O, unsigned long long BitBoardPossible);
	//CMoveList(const unsigned long long P, const unsigned long long O, unsigned long long BitBoardPossible, const int depth);
	CMoveList(CSearch & search, const unsigned long long P, const unsigned long long O, unsigned long long BitBoardPossible, const int depth, const HashTableValueType & htValue);

	inline std::vector<Move>::iterator begin() { return m_Moves.begin(); }
	inline std::vector<Move>::const_iterator cbegin() const { return m_Moves.cbegin(); }
	inline std::vector<Move>::iterator end() { return m_Moves.end(); }
	inline std::vector<Move>::const_iterator cend() const { return m_Moves.cend(); }
	inline std::size_t size() const { return m_Moves.size(); }

	inline unsigned char BestMove() { return m_Moves[0].move; }
	inline unsigned char NextBestMove() { return size() > 1 ? m_Moves[1].move : 64; }
	
	//inline const int size() { return m_Moves.size(); }
	//inline const Move & front() { return m_Moves.front(); }
	//inline void pop() { std::pop_heap(m_Moves.begin(), m_Moves.end(), [](const Move& Move1, const Move& Move2){ return Move1.m_Value > Move2.m_Value; }); m_Moves.pop_back(); }

private:
	std::vector<Move> m_Moves;
};