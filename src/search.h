#pragma once
#include "game.h"
#include "hashtable.h"
#include "line.h"
#include "position.h"
#include "selectivity.h"
#include "statistics.h"
#include "utility.h"

class CSearch
{
public:
	static const signed char END = SCHAR_MAX;
	enum NodeType { PV_Node, All_Node, Cut_Node };

	unsigned long long P;									  //  64 Bit
	unsigned long long O;									  //  64 Bit
	unsigned long long NodeCounter;							  //  64 Bit
	std::chrono::high_resolution_clock::time_point startTime; //  64 Bit
	std::chrono::high_resolution_clock::time_point endTime;	  //  64 Bit
	CHashTable* hashTable;									  //  64 Bit
	signed char alpha;										  //   8 Bit
	signed char beta;										  //   8 Bit
	signed char depth;										  //   8 Bit
	unsigned char selectivity;								  //   8 Bit
	signed char score;										  //   8 Bit
	bool stop;												  //   8 Bit
	int probCutLevel = 0;
	                                                          // -------
	                                                          //  Bit
	NodeType nodeType;
	CLine PV_line;
	
	CSearch(unsigned long long P, unsigned long long O, signed char alpha, signed char beta, signed char depth, unsigned char selectivity, std::chrono::high_resolution_clock::time_point endTime, CHashTable* hashTable, unsigned char PV_depth) :
		P(P), O(O),
		NodeCounter(0),
		startTime(std::chrono::high_resolution_clock::now()),
		endTime(endTime),
		hashTable(hashTable),
		alpha(alpha),
		beta(beta),
		depth(depth),
		selectivity(selectivity),
		score(-128),
		stop(false),
		nodeType(nodeType),
		PV_line(CLine(PV_depth))
	{}

	CSearch(unsigned long long P, unsigned long long O, signed char alpha, signed char beta, std::chrono::milliseconds duration, CHashTable* hashTable, unsigned char PV_depth)
		: CSearch(P, O, alpha, beta, END, 0, std::chrono::high_resolution_clock::now() + duration, hashTable, PV_depth) {}

	// 10 years time
	CSearch(unsigned long long P, unsigned long long O, signed char alpha, signed char beta, signed char depth, unsigned char selectivity, CHashTable* hashTable, unsigned char PV_depth)
		: CSearch(P, O, alpha, beta, depth, selectivity, std::chrono::high_resolution_clock::now() + std::chrono::hours(24 * 365 * 10), hashTable, PV_depth) {}
	
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
			stop = other.stop;
			nodeType = other.nodeType;
			PV_line = other.PV_line;
		}
		return *this;
	}

	inline bool TestTimeOut() { return stop |= (std::chrono::high_resolution_clock::now() > endTime); }

	inline bool HashTableLookUp(const unsigned long long P, const unsigned long long O, HashTableValueType & HashValue) const { return hashTable->LookUp(P, O, HashValue); }
	void HashTableUpdate(const unsigned long long P, const unsigned long long O, const unsigned long long NodeCounter, signed char depth, unsigned char selectivity, signed char alpha, signed char beta, unsigned char PV, unsigned char AV) 
	{ hashTable->Update(P, O, HashTableValueType(NodeCounter,depth, selectivity, alpha, beta, PV, AV)); }
	inline const unsigned char PV(const int index) { return PV_line.line[index]; }
	inline const std::string GetPV() const { return PV_line.GetPV(); }
	inline const std::string GetPV(const int depth) const { return PV_line.GetPV(depth); }
	inline const std::string GetPV(const int StartDepth, const int num) const { return PV_line.GetPV(StartDepth, num); }
	std::string DepthSelectivity(signed char depth, unsigned char selectivity);
	void print_header(const int num_width, const bool verbose);
	void print_result(const int num_width, const int num, const bool verbose);
	//int EvaluateEnd(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const unsigned char selectivity, std::chrono::high_resolution_clock::time_point startTime);
	//int EvaluateEnd(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const unsigned char selectivity, CLine & pline, std::chrono::high_resolution_clock::time_point startTime);
	//int EvaluateLimitedDepth(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const signed char depth, const unsigned char selectivity, std::chrono::high_resolution_clock::time_point startTime);
	//int EvaluateLimitedDepth(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const signed char depth, const unsigned char selectivity, CLine & pline, std::chrono::high_resolution_clock::time_point startTime);
	void Evaluate(const bool verbose = false);
	void EvaluateDirect(const bool verbose = false);
	void EvaluateIterativeDeepening(const bool verbose = false);
private:
	void print_stats(CSearch & search, int score, const signed char depth, const unsigned char selectivity);
};