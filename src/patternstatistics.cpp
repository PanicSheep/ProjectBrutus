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

template <typename K>
class CMatrix
{
public:
	unsigned int n, m;
	CMatrix(const unsigned int n, const unsigned int m) : n(n), m(m) { m_elements = new K[n*m]; }
	~CMatrix() { delete[] m_elements; }
	K& operator()(const unsigned int i, const unsigned int j) { return m_elements[i*m+j]; }
	std::vector<K> operator*(const std::vector<K>& v)
	{
		assert(v.size() == m);
		std::vector<K> ret(n);
		for (unsigned int i = 0; i < m; ++i){
			ret[i] = 0;
			for (unsigned int j = 0; j < n; ++j)
				ret[i] += this->operator()(i, j) * v[j];
		}
		return ret;
	}
private:
	K * m_elements;
};

//void Penis(std::vector<CPosition>& Positions)
//{
//	CMatrix<float> X(Positions.size(), Features::NumberOfFeatures);
//	CMatrix<float> XTX(Features::NumberOfFeatures, Features::NumberOfFeatures);
//	std::vector<float> Y(Positions.size());
//	std::vector<float> XTY(Features::NumberOfFeatures);
//
//	float scores[Features::NumberOfFeatures];
//
//	// Fill X and Y
//	for (unsigned int i = 0; i < Positions.size(); ++i)
//	{
//		Y[i] = EvaluateFeatures(Positions[i].P, Positions[i].O, scores);
//		for (unsigned int j = 0; j < Features::NumberOfFeatures; ++j)
//			X(i, j) = scores[j];
//	}
//	
//	// Calculate XTY = X^T * Y
//	for (unsigned int i = 0; i < Positions.size(); ++i)
//	{
//		XTY[i] = 0;
//		for (unsigned int j = 0; j < Features::NumberOfFeatures; ++j)
//			XTY[i] += X(j, i) * Y[i];
//	}
//
//	// Calculate XTX = X^T * X
//	for (unsigned int i = 0; i < Features::NumberOfFeatures; ++i)
//	{
//		XTX(i) = 0;
//		for (unsigned int j = 0; j < Features::NumberOfFeatures; ++j)
//			XTX[i] += X(j, i) * X(i, j);
//	}
//}


template <class T>
void PatternStatistics(const std::string& filename, const int n, const bool verbose)
{
	std::vector<T> positions = read_vector<T>(filename);
	float scores[Features::NumberOfFeatures];
	float E_of_X[Features::NumberOfFeatures];
	float E_of_X_sq[Features::NumberOfFeatures];
	int Feature_score, Stored_score, err;
	std::size_t N = 0;

	for (std::size_t i = 0; i < Features::NumberOfFeatures; ++i) scores[i] = 0.0;
	for (std::size_t i = 0; i < Features::NumberOfFeatures; ++i) E_of_X[i] = 0.0;
	for (std::size_t i = 0; i < Features::NumberOfFeatures; ++i) E_of_X_sq[i] = 0.0;

	printf("names:   ");
	for (std::size_t i = 0; i < Features::NumberOfFeatures; ++i) printf("\t %2s", Features::Names[i]);
	printf("\n");

	for (auto& pos : positions)
	{
		Feature_score = EvaluateFeatures(pos.P, pos.O, scores);
		Stored_score = pos.score;
		err = Feature_score - Stored_score;

		printf("%+2.2d : %+2.2d\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\n", Stored_score, Feature_score, scores[0], scores[1], scores[2], scores[3], scores[4], scores[5], scores[6], scores[7], scores[8], scores[9], scores[10]);

		++N;
		for (std::size_t i = 0; i < Features::NumberOfFeatures; ++i) E_of_X[i] += (static_cast<float>(std::abs(scores[i])) - E_of_X[i]) / static_cast<float>(N);
		for (std::size_t i = 0; i < Features::NumberOfFeatures; ++i) E_of_X_sq[i] += (static_cast<float>(std::abs(scores[i]))*static_cast<float>(std::abs(scores[i])) - E_of_X_sq[i]) / static_cast<float>(N);
	}

	printf("avg diff:\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\n", E_of_X[0], E_of_X[1], E_of_X[2], E_of_X[3], E_of_X[4], E_of_X[5], E_of_X[6], E_of_X[7], E_of_X[8], E_of_X[9], E_of_X[10]);
	printf("std diff:\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\n", 
		std::sqrt(E_of_X_sq[ 0] - E_of_X[ 0] * E_of_X[ 0]), 
		std::sqrt(E_of_X_sq[ 1] - E_of_X[ 1] * E_of_X[ 1]), 
		std::sqrt(E_of_X_sq[ 2] - E_of_X[ 2] * E_of_X[ 2]), 
		std::sqrt(E_of_X_sq[ 3] - E_of_X[ 3] * E_of_X[ 3]), 
		std::sqrt(E_of_X_sq[ 4] - E_of_X[ 4] * E_of_X[ 4]), 
		std::sqrt(E_of_X_sq[ 5] - E_of_X[ 5] * E_of_X[ 5]), 
		std::sqrt(E_of_X_sq[ 6] - E_of_X[ 6] * E_of_X[ 6]), 
		std::sqrt(E_of_X_sq[ 7] - E_of_X[ 7] * E_of_X[ 7]), 
		std::sqrt(E_of_X_sq[ 8] - E_of_X[ 8] * E_of_X[ 8]), 
		std::sqrt(E_of_X_sq[ 9] - E_of_X[ 9] * E_of_X[ 9]), 
		std::sqrt(E_of_X_sq[10] - E_of_X[10] * E_of_X[10]));
}

