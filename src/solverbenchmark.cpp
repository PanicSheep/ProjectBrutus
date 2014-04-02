#include "solverbenchmark.h"

std::atomic<unsigned long long> NodeCounter;
std::atomic<std::size_t> PositionCounter;

DATASET_POSITON_SCORE * CPositionManager::TryGetWork()
{
	std::size_t Index = m_index_counter.fetch_add(1, std::memory_order_release);
	if (Index < m_Positions.size())
		return &(m_Positions[Index]);
	else // Out of work
		return NULL;
}

void Work(CPositionManager& PositionManager, const signed char depth, const unsigned char selectivity, CHashTable * HashTable)
{
	DATASET_POSITON_SCORE * data;
	while (data = PositionManager.TryGetWork())
	{
		CSearch search(data->P, data->O, -64, 64, depth, selectivity, HashTable, CSearch::NodeType::PV_Node);
		search.Evaluate();
		HashTable->AdvanceDate();
		NodeCounter.fetch_add(search.NodeCounter, std::memory_order_relaxed);
	}
}

void Solve(const std::string Filename, const int n, const unsigned char NumberOfEmptyStones, const signed char depth, const unsigned char selectivity, const int nthreads, const int HashTableBits)
{
	NodeCounter = 0;
	std::chrono::high_resolution_clock::time_point startTime, endTime;
	std::vector<std::thread> threads(nthreads-1);
	std::vector<CHashTable*> HashTables(nthreads);
	CPositionManager PositionManager(Filename, n);

	for (auto& ht : HashTables)
		ht = new CHashTable(HashTableBits);

	startTime = std::chrono::high_resolution_clock::now(); //Start Time

	// Start workers
	for (int i = 0; i < nthreads-1; ++i)
		threads[i] = std::thread(Work, std::ref(PositionManager), depth, selectivity, HashTables[i]);
	Work(PositionManager, depth, selectivity, HashTables[nthreads-1]);
  
	// Join workers
	for (auto& t : threads)
		t.join();

	endTime = std::chrono::high_resolution_clock::now(); //Stop Time

	for (auto& ht : HashTables)
		delete ht;

	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (depth == CSearch::END)
		printf("%3u|   | %14s | %14llu | %9d | %9d | %s\n", NumberOfEmptyStones, time_format(std::chrono::duration_cast<std::chrono::milliseconds>(duration)), 
			NodeCounter, NodeCounter*1000/duration.count(), n*1000LL/duration.count(), short_time_format(std::chrono::duration_cast<std::chrono::duration<long long, std::pico>>(duration)/n));
	else
		printf("%3u|%3u| %14s | %14llu | %9d | %9d | %s\n", NumberOfEmptyStones, depth, time_format(std::chrono::duration_cast<std::chrono::milliseconds>(duration)), 
			NodeCounter, NodeCounter*1000/duration.count(), n*1000LL/duration.count(), short_time_format(std::chrono::duration_cast<std::chrono::duration<long long, std::pico>>(duration)/n));
}

void SolverBenchmarkEmpties(const int LowerEmpties, const int UpperEmpties, const int secounds, const signed char depth, const unsigned char selectivity, const int nthreads, const int HashTableBits)
{
	std::string filename;
	int n;
	std::size_t SizeEnd[21] = {
		2500000, //d0
		2500000, //d1
		2500000, //d2
		1800000, //d3
		1000000, //d4
		 450000, //d5
		 180000, //d6
		  65000, //d7
		  23000, //d8
		   9500, //d9
		   3800, //d10
		   1600, //d11
		    650, //d12
		    280, //d13
		    130, //d14
		     50, //d15
		     25, //d16
		     10, //d17
		      5, //d18
		      2, //d19
		      1  //d20
	};
	printf(" E | d |       time (s) |      nodes (N) |    N/s    |    P/s    | time/P \n");
	printf("---+---+----------------+----------------+-----------+-----------+--------\n");

	for (int E = LowerEmpties; E <= UpperEmpties; ++E)
	{
		filename = "F:\\Reversi\\pos\\rnd_d" + std::to_string(E) + "_10M.ps";
		n = min(10000000, SizeEnd[E] * secounds * nthreads);

		Solve(filename, n, E, depth, selectivity, nthreads, HashTableBits);
	}
}

void SolverBenchmarkDepth(const int Empties, const int secounds, const signed char Lowerdepth, const signed char Upperdepth, const unsigned char selectivity, const int nthreads, const int HashTableBits)
{
	std::string filename;
	int n;
	std::size_t SizeDepth[21] = {
		1000000, //d0
		 210000, //d1
		  46000, //d2
		  10000, //d3
		   2900, //d4
		    920, //d5
		    350, //d6
		    110, //d7
		     50, //d8
		     21, //d9
		     12, //d10
		      6, //d11
		     24, //d12
		     15, //d13
		      9, //d14
		      6, //d15
		      4, //d16
		      2, //d17
		      1, //d18
		      1, //d19
		      1  //d20
	};
	printf(" E | d |       time (s) |      nodes (N) |    N/s    |    P/s    | time/P \n");
	printf("---+---+----------------+----------------+-----------+-----------+--------\n");

	for (int d = Lowerdepth; d <= Upperdepth; ++d)
	{
		filename = "F:\\Reversi\\pos\\rnd_d" + std::to_string(Empties) + "_1M.ps";
		n = min(1000000, SizeDepth[d] * secounds * nthreads);

		Solve(filename, n, Empties, d, selectivity, nthreads, HashTableBits);
	}
}