#pragma once
#include <vector>
#include <atomic>
#include <emmintrin.h>
#include <limits.h>
#include <iostream>
#include "datamanipulation.h"
#include "utility.h"

class HashTableValueType
{
public:
	unsigned char cost;			//   8 Bit
	signed char depth;			//   8 Bit
	unsigned char selectivity;	//   8 Bit
	signed char alpha;			//   8 Bit
	signed char beta;			//   8 Bit
	unsigned char PV;			//   8 Bit
	unsigned char AV;			//   8 Bit
								// -------
								//  56 Bit = 7 Byte

	HashTableValueType() : cost(0), depth(-1), selectivity(0), alpha(SCHAR_MIN), beta(SCHAR_MAX), PV(64), AV(64) {}
	HashTableValueType(const unsigned long long NodeCounter, const signed char depth, const unsigned char selectivity, const signed char alpha, const signed char beta, const unsigned char PV, const unsigned char AV) :
		cost(BitScanMSB(NodeCounter)), depth(depth), selectivity(selectivity), alpha(alpha), beta(beta), PV(PV), AV(AV) {}
};

CACHE_LINE_ALIGNMENT
class CTwoNode
{
public:
	unsigned long long m_P1, m_O1, m_P2, m_O2;	// 256 Bit
	HashTableValueType m_value1, m_value2;		// 112 Bit
	unsigned char m_date1, m_date2;				//  16 Bit
protected:
	mutable std::atomic_flag spinlock;			//  64 Bit
	// -------
	// 448 Bit = 56 Byte
public:

	CTwoNode() : m_P1(0), m_O1(0), m_P2(0), m_O2(0), m_date1(0), m_date2(0) { spinlock.clear(); }
	CTwoNode(const unsigned long long P1, const unsigned long long O1, const HashTableValueType& Value1, const unsigned long long P2, const unsigned long long O2, const HashTableValueType& Value2) :
		m_P1(0), m_O1(0), m_P2(0), m_O2(0), m_date1(0), m_date2(0), m_value1(Value1), m_value2(Value2) {
		spinlock.clear();
	}

	void Update(const unsigned long long P, const unsigned long long O, const HashTableValueType& Value, const unsigned char date);
	bool LookUp(const unsigned long long P, const unsigned long long O, HashTableValueType& Value) const; // The return value is stored in 'Value'
	void Refresh(const unsigned long long P, const unsigned long long O, const unsigned char date);
	void Clear();
};


class CHashTable
{
	typedef CTwoNode NodeType;
	typedef HashTableValueType ValueType;
public:
	CHashTable(const unsigned int Bits) : Bits(Bits), BitShift(64 - Bits), date(0), LookUpCounter(0), UpdateCounter(0), HitCounter(0) { table = new NodeType[1ULL << Bits]; }
	~CHashTable() { delete[] table; }
	inline void Update(const unsigned long long P, const unsigned long long O, const ValueType& Value) {
		UpdateCounter++;
		table[Hash(P, O)].Update(P, O, Value, date); 
	}
	inline bool LookUp(const unsigned long long P, const unsigned long long O, ValueType& Value) const {
		LookUpCounter++;
		bool b = table[Hash(P, O)].LookUp(P, O, Value);
		if (b) HitCounter++;
		return b;
	}
	inline void Refresh(const unsigned long long P, const unsigned long long O) {
		RefreshCounter++;
		table[Hash(P, O)].Refresh(P, O, date);
	}
	inline void AdvanceDate() { date++; }
	void Clear(){
		UpdateCounter = 0;
		LookUpCounter = 0;
		RefreshCounter = 0;
		HitCounter = 0;
		for (std::size_t i = 0; i < (1ULL << Bits); i++)
			table[i].Clear();
	}
	void Save(const std::string & filename) const;
	void Load(const std::string & filename);
	void print_stats(){
		printf("Updates: %d\nLookUps: %d\nRefreshs: %d\nHits: %d\nHit rate: %2.1f%%\n", UpdateCounter, LookUpCounter, RefreshCounter, HitCounter, static_cast<float>(HitCounter * 100) / static_cast<float>(LookUpCounter));
		unsigned long long counter0 = 0;
		unsigned long long counter1 = 0;
		unsigned long long counter2 = 0;
		float total;
		int num;
		for (std::size_t i = 0; i < (1ULL << Bits); i++){
			num = 0;
			if ((table[i].m_P1 != 0) || (table[i].m_O1 != 0)) num++;
			if ((table[i].m_P2 != 0) || (table[i].m_O2 != 0)) num++;
			if (num == 0) counter0++;
			else if (num == 1) counter1++;
			else if (num == 2) counter2++;
		}
		total = counter0 + counter1 + counter2;
		printf("Empty Nodes : %2.1f%%\n1 Entry Nodes : %2.1f%%\n2 Entry Nodes : %2.1f%%\n", counter0 / total * 100, counter1 / total * 100, counter2 / total * 100);
	}

private:
	NodeType* table;
	const unsigned int Bits;
	const unsigned int BitShift;
	unsigned char date;
	mutable std::atomic<unsigned long long> UpdateCounter, LookUpCounter, RefreshCounter, HitCounter;

	inline std::size_t Hash(unsigned long long P, unsigned long long O) const
	{
		P ^= P >> 33;
		P *= 0xff51afd7ed558ccdULL;
		O ^= O >> 31;
		O += O << 17;
		O ^= O >> 23;
		O += O << 41;
		return (P + O) >> BitShift;
	}
};