#pragma once
#include <math.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>

class CDataset_Old;                // .b
class CDataset_Position_Score;     // .ps
class CDataset_Position_Score_PV;  // .psp
class CDataset_Position_FullScore; // .pfs

enum DataType { Old, Position_Score, Position_Score_PV, Position_FullScore };

template <DataType datatype> struct CDataset_type { typedef void type; };
template <> struct CDataset_type<DataType::Old>                { typedef CDataset_Old                type; };
template <> struct CDataset_type<DataType::Position_Score>     { typedef CDataset_Position_Score     type; };
template <> struct CDataset_type<DataType::Position_Score_PV>  { typedef CDataset_Position_Score_PV  type; };
template <> struct CDataset_type<DataType::Position_FullScore> { typedef CDataset_Position_FullScore type; };

inline DataType Ending_to_DataType(const std::string & s){
	if (s == "b"  ) return DataType::Old;
	if (s == "ps" ) return DataType::Position_Score;
	if (s == "psp") return DataType::Position_Score_PV;
	if (s == "pfs") return DataType::Position_FullScore;
}

inline std::string DataType_to_Ending(const DataType & d){
	if (d == DataType::Old)                return "b";
	if (d == DataType::Position_Score)     return "ps";
	if (d == DataType::Position_Score_PV)  return "psp";
	if (d == DataType::Position_FullScore) return "pfs";
}

const   signed char DATASET_DEFAULT_depth       = -1;
const unsigned char DATASET_DEFAULT_selectivity = 0;
const   signed char DATASET_DEFAULT_score       = SCHAR_MIN;
const unsigned char DATASET_DEFAULT_PV          = 64;


struct CPosition
{
	unsigned long long P, O;
	CPosition(unsigned long long P, unsigned long long O) : P(P), O(O) {}
	bool operator==(const CPosition& other) const { return (this->P == other.P) && (this->O == other.O); }
	bool operator!=(const CPosition& other) const { return (this->P != other.P) || (this->O != other.O); }
	bool operator<=(const CPosition& other) const { return (this->P <= other.P) || ((this->P == other.P) && (this->O <= other.O)); }
	bool operator>=(const CPosition& other) const { return (this->P >= other.P) || ((this->P == other.P) && (this->O >= other.O)); }
	bool operator< (const CPosition& other) const { return (this->P <  other.P) || ((this->P == other.P) && (this->O <  other.O)); }
	bool operator> (const CPosition& other) const { return (this->P >  other.P) || ((this->P == other.P) && (this->O >  other.O)); }
};


#pragma pack(1)

// .b
// 144 Bit = 18 Byte
class CDataset_Old
{
public:
	static const std::string FileNameEnding;
	static const DataType datatype = DataType::Old;
	unsigned long long P;		// 64 bit
	unsigned long long O;		// 64 bit
	signed char depth;			//  8 bit
	signed char score;			//  8 bit

	inline void Reset(){
		depth = DATASET_DEFAULT_depth;
		score = DATASET_DEFAULT_score;
	}

	CDataset_Old() : P(0), O(0), depth(DATASET_DEFAULT_depth), score(DATASET_DEFAULT_score) {}
	CDataset_Old(const unsigned long long P, const unsigned long long O, const signed char depth, const signed char score) : P(P), O(O), depth(depth), score(score) {}
};

// .ps
// 152 Bit = 19 Byte
class CDataset_Position_Score
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

	CDataset_Position_Score() : P(0), O(0), depth(DATASET_DEFAULT_depth), selectivity(DATASET_DEFAULT_selectivity), score(DATASET_DEFAULT_score) {}
	CDataset_Position_Score(const unsigned long long P, const unsigned long long O, const signed char depth, const unsigned char selectivity, const signed char score) : P(P), O(O), depth(depth), selectivity(selectivity), score(score) {}

	explicit CDataset_Position_Score(const CDataset_Old&                that);
	explicit CDataset_Position_Score(const CDataset_Position_Score_PV&  that);
	explicit CDataset_Position_Score(const CDataset_Position_FullScore& that);
};

// .psp
// 192 Bit = 24 Byte
class CDataset_Position_Score_PV
{
public:
	unsigned long long P;		// 64 bit
	unsigned long long O;		// 64 bit
	signed char depth;			//  8 bit
	unsigned char selectivity;	//  8 Bit
	signed char score;			//  8 bit
	unsigned char PV[5];		// 40 bit

	void Reset();

	CDataset_Position_Score_PV();
	CDataset_Position_Score_PV(const unsigned long long P, const unsigned long long O, const signed char depth, const unsigned char selectivity, const signed char score) : P(P), O(O), depth(depth), selectivity(selectivity), score(score) {}

	explicit CDataset_Position_Score_PV(const CDataset_Old&                that);
	explicit CDataset_Position_Score_PV(const CDataset_Position_Score&     that);
	explicit CDataset_Position_Score_PV(const CDataset_Position_FullScore& that);
};

// .pfs
// 1168 Bit = 146 Byte
class CDataset_Position_FullScore
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

	CDataset_Position_FullScore();
	CDataset_Position_FullScore(const unsigned long long P, const unsigned long long O, const signed char depth, const unsigned char selectivity);

	explicit CDataset_Position_FullScore(const CDataset_Old&               that);
	explicit CDataset_Position_FullScore(const CDataset_Position_Score&    that);
	explicit CDataset_Position_FullScore(const CDataset_Position_Score_PV& that);
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
void write_to_file(const std::string & filename, std::vector<T>& vector)
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

void read_vector(const std::string & filename, std::size_t size, std::vector<CPosition>& vector);


bool Test_datamanipulation();