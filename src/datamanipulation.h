#pragma once
#include <math.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>

class DATASET_OLD;                // .b
class DATASET_POSITON_SCORE;      // .ps
class DATASET_POSITON_SCORE_PV;   // .psp
class DATASET_POSITON_FULL_SCORE; // .pfs

enum DATASET { OLD, POSITON_SCORE, POSITON_SCORE_PV, POSITON_FULL_SCORE };

inline DATASET Ending_to_DATASET(const std::string & s){
	if (s == "b"  ) return DATASET::OLD;
	if (s == "ps" ) return DATASET::POSITON_SCORE;
	if (s == "psp") return DATASET::POSITON_SCORE_PV;
	if (s == "pfs") return DATASET::POSITON_FULL_SCORE;
}

const signed char   DATASET_DEFAULT_depth = -1;
const unsigned char DATASET_DEFAULT_selectivity = 0;
const signed char   DATASET_DEFAULT_score = SCHAR_MIN;
const unsigned char DATASET_DEFAULT_PV = 64;

#pragma pack(1)
// .b
// 144 Bit = 18 Byte
class DATASET_OLD
{
public:
	unsigned long long P;		// 64 bit
	unsigned long long O;		// 64 bit
	signed char depth;			//  8 bit
	signed char score;			//  8 bit

	inline void Reset(){
		depth = DATASET_DEFAULT_depth;
		score = DATASET_DEFAULT_score;
	}

	DATASET_OLD() : P(0), O(0), depth(DATASET_DEFAULT_depth), score(DATASET_DEFAULT_score) {}
	DATASET_OLD(const unsigned long long P, const unsigned long long O, const signed char depth, const signed char score) : P(P), O(O), depth(depth), score(score) {}
};

// .ps
// 152 Bit = 19 Byte
class DATASET_POSITON_SCORE
{
public:
	unsigned long long P;		// 64 bit
	unsigned long long O;		// 64 bit
	signed char depth;			//  8 bit
	unsigned char selectivity;	//  8 Bit
	signed char score;			//  8 bit

	inline void Reset(){
		depth = DATASET_DEFAULT_depth;
		selectivity = DATASET_DEFAULT_selectivity;
		score = DATASET_DEFAULT_score;
	}

	DATASET_POSITON_SCORE() : P(0), O(0), depth(DATASET_DEFAULT_depth), selectivity(DATASET_DEFAULT_selectivity), score(DATASET_DEFAULT_score) {}
	DATASET_POSITON_SCORE(const unsigned long long P, const unsigned long long O, const signed char depth, const unsigned char selectivity, const signed char score) : P(P), O(O), depth(depth), selectivity(selectivity), score(score) {}

	explicit DATASET_POSITON_SCORE(const DATASET_OLD&                that);
	explicit DATASET_POSITON_SCORE(const DATASET_POSITON_SCORE_PV&   that);
	explicit DATASET_POSITON_SCORE(const DATASET_POSITON_FULL_SCORE& that);
};

// .psp
// 192 Bit = 24 Byte
class DATASET_POSITON_SCORE_PV
{
public:
	unsigned long long P;		// 64 bit
	unsigned long long O;		// 64 bit
	signed char depth;			//  8 bit
	unsigned char selectivity;	//  8 Bit
	signed char score;			//  8 bit
	unsigned char PV[5];		// 40 bit

	void Reset();

	DATASET_POSITON_SCORE_PV();
	DATASET_POSITON_SCORE_PV(const unsigned long long P, const unsigned long long O, const signed char depth, const unsigned char selectivity, const signed char score) : P(P), O(O), depth(depth), selectivity(selectivity), score(score) {}

	explicit DATASET_POSITON_SCORE_PV(const DATASET_OLD&                that);
	explicit DATASET_POSITON_SCORE_PV(const DATASET_POSITON_SCORE&      that);
	explicit DATASET_POSITON_SCORE_PV(const DATASET_POSITON_FULL_SCORE& that);
};

// .pfs
// 1168 Bit = 146 Byte
class DATASET_POSITON_FULL_SCORE
{
public:
	unsigned long long P;		//  64 bit
	unsigned long long O;		//  64 bit
	signed char depth;			//   8 bit
	unsigned char selectivity;	//   8 Bit
	signed char score[64];		// 512 bit
	unsigned char PV[64];		// 512 bit

	// score[27] != DATASET_DEFAULT_score <==> Game over.
	// score[36] != DATASET_DEFAULT_score <==> Only best move score aviable.

	void Reset();

	DATASET_POSITON_FULL_SCORE();
	DATASET_POSITON_FULL_SCORE(const unsigned long long P, const unsigned long long O, const signed char depth, const unsigned char selectivity);

	explicit DATASET_POSITON_FULL_SCORE(const DATASET_OLD&              that);
	explicit DATASET_POSITON_FULL_SCORE(const DATASET_POSITON_SCORE&    that);
	explicit DATASET_POSITON_FULL_SCORE(const DATASET_POSITON_SCORE_PV& that);
};

#pragma pack()


template<class T>
std::vector<T> read_vector(const std::string & filename)
{
	FILE* file;
	fopen_s(&file, filename.c_str(), "rb");
	if (!file){
		std::cerr << "ERROR: File '" << filename << "' could not be opened!" << std::endl;
		throw "File could not be opened.";
		return std::vector<T>();
	}

	const std::size_t N = 4 * 1024;
	std::size_t ValidData;
	std::vector<T> vector;
	T* DataArray = new T[N];
	while (ValidData = fread(DataArray, sizeof(T), N, file))
		vector.insert(vector.end(), DataArray, DataArray + ValidData);

	fclose(file);
	delete[] DataArray;

	return vector;
}

template<class T>
std::vector<T> read_vector(const std::string & filename, std::size_t size)
{
	FILE* file;
	fopen_s(&file, filename.c_str(), "rb");
	if (!file){
		std::cerr << "ERROR: File '" << filename << "' could not be opened!" << std::endl;
		throw "File could not be opened.";
		return std::vector<T>();
	}

	const std::size_t N = 4 * 1024;
	std::size_t ValidData;
	std::vector<T> vector;
	T* DataArray = new T[N];
	while (ValidData = fread(DataArray, sizeof(T), (N < size ? N : size), file)){
		vector.insert(vector.end(), DataArray, DataArray + ValidData);
		size -= ValidData;
	}

	fclose(file);
	delete[] DataArray;

	return vector;
}

template<class T>
void read_vector(const std::string & filename, std::vector<T>& vector)
{
	FILE* file;
	fopen_s(&file, filename.c_str(), "rb");
	if (!file){
		std::cerr << "ERROR: File '" << filename << "' could not be opened!" << std::endl;
		throw "File could not be opened.";
		return;
	}

	const std::size_t N = 4 * 1024;
	std::size_t ValidData;
	T* DataArray = new T[N];
	while (ValidData = fread(DataArray, sizeof(T), N, file))
		vector.insert(vector.end(), DataArray, DataArray + ValidData);

	fclose(file);
	delete[] DataArray;
}

template<class T>
void write_to_file(const std::string filename, std::vector<T>& vector)
{
	FILE* file;
	fopen_s(&file, filename.c_str(), "wb");
	if (!file){
		std::cerr << "ERROR: File '" << filename << "' could not be opened!" << std::endl;
		throw "File could not be opened.";
		return;
	}

	for (T& data : vector)
		fwrite(&data, sizeof(T), 1, file);

	fclose(file);
}



bool Test_datamanipulation();