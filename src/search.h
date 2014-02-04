#pragma once
#include "endgame.h"
#include "hashtable.h"
#include "position.h"
#include "utility.h"

class CSearch
{
public:
	static const signed char END = SCHAR_MAX;
	typedef CHashTable<CTwoNodeUpdate, CTwoNodeUpdate::KeyType, CTwoNodeUpdate::ValueType, 65521, HashFunction1> HashTablePV;
	typedef CHashTable<CTwoNodeDeep, CTwoNodeDeep::KeyType, CTwoNodeDeep::ValueType, 65521, HashFunction2> HashTableL1;
	typedef CHashTable<CTwoNodeUpdate, CTwoNodeUpdate::KeyType, CTwoNodeUpdate::ValueType, 1048573, HashFunction3> HashTableL2;
	enum NodeType { PV_Node, All_Node, Cut_Node };

	unsigned long long P;				//  64 Bit
	unsigned long long O;				//  64 Bit
	unsigned long long NodeCounter;		//  64 Bit
	signed char depth;					//   8 Bit
	unsigned char selectivity;			//   8 Bit
	signed char alpha;					//   8 Bit
	signed char beta;					//   8 Bit
	signed char score;					//   8 Bit
	HashTablePV* hashTablePV;			//  64 Bit
	HashTableL1* hashTableL1;			//  64 Bit
	HashTableL2* hashTableL2;			//  64 Bit
	NodeType nodeType;					// Expected node type
	unsigned char PV[60];				// 480 Bit

	CSearch() : P(0), O(0), NodeCounter(0), depth(END), selectivity(0), alpha(-64), beta(64), score(-128), hashTablePV(0), hashTableL1(0), hashTableL2(0), nodeType(PV_Node) 
	{ memset(PV, static_cast<unsigned char>(64), 60*sizeof(unsigned char)); }

	CSearch(const unsigned long long P, const unsigned long long O, signed char alpha, signed char beta, HashTablePV* hashTablePV, HashTableL1* hashTableL1, HashTableL2* hashTableL2)
		: P(P), O(O), NodeCounter(0), depth(END), selectivity(0), alpha(alpha), beta(beta), score(-128), hashTablePV(hashTablePV), hashTableL1(hashTableL1), hashTableL2(hashTableL2), nodeType(PV_Node) 
	{ memset(PV, static_cast<unsigned char>(64), 60*sizeof(unsigned char)); }

	CSearch(const unsigned long long P, const unsigned long long O, signed char alpha, signed char beta, const signed char depth, unsigned char selectivity, HashTablePV* hashTablePV, HashTableL1* hashTableL1, HashTableL2* hashTableL2, NodeType nodeType)
		: P(P), O(O), NodeCounter(0), depth(depth), selectivity(selectivity), alpha(alpha), beta(beta), score(-128), hashTablePV(hashTablePV), hashTableL1(hashTableL1), hashTableL2(hashTableL2), nodeType(nodeType) 
	{ memset(PV, static_cast<unsigned char>(64), 60*sizeof(unsigned char)); }

	std::pair<bool, HashTableValueType> HashTableLookUp(const unsigned long long P, const unsigned long long O) const;
	void HashTableUpdate(const unsigned long long P, const unsigned long long O, const unsigned long long NodeCounter, unsigned char empties, signed char depth, unsigned char selectivity, signed char alpha, signed char beta, unsigned char PV, unsigned char AV, bool b_PV, bool b_L1, bool b_L2);
	const char * const GetPV(const int depth) const;
	void Evaluate();
};