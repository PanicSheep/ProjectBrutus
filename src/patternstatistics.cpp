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
#include <random>

template <typename K>
class CMatrix
{
public:
	unsigned int n, m;
	CMatrix(const unsigned int n, const unsigned int m) : n(n), m(m) { m_elements = new K[n*m]; }
	~CMatrix() { delete[] m_elements; }
	K& operator()(const unsigned int i, const unsigned int j) { return m_elements[i*m+j]; }
	const K& operator()(const unsigned int i, const unsigned int j) const { return m_elements[i*m+j]; }
	std::vector<K> operator*(const std::vector<K>& v) const
	{
		assert(v.size() == m);
		std::vector<K> ret(n);
		for (unsigned int i = 0; i < n; ++i){
			ret[i] = 0;
			for (unsigned int j = 0; j < m; ++j)
				ret[i] += this->operator()(i, j) * v[j];
		}
		return ret;
	}
private:
	K * m_elements;
};

template <typename K>
std::vector<K> operator+(const std::vector<K>& a, const std::vector<K>& b)
{
	assert(a.size() == b.size());
	std::vector<K> c(a.size());
	for (int i = 0; i < a.size(); ++i)
		c[i] = a[i] + b[i];
	return c;
}

template <typename K>
std::vector<K> operator-(const std::vector<K>& a, const std::vector<K>& b)
{
	assert(a.size() == b.size());
	std::vector<K> c(a.size());
	for (int i = 0; i < a.size(); ++i)
		c[i] = a[i] - b[i];
	return c;
}

template <typename K>
std::vector<K> operator*(const long double a, const std::vector<K>& b)
{
	std::vector<K> c(b.size());
	for (int i = 0; i < b.size(); ++i)
		c[i] = a * b[i];
	return c;
}

template <typename K>
long double dot(const std::vector<K>& a, const std::vector<K>& b)
{
	assert(a.size() == b.size());
	long double sum = 0.0;
	const std::size_t size = a.size();
	for (std::size_t i = 0; i < size; ++i)
		sum += a[i] * b[i];
	return sum;
}

template <typename K>
long double dot(const std::vector<K>& a)
{ 
	long double sum = 0.0;
	const std::size_t size = a.size();
	for (std::size_t i = 0; i < size; ++i)
		sum += a[i] * a[i];
	return sum;
}

template <typename K>
long double norm(const std::vector<K>& a) { return std::sqrtl(dot(a)); }

template <typename K>
std::vector<K> CG(const CMatrix<K>& A, const std::vector<K> b)
{
	auto rnd = std::bind(std::uniform_real_distribution<K>(-1, 1), std::mt19937_64(13));
	long double alpha, beta, tmp;
	std::vector<K> x(A.m);
	for (auto& elem : x) elem = rnd();
	std::vector<K> r = b - A * x;
	std::vector<K> p = r;
	std::vector<K> z;

	for (int it = 0; it < 3 * A.m; ++it) {
		z = A * p;
		tmp = dot(r, r);
		alpha = tmp / dot(p, z);
		x = x + alpha * p;
		r = r - alpha * z;
		if (norm(r) < 1e-8) return x;
		beta = dot(r, r) / tmp;
		p = r + beta * p;
	}
	std::cerr << "ERROR: CG did not converge." << std::endl;
	return x;
}

