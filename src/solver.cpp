#include "solver.h"

CSearch::HashTablePV * HashTablePV;
CSearch::HashTableL1 * HashTableL1;
CSearch::HashTableL2 * HashTableL2; 
unsigned long long NodeCounter;
std::atomic<std::size_t> PositionCounter;
std::atomic_flag Spinlock;

void CPositionManager::LoopOverSolvedPositions(const unsigned char depth)
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

void Work(CPositionManager& PositionManager, const int depth)
{
	DATASET_POSITON_SCORE * data;
	while (data = PositionManager.TryGetWork())
	{
		CSearch search(data->P, data->O, -64, 64, HashTablePV, HashTableL1, HashTableL2);
		search.Evaluate();
		data->depth = depth;
		//if (data->score != search.score)
		//{
		//	while (Spinlock.test_and_set());
		//	std::cout << std::dec << (int)data->score << " : " << (int)search.score << " : " << std::hex << data->P << " : " << std::hex << data->O << std::dec << std::endl;
		//	Spinlock.clear();
		//}
		data->score = search.score;
		NodeCounter += search.NodeCounter;
		PositionCounter.fetch_add(1, std::memory_order_relaxed);
	}
}

void Solve(const char* Filename, const int n, const int depth, const int nthreads)
{
	HashTablePV = new CSearch::HashTablePV();
	HashTableL1 = new CSearch::HashTableL1();
	HashTableL2 = new CSearch::HashTableL2();
	NodeCounter = 0;

	std::cout << nthreads << " threads in total.\n";

	std::chrono::high_resolution_clock::time_point startTime, endTime;
	std::vector<std::thread> threads(nthreads-1);
	CPositionManager PositionManager(Filename, n, depth);

	startTime = std::chrono::high_resolution_clock::now(); //Start Time

	// Start workers
	for (int i = 0; i < nthreads-1; ++i)
		threads[i] = std::thread(Work, std::ref(PositionManager), depth);
	Work(PositionManager, depth);
  
	// Join workers
	for (int i = 0; i < nthreads-1; ++i)
		threads[i].join();

	endTime = std::chrono::high_resolution_clock::now(); //Stop Time

	PositionManager.SavePositions();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	std::cout << PositionCounter.load(std::memory_order_acquire) << " positions solved in: " << time_format(duration) << std::endl;
	std::cout << PositionCounter.load(std::memory_order_acquire) * 1000.0 / duration.count() << " positions per second." << std::endl;
	std::cout << NodeCounter * 1000 / duration.count() << " nodes per second." << std::endl;

	delete HashTablePV;
	delete HashTableL1;
	delete HashTableL2;
}

int main(int argc, char* argv[])
{
	PositionCounter.store(0, std::memory_order_release);
	Spinlock.clear(std::memory_order_release);
	bool b_file = false;
	int d = CSearch::END;
	int n = 100;
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

	//Solve(std::string("G:\\Reversi\\pos\\rnd_d5_100M.b").c_str(), n, d, t);
	
	if (b_file)
		Solve(Filename, n, d, t);

	return 0;
}