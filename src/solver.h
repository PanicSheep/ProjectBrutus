#pragma once
#include <atomic>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <windows.h>
#include "datamanipulation.h"
#include "features.h"
#include "game.h"
#include "position.h"
#include "search.h"
#include "utility.h"

std::atomic<unsigned long long> NodeCounter;
std::atomic<std::size_t> PositionCounter;
std::atomic_flag Spinlock;

template <class DATASET>
class CPositionManager
{
private:
    CPositionManager(const CPositionManager&); // not defined
    CPositionManager& operator=(const CPositionManager&); // not defined
public:
	std::atomic<std::size_t> m_index_counter;
	std::vector<DATASET> m_Positions;
	FILE* m_file;
	fpos_t m_pos;

	CPositionManager(const std::string& Filename, const std::size_t Size, const unsigned char depth, const unsigned char selectivity, const bool SkipSolved)
	{
		m_index_counter.store(0, std::memory_order_release);
		fopen_s(&m_file, Filename.c_str(), "r+b");
		if (SkipSolved)
			LoopOverSolvedPositions(depth, selectivity);
		LoadPositions(Size);
	}
	~CPositionManager() { fclose(m_file); }

	void LoopOverSolvedPositions(const signed char depth, const unsigned char selectivity)
	{
		size_t LoopedPosition = 0;
		const int N = 4 * 1024;
		int ValidData;
		DATASET* DataArray = new DATASET[N];
		fgetpos(m_file, &m_pos);
		while (ValidData = fread(DataArray, sizeof(DATASET), N, m_file))
		{
			for (int i = 0; i < ValidData; ++i)
			{
				if ((DataArray[i].depth < depth) || (DataArray[i].selectivity > selectivity)) // Needs to be solved
				{
					std::cout << "Looped over " << LoopedPosition << " already solved positions." << std::endl;
					fsetpos(m_file, &m_pos);
					fread(DataArray, sizeof(DATASET), i, m_file);
					delete[] DataArray;
					return;
				}
				++LoopedPosition;
			}
			fgetpos(m_file, &m_pos);
		}
		std::cout << "All positions were already solved!" << std::endl;
		delete[] DataArray;
	}

	void LoadPositions(const std::size_t Size)
	{
		DATASET* DataArray = new DATASET[Size];
		fgetpos(m_file, &m_pos);
		std::size_t ValidData = fread(DataArray, sizeof(DATASET), Size, m_file);
		m_Positions.insert(m_Positions.end(), DataArray, DataArray + ValidData);
		delete[] DataArray;
	}

	void SavePositions()
	{
		const std::size_t size = m_Positions.size();

		DATASET* DataArray = new DATASET[size];
		for (std::size_t i = 0; i < size; ++i)
			DataArray[i] = m_Positions[i];
		fsetpos(m_file, &m_pos);
		fwrite(DataArray, sizeof(DATASET), size, m_file);
		delete[] DataArray;
	}

	DATASET * TryGetWork()
	{
		std::size_t Index = m_index_counter.fetch_add(1, std::memory_order_release);
		if (Index < m_Positions.size())
			return &(m_Positions[Index]);
		else // Out of work
			return NULL;
	}
};

template <class DATASET>
void Work(CPositionManager<DATASET>& PositionManager, const signed char depth, const unsigned char selectivity, CHashTable * HashTable, const bool verbose);

template <> void Work<CDataset_Position_Score>(CPositionManager<CDataset_Position_Score>& PositionManager, const signed char depth, const unsigned char selectivity, CHashTable * HashTable, const bool verbose)
{
	unsigned long long NumSolved;
	CDataset_Position_Score * data;
	while (data = PositionManager.TryGetWork())
	{
		CSearch search(data->P, data->O, -64, 64, depth, selectivity, HashTable, 0);
		search.Evaluate();
		HashTable->AdvanceDate();
		data->depth = depth;
		data->selectivity = selectivity;
		data->score = search.score;
		NodeCounter.fetch_add(search.NodeCounter, std::memory_order_relaxed);
		NumSolved = PositionCounter.fetch_add(1, std::memory_order_relaxed);
		if (verbose)
		{
			while (Spinlock.test_and_set());
			search.print_result(9, NumSolved + 1, false);
			Spinlock.clear();
		}
		else
		{
			if ((NumSolved & 0xF) == 0)
			{
				if (!Spinlock.test_and_set())
				{
					std::cout << "\r";
					std::cerr << progressbar_percentage(50, static_cast<float>(NumSolved) / static_cast<float>(PositionManager.m_Positions.size()));
					Spinlock.clear();
				}
			}
		}
	}
}

