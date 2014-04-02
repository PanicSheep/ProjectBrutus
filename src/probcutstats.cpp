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
#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <omp.h>
#include <Windows.h>

class CComparisonPair
{
public:
	std::vector<std::string> FileNames;
	int d, s, D, S;
	std::atomic<unsigned long long> occurences[129][129];
	CSimpleLinearRegression<int> LinReg;

	CComparisonPair() = delete;
	CComparisonPair(const CComparisonPair& that) = delete;
	CComparisonPair(int d, const int s, const int D, const int S) : FileNames(FileNames), d(d), s(s), D(D), S(S)
	{
		for (int i = 0; i < 129; ++i)
			for (int j = 0; j < 129; ++j)
				occurences[i][j].store(0);
	}

	void Add(const std::string & FileName) { FileNames.push_back(FileName); }

	bool Contains(const std::string & FileName) const
	{
		for (auto& filename : FileNames)
			if (FileName.compare(filename))
				return true;
		return false;
	}
};

struct CSearchDepths
{
	bool depth[128][7];
	int score[128][7];
	CSearchDepths()
	{
		for (int d = 0; d < 128; ++d)
			for (int s = 0; s < 7; ++s)
				depth[d][s] = false;
	}
	CSearchDepths(std::vector<CComparisonPair*>& ComparisonPairs, std::string FileName)
	{
		for (int d = 0; d < 128; ++d)
			for (int s = 0; s < 7; ++s)
				depth[d][s] = false;
		for (CComparisonPair* Pair : ComparisonPairs)
			if (Pair->Contains(FileName))
			{
				depth[Pair->d][Pair->s] = true;
				depth[Pair->D][Pair->S] = true;
			}
	}

	void Distribute(std::vector<CComparisonPair*>& ComparisonPairs, std::string FileName)
	{
		for (CComparisonPair* Pair : ComparisonPairs)
			if (Pair->Contains(FileName))
			{
				Pair->occurences[score[Pair->D][Pair->S]+64][score[Pair->d][Pair->s]+64].fetch_add(1, std::memory_order_relaxed);
				Pair->LinReg.Add(score[Pair->d][Pair->s], score[Pair->D][Pair->S]);
			}
	}
};

std::vector<std::string> FileNames(std::vector<CComparisonPair*>& ComparisonPairs)
{
	std::vector<std::string> tmp;
	for (CComparisonPair* Pair : ComparisonPairs)
		tmp.insert(tmp.end(), Pair->FileNames.begin(), Pair->FileNames.end());
	std::sort(tmp.begin(), tmp.end());
	tmp.erase(std::unique(tmp.begin(), tmp.end()), tmp.end());
	return tmp;
}

