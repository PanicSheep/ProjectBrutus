#pragma once
#include <math.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "utility.h"

class CDataset_Old;					// .b
class CDataset_Position_Score;		// .ps
class CDataset_Position_Score_PV;	// .psp
class CDataset_Position_FullScore;	// .pfs
class CDataset_Edax;				// .obf
struct CPosition;
struct CPositionScore;

enum DataType { Old, Position_Score, Position_Score_PV, Position_FullScore, Edax, Position, PositionScore };

template <DataType datatype> struct CDataset_type { typedef void type; };
template <> struct CDataset_type<DataType::Old>                { typedef CDataset_Old                type; };
template <> struct CDataset_type<DataType::Position_Score>     { typedef CDataset_Position_Score     type; };
template <> struct CDataset_type<DataType::Position_Score_PV>  { typedef CDataset_Position_Score_PV  type; };
template <> struct CDataset_type<DataType::Position_FullScore> { typedef CDataset_Position_FullScore type; };
template <> struct CDataset_type<DataType::Edax>               { typedef CDataset_Edax               type; };
template <> struct CDataset_type<DataType::Position>           { typedef CPosition                   type; };
template <> struct CDataset_type<DataType::PositionScore>      { typedef CPositionScore              type; };

inline DataType Ending_to_DataType(const std::string & s){
	if (s == "b"  ) return DataType::Old;
	if (s == "ps" ) return DataType::Position_Score;
	if (s == "psp") return DataType::Position_Score_PV;
	if (s == "pfs") return DataType::Position_FullScore;
	if (s == "obf") return DataType::Edax;
}

inline std::string DataType_to_Ending(const DataType & d){
	if (d == DataType::Old)                return "b";
	if (d == DataType::Position_Score)     return "ps";
	if (d == DataType::Position_Score_PV)  return "psp";
	if (d == DataType::Position_FullScore) return "pfs";
	if (d == DataType::Edax)               return "obf";
}

const   signed char DATASET_DEFAULT_depth       = -1;
const   signed char DATASET_depth_END			= SCHAR_MAX;
const unsigned char DATASET_DEFAULT_selectivity = 0;
const   signed char DATASET_DEFAULT_score       = SCHAR_MIN;
const unsigned char DATASET_DEFAULT_PV          = 64;


#pragma pack(1)

class CPosition
{
public:
	unsigned long long P, O;
	
	CPosition() : P(0), O(0) {}
	CPosition(unsigned long long P, unsigned long long O) : P(P), O(O) {}

	inline bool operator==(const CPosition& other) const { return (this->P == other.P) && (this->O == other.O); }
	inline bool operator!=(const CPosition& other) const { return (this->P != other.P) || (this->O != other.O); }
	inline bool operator<=(const CPosition& other) const { return (this->P <= other.P) || ((this->P == other.P) && (this->O <= other.O)); }
	inline bool operator>=(const CPosition& other) const { return (this->P >= other.P) || ((this->P == other.P) && (this->O >= other.O)); }
	inline bool operator< (const CPosition& other) const { return (this->P <  other.P) || ((this->P == other.P) && (this->O <  other.O)); }
	inline bool operator> (const CPosition& other) const { return (this->P >  other.P) || ((this->P == other.P) && (this->O >  other.O)); }
};

class CPositionScore
{
public:
	unsigned long long P, O;
	signed char score;

	CPositionScore(unsigned long long P, unsigned long long O, signed char score) : P(P), O(O), score(score) {}

	explicit CPositionScore(const CDataset_Old&                that);
	explicit CPositionScore(const CDataset_Position_Score_PV&  that);
	explicit CPositionScore(const CDataset_Position_FullScore& that);
	explicit CPositionScore(const CDataset_Edax&               that);
};

// .b
// 144 Bit = 18 Byte
class CDataset_Old
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
	explicit CDataset_Position_Score(const CDataset_Edax&               that);
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

	inline void Reset(){
		depth = DATASET_DEFAULT_depth;
		selectivity = DATASET_DEFAULT_selectivity;
		score = DATASET_DEFAULT_score;
		for (int i = 0; i < 5; i++) PV[i] = DATASET_DEFAULT_PV;
	}

	CDataset_Position_Score_PV() : P(0), O(0) { Reset(); }
	CDataset_Position_Score_PV(const unsigned long long P, const unsigned long long O) : P(P), O(O) { Reset(); }

	explicit CDataset_Position_Score_PV(const CDataset_Old&                that);
	explicit CDataset_Position_Score_PV(const CDataset_Position_Score&     that);
	explicit CDataset_Position_Score_PV(const CDataset_Position_FullScore& that);
	explicit CDataset_Position_Score_PV(const CDataset_Edax&               that);
};

