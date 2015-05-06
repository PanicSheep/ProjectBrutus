#include "configfile.h"
#include "datamanipulation.h"
#include "features.h"
#include "game.h"
#include "hashtable.h"
#include "move.h"
#include "position.h"
#include "search.h"
#include "statistics.h"
#include "utility.h"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <iostream>
#include <map>
#include <omp.h>
#include <string>
#include <vector>

// empties depth Depth
struct CEDD
{
	unsigned int empties, depth, Depth;
	CEDD() = delete;
	CEDD(const unsigned int empties, const unsigned int depth, const unsigned int Depth) : empties(empties), depth(depth), Depth(Depth) {}

	inline bool operator< (const CEDD& other) const 
	{ 
		return (this->Depth <  other.Depth) 
		   || ((this->Depth == other.Depth) && (this->depth <  other.depth)) 
		   || ((this->Depth == other.Depth) && (this->depth == other.depth) && (this->empties < other.empties)); 
	}
};

class CComparisonPair
{
public:
	std::atomic<unsigned long long> occurences[129][129];

	CComparisonPair()
	{
		for (int i = 0; i < 129; ++i)
			for (int j = 0; j < 129; ++j)
				occurences[i][j].store(0);
	}

	void Add(const int score, const int Score) { occurences[score+64][Score+64].fetch_add(1, std::memory_order_relaxed); }
};

std::map<CEDD, CSimpleLinearRegression<int>> LinReg;
std::map<CEDD, CComparisonPair> CompPair;

inline void Add(const unsigned int empties, const unsigned int depth, const unsigned int Depth, const int score, const int Score)
{
	CEDD edd(empties, depth, Depth);
	LinReg[edd].Add(score, Score);
	CompPair[edd].Add(score, Score);
}


void AddStats(const std::string& filename, CHashTable* hashTable, const int Depth, int size)
{
	std::vector<CPositionScore> data = read_vector<CPositionScore>(filename, size);
	size = data.size();
	std::vector<unsigned int> depths;
	std::atomic<unsigned long long> counter;
	std::chrono::high_resolution_clock::time_point TimePoint;
	const int empties = NumberOfEmptyStones(data[0].P, data[0].O);

	std::cerr << "Processing file: " << filename << std::endl;
	std::cerr << "\r" << progressbar_percentage(50, 0);
	TimePoint = std::chrono::high_resolution_clock::now();
	counter = 0;

	// Fill list of search depths
	for (int i = 0; (i <= Depth) && (i <= empties - 10); ++i)
		depths.push_back(i);
	if (Depth >= empties)
		depths.push_back(empties);

	#pragma omp parallel for schedule(dynamic, 1)
	for (int i = 0; i < size; ++i)
	{
		CSearch search;
		std::vector<int> scores(129);
		for (auto d : depths){
			if (d == empties)
				scores[d] = data[i].score;
			else{
				search = CSearch(data[i].P, data[i].O, -64, 64, d, 0, hashTable, 0);
				search.Evaluate();
				scores[d] = search.score;
			}
		}
		hashTable->AdvanceDate();

		for (auto D = depths.cbegin(); D != depths.cend(); ++D)
			for (auto d = depths.cbegin(); d != D; ++d)
				if ((*d == 0) || (*d % 2 == *D % 2))
					Add(empties, *d, *D, scores[*d], scores[*D]);
		
		counter.fetch_add(1, std::memory_order_relaxed);
		#pragma omp critical
		{
			if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - TimePoint).count() > 1000)
			{
				std::cerr << "\r" << progressbar_percentage(50, counter / static_cast<double>(size));
				TimePoint = std::chrono::high_resolution_clock::now();
			}
		}
	}
	std::cerr << "\r" << progressbar_percentage(50, 1) << std::endl << std::endl;
}

void PrintStats()
{
	unsigned long long N;
	double a, b, value, Sum_X, Sum_X_sq;

	printf("probcut| E| D| d| |       a |      b |  sigma | delta s|\n");
	printf("-------+--+--+--+-+---------+--------+--------+--------+-----\n");

	for (const auto& item : LinReg)
	{
		Sum_X = 0;
		Sum_X_sq = 0;
		N = 0;
		a = item.second.a();
		b = item.second.b();
		for (int i = 0; i < 129; i++){
			for (int j = 0; j < 129; j++)
			{
				value = a + b * (j-64) - (i-64);
				N += CompPair[item.first].occurences[i][j];
				Sum_X += CompPair[item.first].occurences[i][j] * value;
				Sum_X_sq += CompPair[item.first].occurences[i][j] * value * value;
			}
		}
		Sum_X /= static_cast<double>(N);
		Sum_X_sq /= static_cast<double>(N);
		printf("probcut %02u %02u %02u = % 6.6f %6.6f %6.6f %6.6f %s\n", item.first.empties, item.first.Depth == CSearch::END ? item.first.empties : item.first.Depth, item.first.depth, 
			a, b, std::sqrt(Sum_X_sq - Sum_X * Sum_X), std::sqrt(Sum_X_sq - Sum_X * Sum_X) / std::sqrtl(2.0 * (static_cast<long double>(N) + 1.0)), 
			(2 * std::floor(item.first.Depth * 0.25) + (item.first.Depth & 1) == item.first.depth) ? "true" : "false");
	}
}

void CalcStats(const std::vector<std::string>& FileNames, const int d, const int n, const int bit, const bool verbose)
{
	CHashTable* hashTable = new CHashTable(bit);
	for (auto filename : FileNames)
		AddStats(filename, hashTable, d, n);
	delete hashTable;
}

void Print_help()
{
	std::cout << std::endl << "Calculates ProbCut statistics." << std::endl
						   << "Attributes:" << std::endl
						   << "-f\tposition files." << std::endl
						   << "-d\tdepth." << std::endl
						   << "-n\tnumber of positions." << std::endl
						   << "-bit\tbit size of hash table." << std::endl
						   << "-v\tverbose." << std::endl
						   << "-h\tprints this help." << std::endl << std::endl;
}

int main(int argc, char* argv[])
{
	std::vector<std::string> FileNames;
	int d;
	int n = 1000;
	int bit = 24;
	bool verbose = false;

	for (int i = 0; i < argc; ++i)
	{
		if (std::string(argv[i]) == "-f"){
			++i;
			while ((i < argc) && ((char)(*argv[i]) != '-'))
				FileNames.push_back(std::string(argv[i++]));
			--i;
		}
		else if (std::string(argv[i]) == "-d")
			d = atoi(argv[++i]);
		else if (std::string(argv[i]) == "-n")
			n = atoi(argv[++i]);
		else if (std::string(argv[i]) == "-bit")
			bit = atoi(argv[++i]);
		else if (std::string(argv[i]) == "-v")
			verbose = true;
		else if (std::string(argv[i]) == "-h"){
			Print_help();
			return 0;
		}
	}

	ConfigFile::Initialize(argv[0], std::string("ProjectBrutus.ini"));
	Features::Initialize();
	Midgame::Initialize();

	std::cout << "ProbCut statistics started for n=" << n << std::endl;
	std::cout << "Start time: " << DateTimeNow() << std::endl;
	
	std::chrono::high_resolution_clock::time_point startTime, endTime;
	startTime = std::chrono::high_resolution_clock::now();
	CalcStats(FileNames, d, n, bit, verbose);
	PrintStats();
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	std::cout << std::endl << "End time: " << DateTimeNow() 
		<< "Computation time: " << time_format(duration) << std::endl;

	Features::Finalize();
	return 0;
}