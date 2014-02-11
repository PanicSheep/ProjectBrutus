#pragma once
#include <vector>
#include <atomic>
#include <emmintrin.h>
#include <limits.h>
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
		cost(BIT_SCAN_MS1B(NodeCounter)), depth(depth), selectivity(selectivity), alpha(alpha), beta(beta), PV(PV), AV(AV) {}
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
		m_P1(0), m_O1(0), m_P2(0), m_O2(0), m_date1(0), m_date2(0), m_value1(Value1), m_value2(Value2) { spinlock.clear(); }

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
	CHashTable(const int Bits) : Bits(Bits), BitShift(64-Bits), date(0) { m_table = new NodeType[1ULL << Bits]; }
	~CHashTable() { delete[] m_table; }
	inline void Update(const unsigned long long P, const unsigned long long O, const ValueType& Value) { m_table[Hash(P, O)].Update(P, O, Value, date); }
	inline bool LookUp(const unsigned long long P, const unsigned long long O, ValueType& Value) const { return m_table[Hash(P, O)].LookUp(P, O, Value); }
	inline void Refresh(const unsigned long long P, const unsigned long long O) { m_table[Hash(P, O)].Refresh(P, O, date); }
	inline void AdvanceDate() { date++; }
	void Clear(){
		for (std::size_t i = 0; i < (1ULL << Bits); ++i)
			m_table[i].Clear();
	}
	//void Save(const char * const filename) const { write_to_file(filename, m_table); }
	//void Load(const char * const filename) { read_vector(filename, m_table); }

private:
	NodeType* m_table;
	const int Bits;
	const int BitShift;
	unsigned char date;

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