template <> void Work<CDataset_Position_Score_PV>(CPositionManager<CDataset_Position_Score_PV>& PositionManager, const signed char depth, const unsigned char selectivity, CHashTable * HashTable, const bool verbose)
{
	unsigned long long NumSolved;
	CDataset_Position_Score_PV * data;
	while (data = PositionManager.TryGetWork())
	{
		{
			unsigned long long P = data->P;
			unsigned long long O = data->O;

			for (int i = 0; i < 5; i++)
			{
				HashTable->Update(P, O, HashTableValueType(0, data->depth - i, data->selectivity, data->score, data->score, data->PV[i], 64));
				if (data->PV[i] != 64)
					PlayStone(P, O, data->PV[i]);
				else
					break;
			}
			if (data->PV[4] != 64)
				HashTable->Update(P, O, HashTableValueType(0, data->depth - 5, data->selectivity, data->score, data->score, 64, 64));
		}
		CSearch search(data->P, data->O, -64, 64, depth, selectivity, HashTable, 5);
		search.Evaluate(false);
		HashTable->AdvanceDate();
		data->depth = depth;
		data->selectivity = selectivity;
		data->score = search.score;
		for (int i = 0; i < 5; ++i)
			data->PV[i] = search.PV(i);
		NodeCounter.fetch_add(search.NodeCounter, std::memory_order_relaxed);
		NumSolved = PositionCounter.fetch_add(1, std::memory_order_relaxed);
		if (verbose)
		{
			while (Spinlock.test_and_set());
			search.print_result(9, NumSolved + 1, false);
			Spinlock.clear();
		}
		else
		{
			if ((NumSolved & 0xF) == 0)
			{
				if (!Spinlock.test_and_set())
				{
					std::cout << "\r";
					std::cerr << progressbar_percentage(50, static_cast<float>(NumSolved) / static_cast<float>(PositionManager.m_Positions.size()));
					Spinlock.clear();
				}
			}
		}
	}
}

template <> void Work<CDataset_Position_FullScore>(CPositionManager<CDataset_Position_FullScore>& PositionManager, const signed char depth, const unsigned char selectivity, CHashTable * HashTable, const bool verbose)
{
	unsigned long long NumSolved;
	CDataset_Position_FullScore * data;
	while (data = PositionManager.TryGetWork())
	{
		if (data->score[27] != DATASET_DEFAULT_score) // Game over.
		{
			data->depth = depth;
			data->selectivity = selectivity;
			NumSolved = PositionCounter.fetch_add(1, std::memory_order_relaxed);
			if (!Spinlock.test_and_set())
			{
				std::cout << "\r";
				std::cerr << progressbar_percentage(50, static_cast<float>(NumSolved) / static_cast<float>(PositionManager.m_Positions.size()));
				Spinlock.clear();
			}
			continue;
		}
		if (data->score[36] != DATASET_DEFAULT_score) // Only best move score aviable.
		{
			//Search best move
			CSearch search(data->P, data->O, data->score[36]-1, data->score[36]+1, depth, selectivity, HashTable, 2);
			search.Evaluate();
			NodeCounter.fetch_add(search.NodeCounter, std::memory_order_relaxed);

			if (search.PV(0) == DATASET_DEFAULT_PV) // No best move returned
			{
				if ((PossibleMoves(data->P, data->O) == 0) && PossibleMoves(data->O, data->P) == 0) // Game over
				{
					data->score[27] = data->score[36];
					data->score[36] = DATASET_DEFAULT_score;
				}
				else
				{
					unsigned long long P = data->P;
					unsigned long long O = data->O;
					unsigned long long flipped;
					unsigned char Move;

					if (PossibleMoves(P, O) == 0)
						std::swap(P, O);
					
					unsigned long long BitBoardPossible = PossibleMoves(P, O);
					while (BitBoardPossible)
					{
						Move = BitScanLSB(BitBoardPossible);
						RemoveLSB(BitBoardPossible);
						if (flipped = flip(P, O, Move))
						{
							CSearch search(O ^ flipped, P ^ (1ULL << Move) ^ flipped, -64, 64, depth-1, selectivity, HashTable, 1);
							search.Evaluate();
							data->score[Move] = search.score;
							data->PV[Move] = search.PV(0);
							NodeCounter.fetch_add(search.NodeCounter, std::memory_order_relaxed);
						}
					}
				}
			}
			else
			{
				data->score[search.PV(0)] = data->score[36];
				data->score[36] = DATASET_DEFAULT_score;
				data->PV[search.PV(0)] = search.PV(1);

				unsigned long long P = data->P;
				unsigned long long O = data->O;
				unsigned long long flipped;
				unsigned char Move;

				if (PossibleMoves(P, O) == 0)
					std::swap(P, O);

				unsigned long long BitBoardPossible = PossibleMoves(P, O) ^ (1ULL << search.PV(0));
				while (BitBoardPossible)
				{
					Move = BitScanLSB(BitBoardPossible);
					RemoveLSB(BitBoardPossible);
					if (flipped = flip(P, O, Move))
					{
						CSearch search(O ^ flipped, P ^ (1ULL << Move) ^ flipped, -64, 64, depth - 1, selectivity, HashTable, 1);
						search.Evaluate();
						data->score[Move] = search.score;
						data->PV[Move] = search.PV(0);
						NodeCounter.fetch_add(search.NodeCounter, std::memory_order_relaxed);
					}
				}
			}
			HashTable->AdvanceDate();
			data->depth = depth;
			data->selectivity = selectivity;
			NumSolved = PositionCounter.fetch_add(1, std::memory_order_relaxed);
			if (!Spinlock.test_and_set())
			{
				std::cout << "\r";
				std::cerr << progressbar_percentage(50, static_cast<float>(NumSolved) / static_cast<float>(PositionManager.m_Positions.size()));
				Spinlock.clear();
			}
			continue;
		}

		unsigned long long P = data->P;
		unsigned long long O = data->O;
		unsigned long long flipped;
		unsigned char Move;

		if (PossibleMoves(P, O) == 0)
		{
			if (PossibleMoves(O, P) == 0) // Game over
			{
				CSearch search(P, O, -64, 64, depth, selectivity, HashTable, 0);
				search.Evaluate();
				HashTable->AdvanceDate();
				data->depth = depth;
				data->selectivity = selectivity;
				data->score[27] = search.score;
				NodeCounter.fetch_add(search.NodeCounter, std::memory_order_relaxed);
				NumSolved = PositionCounter.fetch_add(1, std::memory_order_relaxed);
				if (!Spinlock.test_and_set())
				{
					std::cout << "\r";
					std::cerr << progressbar_percentage(50, static_cast<float>(NumSolved) / static_cast<float>(PositionManager.m_Positions.size()));
					Spinlock.clear();
				}
				continue;
			}
			std::swap(P, O);
		}

		unsigned long long BitBoardPossible = PossibleMoves(P, O);
		while (BitBoardPossible)
		{
			Move = BitScanLSB(BitBoardPossible);
			RemoveLSB(BitBoardPossible);
			if (flipped = flip(P, O, Move))
			{
				CSearch search(O ^ flipped, P ^ (1ULL << Move) ^ flipped, -64, 64, depth - 1, selectivity, HashTable, 1);
				search.Evaluate();
				data->score[Move] = search.score;
				data->PV[Move] = search.PV(0);
				NodeCounter.fetch_add(search.NodeCounter, std::memory_order_relaxed);
			}
		}

		HashTable->AdvanceDate();
		data->depth = depth;
		data->selectivity = selectivity;
		NumSolved = PositionCounter.fetch_add(1, std::memory_order_relaxed);
		if ((NumSolved & 0xF) == 0)
		{
			if (!Spinlock.test_and_set())
			{
				std::cout << "\r";
				std::cerr << progressbar_percentage(50, static_cast<float>(NumSolved) / static_cast<float>(PositionManager.m_Positions.size()));
				Spinlock.clear();
			}
		}
	}
}

