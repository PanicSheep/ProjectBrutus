#include "benchmark.h"
#include <thread>
#include <future>

class CPositionManager2
{
private:
    CPositionManager2(const CPositionManager2&); // not defined
    CPositionManager2& operator=(const CPositionManager2&); // not defined
public:
	std::atomic<std::size_t> m_index_counter;
	std::atomic<std::size_t> m_NodeCounter;
	std::atomic<std::size_t> m_PositionCounter;
	std::atomic<std::size_t> m_Duration;
	std::vector<CPosition> m_Positions;
	std::atomic_flag m_Spinlock;

	CPositionManager2()
	{
		m_index_counter.store(0, std::memory_order_release);
		m_NodeCounter.store(0, std::memory_order_release);
		m_PositionCounter.store(0, std::memory_order_release);
		m_Duration.store(0, std::memory_order_release);
		m_Spinlock.clear();
	}

	void Append(const std::vector<std::string>& Filenames, const std::size_t size)
	{
		m_index_counter.store(0, std::memory_order_release);
		m_NodeCounter.store(0, std::memory_order_release);
		m_PositionCounter.store(0, std::memory_order_release);
		m_Duration.store(0, std::memory_order_release);
		m_Spinlock.clear();
		std::vector<CPosition> tmp;
		for (auto& filename : Filenames){
			tmp = read_vector<CPosition>(filename, size);
			m_Positions.insert(m_Positions.begin(), tmp.begin(), tmp.end());
		}
	}

	void Reset()
	{
		m_index_counter.store(0, std::memory_order_release);
		m_NodeCounter.store(0, std::memory_order_release);
		m_PositionCounter.store(0, std::memory_order_release);
		m_Duration.store(0, std::memory_order_release);
		m_Spinlock.clear();
	}

	CPosition * TryGetWork()
	{
		std::size_t Index = m_index_counter.fetch_add(1, std::memory_order_release);
		if (Index < m_Positions.size())
			return &(m_Positions[Index]);
		else // Out of work
			return NULL;
	}
	
	void Work(const signed char depth, const unsigned char selectivity, CHashTable * HashTable)
	{
		std::chrono::high_resolution_clock::time_point startTime, endTime;
		unsigned long long NumSolved;
		CPosition * data;
		while (data = TryGetWork())
		{
			HashTable->Clear();
			CSearch search(data->P, data->O, -64, 64, depth, selectivity, HashTable, 5);
			startTime = std::chrono::high_resolution_clock::now();
			search.Evaluate(false);
			endTime = std::chrono::high_resolution_clock::now();
			HashTable->AdvanceDate();
			m_NodeCounter.fetch_add(search.NodeCounter, std::memory_order_relaxed);
			std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
			m_Duration.fetch_add(duration.count(), std::memory_order_relaxed);
			NumSolved = m_PositionCounter.fetch_add(1, std::memory_order_relaxed);
			//if ((NumSolved & 0xF) == 0)
			//{
			if (m_Spinlock.test_and_set() == false)
			{
				std::cerr << "\r";
				std::cerr << progressbar_percentage(50, static_cast<float>(NumSolved) / static_cast<float>(m_Positions.size()));
				m_Spinlock.clear();
			}
			//}
		}
	}

	void Solve(const signed char depth, const unsigned char selectivity)
	{
		int nthreads = std::thread::hardware_concurrency();
		std::chrono::high_resolution_clock::time_point startTime, endTime;
		std::vector<std::thread> threads(nthreads - 1);
		std::vector<CHashTable*> HashTables(nthreads);

		for (auto& ht : HashTables)
			ht = new CHashTable(20);

		startTime = std::chrono::high_resolution_clock::now();

		// Start workers
		for (int i = 0; i < nthreads - 1; ++i)
			threads[i] = std::thread([=]{ Work(depth, selectivity, HashTables[i]); });
		Work(depth, selectivity, HashTables[nthreads-1]);

		// Join workers
		for (auto& t : threads)
			t.join();

		endTime = std::chrono::high_resolution_clock::now();

		for (auto& ht : HashTables)
			delete ht;

		std::cerr << "\r";
		std::cerr << progressbar_percentage(50, 1); std::cout << std::endl;
			
		std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
		std::cout << m_PositionCounter.load(std::memory_order_acquire) << " positions solved in: " << time_format(duration) << std::endl;
		std::cout << m_PositionCounter.load(std::memory_order_acquire) * 1000.0 / duration.count() << " positions per second." << std::endl;
		std::cout << m_Duration.load(std::memory_order_acquire) << " ms CPU time." << std::endl;
		std::cout << ThousandsSeparator(m_NodeCounter) << " Nodes." << std::endl;
		if (duration.count() == 0)
			std::cout << "### nodes per second." << std::endl;
		else
			std::cout << ThousandsSeparator(m_NodeCounter * 1000 / duration.count()) << " nodes per second." << std::endl;
	}
};

void PreheatCPU(const int seconds)
{
	std::chrono::high_resolution_clock::time_point startTime;
	auto rnd = std::bind(std::uniform_int_distribution<unsigned long long>(0, -1), std::mt19937_64(seconds));
	unsigned long long A = rnd();
	unsigned long long B = rnd();
	unsigned long long C = rnd();

	std::cout << "CPU is preheating...";
	startTime = std::chrono::high_resolution_clock::now();
	while (std::chrono::nanoseconds(std::chrono::high_resolution_clock::now() - startTime).count()*1e-9 < seconds)
	{
		for (int i = 0; i < 1024 * 1024; ++i)
		{
			A = B + C / 3;
			B = C + A * 5;
			C = A + B * 7;
		}
	}

	std::cout << "done! CPU is now preheated to " << 100 + seconds << "C." << std::endl;
}

