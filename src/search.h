#pragma once
#include "game.h"
#include "hashtable.h"
#include "position.h"
#include "utility.h"
#include <string>

struct CSelectivity
{
	float T, percentile;
	CSelectivity(const float T, const float percentile) : T(T), percentile(percentile) {}
};

static const CSelectivity SelectivityTable[10] = {
	CSelectivity(99.f, 100.0f), // 0
	CSelectivity(3.3f,  99.9f), // 1
	CSelectivity(2.6f,  99.0f), // 2
	CSelectivity(2.0f,  95.0f), // 3
	CSelectivity(1.5f,  87.0f), // 4
	CSelectivity(1.2f,  77.0f), // 5
	CSelectivity(1.0f,  68.0f), // 6
	CSelectivity(0.7f,  52.0f), // 7
	CSelectivity(0.5f,  38.0f), // 8
	CSelectivity(0.2f,  16.0f), // 9
};

class CSearch
{
public:
	static const signed char END = SCHAR_MAX;
	enum NodeType { PV_Node, All_Node, Cut_Node };

	unsigned long long P;				//  64 Bit
	unsigned long long O;				//  64 Bit
	unsigned long long NodeCounter;		//  64 Bit
	signed char depth;					//   8 Bit
	unsigned char selectivity;			//   8 Bit
	signed char alpha;					//   8 Bit
	signed char beta;					//   8 Bit
	signed char score;					//   8 Bit
	CHashTable* hashTable;				//  64 Bit
	NodeType nodeType;					// Expected node type
	unsigned char PV[60];				// 480 Bit

	CSearch() : P(0), O(0), NodeCounter(0), depth(END), selectivity(0), alpha(-64), beta(64), score(-128), hashTable(0), nodeType(PV_Node) 
	{ memset(PV, static_cast<unsigned char>(64), 60*sizeof(unsigned char)); }

	CSearch(const unsigned long long P, const unsigned long long O, signed char alpha, signed char beta, CHashTable* hashTable)
		: P(P), O(O), NodeCounter(0), depth(END), selectivity(0), alpha(alpha), beta(beta), score(-128), hashTable(hashTable), nodeType(PV_Node) 
	{ memset(PV, static_cast<unsigned char>(64), 60*sizeof(unsigned char)); }

	CSearch(const unsigned long long P, const unsigned long long O, signed char alpha, signed char beta, const signed char depth, unsigned char selectivity, CHashTable* hashTable, NodeType nodeType)
		: P(P), O(O), NodeCounter(0), depth(depth), selectivity(selectivity), alpha(alpha), beta(beta), score(-128), hashTable(hashTable), nodeType(nodeType) 
	{ memset(PV, static_cast<unsigned char>(64), 60*sizeof(unsigned char)); }

	inline bool HashTableLookUp(const unsigned long long P, const unsigned long long O, HashTableValueType & HashValue) const { return hashTable->LookUp(P, O, HashValue); }
	void HashTableUpdate(const unsigned long long P, const unsigned long long O, const unsigned long long NodeCounter, signed char depth, unsigned char selectivity, signed char alpha, signed char beta, unsigned char PV, unsigned char AV) 
	{ hashTable->Update(P, O, HashTableValueType(NodeCounter,depth, selectivity, alpha, beta, PV, AV)); }
	const std::string GetPV(const int depth) const;
	void Evaluate();
};