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
	std::vector<DATASET_POSITON_SCORE> m_Positions;

	CPositionManager(const std::string Filename, const std::size_t Size)
	{
		FILE* File;
		m_index_counter.store(0, std::memory_order_release);
		m_Positions = read_vector<DATASET_POSITON_SCORE>(Filename, Size);
	}

	DATASET_POSITON_SCORE * TryGetWork();
};

void SolverBenchmarkEmpties(const int LowerEmpties, const int UpperEmpties, const int secounds, const signed char depth, const unsigned char selectivity, const int nthreads, const int HashTableBits);
void SolverBenchmarkDepth(const int Empties, const int secounds, const signed char Lowerdepth, const signed char Upperdepth, const unsigned char selectivity, const int nthreads, const int HashTableBits);