void FForum_Benchmark(std::string filename, bool verbose, CHashTable* hashTable)
{
	unsigned long long OverallNodeCounter = 0;
	std::chrono::high_resolution_clock::time_point startTime, endTime;
	//std::chrono::high_resolution_clock::time_point sTime, eTime;
	std::vector<CDataset_Position_Score> PositionVec = read_vector<CDataset_Position_Score>(filename);
	CSearch search;
	int i = 1;

	CSearch(PositionVec[0].P, PositionVec[0].O, -64, 64, hashTable, 5).print_header(3, false);

	startTime = std::chrono::high_resolution_clock::now();
	for (auto& Pos : PositionVec){
		//search.score = Endgame::NegaMax(search.P, search.O, search.NodeCounter);
		search = CSearch(Pos.P, Pos.O, -64, 64, hashTable, 5);
		search.Evaluate();
		hashTable->AdvanceDate();
		OverallNodeCounter += search.NodeCounter;
		if (search.score != Pos.score)
			printf("%3u was miscalculated. It's score should be %+2.2d.\n", i, Pos.score);
		//sTime = std::chrono::high_resolution_clock::now();
		if (verbose)
			search.print_result(3, i, false);
		//eTime = std::chrono::high_resolution_clock::now();
		//std::chrono::milliseconds t_span = std::chrono::duration_cast<std::chrono::milliseconds>(eTime - sTime);
		//printf("%s\n", time_format(t_span).c_str());
		++i;
	}
	endTime = std::chrono::high_resolution_clock::now();

	std::chrono::milliseconds time_span = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (time_span == std::chrono::milliseconds(0))
		printf("       %s nodes in %s.\n\n", ThousandsSeparator(OverallNodeCounter), time_format(time_span).c_str());
	else
		printf(       "%s nodes in %s (%s N/s).\n\n", ThousandsSeparator(OverallNodeCounter), time_format(time_span).c_str(), ThousandsSeparator(OverallNodeCounter*1000/time_span.count()));
}