void PatternStatistics(std::vector<CPositionScore>& Positions)
{
	const std::size_t NumberOfPattern = Features::FeatureList.size();
	CMatrix<double> X(Positions.size(), NumberOfPattern);
	CMatrix<double> XTX(NumberOfPattern, NumberOfPattern);
	std::vector<double> Y(Positions.size());
	std::vector<double> XTY(NumberOfPattern);
	std::vector<double> err(Positions.size());
	std::vector<double> Std(NumberOfPattern);
	std::vector<double> b;
	std::vector<double> e;
	long double sigma, s_sq;

	float * scores = new float[NumberOfPattern];

	// Fill X and Y and e
	for (unsigned int i = 0; i < Positions.size(); ++i)
	{
		Y[i] = EvaluateFeatures(Positions[i].P, Positions[i].O, scores);
		err[i] = Y[i] - Positions[i].score;
		for (unsigned int j = 0; j < NumberOfPattern; ++j)
			X(i, j) = scores[j];
	}
	
	delete[] scores;

	// Calculating standard deviation
	sigma = StandardDeviation(err);
	printf("stdev: %s\n", ScientificNotaion(sigma, sigma / std::sqrtl(2 * (err.size() - 1)), 2).c_str());

	// Calculating avg abs err
	printf("avg abs err: %3.3f\n\n", AverageAbs(err));
	
	// Calculate XTY = X^T * Y
	for (unsigned int i = 0; i < NumberOfPattern; ++i)
	{
		XTY[i] = 0;
		for (unsigned int j = 0; j < Positions.size(); ++j)
			XTY[i] += X(j, i) * Y[j];
	}

	// Calculate XTX = X^T * X
	for (unsigned int i = 0; i < NumberOfPattern; ++i)
		for (unsigned int j = 0; j < NumberOfPattern; ++j)
		{
			XTX(i, j) = 0;
			for (unsigned int k = 0; k < Positions.size(); ++k)
				XTX(i, j) += X(k, i) * X(k, j);
		}

	b = CG(XTX, XTY);
	e = Y - (X * b);
	s_sq = dot(e, e) / static_cast<double>(Positions.size() - NumberOfPattern);

	// Calculating Std
	for (unsigned int i = 0; i < NumberOfPattern; ++i)
	{
		std::vector<double> unit(NumberOfPattern);
		for (auto& elem : unit) elem = 0;
		unit[i] = 1;
		Std[i] = s_sq * CG(XTX, unit)[i];
	}


	printf(" Pattern  |    b    |    std    |     t     \n");
	printf("----------+---------+-----------+-----------\n");

	for (std::size_t i = 0; i < Features::FeatureList.size(); ++i)
		printf(" %-9s| %1.5f | %1.2E | %1.2E \n", Features::FeatureList[i]->Name(), b[i], Std[i], b[i]/Std[i]);
}

void PatternStatistics(const std::vector<std::string>& FileNames, const int size, const bool verbose)
{
	std::cout << "Calculating pattern statistics.\n";
	for (auto& filename : FileNames)
		std::cout << filename << "\n";
	std::cout << "Number of positions per file to process: " << size << "\n"
			  << (verbose ? "Verbose\n" : "Non verbose\n")
			  << "Start time: " << DateTimeNow() << std::endl;
	
	std::vector<CPositionScore> Positions;

	for (auto& filename : FileNames)
	{
		std::vector<CPositionScore> vec = read_vector<CPositionScore>(filename, size);
		Positions.insert(Positions.end(), vec.begin(), vec.end());
	}

	PatternStatistics(Positions);
}

void Print_help()
{
	std::cout << std::endl << "Calculates pattern statistics." << std::endl
						   << "Attributes:" << std::endl
						   << "-f\tposition files." << std::endl
						   << "-n\tnumber of positions." << std::endl
						   << "-v\tverbose." << std::endl
						   << "-h\tprints this help." << std::endl << std::endl;
}

int main(int argc, char* argv[])
{
	std::vector<std::string> FileNames;
	int n = 10000;
	bool v = false;
	int conf = 0;

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
		else if (std::string(argv[i]) == "-v")
			v = true;
		else if (std::string(argv[i]) == "-conf")
			conf = atoi(argv[++i]);
		else if (std::string(argv[i]) == "-h"){
			Print_help();
			return 0;
		}
	}

	SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);
	ConfigFile::Initialize(argv[0], std::string("ProjectBrutus.ini"));
	Features::Initialize(conf);

	//for (int i = 7; i < 8; ++i)
	//	FileNames.push_back(std::string("G:\\Reversi\\pos\\rnd_d" + std::to_string(i) + "_1M.ps"));

	PatternStatistics(FileNames, n, v);
	return 0;
}