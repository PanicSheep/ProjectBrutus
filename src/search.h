#pragma once
#include "game.h"
#include "hashtable.h"
#include "line.h"
#include "position.h"
#include "utility.h"
#include <string>

struct CSelectivity
{
	float T;
	std::string percentile;
	CSelectivity(const float T, const std::string percentile) : T(T), percentile(percentile) {}
};

static const CSelectivity SelectivityTable[10] = {
	CSelectivity(99.f, "100.0"), // 0
	CSelectivity(3.3f, " 99.9"), // 1
	CSelectivity(2.6f, " 99.0"), // 2
	CSelectivity(2.0f, " 95.0"), // 3
	CSelectivity(1.5f, " 87.0"), // 4
	CSelectivity(1.2f, " 77.0"), // 5
	CSelectivity(1.0f, " 68.0"), // 6
	CSelectivity(0.7f, " 52.0"), // 7
	CSelectivity(0.5f, " 38.0"), // 8
	CSelectivity(0.2f, " 16.0"), // 9
};

class CSearch
{
public:
	static const signed char END = SCHAR_MAX;
	enum NodeType { PV_Node, All_Node, Cut_Node };

	unsigned long long P;									//  64 Bit
	unsigned long long O;									//  64 Bit
	unsigned long long NodeCounter;							//  64 Bit
	std::chrono::high_resolution_clock::time_point endTime;	//  64 Bit
	CHashTable* hashTable;									//  64 Bit
	signed char alpha;										//   8 Bit
	signed char beta;										//   8 Bit
	signed char depth;										//   8 Bit
	unsigned char selectivity;								//   8 Bit
	signed char score;										//   8 Bit
	                                                        // -------
	                                                        // 368 Bit
	NodeType nodeType;
	CLine PV_line;

	CSearch(unsigned long long P, unsigned long long O, signed char alpha, signed char beta, signed char depth, unsigned char selectivity, std::chrono::high_resolution_clock::time_point endTime, CHashTable* hashTable, unsigned char PV_depth) : 
		P(P), O(O),
		NodeCounter(0),
		endTime(endTime),
		hashTable(hashTable),
		alpha(alpha),
		beta(beta),
		depth(depth),
		selectivity(selectivity),
		score(-128),
		nodeType(nodeType),
		PV_line(CLine(PV_depth))
	{}

	CSearch(unsigned long long P, unsigned long long O, signed char alpha, signed char beta, signed char depth, unsigned char selectivity, std::chrono::seconds duration, CHashTable* hashTable, unsigned char PV_depth)
		: CSearch(P,O,alpha,beta, depth, selectivity, std::chrono::high_resolution_clock::time_point() + duration, hashTable, PV_depth) {}

	// 10 years time
	CSearch(unsigned long long P, unsigned long long O, signed char alpha, signed char beta, signed char depth, unsigned char selectivity, CHashTable* hashTable, unsigned char PV_depth)
		: CSearch(P,O,alpha,beta, depth, selectivity, std::chrono::high_resolution_clock::time_point() + std::chrono::hours(24 * 365 * 10), hashTable, PV_depth) {}
	
	// Exact Evaluation
	CSearch(const unsigned long long P, const unsigned long long O, signed char alpha, signed char beta, CHashTable* hashTable, unsigned char PV_depth)
		: CSearch(P, O, alpha, beta, END, 0, hashTable, PV_depth) {}

	// Exact Evaluation
	CSearch(const unsigned long long P, const unsigned long long O, signed char alpha, signed char beta, CHashTable* hashTable)
		: CSearch(P, O, alpha, beta, END, 0, hashTable, 1) {}

	CSearch() : CSearch(0, 0, -64, 64, 0) {}

	CSearch(const CSearch & o) : CSearch(o.P, o.O, o.alpha, o.beta, o.depth, o.selectivity, o.endTime, o.hashTable, o.PV_line.size) {}

	CSearch & operator=(const CSearch & other)
	{
		if (this != &other)
		{
			P = other.P;
			O = other.O;
			NodeCounter = other.NodeCounter;
			endTime = other.endTime;
			hashTable = other.hashTable;
			alpha = other.alpha;
			beta = other.beta;
			depth = other.depth;
			selectivity = other.selectivity;
			score = other.score;
			nodeType = other.nodeType;
			PV_line = other.PV_line;
		}
		return *this;
	}

	inline bool HashTableLookUp(const unsigned long long P, const unsigned long long O, HashTableValueType & HashValue) const { return hashTable->LookUp(P, O, HashValue); }
	void HashTableUpdate(const unsigned long long P, const unsigned long long O, const unsigned long long NodeCounter, signed char depth, unsigned char selectivity, signed char alpha, signed char beta, unsigned char PV, unsigned char AV) 
	{ hashTable->Update(P, O, HashTableValueType(NodeCounter,depth, selectivity, alpha, beta, PV, AV)); }
	inline const unsigned char PV(const int index) { return PV_line.line[index]; }
	inline const std::string GetPV() const { return PV_line.GetPV(); }
	inline const std::string GetPV(const int depth) const { return PV_line.GetPV(depth); }
	inline const std::string GetPV(const int StartDepth, const int num) const { return PV_line.GetPV(StartDepth, num); }
	void print_header(const bool verbose);
	void print_result(const bool verbose);
	int EvaluateEnd(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const unsigned char selectivity, std::chrono::high_resolution_clock::time_point startTime);
	int EvaluateEnd(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const unsigned char selectivity, CLine & pline, std::chrono::high_resolution_clock::time_point startTime);
	int EvaluateLimitedDepth(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const signed char depth, const unsigned char selectivity, std::chrono::high_resolution_clock::time_point startTime);
	int EvaluateLimitedDepth(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const signed char depth, const unsigned char selectivity, CLine & pline, std::chrono::high_resolution_clock::time_point startTime);
	void Evaluate(std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::time_point());
private:
	void print_stats(const signed char depth, const unsigned char selectivity, std::chrono::high_resolution_clock::time_point startTime);
};