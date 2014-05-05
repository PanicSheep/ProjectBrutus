#pragma once
#include <atomic>
#include <thread>
#include <iostream>
#include <Windows.h>
#include "datamanipulation.h"
#include "search.h"
#include "hashtable.h"

class CPositionManager
{
private:
    CPositionManager(const CPositionManager&); // not defined
    CPositionManager& operator=(const CPositionManager&); // not defined
public:
	std::atomic<std::size_t> m_index_counter;
	std::vector<CDataset_Position_Score> m_Positions;

	CPositionManager(const std::string Filename, const std::size_t Size)
	{
		FILE* File;
		m_index_counter.store(0, std::memory_order_release);
		m_Positions = read_vector<CDataset_Position_Score>(Filename, Size);
	}

	CDataset_Position_Score * TryGetWork();
};

void SolverBenchmarkEmpties(const int LowerEmpties, const int UpperEmpties, const int seconds, const signed char depth, const unsigned char selectivity, const int nthreads, const int HashTableBits);
void SolverBenchmarkDepth(const int Empties, const int seconds, const signed char Lowerdepth, const signed char Upperdepth, const unsigned char selectivity, const int nthreads, const int HashTableBits);