// .pfs
// 656 Bit = 82 Byte
class CDataset_Position_FullScore
{
public:
	unsigned long long P;		//  64 bit
	unsigned long long O;		//  64 bit
	signed char depth;			//   8 bit
	unsigned char selectivity;	//   8 Bit
	signed char score[64];		// 512 bit

	// score[27] != DATASET_DEFAULT_score <==> Game over.
	// score[36] != DATASET_DEFAULT_score <==> Only best move score aviable.

	inline void Reset(){
		depth = DATASET_DEFAULT_depth;
		selectivity = DATASET_DEFAULT_selectivity;
		for (int i = 0; i < 64; ++i) score[i] = DATASET_DEFAULT_score;
	}
	inline signed char BestScore() const {
		signed char ret = DATASET_DEFAULT_score;
		for (int i = 0; i < 64; ++i)
			if (score[i] > ret)
				ret = score[i];
		return ret;
	}
	inline unsigned char BestMove() const {
		signed char bestScore = BestScore();
		for (int i = 0; i < 64; ++i)
			if (score[i] == bestScore)
				return i;
		return DATASET_DEFAULT_PV;
	}

	CDataset_Position_FullScore() : P(0), O(0) { Reset(); }
	CDataset_Position_FullScore(const unsigned long long P, const unsigned long long O) : P(P), O(O) { Reset(); }

	explicit CDataset_Position_FullScore(const CDataset_Old&               that);
	explicit CDataset_Position_FullScore(const CDataset_Position_Score&    that);
	explicit CDataset_Position_FullScore(const CDataset_Position_Score_PV& that);
	explicit CDataset_Position_FullScore(const CDataset_Edax&              that);
};

// .obf
// 640 Bit = 80 Byte
class CDataset_Edax
{
public:
	unsigned long long P;	//  64 bit
	unsigned long long O;	//  64 bit
	signed char score[64];	// 512 bit
	
	inline void Reset() { for (int i = 0; i < 64; ++i) score[i] = DATASET_DEFAULT_score; }

	CDataset_Edax() : P(0), O(0) { Reset(); }
	CDataset_Edax(const unsigned long long P, const unsigned long long O) : P(P), O(O) { Reset(); }
	CDataset_Edax(std::string line);

	explicit CDataset_Edax(const CDataset_Old&                that) : P(that.P), O(that.O) { Reset(); }
	explicit CDataset_Edax(const CDataset_Position_Score&     that) : P(that.P), O(that.O) { Reset(); }
	explicit CDataset_Edax(const CDataset_Position_Score_PV&  that) : P(that.P), O(that.O) { Reset(); }
	explicit CDataset_Edax(const CDataset_Position_FullScore& that) : P(that.P), O(that.O) { for (int i = 0; i < 64; ++i) score[i] = that.score[i]; }

	signed char BestScore() const;
	unsigned char BestMove() const;
	std::string to_string() const;

private:
	static const std::string field[];
};

#pragma pack()


template<class T>
std::vector<T> read_vector2(const std::string & filename, std::size_t size)
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

template<> std::vector<CDataset_Edax > read_vector2(const std::string & filename, std::size_t size);
template<> std::vector<CPosition     > read_vector2(const std::string & filename, std::size_t size);
template<> std::vector<CPositionScore> read_vector2(const std::string & filename, std::size_t size);

template<class T>
std::vector<T> read_vector(const std::string & filename, std::size_t size = 0xFFFFFFFFFFFFFFFFULL) { return read_vector2<T>(filename, size); }


template<class T>
void write_to_file(const std::string & filename, const std::vector<T>& vector)
{
	FILE* file;
	fopen_s(&file, filename.c_str(), "wb");
	if (!file){
		std::cerr << "ERROR: File '" << filename << "' could not be opened!" << std::endl;
		throw "File could not be opened.";
		return;
	}

	for (const T& data : vector) fwrite(&data, sizeof(T), 1, file);

	fclose(file);
}

template<>
void write_to_file(const std::string & filename, const std::vector<CDataset_Edax>& vector);

bool Test_datamanipulation();