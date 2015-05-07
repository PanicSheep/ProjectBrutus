#pragma once
#include <cstdint>
#include <atomic>
#include <limits.h>
#include <iostream>
#include "datamanipulation.h"

class HashTableValueType
{
public:
	uint8_t cost;		//   8 Bit
	int8_t depth;		//   8 Bit
	uint8_t selectivity;	//   8 Bit
	int8_t alpha;		//   8 Bit
	int8_t beta;		//   8 Bit
	uint8_t PV;		//   8 Bit
	uint8_t AV;		//   8 Bit
				// -------
				//  56 Bit = 7 Byte

	HashTableValueType() : cost(0), depth(-1), selectivity(0), alpha(SCHAR_MIN), beta(SCHAR_MAX), PV(64), AV(64) {}
	HashTableValueType(const uint64_t NodeCounter, const int8_t depth, const uint8_t selectivity, const int8_t alpha, const int8_t beta, const uint8_t PV, const uint8_t AV) :
		cost(BitScanMSB(NodeCounter)), depth(depth), selectivity(selectivity), alpha(alpha), beta(beta), PV(PV), AV(AV) {}
};

CACHE_LINE_ALIGNMENT
class CTwoNode
{
public:
	uint64_t m_P1, m_O1, m_P2, m_O2;	// 256 Bit
	HashTableValueType m_value1, m_value2;	// 112 Bit
	uint8_t m_date1, m_date2;		//  16 Bit
protected:
	mutable std::atomic_flag spinlock;	//  64 Bit
						// -------
						// 448 Bit = 56 Byte
public:

	CTwoNode() : m_P1(0), m_O1(0), m_P2(0), m_O2(0), m_date1(0), m_date2(0) { spinlock.clear(); }
	CTwoNode(const uint64_t P1, const uint64_t O1, const HashTableValueType& Value1,
	         const uint64_t P2, const uint64_t O2, const HashTableValueType& Value2) :
		m_P1(0), m_O1(0), m_P2(0), m_O2(0), m_date1(0), m_date2(0), m_value1(Value1), m_value2(Value2) { spinlock.clear(); }

	void Update(const uint64_t P, const uint64_t O, const HashTableValueType& Value, const uint8_t date);
	bool LookUp(const uint64_t P, const uint64_t O, HashTableValueType& Value) const; // The return value is stored in 'Value'
	void Refresh(const uint64_t P, const uint64_t O, const uint8_t date);
	void Clear();
};


class CHashTable
{
	typedef CTwoNode NodeType;
	typedef HashTableValueType ValueType;
public:
	CHashTable(const unsigned int Bits) : Bits(Bits), BitShift(64 - Bits), date(0), UpdateCounter(0), LookUpCounter(0), RefreshCounter(0), HitCounter(0) { table = new NodeType[1ULL << Bits]; }
	CHashTable() : CHashTable(0) {}
	~CHashTable() { delete[] table; }
	inline void Update(const uint64_t P, const uint64_t O, const ValueType& Value) {
		UpdateCounter++;
		table[Hash(P, O)].Update(P, O, Value, date); 
	}
	inline bool LookUp(const uint64_t P, const uint64_t O, ValueType& Value) const {
		LookUpCounter++;
		bool b = table[Hash(P, O)].LookUp(P, O, Value);
		if (b) HitCounter++;
		return b;
	}
	inline void Refresh(const uint64_t P, const uint64_t O) {
		RefreshCounter++;
		table[Hash(P, O)].Refresh(P, O, date);
	}
	inline void AdvanceDate() { date++; }
	void Clear();
	void Save(const std::string & filename) const;
	void Load(const std::string & filename);
	void print_stats();

private:
	NodeType* table;
	const unsigned int Bits;
	const unsigned int BitShift;
	uint8_t date;
	mutable std::atomic<uint64_t> UpdateCounter, LookUpCounter, RefreshCounter, HitCounter;

	inline std::size_t Hash(uint64_t P, uint64_t O) const
	{
		P ^= P >> 33;
		O ^= O >> 33;
		P *= 0xFF14AFD7ED558CCDULL;
		O *= 0xFF14AFD7ED558CCDULL;
		O ^= O >> 33;
		return (P + O + (O << 41)) >> BitShift;
	}
};