void PossibleMoves_Benchmark(const int N)
{
	unsigned long long a = 0x0569871035164909ULL &  0x9601656461938970ULL;
	unsigned long long b = 0x0569871035164909ULL & ~0x9601656461938970ULL;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = PossibleMoves(a, b);
		b = PossibleMoves(a, b);
		a = PossibleMoves(a, b);
		b = PossibleMoves(a, b);
		a = PossibleMoves(a, b);
		b = PossibleMoves(a, b);
		a = PossibleMoves(a, b);
		b = PossibleMoves(a, b);
		a = PossibleMoves(a, b);
		b = PossibleMoves(a, b);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("PossibleMoves  | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("PossibleMoves  | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void PossibleMoves2_Benchmark(const int N)
{
	unsigned long long a = 0x0569871035164909ULL &  0x9601656461938970ULL;
	unsigned long long b = 0x0569871035164909ULL & ~0x9601656461938970ULL;
	unsigned long long c, d;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		PossibleMoves(a, b, c, d);
		PossibleMoves(c, d, a, b);
		PossibleMoves(a, b, c, d);
		PossibleMoves(c, d, a, b);
		PossibleMoves(a, b, c, d);
		PossibleMoves(c, d, a, b);
		PossibleMoves(a, b, c, d);
		PossibleMoves(c, d, a, b);
		PossibleMoves(a, b, c, d);
		PossibleMoves(c, d, a, b);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("PossibleMoves2 | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("PossibleMoves2 | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void PlayStone_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<int>(0, 63), std::mt19937_64(N));
	std::vector<unsigned char> c(N+9);
	unsigned long long a = 0x0569871035164909ULL &  0x9601656461938970ULL;
	unsigned long long b = 0x0569871035164909ULL & ~0x9601656461938970ULL;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < N+9; i++) c[i] = rnd();

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		PlayStone(a, b, c[i  ]);
		PlayStone(a, b, c[i+1]);
		PlayStone(a, b, c[i+2]);
		PlayStone(a, b, c[i+3]);
		PlayStone(a, b, c[i+4]);
		PlayStone(a, b, c[i+5]);
		PlayStone(a, b, c[i+6]);
		PlayStone(a, b, c[i+7]);
		PlayStone(a, b, c[i+8]);
		PlayStone(a, b, c[i+9]);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("PlayStone      | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("PlayStone      | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

//void TryPlayStone_Benchmark(const int N)
//{
//	auto rnd = std::bind(std::uniform_int_distribution<int>(0, 63), std::mt19937_64(N));
//	std::vector<unsigned char> c(N+9);
//	unsigned long long a = 0x0569871035164909ULL &  0x9601656461938970ULL;
//	unsigned long long b = 0x0569871035164909ULL & ~0x9601656461938970ULL;
//	std::chrono::high_resolution_clock::time_point startTime, endTime;
//
//	for (int i = 0; i < N+9; i++) c[i] = rnd();
//
//	startTime = std::chrono::high_resolution_clock::now();
//	for (int i = 0; i < N; i++)
//	{
//		TryPlayStone(a, b, c[i  ]);
//		TryPlayStone(a, b, c[i+1]);
//		TryPlayStone(a, b, c[i+2]);
//		TryPlayStone(a, b, c[i+3]);
//		TryPlayStone(a, b, c[i+4]);
//		TryPlayStone(a, b, c[i+5]);
//		TryPlayStone(a, b, c[i+6]);
//		TryPlayStone(a, b, c[i+7]);
//		TryPlayStone(a, b, c[i+8]);
//		TryPlayStone(a, b, c[i+9]);
//	}
//	endTime = std::chrono::high_resolution_clock::now();
//	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
//	if (a) //Prevent dead code elimination.
//		printf("TryPlayStone   | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
//	else
//		printf("TryPlayStone   | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
//}

void Flip_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<int>(0, 63), std::mt19937_64(N));
	std::vector<unsigned char> c(N+9);
	unsigned long long a = 0x0569871035164909ULL &  0x9601656461938970ULL;
	unsigned long long b = 0x0569871035164909ULL & ~0x9601656461938970ULL;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < N+9; i++) c[i] = rnd();

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = flip(a, b, c[i  ]);
		b = flip(a, b, c[i+1]);
		a = flip(a, b, c[i+2]);
		b = flip(a, b, c[i+3]);
		a = flip(a, b, c[i+4]);
		b = flip(a, b, c[i+5]);
		a = flip(a, b, c[i+6]);
		b = flip(a, b, c[i+7]);
		a = flip(a, b, c[i+8]);
		b = flip(a, b, c[i+9]);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("Flip           | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("Flip           | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void Count_last_flip_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<int>(0, 63), std::mt19937_64(N));
	std::vector<unsigned char> c(N+9);
	unsigned long long a = 0x0569871035164909ULL &  0x9601656461938970ULL;
	unsigned long long b = 0x0569871035164909ULL & ~0x9601656461938970ULL;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < N+9; i++) c[i] = rnd();

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		b = count_last_flip(a, c[i  ]);
		a = count_last_flip(b, c[i+1]);
		b = count_last_flip(a, c[i+2]);
		a = count_last_flip(b, c[i+3]);
		b = count_last_flip(a, c[i+4]);
		a = count_last_flip(b, c[i+5]);
		b = count_last_flip(a, c[i+6]);
		a = count_last_flip(b, c[i+7]);
		b = count_last_flip(a, c[i+8]);
		a = count_last_flip(b, c[i+9]);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("Count last flip| %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("Count last flip| %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void PopCount_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<unsigned long long>(0ULL, 0xFFFFFFFFFFFFFFFFULL), std::mt19937_64(N));
	unsigned long long b[64];
	unsigned long long a = 0;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < 64; i++) b[i] = rnd();

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = PopCount(b[a]);
		a = PopCount(a);
		a = PopCount(a);
		a = PopCount(a);
		a = PopCount(a);
		a = PopCount(a);
		a = PopCount(a);
		a = PopCount(a);
		a = PopCount(a);
		a = PopCount(a);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("PopCount      | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("PopCount      | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void PopCount_max15_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<unsigned long long>(0ULL, 0xFFFFFFFFFFFFFFFFULL), std::mt19937_64(N));
	unsigned long long b[64];
	unsigned long long a = 0;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < 64; i++) b[i] = rnd();

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = PopCount_max15(b[a]);
		a = PopCount_max15(a);
		a = PopCount_max15(a);
		a = PopCount_max15(a);
		a = PopCount_max15(a);
		a = PopCount_max15(a);
		a = PopCount_max15(a);
		a = PopCount_max15(a);
		a = PopCount_max15(a);
		a = PopCount_max15(a);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("PopCount_max15| %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("PopCount_max15| %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void Parity_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<unsigned long long>(0ULL, 0xFFFFFFFFFFFFFFFFULL), std::mt19937_64(N));
	unsigned long long b[64];
	unsigned long long a = 0;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < 64; i++) b[i] = rnd();

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = parity(b[a]);
		a = parity(a);
		a = parity(a);
		a = parity(a);
		a = parity(a);
		a = parity(a);
		a = parity(a);
		a = parity(a);
		a = parity(a);
		a = parity(a);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("Parity         | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("Parity         | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void BitScanLSB_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<unsigned long long>(1ULL, 0xFFFFFFFFFFFFFFFFULL), std::mt19937_64(N));
	unsigned long long b[64];
	unsigned long long a = 0;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < 64; i++) b[i] = rnd();

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = BitScanLSB(b[a]);
		a = BitScanLSB(a);
		a = BitScanLSB(a);
		a = BitScanLSB(a);
		a = BitScanLSB(a);
		a = BitScanLSB(a);
		a = BitScanLSB(a);
		a = BitScanLSB(a);
		a = BitScanLSB(a);
		a = BitScanLSB(a);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("BitScanLSB  | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("BitScanLSB  | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void RemoveLSB_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<unsigned long long>(1ULL, 0xFFFFFFFFFFFFFFFFULL), std::mt19937_64(N));
	std::vector<unsigned long long> a(N);
	unsigned long long b = 0ULL;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < N; i++) a[i] = rnd();

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		b |= a[i];
		RemoveLSB(b);
		RemoveLSB(b);
		RemoveLSB(b);
		RemoveLSB(b);
		RemoveLSB(b);
		RemoveLSB(b);
		RemoveLSB(b);
		RemoveLSB(b);
		RemoveLSB(b);
		RemoveLSB(b);
	}
	endTime = std::chrono::high_resolution_clock::now();

	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (b) //Prevent dead code elimination.
		printf("RemoveLSB    | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("RemoveLSB    | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void StableStones_corner_and_co_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<unsigned long long>(0ULL, 0xFFFFFFFFFFFFFFFFULL), std::mt19937_64(N));
	unsigned long long b[64];
	unsigned long long a = 0;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < 64; i++) b[i] = rnd();

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = StableStones_corner_and_co(b[a % 64]);
		a = StableStones_corner_and_co(a);
		a = StableStones_corner_and_co(a);
		a = StableStones_corner_and_co(a);
		a = StableStones_corner_and_co(a);
		a = StableStones_corner_and_co(a);
		a = StableStones_corner_and_co(a);
		a = StableStones_corner_and_co(a);
		a = StableStones_corner_and_co(a);
		a = StableStones_corner_and_co(a);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("corner and co  | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("corner and co  | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void StableStones_affectables_Benchmark(const int N)
{
	unsigned long long a = 0x0569871035164909ULL &  0x9601656461938970ULL;
	unsigned long long b = 0x0569871035164909ULL & ~0x9601656461938970ULL;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = StableStones_affectables(a, b);
		b = StableStones_affectables(a, b);
		a = StableStones_affectables(a, b);
		b = StableStones_affectables(a, b);
		a = StableStones_affectables(a, b);
		b = StableStones_affectables(a, b);
		a = StableStones_affectables(a, b);
		b = StableStones_affectables(a, b);
		a = StableStones_affectables(a, b);
		b = StableStones_affectables(a, b);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("affectables    | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("affectables    | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void StableStones_full_edges_second_order_Benchmark(const int N)
{
	unsigned long long a = 0x0569871035164909ULL &  0x9601656461938970ULL;
	unsigned long long b = 0x0569871035164909ULL & ~0x9601656461938970ULL;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = StableStones_full_edges_second_order(a, b);
		b = StableStones_full_edges_second_order(a, b);
		a = StableStones_full_edges_second_order(a, b);
		b = StableStones_full_edges_second_order(a, b);
		a = StableStones_full_edges_second_order(a, b);
		b = StableStones_full_edges_second_order(a, b);
		a = StableStones_full_edges_second_order(a, b);
		b = StableStones_full_edges_second_order(a, b);
		a = StableStones_full_edges_second_order(a, b);
		b = StableStones_full_edges_second_order(a, b);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("full_edges     | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("full_edges     | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void StableStones_triangles_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<unsigned long long>(0ULL, 0xFFFFFFFFFFFFFFFFULL), std::mt19937_64(N));
	unsigned long long b[64];
	unsigned long long a = 0;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < 64; i++) b[i] = rnd();

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = StableStones_triangles(b[a % 64]);
		a = StableStones_triangles(a);
		a = StableStones_triangles(a);
		a = StableStones_triangles(a);
		a = StableStones_triangles(a);
		a = StableStones_triangles(a);
		a = StableStones_triangles(a);
		a = StableStones_triangles(a);
		a = StableStones_triangles(a);
		a = StableStones_triangles(a);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("triangles      | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("triangles      | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void StableStones_skyline_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<unsigned long long>(0ULL, 0xFFFFFFFFFFFFFFFFULL), std::mt19937_64(N));
	unsigned long long b[64];
	unsigned long long a = 0;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < 64; i++) b[i] = rnd();

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = StableStones_skyline(b[a % 64]);
		a = StableStones_skyline(a);
		a = StableStones_skyline(a);
		a = StableStones_skyline(a);
		a = StableStones_skyline(a);
		a = StableStones_skyline(a);
		a = StableStones_skyline(a);
		a = StableStones_skyline(a);
		a = StableStones_skyline(a);
		a = StableStones_skyline(a);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("skyline        | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("skyline        | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void StableStones_Benchmark(const int N)
{
	unsigned long long a = 0x0569871035164909ULL &  0x9601656461938970ULL;
	unsigned long long b = 0x0569871035164909ULL & ~0x9601656461938970ULL;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = StableStones(a, b);
		b = StableStones(a, b);
		a = StableStones(a, b);
		b = StableStones(a, b);
		a = StableStones(a, b);
		b = StableStones(a, b);
		a = StableStones(a, b);
		b = StableStones(a, b);
		a = StableStones(a, b);
		b = StableStones(a, b);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("StableStones   | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("StableStones   | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void EvaluateFeatures_Benchmark(const int N)
{
	unsigned long long a = 0x0569871035164909ULL &  0x9601656461938970ULL;
	unsigned long long b = 0x0569871035164909ULL & ~0x9601656461938970ULL;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a ^= EvaluateFeatures(a & b & 0xFFFFFFE7E7FFFFFFULL, a & ~b | 0x0000001818000000ULL);
		b ^= EvaluateFeatures(a & b & 0xFFFFFFE7E7FFFFFFULL, a & ~b | 0x0000001818000000ULL);
		a ^= EvaluateFeatures(a & b & 0xFFFFFFE7E7FFFFFFULL, a & ~b | 0x0000001818000000ULL);
		b ^= EvaluateFeatures(a & b & 0xFFFFFFE7E7FFFFFFULL, a & ~b | 0x0000001818000000ULL);
		a ^= EvaluateFeatures(a & b & 0xFFFFFFE7E7FFFFFFULL, a & ~b | 0x0000001818000000ULL);
		b ^= EvaluateFeatures(a & b & 0xFFFFFFE7E7FFFFFFULL, a & ~b | 0x0000001818000000ULL);
		a ^= EvaluateFeatures(a & b & 0xFFFFFFE7E7FFFFFFULL, a & ~b | 0x0000001818000000ULL);
		b ^= EvaluateFeatures(a & b & 0xFFFFFFE7E7FFFFFFULL, a & ~b | 0x0000001818000000ULL);
		a ^= EvaluateFeatures(a & b & 0xFFFFFFE7E7FFFFFFULL, a & ~b | 0x0000001818000000ULL);
		b ^= EvaluateFeatures(a & b & 0xFFFFFFE7E7FFFFFFULL, a & ~b | 0x0000001818000000ULL);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("EvalFeatures   | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("EvalFeatures   | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}


unsigned long long Solve(const int d, CDataset_Position_Score* DataArray, const unsigned long long size)
{
	CHashTable * HashTable = new CHashTable(24);
	unsigned long long NodeCounter = 0;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	if (d < 5)
	{
		startTime = std::chrono::high_resolution_clock::now();
		for (unsigned long long i = 0; i < size; i++)
			EvaluateEnd(DataArray[i].P, DataArray[i].O, NodeCounter, d);
		endTime = std::chrono::high_resolution_clock::now();
	}
	else
	{
		startTime = std::chrono::high_resolution_clock::now();
		for (unsigned long long i = 0; i < size; i++)
		{
			CSearch search(DataArray[i].P, DataArray[i].O, -64, 64, HashTable);
			search.Evaluate();
			NodeCounter += search.NodeCounter;
		}
		endTime = std::chrono::high_resolution_clock::now();
	}

	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	printf("%3u| %14s | %14llu | %9d | %9d | %s\n", d, time_format(std::chrono::duration_cast<std::chrono::milliseconds>(duration)), 
		NodeCounter, NodeCounter*1000/duration.count(), size*1000/duration.count(), short_time_format(std::chrono::duration_cast<std::chrono::duration<long long, std::pico>>(duration)/size));

	delete HashTable;
	return NodeCounter;
}

void HugeBenchmak_(std::vector<std::vector<CDataset_Position_Score>>& Datas, std::atomic_flag& spinlock, const int depth, const int s, const int nthreads, CHashTable * HashTable)
{
	for (int d = depth+3; d <= 16; d += nthreads)
	{
		unsigned long long NodeCounter = 0;
		unsigned long long PositionCounter = 0;
		std::chrono::high_resolution_clock::time_point startTime, endTime;
		startTime = std::chrono::high_resolution_clock::now();
		for (int f = d + 7; f < 50; f += 4)
		{
			for (auto& data : Datas[f])
			{
				CSearch search(data.P, data.O, -64, 64, d, s, HashTable, 1);
				search.Evaluate();
				NodeCounter += search.NodeCounter;
				PositionCounter++;
			}
		}
		endTime = std::chrono::high_resolution_clock::now();
		std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
		while (spinlock.test_and_set());
		printf("%2u@%5s%%| %14s | %14llu | %9d | %9d | %s\n",
			d,
			SelectivityTable[s].percentile.c_str(),
			time_format(duration).c_str(),
			NodeCounter,
			NodeCounter * 1000 / duration.count(),
			PositionCounter * 1000 / duration.count(),
			short_time_format(std::chrono::duration_cast<std::chrono::duration<long long, std::pico>>(duration) / PositionCounter).c_str()
			);
		spinlock.clear();
		HashTable->Clear();
	}
}


void HugeBenchmak()
{
	const int size = 5;
	const int nthreads = 4;
	std::vector<std::vector<CDataset_Position_Score>> Datas(50);
	std::vector<std::thread> threads(nthreads - 1);
	std::vector<CHashTable *> HashTables(nthreads);
	std::atomic_flag spinlock;
	spinlock.clear();

	for (auto& ht : HashTables)
		ht = new CHashTable(22);
	
	// Read in data vectors
	for (int i = 20; i < 50; i++)
	{
		std::string s = "G:\\Reversi2\\pos\\rnd_d";
		s.append(std::to_string(i));
		s.append("_1M.ps");
		Datas[i] = read_vector<CDataset_Position_Score>(s, size);
	}

	printf("  depth  |       time (s) |      nodes (N) |    N/s    |    P/s    | time/P \n");
	printf("---------+----------------+----------------+-----------+-----------+--------\n");
	
	for (int s = 6; s >= 0; s -= 2)
	{
		for (int i = 0; i < nthreads-1; ++i)
			threads[i] = std::thread(HugeBenchmak_, std::ref(Datas), std::ref(spinlock), i, s, nthreads, HashTables[i]);
		HugeBenchmak_(Datas, spinlock, nthreads-1, s, nthreads, HashTables[nthreads-1]);

		// Join workers
		for (auto& t : threads)
			t.join();
	}
	for (auto& ht : HashTables)
		delete ht;
}

int main(int argc, char* argv[])
{
	//for (int D = 13; D <= 60; D++)
	//	for (int E = 53; E <= 60; E++)
	//		if (D <= E - 10)
	//			for (int d = 2 * std::floor(D * 0.25) + (D & 1) - 2; d <= 2 * std::floor(D * 0.25) + (D & 1) + 2; d+=2)
	//			{
	//				// printf("probcut %02u %02u %02u = %6.6f % 6.6f %6.6f %s\n", E, D, d, 1.0, 0.0, sigma(D, d), (2 * std::floor(D * 0.25) + (D & 1) == d) ? "true" : "false");
	//				printf("probcut %02u %02u %02u = %6.6f % 6.6f %6.6f\n", E, D, d, 1.0, 0.0, Midgame::sigma(D, d, E));
	//			}
	//return 0;


	//std::vector<std::string> FileNames;
	//int n = 10000;
	//bool v = false;
	//int depth = 0;
	//bool b_fforum = false;
	//bool b_exact = false;

	//for (int i = 0; i < argc; ++i)
	//{
	//	if (std::string(argv[i]) == "-f"){
	//		++i;
	//		while ((i < argc) && ((char)(*argv[i]) != '-'))
	//			FileNames.push_back(std::string(argv[i++]));
	//		--i;
	//	}
	//	else if (std::string(argv[i]) == "-n")
	//		n = atoi(argv[++i]);
	//	else if (std::string(argv[i]) == "-d")
	//		depth = atoi(argv[++i]);
	//	else if (std::string(argv[i]) == "-v")
	//		v = true;
	//	else if (std::string(argv[i]) == "-fforum")
	//		b_fforum = true;
	//	else if (std::string(argv[i]) == "-exact")
	//		b_exact = true;
	//}

	//SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);

	ConfigFile::Initialize(argv[0], std::string("ProjectBrutus.ini"));
	Features::Initialize();
	Midgame::Initialize();
	srand(time(NULL));

	//if (b_fforum){
	//	PreheatCPU(2);
	//	CHashTable * const ht = new CHashTable(24);
	//	FForum_Benchmark("pos\\fforum-1-19.ps", true, ht);
	//	ht->print_stats();
	//	FForum_Benchmark("pos\\fforum-20-39.ps", true, ht);
	//	ht->print_stats();
	//	FForum_Benchmark("pos\\fforum-40-59.ps", true, ht);
	//	ht->print_stats();
	//	delete ht;
	//}
	//if (b_exact){
	//	PreheatCPU(2);
	//	CHashTable * ht = new CHashTable(24);
	//	CSearch s(START_POSITION_ETH_P, START_POSITION_ETH_O, -64, 64, ht, 5);
	//	s.Evaluate(true);
	//	delete ht;
	//}

	//CHashTable * const ht = new CHashTable(24);
	//unsigned long long NodeCounter;
	//std::vector<CPositionScore> Positions;
	//std::vector<int> err;
	//std::chrono::high_resolution_clock::time_point startTime, endTime;
	//std::chrono::milliseconds duration;

	//printf("            Filename            |     Node counter     |    Runtime     \n");
	//printf("--------------------------------+----------------------+----------------\n");

	//for (auto& filename : FileNames)
	//{
	//	Positions = read_vector<CPositionScore>(filename, n);
	//	NodeCounter = 0;
	//	err.clear();
	//	startTime = std::chrono::high_resolution_clock::now();
	//	#pragma omp parallel for schedule(static, 1) reduction(+ : NodeCounter)
	//	for (int i = 0; i < Positions.size(); ++i)
	//	{
	//		CSearch s(Positions[i].P, Positions[i].O, -64, 64, depth, 0, ht, 5);
	//		s.EvaluateIterativeDeepening(v);
	//		NodeCounter += s.NodeCounter;
	//		#pragma omp critical
	//			err.push_back(s.score - Positions[i].score);
	//	}
	//	endTime = std::chrono::high_resolution_clock::now();
	//	duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	//	printf(" %30s | %20s | %14s \n", filename.c_str(), ThousandsSeparator(NodeCounter).c_str(), time_format(duration).c_str());
	//}
	//delete ht;
	////long double sigma = StandardDeviation(err);
	////std::cout << "s: " << sigma << std::endl;
	////std::cout << "stdev: " << ScientificNotaion(sigma, sigma / std::sqrtl(2 * (err.size() + 1)), 2) << std::endl;
	//return 0;


	//printf("   Routine     |  [ns]  |   Runtime [s] \n");
	//printf("---------------+--------+---------------\n");
	////for (int i = 0; i < 5; i++)	PossibleMoves_Benchmark(8000000);
	////for (int i = 0; i < 5; i++)	PossibleMoves2_Benchmark(8000000);
	////for (int i = 0; i < 5; i++)	Flip_Benchmark(10000000);
	////for (int i = 0; i < 5; i++)	Parity_Benchmark(10000000);
	////for (int i = 0; i < 5; i++)	Count_last_flip_Benchmark(20000000);
	////for (int i = 0; i < 5; i++)	PopCount_Benchmark(30000000);
	////for (int i = 0; i < 5; i++)	PopCount_max15_Benchmark(30000000);
	////for (int i = 0; i < 5; i++)	BitScanLSB_Benchmark(200000000);
	////for (int i = 0; i < 5; i++)	RemoveLSB_Benchmark(200000000);
	//for (int i = 0; i < 5; i++)	EvaluateFeatures_Benchmark(1500000);

	int DEPTH = 60;
	int SELECTIVITY = 0;
	int BIT = 24;
	double percentage_true = 0.95;
	double percentage_false = 1.02;
	int LOW = 0;
	int HIGH = 60;
	bool TimeCriteria = false;

	for (int i = 0; i < argc; ++i)
	{
		if (std::string(argv[i]) == "-fforum"){
			PreheatCPU(2);
			CHashTable * const ht = new CHashTable(24);
			FForum_Benchmark("pos\\fforum-1-19.ps", true, ht);
			ht->print_stats();
			FForum_Benchmark("pos\\fforum-20-39.ps", true, ht);
			ht->print_stats();
			FForum_Benchmark("pos\\fforum-40-59.ps", true, ht);
			ht->print_stats();
			delete ht;
		}
		else if (std::string(argv[i]) != "-exact"){
			PreheatCPU(2);
			CHashTable * ht = new CHashTable(BIT);
			CSearch s(START_POSITION_ETH_P, START_POSITION_ETH_O, -64, 64, ht, 5);
			s.Evaluate(true);
			delete ht;
		}
		else if (std::string(argv[i]) == "-d") DEPTH = atoi(argv[++i]);
		else if (std::string(argv[i]) == "-s") SELECTIVITY = atoi(argv[++i]);
		else if (std::string(argv[i]) == "-pt") percentage_true = atof(argv[++i]);
		else if (std::string(argv[i]) == "-pf") percentage_false = atof(argv[++i]);
		else if (std::string(argv[i]) == "-bit") BIT = atoi(argv[++i]);
		else if (std::string(argv[i]) == "-l") LOW = atoi(argv[++i]);
		else if (std::string(argv[i]) == "-h") HIGH = atoi(argv[++i]);
		else if (std::string(argv[i]) == "-time") TimeCriteria = true;
	}

	//std::vector<std::string> Filenames({
	//	//"C:\\Reversi\\pos\\perft5.psp"
	//	//"C:\\Reversi\\pos\\rnd_d49_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d48_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d47_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d46_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d45_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d44_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d43_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d42_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d41_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d40_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d39_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d38_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d37_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d36_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d35_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d34_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d33_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d32_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d31_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d30_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d29_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d28_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d27_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d26_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d25_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d24_1M.ps",
	//	//"C:\\Reversi\\pos\\rnd_d23_1M.ps",
	//	"C:\\Reversi\\pos\\rnd_d22_1M.ps",
	//	"C:\\Reversi\\pos\\rnd_d21_1M.ps",
	//	"C:\\Reversi\\pos\\rnd_d20_1M.ps",
	//	"C:\\Reversi\\pos\\rnd_d19_1M.ps",
	//	"C:\\Reversi\\pos\\rnd_d18_1M.ps",
	//	"C:\\Reversi\\pos\\rnd_d17_1M.ps",
	//	"C:\\Reversi\\pos\\rnd_d16_1M.ps",
	//});
	std::vector<std::string> Filenames1({
		"pos\\perft5.psp",
	});
	CPositionManager2 PosMan1;
	PosMan1.Append(Filenames1, 764);

	//std::vector<std::string> Filenames2({
	//	"C:\\Reversi\\pos\\rnd_d20_1M.ps",
	//	"C:\\Reversi\\pos\\rnd_d19_1M.ps",
	//});
	//CPositionManager2 PosMan2;
	//PosMan2.Append(Filenames2, 0);

	//std::vector<std::string> Filenames3({
	//	"C:\\Reversi\\pos\\rnd_d18_1M.ps",
	//	"C:\\Reversi\\pos\\rnd_d17_1M.ps",
	//	"C:\\Reversi\\pos\\rnd_d16_1M.ps",
	//});
	//CPositionManager2 PosMan3;
	//PosMan3.Append(Filenames3, 0);

	//if (Test_All_Features()) std::cout << "All Tests succeded!\n";
	


	//HugeBenchmak();

	//CHashTable* hashTable = new CHashTable(24);
	//unsigned long long P = 0x0C1008050C183000ULL;
	//unsigned long long O = 0xF22E76FAB3E74641ULL;

	//CSearch search;
	//for (int d = 3; d < 7; d++)
	//{
	//	search = CSearch(P, O, -64, 64, d, 0, hashTable, 0);
	//	search.Evaluate();
	//	std::cout << d << " : " << (int)search.score << std::endl;
	//}
	//delete hashTable;


	//SolverBenchmarkEmpties(11 /*LowerEmpties*/, 24 /*UpperEmpties*/, 10 /*seconds*/, CSearch::END /*depth*/, 6 /*selectivity*/, 4 /*nthreads*/, 24 /*HashTableBits*/);
	//SolverBenchmarkDepth(35/*Empties*/, 5/*seconds*/, 8/*Lowerdepth*/, 20/*Upperdepth*/, 6/*selectivity*/, 4/*nthreads*/, 23/*HashTableBits*/);
	//HugeBenchmak(0, 20, 20);

	//HugeBenchmak(20, 20);

	//std::vector<CDataset_Position_Score> Data;
	//read_vector("F:\\Reversi\\pos\\rnd_d5_10M.b", Data);
	//const std::vector<CDataset_Position_Score>::const_iterator end = Data.cend();
	//for (std::vector<CDataset_Position_Score>::const_iterator it = Data.cbegin(); it != end; ++it)
	//	std::cout << EvaluateFeatures(it->P, it->O) << "\t" << (int)it->score << std::endl;
	//return 0;
	//HugeBenchmak(20, 20);

	//if (Test_All())
	//	std::cout << "Test successful!" << std::endl;
	//else
	//	std::cout << "Test failed!" << std::endl;
	//return 0;

	//HugeBenchmak(20, 20);
	//HugeBenchmak(10, 20);
	//HugeBenchmak(10, 20);
	//HugeBenchmak(10, 20);
	//return 0;

	//printf("   Routine     |  [ns]  |   Runtime [s] \n");
	//printf("---------------+--------+---------------\n");
	//PossibleMoves_Benchmark(8000000);
	//PossibleMoves_Benchmark(8000000);
	//PossibleMoves_Benchmark(8000000);
	//PossibleMoves_Benchmark(8000000);
	//PossibleMoves_Benchmark(8000000);
	//PossibleMoves2_Benchmark(8000000);
	//PossibleMoves2_Benchmark(8000000);
	//PossibleMoves2_Benchmark(8000000);
	//PossibleMoves2_Benchmark(8000000);
	//PossibleMoves2_Benchmark(8000000);
	//PlayStone_Benchmark(10000000);
	//Flip_Benchmark(10000000);
	//Flip_Benchmark(10000000);
	//Flip_Benchmark(10000000);
	//Flip_Benchmark(10000000);
	//Flip_Benchmark(10000000);
	//Parity_Benchmark(10000000);
	//Parity_Benchmark(10000000);
	//Parity_Benchmark(10000000);
	//Parity_Benchmark(10000000);
	//Parity_Benchmark(10000000);
	//Count_last_flip_Benchmark(20000000);
	//PopCount_Benchmark(30000000);
	//PopCount_max15_Benchmark(30000000);
	//PopCount_PLUS_Benchmark(30000000);
	//BitScanLSB_Benchmark(200000000);
	//RemoveLSB_Benchmark(200000000);
	//StableStones_corner_and_co_Benchmark(50000000);
	//StableStones_affectables_Benchmark(10000000);
	//StableStones_full_edges_second_order_Benchmark(50000000);
	//StableStones_triangles_Benchmark(50000000);
	//StableStones_skyline_Benchmark(10000000);
	//StableStones_Benchmark(3000000);
	//EvaluateFeatures_Benchmark(800000);
	//EvaluateFeatures_Benchmark(800000);
	//EvaluateFeatures_Benchmark(800000);

	////Play
	//bool PlayerToPlay = true;
	//CHashTable* hashTable = new CHashTable(25);
	//unsigned long long P = START_POSITION_ETH_P;
	//unsigned long long O = START_POSITION_ETH_O;
	//std::string s;
	//unsigned char move;

	//while (HasMoves(P, O) || HasMoves(O, P))
	//{
	//	print_board(P, O, PossibleMoves(P, O));
	//	//Human
	//	if (!HasMoves(P, O))
	//	{
	//		std::swap(P, O);
	//		PlayerToPlay = !PlayerToPlay;
	//	}
	//	else
	//	{
	//		CSearch search(P, O, -64, 64, std::chrono::milliseconds(4900), hashTable, 5);
	//		search.Evaluate(true);
	//		std::cout << "ProjectBrutus' move: ";
	//		move = search.PV(0);
	//		std::cout << FIELD_NAME(move) << "\n";
	//		std::cout << "Score: " << (int)search.score << "   PV: " << search.GetPV(0, 5) << std::endl;
	//		std::cout << std::endl;
	//		PlayStone(P, O, move);
	//		PlayerToPlay = !PlayerToPlay;
	//	}

	//	if (!HasMoves(P, O) && !HasMoves(O, P))
	//		break;

	//	print_board(O, P, PossibleMoves(P, O));
	//	//Machine
	//	if (!HasMoves(P, O))
	//	{
	//		std::swap(P, O);
	//		PlayerToPlay = !PlayerToPlay;
	//	}
	//	else
	//	{
	//		CSearch search(P, O, -64, 64, std::chrono::milliseconds(4900), hashTable, 5);
	//		search.Evaluate(true);
	//		std::cout << "ProjectBrutus' move: ";
	//		move = search.PV(0);
	//		std::cout << FIELD_NAME(move) << "\n";
	//		std::cout << "Score: " << (int)search.score << "   PV: " << search.GetPV(0, 5) << std::endl;
	//		std::cout << std::endl;
	//		PlayStone(P, O, move);
	//		PlayerToPlay = !PlayerToPlay;
	//	}

	//}
	//delete hashTable;

	//std::cout << "\n\n GAME OVER!\nProjectBrutu's disccount: " << PopCount(PlayerToPlay ? O : P) << "\nPlayer's disccount: " << PopCount(PlayerToPlay ? P : O) << std::endl;

	int d;
	int BestTime = 3600000;
	std::string BestConfig = "";
	std::vector<std::pair<int, int>> ToGo, Done, All;
	for (auto it : Midgame::MPC_aviable)
		if (it.first >= LOW && it.first <= HIGH)
			All.push_back(it);
	ToGo.insert(ToGo.begin(), All.begin(), All.end());
	std::random_shuffle(ToGo.begin(), ToGo.end());

	for (auto it : All)
		BestConfig.append(Midgame::MPC_actives[it.first][it.second] ? "1" : "0");
	std::cout << BestConfig << std::endl;
	//PosMan1.Solve(DEPTH, SELECTIVITY);
	//PosMan2.Solve(DEPTH, SELECTIVITY);
	//PosMan3.Solve(DEPTH, SELECTIVITY);
	//PosMan2.Solve(DEPTH + 3, 6);
	//float Minimum = PosMan1.m_NodeCounter;
	//unsigned long long Minimum = PosMan1.m_NodeCounter + PosMan2.m_NodeCounter + PosMan3.m_NodeCounter;
	float sum;
	CHashTable * hashTable = new CHashTable(BIT);
	CSearch search(START_POSITION_ETH_P, START_POSITION_ETH_O, -64, 64, DEPTH, SELECTIVITY, hashTable, 5);
	search.Evaluate(true);
	delete hashTable;
	float Minimum = search.NodeCounter;
	//std::cout << "value : " << Minimum << std::endl;
	//unsigned long long Minimum = PosMan1.m_NodeCounter;
	BestTime = std::chrono::duration_cast<std::chrono::milliseconds>(search.endTime - search.startTime).count();
	std::cout << std::endl;
	
	std::pair<int, int> elem;
	while (ToGo.size())
	{
		elem = ToGo.back();
		Midgame::Change_MPC_actives(elem.first, elem.second);
		std::cout << BestConfig << " : " << elem.first << " " << elem.second << std::endl;
		//std::string filename[4] = {"G:\\Reversi\\pos\\fforum-1-19.ps", "G:\\Reversi\\pos\\fforum-20-39.ps", "G:\\Reversi\\pos\\fforum-40-59.ps", "G:\\Reversi\\pos\\fforum-60-79.ps"};
		//CHashTable* hashTable = new CHashTable(BIT);
		//hashTable->Load("G:\\Reversi2\\Opening.ht");

		//FForum_Benchmark("G:\\Reversi\\pos\\fforum-1-19.ps", true, hashTable);
		//hashTable->print_stats();
		//FForum_Benchmark("G:\\Reversi\\pos\\fforum-20-39.ps", true, hashTable);
		//hashTable->print_stats();

		CHashTable * hashTable = new CHashTable(BIT);
		CSearch search;
		if (TimeCriteria)
		{
			search = CSearch(START_POSITION_ETH_P, START_POSITION_ETH_O, -64, 64, DEPTH, SELECTIVITY, std::chrono::high_resolution_clock::now() + std::chrono::milliseconds((int)(BestTime * 1.6)), hashTable, 5);
			search.Evaluate(true);
		}
		else
		{
			search = CSearch(START_POSITION_ETH_P, START_POSITION_ETH_O, -64, 64, DEPTH, SELECTIVITY, hashTable, 5);
			search.Evaluate(true);
		}		
		delete hashTable;

		//PosMan1.Reset();
		//PosMan2.Reset();
		//PosMan3.Reset();
		//PosMan1.Solve(DEPTH, SELECTIVITY);
		//PosMan2.Solve(DEPTH, SELECTIVITY);
		//PosMan3.Solve(DEPTH, SELECTIVITY);
		//sum = PosMan1.m_NodeCounter;
		sum = search.NodeCounter;
		//std::cout << "value : " << sum << std::endl;
		if (sum < Minimum * percentage_true){
			std::cout << "####################################" << std::endl;
			std::cout << "############ Found one! ############" << std::endl;
			std::cout << "############ Found one! ############" << std::endl;
			std::cout << "############ Found one! ############" << std::endl;
			std::cout << "####################################" << std::endl;
			BestConfig = "";
			for (auto it : All)
				BestConfig.append(Midgame::MPC_actives[it.first][it.second] ? "1" : "0");
			//Minimum = sum;
			//BestTime = std::chrono::duration_cast<std::chrono::milliseconds>(search.endTime - search.startTime).count();
			Minimum = search.NodeCounter;
			ToGo.pop_back();
			std::random_shuffle(Done.begin(), Done.end());
			std::reverse(ToGo.begin(), ToGo.end());
			while (Done.size())
			{
				ToGo.push_back(Done.back());
				Done.pop_back();
			}
			Done.push_back(elem);
			std::reverse(ToGo.begin(), ToGo.end());
		}
		else if (!Midgame::MPC_actives[elem.first][elem.second] && (sum < Minimum * percentage_false)){
			std::cout << "###########################" << std::endl;
			std::cout << "##### Deactivated one #####" << std::endl;
			std::cout << "###########################" << std::endl;
			BestConfig = "";
			for (auto it : All)
				BestConfig.append(Midgame::MPC_actives[it.first][it.second] ? "1" : "0");
			//Minimum = sum;
			//BestTime = std::chrono::duration_cast<std::chrono::milliseconds>(search.endTime - search.startTime).count();
			Minimum = search.NodeCounter;
			Done.push_back(elem);
			ToGo.pop_back();
		}
		else if (sum > Minimum * 1.5)
		{
			std::cout << "*****************" << std::endl;
			std::cout << "*** Bad Fish! ***" << std::endl;
			std::cout << "*****************" << std::endl;
			Midgame::Change_MPC_actives(elem.first, elem.second);
			ToGo.pop_back();
		}
		else
		{
			Midgame::Change_MPC_actives(elem.first, elem.second);
			Done.push_back(elem);
			ToGo.pop_back();
		}
		//hashTable->print_stats();
		//hashTable->Save("Opening.ht");
		//std::cout << "HashTable saved." << std::endl;
		//delete hashTable;
		std::cout << std::endl;
		std::cout << "Done: ";
		for (auto& it : Done)
			std::cout << "(" << it.first << "," << it.second << ")";
		std::cout << std::endl;
		std::cout << "ToGo: ";
		for (auto& it : ToGo)
			std::cout << "(" << it.first << "," << it.second << ")";
		std::cout << std::endl;
	}
	
	std::cout << std::endl << std::endl;
	std::string s_key;
	for (auto it : All)
	{
		s_key = "probcut ";
		if (it.first < 10) s_key += "0";
		s_key += std::to_string(it.first) + " ";
		if (it.second < 10) s_key += "0";
		s_key += std::to_string(it.second);
		s_key += " = ";
		if (Midgame::MPC_actives[it.first][it.second]) s_key += "true";
		else s_key += "false";
		std::cout << s_key << std::endl;
	}

	//CHashTable* hashTable = new CHashTable(24);
	//CSearch search(START_POSITION_ETH_P, START_POSITION_ETH_O, -64, 64, hashTable, 5);
	//search.Evaluate(true);
	//delete hashTable;

	//CHashTable* hashTable = new CHashTable(24);
	//unsigned long long P1 = START_POSITION_ETH_P;
	//unsigned long long O1 = START_POSITION_ETH_O;
	//unsigned long long P2 = START_POSITION_ETH_P;
	//unsigned long long O2 = START_POSITION_ETH_O;
	//PlayStone(P1, O1, 18);
	//PlayStone(P2, O2, 19);
	//printf(" d |   Runtime [s]  |score|                             PV                              \n");
	//printf("---+----------------+-----+-------------------------------------------------------------\n");
	//std::chrono::high_resolution_clock::time_point startTime, endTime;
	//for (int d = 4; d < 60; d+=2){
	//	CSearch search1(P1, O1, -64, 64, d, 6, hashTable, 20);
	//	CSearch search2(P2, O2, -64, 64, d, 6, hashTable, 20);
	//	startTime = std::chrono::high_resolution_clock::now();
	//	auto t1 = std::async(std::launch::async, [&]{search1.Evaluate();});
	//	auto t2 = std::async(std::launch::async, [&]{search2.Evaluate();});
	//	t1.get();
	//	t2.get();
	//	endTime = std::chrono::high_resolution_clock::now();
	//	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	//	printf("%3u| %14s | %+2.2d | %s\n", d, time_format(duration).c_str(), search1.score, search1.GetPV(0, 20).c_str());
	//	printf("%3u| %14s | %+2.2d | %s\n", d, time_format(duration).c_str(), search2.score, search2.GetPV(0, 20).c_str());
	//	//hashTable->print_stats();
	//	//hashTable->Clear();
	//}
	//delete hashTable;


	//CHashTable* hashTable = new CHashTable(24);
	//FForum_Benchmark( 1, 19, true, hashTable);
	//hashTable->print_stats();
	//FForum_Benchmark(20, 39, true, hashTable);
	//hashTable->print_stats();
	//FForum_Benchmark(40, 59, true, hashTable);
	//hashTable->print_stats();
	//delete hashTable;

	//printf("   Routine     |  [ns]  |   Runtime [s] \n");
	//printf("---------------+--------+---------------\n");
	//for (int i = 0; i < 5; i++)	PossibleMoves_Benchmark(8000000);
	//for (int i = 0; i < 5; i++)	PossibleMoves2_Benchmark(8000000);
	//for (int i = 0; i < 5; i++)	Flip_Benchmark(10000000);
	//for (int i = 0; i < 5; i++)	Parity_Benchmark(10000000);
	//for (int i = 0; i < 5; i++)	Count_last_flip_Benchmark(20000000);
	//for (int i = 0; i < 5; i++)	PopCount_Benchmark(30000000);
	//for (int i = 0; i < 5; i++)	PopCount_max15_Benchmark(30000000);
	//for (int i = 0; i < 5; i++)	BitScanLSB_Benchmark(200000000);
	//for (int i = 0; i < 5; i++)	RemoveLSB_Benchmark(200000000);
	//for (int i = 0; i < 5; i++)	EvaluateFeatures_Benchmark(800000);

	Features::Finalize();
	return 0;
}