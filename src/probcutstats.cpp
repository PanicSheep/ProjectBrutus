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
#include <atomic>
#include <iostream>
#include <string>
#include <vector>
#include <omp.h>

struct CScores
{
	std::atomic<unsigned long long> score[129][129];
	CScores(){
		for (int i = 0; i < 129; ++i)
			for (int j = 0; j < 129; ++j)
				score[i][j] = 0;
	}
};

void CompareToExactNumEmpty(const std::vector<std::string> FileNames, const int Empties, const int size, const bool contour_plot_data)
{
	CHashTable* hashTable = new CHashTable(24);
	ConfigFile::Initialize(std::string("G:\\Reversi\\ProjectBrutus.ini"));
	Features::Initialize();

	CRunningStatistic<int> Stats = CRunningStatistic<int>();
	CScores stats = CScores();
	std::vector<DATASET_POSITON_SCORE> data;
	CSearch search;
	std::atomic<unsigned long long> counter;
	

	for (auto& filename : FileNames)
	{
		std::cout << "Processing file: " << filename << std::endl;
		counter = 0;
		data.clear();
		read_vector(filename, data);
		#pragma omp parallel for private(search)
		for (int i = 0; i < size; ++i)
		{
			search = CSearch(data[i].P, data[i].O, -64, 64, NumberOfEmptyStones(data[i].P, data[i].O)-Empties, 0, hashTable, CSearch::NodeType::PV_Node);
			search.Evaluate();
			stats.score[search.score+64][data[i].score+64]++;
			Stats.Add(search.score - data[i].score);

			hashTable->AdvanceDate();
			
			if ((counter.fetch_add(1, std::memory_order_relaxed) & 0x3FF) == 0)
			{
				#pragma omp critical
				{
					std::cout << "\r"; print_progressbar_percentage(50, counter / static_cast<double>(size));
				}
			}
		}
		std::cout << "\r"; print_progressbar_percentage(50, 1); std::cout << std::endl;
	}

	std::cout << std::endl;
	std::cout << "Depth: " << Empties << std::endl;
	std::cout << "Average: " << Stats.mu() << std::endl;
	std::cout << "Sigma:   " << Stats.sigma() << std::endl;
	if (contour_plot_data)
	{
		for (int l = 0; l <= 128; ++l)
		{
			for (int m = 0; m <= 128; ++m)
				std::cout << stats.score[l][m] << "\t";
			std::cout << std::endl;
		}
	}

	Features::Finalize();
}

void CompareToExact(const std::vector<std::string> FileNames, const int d, const int size, const bool contour_plot_data)
{
	CHashTable* hashTable = new CHashTable(24);
	ConfigFile::Initialize(std::string("G:\\Reversi\\ProjectBrutus.ini"));
	Features::Initialize();

	CRunningStatistic<int> * Stats = new CRunningStatistic<int>[d+1];
	CScores * stats = new CScores[d+1];
	std::vector<DATASET_POSITON_SCORE> data;
	CSearch search;
	std::atomic<unsigned long long> counter;
	

	for (auto& filename : FileNames)
	{
		std::cout << "Processing file: " << filename << std::endl;
		counter = 0;
		data.clear();
		read_vector(filename, data);
		#pragma omp parallel for private(search)
		for (int i = 0; i < size; ++i)
		{
			for (int j = 0; j <= d; ++j)
			{
				search = CSearch(data[i].P, data[i].O, -64, 64, j, 0, hashTable, CSearch::NodeType::PV_Node);
				search.Evaluate();
				stats[j].score[search.score+64][data[i].score+64]++;
				Stats[j].Add(search.score - data[i].score);
			}
			hashTable->AdvanceDate();
			
			if ((counter.fetch_add(1, std::memory_order_relaxed) & 0x3FF) == 0)
			{
				#pragma omp critical
				{
					std::cout << "\r"; print_progressbar_percentage(50, counter / static_cast<double>(size));
				}
			}
		}
		std::cout << "\r"; print_progressbar_percentage(50, 1); std::cout << std::endl;
	}

	for (int j = 0; j <= d; ++j)
	{
		std::cout << std::endl;
		std::cout << "Depth: " << j << std::endl;
		std::cout << "Average: " << Stats[j].mu() << std::endl;
		std::cout << "Sigma:   " << Stats[j].sigma() << std::endl;
		if (contour_plot_data)
		{
			for (int l = 0; l <= 128; ++l)
			{
				for (int m = 0; m <= 128; ++m)
					std::cout << stats[j].score[l][m] << "\t";
				std::cout << std::endl;
			}
		}
	}

	delete stats;
	Features::Finalize();
}

