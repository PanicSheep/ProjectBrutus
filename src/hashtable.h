#pragma once
#include <vector>
#include <atomic>
#include <emmintrin.h>
#include <limits.h>
#include "datamanipulation.h"
#include "utility.h"

//#pragma pack(push)
//#pragma pack(16)
class HashTableValueType // 64 Bit = 8 Byte
{
public:
	unsigned char LogNodeCounter;		//   8 Bit
	unsigned char empties;				//   8 Bit
	signed char depth;					//   8 Bit
	unsigned char selectivity;			//   8 Bit
	signed char alpha;					//   8 Bit
	signed char beta;					//   8 Bit
	unsigned char PV;					//   8 Bit
	unsigned char AV;					//   8 Bit
										// -------
										//  64 Bit = 8 Byte

	HashTableValueType() {}
	HashTableValueType(unsigned long long NodeCounter, unsigned char empties, signed char depth, unsigned char selectivity, signed char alpha, signed char beta, unsigned char PV, unsigned char AV) :
		LogNodeCounter(BIT_SCAN_MS1B(NodeCounter)),
		empties(empties),
		depth(depth),
		selectivity(selectivity),
		alpha(alpha),
		beta(beta),
		PV(PV),
		AV(AV)
	{}
	HashTableValueType(unsigned long long NodeCounter, unsigned char empties, signed char depth, signed char alpha, signed char beta, unsigned char PV) :
		LogNodeCounter(BIT_SCAN_MS1B(NodeCounter)),
		empties(empties),
		depth(depth),
		selectivity(0),
		alpha(alpha),
		beta(beta),
		PV(PV),
		AV(64)
	{}
};
//#pragma pack(pop)

class CTwoNode
{
public:
	typedef __m128i KeyType;
	typedef HashTableValueType ValueType;

	__m128i m_Position1, m_Position2;					// 256 Bit
	unsigned char m_LogNodeCounter1, m_LogNodeCounter2;	//  16 Bit 
	unsigned char m_empties1, m_empties2;				//  16 Bit
	signed char m_depth1, m_depth2;						//  16 Bit
	unsigned char m_selectivity1, m_selectivity2;		//  16 Bit
	signed char m_alpha1, m_alpha2;						//  16 Bit
	signed char m_beta1, m_beta2;						//  16 Bit
	unsigned char m_PV1, m_PV2;							//  16 Bit
	unsigned char m_AV1, m_AV2;							//  16 Bit
protected:
	mutable std::atomic_flag spinlock;					//  64 Bit
														// -------
														// 448 Bit = 56 Byte

	inline static bool Equal(const __m128i Position1, const __m128i Position2) { return (((unsigned long long *)&Position1)[1] == ((unsigned long long *)&Position2)[1]) && (((unsigned long long *)&Position1)[0] == ((unsigned long long *)&Position2)[0]); }
public:

	CTwoNode() : 
		m_Position1(_mm_setzero_si128()), m_Position2(_mm_setzero_si128()),
		m_LogNodeCounter1(0), m_LogNodeCounter2(0),
		m_empties1(0), m_empties2(0),
		m_depth1(-1), m_depth2(-1),
		m_selectivity1(0), m_selectivity2(0),
		m_alpha1(SCHAR_MIN), m_alpha2(SCHAR_MIN),
		m_beta1(SCHAR_MAX), m_beta2(SCHAR_MAX),
		m_PV1(64), m_PV2(64),
		m_AV1(64), m_AV2(64)
	{ spinlock.clear(); }

	CTwoNode(const __m128i Position, const HashTableValueType& Value) : 
		m_Position1(Position), m_Position2(_mm_setzero_si128()),
		m_LogNodeCounter1(Value.LogNodeCounter), m_LogNodeCounter2(0),
		m_empties1(Value.empties), m_empties2(0),
		m_depth1(Value.depth), m_depth2(-1),
		m_selectivity1(Value.selectivity), m_selectivity2(0),
		m_alpha1(Value.alpha), m_alpha2(SCHAR_MIN),
		m_beta1(Value.beta), m_beta2(SCHAR_MAX),
		m_PV1(Value.PV), m_PV2(64),
		m_AV1(Value.AV), m_AV2(64)
	{ spinlock.clear(); }

	void Update(const __m128i Position, const HashTableValueType& Value);
	bool LookUp(const __m128i Position, HashTableValueType& Value) const; // The return value is stored in 'Value'
	void Clear();
};

class CTwoNodeBig : public CTwoNode
{
public:
	void Update(const __m128i Position, const HashTableValueType& Value);
};

class CTwoNodeDeep : public CTwoNode
{
public:
	void Update(const __m128i Position, const HashTableValueType& Value);
};

class CTwoNodeUpdate : public CTwoNode
{
public:
	void Update(const __m128i Position, const HashTableValueType& Value);
};


template <class NodeType, class KeyType, class ValueType, std::size_t TableSize, std::size_t(*HashFuntion)(const KeyType)>
class CHashTable
{
public:
	CHashTable() { m_table = std::vector<NodeType>(TableSize); }
	~CHashTable() { m_table.clear(); }
	inline void Insert(const KeyType& Key, const ValueType& Value){ m_table[Hash(Key)].Insert(Key, Value); }
	inline void Update(const KeyType& Key, const ValueType& Value){ m_table[Hash(Key)].Update(Key, Value); }
	inline bool LookUp(const KeyType& Key, ValueType& Value) const { return m_table[Hash(Key)].LookUp(Key, Value); }
	void Clear(){
		for (std::size_t i = 0; i < TableSize; ++i)
			m_table[i].Clear();
	}
	void Save(const char * const filename) const { write_to_file(filename, m_table); }

private:
	CACHE_LINE_ALIGNMENT std::vector<NodeType> m_table;

	inline std::size_t Hash(const KeyType& Key) const { return HashFuntion(Key) % TableSize; }
};

inline std::size_t HashFunction1(const __m128i Key) { return (((unsigned long long *)&Key)[0]) ^ (((unsigned long long *)&Key)[1] * 0x00001922AAAD1753ULL); }
inline std::size_t HashFunction2(const __m128i Key) { return (((unsigned long long *)&Key)[0] * 0x00002E3088666B27ULL) ^ (((unsigned long long *)&Key)[1]); }
inline std::size_t HashFunction3(const __m128i Key) { return (((unsigned long long *)&Key)[0] * 0x39B32994E45A2D83ULL) ^ (((unsigned long long *)&Key)[1]); }
