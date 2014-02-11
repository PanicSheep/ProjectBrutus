#include "solver.h"

std::atomic<unsigned long long> NodeCounter;
std::atomic<std::size_t> PositionCounter;
std::atomic_flag Spinlock;

void CPositionManager::LoopOverSolvedPositions(const signed char depth)
{
	size_t LoopedPosition = 0;
	const int N = 10000;
	int ValidData;
	DATASET_POSITON_SCORE DataArray[N];
	fgetpos(m_file, &m_pos);
	while (ValidData = fread(&DataArray, sizeof(DATASET_POSITON_SCORE), N, m_file))
	{
		for (int i = 0; i < ValidData; ++i)
		{
			if (DataArray[i].depth < depth) // Needs to be solved
			{
				std::cout << "Looped over " << LoopedPosition << " already solved positions." << std::endl;
				fsetpos(m_file, &m_pos);
				fread(DataArray, sizeof(DATASET_POSITON_SCORE), i, m_file);
				return;
			}
			++LoopedPosition;
		}
		fgetpos(m_file, &m_pos);
	}
	std::cout << "All positions were already solved!" << std::endl;
}

void CPositionManager::LoadPositions(const std::size_t Size)
{
	DATASET_POSITON_SCORE* DataArray = new DATASET_POSITON_SCORE[Size];
	fgetpos(m_file, &m_pos);
	std::size_t ValidData = fread(DataArray, sizeof(DATASET_POSITON_SCORE), Size, m_file);
	for (std::size_t i = 0; i < ValidData; ++i)
		m_Positions.push_back(DataArray[i]);
	delete[] DataArray;
}

void CPositionManager::SavePositions()
{
	const std::size_t size = m_Positions.size();

	DATASET_POSITON_SCORE* DataArray = new DATASET_POSITON_SCORE[size];
	for (std::size_t i = 0; i < size; ++i)
		DataArray[i] = m_Positions[i];
	fsetpos(m_file, &m_pos);
	fwrite(DataArray, sizeof(DATASET_POSITON_SCORE), size, m_file);
	delete[] DataArray;
}

DATASET_POSITON_SCORE * CPositionManager::TryGetWork()
{
	std::size_t Index = m_index_counter.fetch_add(1, std::memory_order_release);
	if (Index < m_Positions.size())
		return &(m_Positions[Index]);
	else // Out of work
		return NULL;
}

void Work(CPositionManager& PositionManager, const int depth, CHashTable * HashTable)
{
	unsigned long long NumSolved;
	DATASET_POSITON_SCORE * data;
	while (data = PositionManager.TryGetWork())
	{
		CSearch search(data->P, data->O, -64, 64, depth, 0, HashTable, CSearch::NodeType::PV_Node);
		search.Evaluate();
		HashTable->AdvanceDate();
		data->depth = depth;
		//if (data->score != search.score)
		//{
		//	while (Spinlock.test_and_set());
		//	std::cout << std::dec << (int)data->score << " : " << (int)search.score << " : " << std::hex << data->P << " : " << std::hex << data->O << std::dec << std::endl;
		//	Spinlock.clear();
		//}
		data->score = search.score;
		NodeCounter.fetch_add(search.NodeCounter, std::memory_order_relaxed);
		NumSolved = PositionCounter.fetch_add(1, std::memory_order_relaxed);
		while (Spinlock.test_and_set());
			std::cout << "\r";
			print_progressbar_percentage(50, static_cast<float>(NumSolved) / static_cast<float>(PositionManager.m_Positions.size()));
		Spinlock.clear();
	}
}

void Solve(const char* Filename, const int n, const int depth, const int nthreads)
{
	NodeCounter = 0;

	std::cout << nthreads << " threads in total.\n";

	std::chrono::high_resolution_clock::time_point startTime, endTime;
	std::vector<std::thread> threads(nthreads-1);
	std::vector<CHashTable*> HashTables(nthreads);
	CPositionManager PositionManager(Filename, n, depth);

	for (auto& ht : HashTables)
		ht = new CHashTable(24);

	startTime = std::chrono::high_resolution_clock::now(); //Start Time

	// Start workers
	for (int i = 0; i < nthreads-1; ++i)
		threads[i] = std::thread(Work, std::ref(PositionManager), depth, HashTables[i]);
	Work(PositionManager, depth, HashTables[nthreads-1]);
  
	// Join workers
	for (auto& t : threads)
		t.join();

	endTime = std::chrono::high_resolution_clock::now(); //Stop Time

	std::cout << "\r";
	print_progressbar_percentage(50, 1); std::cout << std::endl;

	for (auto& ht : HashTables)
		delete ht;

	PositionManager.SavePositions();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	std::cout << PositionCounter.load(std::memory_order_acquire) << " positions solved in: " << time_format(duration) << std::endl;
	std::cout << PositionCounter.load(std::memory_order_acquire) * 1000.0 / duration.count() << " positions per second." << std::endl;
	std::cout << NodeCounter * 1000 / duration.count() << " nodes per second." << std::endl;
}

int main(int argc, char* argv[])
{
	PositionCounter.store(0, std::memory_order_release);
	Spinlock.clear(std::memory_order_release);
	bool b_file = false;
	int d = CSearch::END;
	int n = 10;
	int t = 4;
	char* Filename;

	for(int i = 0; i < argc; ++i)
	{
		if (std::string(argv[i]) == "-f"){
			Filename = argv[++i];
			b_file = true;
		}
		else if (std::string(argv[i]) == "-n")
			n = atoi(argv[++i]);
		else if (std::string(argv[i]) == "-d")
			d = atoi(argv[++i]);
		else if (std::string(argv[i]) == "-t")
			t = atoi(argv[++i]);
		else if (std::string(argv[i]) == "-h")
			std::cout << "Solves a file of given reversi positions." << std::endl <<
					"Arguments:" << std::endl <<
					"-f\tFilename." << std::endl <<
					"-n\tNumber of positions to solve (default: 100)" << std::endl <<
					"-d\tDepth to solve for (default: Exact)" << std::endl <<
					"-t\tNumber of threads" << std::endl <<
					"-h\tDisplays help." << std::endl;
	}

	SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);

	//Solve(std::string("G:\\Reversi\\pos\\rnd_d19_10M.b").c_str(), n, d, t);
	
	if (b_file)
		Solve(Filename, n, d, t);

	return 0;
}