void CalcStats(const std::vector<std::string> FileNames, const int d, const int size, const bool contour_plot_data)
{
	CHashTable* hashTable = new CHashTable(24);
	ConfigFile::Initialize(std::string("G:\\Reversi\\ProjectBrutus.ini"));
	Features::Initialize();

	CRunningStatistic<int> * Stats = new CRunningStatistic<int>[(d+1)*(d+1)];
	CScores * stats = new CScores[(d+1)*(d+1)];
	std::vector<DATASET_POSITON_SCORE> data;
	CSearch search;
	std::atomic<unsigned long long> counter;
	

	for (auto& filename : FileNames)
	{
		std::cout << "Processing file: " << filename << std::endl;
		counter = 0;
		data.clear();
		read_vector(filename, data);
		#pragma omp parallel for private(search)
		for (int i = 0; i < size; ++i)
		{
			unsigned long long * score = new unsigned long long[d+1];

			for (int j = 0; j <= d; ++j)
			{
				search = CSearch(data[i].P, data[i].O, -64, 64, j, 0, hashTable, CSearch::NodeType::PV_Node);
				search.Evaluate();
				score[j] = search.score;
			}
			hashTable->AdvanceDate();

			for (int j = 0; j <= d; ++j)
				for (int k = j+1; k <= d; ++k){
					stats[j*(d+1) + k].score[score[j]+64][score[k]+64]++;
					Stats[j*(d+1) + k].Add(score[j] - score[k]);
				}

			if ((counter.fetch_add(1, std::memory_order_relaxed) & 0x3FF) == 0)
			{
				#pragma omp critical
				{
					std::cout << "\r"; print_progressbar_percentage(50, counter / static_cast<double>(size));
				}
			}

			delete[] score;
		}
		std::cout << "\r"; print_progressbar_percentage(50, 1); std::cout << std::endl;
	}

	for (int j = 0; j <= d; ++j)
		for (int k = j+1; k <= d; ++k)
		{
			std::cout << std::endl;
			std::cout << "Depths: " << j << " " << k << std::endl;
			std::cout << "Average: " << Stats[j*(d+1) + k].mu() << std::endl;
			std::cout << "Sigma:   " << Stats[j*(d+1) + k].sigma() << std::endl;
			if (contour_plot_data)
			{
				for (int l = 0; l <= 128; ++l)
				{
					for (int m = 0; m <= 128; ++m)
						std::cout << stats[j*(d+1) + k].score[l][m] << "\t";
					std::cout << std::endl;
				}
			}
		}

	delete stats;
	Features::Finalize();
}

void Print_help()
{
	std::cout << std::endl << "Calculates the feature weights." << std::endl
						   << "Attributes:" << std::endl
						   << "-f\tposition files." << std::endl
						   << "-d\treduced depth." << std::endl 
						   << "-D\tnon reduced depth." << std::endl 
						   << "-h\tprints this help." << std::endl << std::endl;
}

int main(int argc, char* argv[])
{
	std::vector<std::string> FileNames;
	int d, n;

	for(int i = 0; i < argc; ++i) 
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
		else if (std::string(argv[i]) == "-h"){
			Print_help();
			return 0;
		}
	}

	//FileNames.push_back(std::string("G:\\Reversi\\pos\\rnd_d9_1M.b"));
	
	//FileNames.push_back(std::string("G:\\Reversi\\pos\\rnd_d13_1M.b"));
	//FileNames.push_back(std::string("G:\\Reversi\\pos\\rnd_d14_1M.b"));
	//FileNames.push_back(std::string("G:\\Reversi\\pos\\rnd_d15_1M.b"));
	//FileNames.push_back(std::string("G:\\Reversi\\pos\\rnd_d16_1M.b"));
	//FileNames.push_back(std::string("G:\\Reversi\\pos\\rnd_d17_1M.b"));
	//FileNames.push_back(std::string("G:\\Reversi\\pos\\rnd_d18_1M.b"));
	//FileNames.push_back(std::string("G:\\Reversi\\pos\\rnd_d19_1M.b"));
	//FileNames.push_back(std::string("G:\\Reversi\\pos\\rnd_d20_1M.b"));
	//FileNames.push_back(std::string("G:\\Reversi\\pos\\rnd_d21_1M.b"));
	//FileNames.push_back(std::string("G:\\Reversi\\pos\\rnd_d22_1M.b"));
	//d = 3;
	//n = 10000;

	//CompareToExactNumEmpty(FileNames, d, n, false);
	CompareToExact(FileNames, d, n, false);
	//CalcStats(FileNames, d, n, false);

	return 0;
}