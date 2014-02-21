#pragma once
#include <atomic>
#include <iostream>
#include <vector>
#include <thread>
#include "datamanipulation.h"
#include "features.h"
#include "position.h"
#include "utility.h"
#include "search.h"
#include "endgame.h"
#include <windows.h>

class CPositionManager
{
private:
    CPositionManager(const CPositionManager&); // not defined
    CPositionManager& operator=(const CPositionManager&); // not defined
public:
	std::atomic<std::size_t> m_index_counter;
	std::vector<DATASET_POSITON_SCORE> m_Positions;
	FILE* m_file;
	fpos_t m_pos;

	CPositionManager(const char * const Filename, const std::size_t Size, unsigned char depth)
	{
		m_index_counter.store(0, std::memory_order_release);
		fopen_s(&m_file, Filename, "r+b");
		LoopOverSolvedPositions(depth);
		LoadPositions(Size);
	}
	~CPositionManager() { fclose(m_file); }

	void LoopOverSolvedPositions(const signed char depth);
	void LoadPositions(const std::size_t Size);
	void SavePositions();
	DATASET_POSITON_SCORE * TryGetWork();
};