template <class DATASET> void printHeader();
template <> void printHeader<CDataset_Position_Score>()
{ 
	printf("       # |                                                                | depth  |score\n");
	printf("---------+----------------------------------------------------------------+--------+-----\n");
}
template <> void printHeader<CDataset_Position_Score_PV>()
{
	printf("       # |                                                                | depth  |score| PV                 \n");
	printf("---------+----------------------------------------------------------------+--------+-----+--------------------\n");
}
template <> void printHeader<CDataset_Position_FullScore>()
{
	printf("       # |                                                                | depth  |score\n");
	printf("---------+----------------------------------------------------------------+--------+-----\n");
}


template <class DATASET>
void Solve(const std::string& Filename, const int n, const signed char depth, const unsigned char selectivity, const int nthreads, const bool verbose, const bool SkipSolved, const bool Save)
{
	NodeCounter = 0;

	std::cout << nthreads << " threads in total.\n";
	
	std::chrono::high_resolution_clock::time_point startTime, endTime;
	std::vector<std::thread> threads(nthreads - 1);
	std::vector<CHashTable*> HashTables(nthreads);
	CPositionManager<DATASET> PositionManager(Filename, n, depth, selectivity, SkipSolved);

	for (auto& ht : HashTables)
		ht = new CHashTable(20);

	if (verbose)
		CSearch(0, 0, -64, 64, 0, 5).print_header(9, false);

	startTime = std::chrono::high_resolution_clock::now(); //Start Time

	// Start workers
	for (int i = 0; i < nthreads - 1; ++i)
		threads[i] = std::thread(Work<DATASET>, std::ref(PositionManager), depth, selectivity, HashTables[i], verbose);
	Work<DATASET>(PositionManager, depth, selectivity, HashTables[nthreads - 1], verbose);

	// Join workers
	for (auto& t : threads)
		t.join();

	endTime = std::chrono::high_resolution_clock::now(); //Stop Time

	std::cerr << "\r";
	std::cerr << progressbar_percentage(50, 1); std::cout << std::endl;

	for (auto& ht : HashTables)
		delete ht;

	if (Save)
		PositionManager.SavePositions();

	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	std::cout << PositionCounter.load(std::memory_order_acquire) << " positions solved in: " << time_format(duration) << std::endl;
	std::cout << PositionCounter.load(std::memory_order_acquire) * 1000.0 / duration.count() << " positions per second." << std::endl;
	std::cout << ThousandsSeparator(NodeCounter) << " Nodes." << std::endl;
	std::cout << ThousandsSeparator(NodeCounter * 1000 / duration.count()) << " nodes per second." << std::endl;
}