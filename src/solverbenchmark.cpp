#include "solverbenchmark.h"

std::atomic<unsigned long long> NodeCounter;
std::atomic<std::size_t> PositionCounter;

CDataset_Position_Score * CPositionManager::TryGetWork()
{
	std::size_t Index = m_index_counter.fetch_add(1, std::memory_order_release);
	if (Index < m_Positions.size())
		return &(m_Positions[Index]);
	else // Out of work
		return NULL;
}

void Work(CPositionManager& PositionManager, const signed char depth, const unsigned char selectivity, CHashTable * HashTable)
{
	CDataset_Position_Score * data;
	while (data = PositionManager.TryGetWork())
	{
		CSearch search(data->P, data->O, -64, 64, depth, selectivity, HashTable, 1);
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
		printf("%3u|   | %14s | %14llu | %9d | %9d | %s\n", NumberOfEmptyStones, time_format(duration), 
			NodeCounter, NodeCounter*1000/duration.count(), n*1000LL/duration.count(), short_time_format(std::chrono::duration_cast<std::chrono::duration<long long, std::pico>>(duration)/n));
	else
		printf("%3u|%3u| %14s | %14llu | %9d | %9d | %s\n", NumberOfEmptyStones, depth, time_format(duration), 
			NodeCounter, NodeCounter*1000/duration.count(), n*1000LL/duration.count(), short_time_format(std::chrono::duration_cast<std::chrono::duration<long long, std::pico>>(duration)/n));
}

void SolverBenchmarkEmpties(const int LowerEmpties, const int UpperEmpties, const int seconds, const signed char depth, const unsigned char selectivity, const int nthreads, const int HashTableBits)
{
	std::string filename;
	int n;
	std::size_t SizeEnd[25] = {
		2500000, //d0
		2500000, //d1
		2500000, //d2
		1800000, //d3
		1000000, //d4
		 200000, //d5
		 100000, //d6
		  40000, //d7
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
		      5, //d19
		      5, //d20
		      2, //d21
		      2, //d22
		      1, //d23
		      1, //d24
	};
	printf(" E | d |       time (s) |      nodes (N) |    N/s    |    P/s    | time/P \n");
	printf("---+---+----------------+----------------+-----------+-----------+--------\n");

	for (int E = LowerEmpties; E <= UpperEmpties; ++E)
	{
		filename = "G:\\Reversi2\\pos\\rnd_d" + std::to_string(E) + "_1M.ps";
		n = min(1000000, SizeEnd[E] * seconds * nthreads);

		Solve(filename, n, E, depth, selectivity, nthreads, HashTableBits);
	}
}

void SolverBenchmarkDepth(const int Empties, const int seconds, const signed char Lowerdepth, const signed char Upperdepth, const unsigned char selectivity, const int nthreads, const int HashTableBits)
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
		      6, //d12
		      4, //d13
		      2, //d14
		      1, //d15
		      1, //d16
		      1, //d17
		      1, //d18
		      1, //d19
		      1  //d20
	};
	printf(" E | d |       time (s) |      nodes (N) |    N/s    |    P/s    | time/P \n");
	printf("---+---+----------------+----------------+-----------+-----------+--------\n");

	for (int d = Lowerdepth; d <= Upperdepth; ++d)
	{
		filename = "G:\\Reversi2\\pos\\rnd_d" + std::to_string(Empties) + "_1M.ps";
		n = min(1000000, SizeDepth[d] * seconds * nthreads);

		Solve(filename, n, Empties, d, selectivity, nthreads, HashTableBits);
	}
}