void CalcStats(std::vector<CComparisonPair*>& ComparisonPairs, const int size, const bool contour_plot_data)
{
	CHashTable* hashTable = new CHashTable(24);
	ConfigFile::Initialize(std::string("G:\\Reversi\\ProjectBrutus.ini"));
	Features::Initialize();

	std::vector<DATASET_POSITON_SCORE> data;
	CSearch search;
	std::atomic<unsigned long long> counter;
	std::chrono::high_resolution_clock::time_point TimePoint;

	std::vector<std::string> filenames = FileNames(ComparisonPairs);

	for (auto& filename : filenames)
	{
		std::cout << "Processing file: " << filename << std::endl;
		std::cout << "\r"; print_progressbar_percentage(50, 0);
		TimePoint = std::chrono::high_resolution_clock::now();
		counter = 0;
		data.clear();
		read_vector(filename, data);
		#pragma omp parallel for private(search) schedule(static, 1)
		for (int i = 0; i < size; ++i)
		{
			CSearchDepths searchdepths(ComparisonPairs, filename);

			for (int d = 0; d < 127; ++d)
			{
				for (int s = 6; s >= 0; --s)
				{
					if (searchdepths.depth[d][s])
					{
						search = CSearch(data[i].P, data[i].O, -64, 64, d, s, hashTable, CSearch::NodeType::PV_Node);
						search.Evaluate();
						searchdepths.score[d][s] = search.score;
					}
				}
			}
			if (searchdepths.depth[CSearch::END][0])
				searchdepths.score[CSearch::END][0] = data[i].score;
			hashTable->AdvanceDate();

			searchdepths.Distribute(ComparisonPairs, filename);

			counter.fetch_add(1, std::memory_order_relaxed);
			#pragma omp critical
			{
				if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - TimePoint).count() > 1000)
				{
					std::cout << "\r"; print_progressbar_percentage(50, counter / static_cast<double>(size));
					TimePoint = std::chrono::high_resolution_clock::now();
				}
			}
		}
		std::cout << "\r"; print_progressbar_percentage(50, 1); std::cout << std::endl;
	}

	std::cout << std::endl << "############" << std::endl;
	
	unsigned long long N;
	double a, b, value, Sum_X, Sum_X_sq;
	for (auto& Pair : ComparisonPairs)
	{
		Sum_X = 0;
		Sum_X_sq = 0;
		N = 0;
		a = Pair->LinReg.a();
		b = Pair->LinReg.b();
		std::cout << std::endl;
		std::cout << "Depths: " << Pair->d << "(" << Pair->s << ")" << " " << Pair->D << "(" << Pair->S << ")" << std::endl;
		std::cout << "a:    " << a << std::endl;
		std::cout << "b:    " << b << std::endl;
		std::cout << "R_sq: " << Pair->LinReg.R_sq() << std::endl;
		for (int i = 0; i < 129; i++){
			for (int j = 0; j < 129; j++)
			{
				value = a + b * (j-64) - (i-64);
				N += Pair->occurences[i][j];
				Sum_X += Pair->occurences[i][j] * value;
				Sum_X_sq += Pair->occurences[i][j] * value * value;
			}
		}
		Sum_X /= static_cast<double>(N);
		Sum_X_sq /= static_cast<double>(N);
		std::cout << "sigma: " << std::sqrt(Sum_X_sq - Sum_X * Sum_X) << std::endl;
	}

	std::cout << std::endl << "############" << std::endl;

	if (contour_plot_data)
	{
		for (auto& Pair : ComparisonPairs)
		{
			std::cout << std::endl;
			std::cout << "Depths: " << Pair->d << "(" << Pair->s << ")" << " " << Pair->D << "(" << Pair->S << ")" << std::endl;
			for (int l = 0; l <= 128; ++l)
			{
				for (int m = 0; m <= 128; ++m)
					std::cout << Pair->occurences[l][m] << "\t";
				std::cout << std::endl;
			}
		}
	}
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
	SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);

	std::vector<std::string> FileNames;
	int d, n;

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
	//d = 8;
	//n = 100000;
	std::vector<CComparisonPair*> ComparisonPairs;
	CComparisonPair* tmp;

	//for (int s = 6; s >= 0; s--)
	//	for (int S = s - 1; S >= 0; S--)
	//	{
	//		tmp = new CComparisonPair(16/*d*/, s/*s*/, 16/*D*/, S/*S*/);
	//		tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d19_1M.b"));
	//		tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d20_1M.b"));
	//		tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d21_1M.b"));
	//		tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d22_1M.b"));
	//		ComparisonPairs.push_back(tmp);
	//	}

	tmp = new CComparisonPair(16/*d*/, 6/*s*/, 16/*D*/, 0/*S*/);
	//tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d23_1M.b"));
	//tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d24_1M.b"));
	//tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d25_1M.b"));
	//tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d26_1M.b"));
	//tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d27_1M.b"));
	//tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d28_1M.b"));
	//tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d29_1M.b"));
	//tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d30_1M.b"));
	//tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d31_1M.b"));
	//tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d32_1M.b"));
	//tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d33_1M.b"));
	//tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d34_1M.b"));
	//tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d35_1M.b"));
	//tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d36_1M.b"));
	//tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d37_1M.b"));
	ComparisonPairs.push_back(tmp);

	//tmp = new CComparisonPair(10/*d*/, 6/*s*/, 10/*D*/, 0/*S*/);
	//tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d19_1M.b"));
	//tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d20_1M.b"));
	//tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d21_1M.b"));
	//tmp->Add(std::string("G:\\Reversi\\pos\\rnd_d22_1M.b"));
	//ComparisonPairs.push_back(tmp);

	//CompareToExactNumEmpty(FileNames, d, n, false);
	//CompareToExact(FileNames, d, n, false);
	std::chrono::high_resolution_clock::time_point startTime, endTime;
	startTime = std::chrono::high_resolution_clock::now();
	CalcStats(ComparisonPairs, 10, true);
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	std::cout << time_format(duration) << std::endl;

	return 0;
}