template <>
void PatternStatistics<CDataset_Position_FullScore>(const std::string& filename, const int n, const bool verbose)
{
	std::vector<CDataset_Position_FullScore> positions = read_vector<CDataset_Position_FullScore>(filename);
	float scores[Features::NumberOfFeatures];
	float E_of_X[Features::NumberOfFeatures];
	float E_of_X_sq[Features::NumberOfFeatures];
	int Feature_score, Stored_score, err;
	std::size_t N = 0;

	for (std::size_t i = 0; i < Features::NumberOfFeatures; ++i) scores[i] = 0.0;
	for (std::size_t i = 0; i < Features::NumberOfFeatures; ++i) E_of_X[i] = 0.0;
	for (std::size_t i = 0; i < Features::NumberOfFeatures; ++i) E_of_X_sq[i] = 0.0;

	for (auto& pos : positions)
	{
		Feature_score = EvaluateFeatures(pos.P, pos.O, scores);
		Stored_score = -128;
		for (std::size_t i = 0; i < 64; ++i) if (pos.score[i] > Stored_score) Stored_score = pos.score[i];
		err = Feature_score - Stored_score;

		printf("%+2.2d : %+2.2d\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\t%1.3f\n", Stored_score, Feature_score, scores[0], scores[1], scores[2], scores[3], scores[4], scores[5], scores[6], scores[7], scores[8], scores[9], scores[10]);

		//++N;
		//for (std::size_t i = 0; i < Features::NumberOfFeatures; ++i) E_of_X[i] += (static_cast<float>(Feature_score) - E_of_X[i]) / static_cast<float>(N);
		//for (std::size_t i = 0; i < Features::NumberOfFeatures; ++i) E_of_X_sq[i] += (static_cast<float>(Feature_score)*static_cast<float>(Feature_score) - E_of_X_sq[i]) / static_cast<float>(N);
	}
}

void PatternStatistics(const std::vector<std::string>& FileNames, const int n, const bool verbose)
{
	std::cout << "Calculating pattern statistics.\n";
	for (auto& filename : FileNames)
		std::cout << filename << "\n" << std::endl;
	std::cout << "Number of positions per file to process: " << n << "\n"
			  << (verbose ? "Verbose\n" : "Non verbose\n")
			  << "Start time: " << DateTimeNow() << std::endl;
	
	for (auto& filename : FileNames)
	{
		std::string ending = filename.substr(filename.rfind(".") + 1, filename.length());
		switch (Ending_to_DataType(ending))
		{
		//case DataType::Old:                Solve<CDataset_Old               >(Filename, n, d, s, t, v, test); break;
		case DataType::Position_Score:     PatternStatistics<CDataset_Position_Score    >(filename, n, verbose); break;
		case DataType::Position_Score_PV:  PatternStatistics<CDataset_Position_Score_PV >(filename, n, verbose); break;
		case DataType::Position_FullScore: PatternStatistics<CDataset_Position_FullScore>(filename, n, verbose); break;
		}
	}
}

void Print_help()
{
	std::cout << std::endl << "Calculates pattern statistics." << std::endl
						   << "Attributes:" << std::endl
						   << "-f\tposition files." << std::endl
						   << "-n\tnumber of positions." << std::endl
						   << "-bit\tbit size of hash table." << std::endl
						   << "-v\tverbose." << std::endl
						   << "-h\tprints this help." << std::endl << std::endl;
}

int main(int argc, char* argv[])
{
	SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);
	ConfigFile::Initialize(argv[0], std::string("ProjectBrutus.ini"));
	Features::Initialize();

	std::vector<std::string> FileNames;
	int n = 10000;
	int bit = 24;
	bool v = false;

	for (int i = 0; i < argc; ++i)
	{
		if (std::string(argv[i]) == "-f"){
			++i;
			while ((i < argc) && ((char)(*argv[i]) != '-'))
				FileNames.push_back(std::string(argv[i++]));
			--i;
		}
		else if (std::string(argv[i]) == "-n")
			n = atoi(argv[++i]);
		else if (std::string(argv[i]) == "-bit")
			bit = atoi(argv[++i]);
		else if (std::string(argv[i]) == "-v")
			v = true;
		else if (std::string(argv[i]) == "-h"){
			Print_help();
			return 0;
		}
	}

	for (int i = 7; i < 49; ++i)
		FileNames.push_back(std::string("G:\\Reversi\\pos\\rnd_d" + std::to_string(i) + "_1M.ps"));

	PatternStatistics(FileNames, n, v);
	return 0;
}