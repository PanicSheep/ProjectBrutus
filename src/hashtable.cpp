#include "hashtable.h"

FORCE_INLINE void Upgrade(HashTableValueType& Value_db, const HashTableValueType& Value_new)
{
	if (Value_new.depth > Value_db.depth)
		Value_db = Value_new; // Replace
	else if (Value_new.depth == Value_db.depth && Value_new.selectivity < Value_db.selectivity)
		Value_db = Value_new; // Replace
	else if (Value_new.depth == Value_db.depth && Value_new.selectivity == Value_db.selectivity)
	{ // Update
		Value_db.cost = MAX(Value_db.cost, Value_new.cost); // ############ Not shure if this is the right thing to do.
		Value_db.alpha = MAX(Value_db.alpha, Value_new.alpha);
		Value_db.beta  = MIN(Value_db.beta, Value_new.beta);
		Value_db.PV = Value_new.PV;
		Value_db.AV = Value_new.AV;
	}
}

void CTwoNode::Update(const unsigned long long P, const unsigned long long O, const HashTableValueType& Value, const unsigned char date)
{
	static const unsigned char OldAge = 6;
	while (spinlock.test_and_set(std::memory_order_acquire)) ;

	if (P == m_P1 && O == m_O1){
		Upgrade(m_value1, Value);
		m_date1 = date;
	}
	else if (P == m_P2 && O == m_O2){
		Upgrade(m_value2, Value);
		m_date2 = date;
	}
	else if (Value.cost > m_value1.cost){
		m_P2 = m_P1;
		m_O2 = m_O1;
		m_value2 = m_value1;
		m_date2 = m_date1;

		m_P1 = P;
		m_O1 = O;
		m_value1 = Value;
		m_date1 = date;
	}
	else if (Value.cost > m_value2.cost){
		m_P2 = P;
		m_O2 = O;
		m_value2 = Value;
		m_date2 = date;
	}
	else
	{
		bool IsOld1 = (date - m_date1 >= OldAge);
		bool IsOld2 = (date - m_date2 >= OldAge);
		if (IsOld1 && IsOld2){
			m_P2 = m_P1;
			m_O2 = m_O1;
			m_value2 = m_value1;
			m_date2 = m_date1;

			m_P1 = P;
			m_O1 = O;
			m_value1 = Value;
			m_date1 = date;
		}
		else if (!IsOld1 && IsOld2){
			m_P2 = P;
			m_O2 = O;
			m_value2 = Value;
			m_date2 = date;
		}
		else if (IsOld1 && !IsOld2){
			m_P1 = m_P2;
			m_O1 = m_O2;
			m_value1 = m_value2;
			m_date1 = m_date2;
			
			m_P2 = P;
			m_O2 = O;
			m_value2 = Value;
			m_date2 = date;
		}
	}

	spinlock.clear(std::memory_order_release);
}

// The return value is stored in 'Value'
bool CTwoNode::LookUp(const unsigned long long P, const unsigned long long O, HashTableValueType& Value) const
{
	if ((P == m_P1 && O == m_O1) || (P == m_P2 && O == m_O2))
	{
		while (spinlock.test_and_set(std::memory_order_acquire)) ;

		if (P == m_P1 && O == m_O1)
		{
			Value = m_value1;

			spinlock.clear(std::memory_order_release);
			return true;
		}
		if (P == m_P2 && O == m_O2)
		{
			Value = m_value2;

			spinlock.clear(std::memory_order_release);
			return true;
		}
				
		spinlock.clear(std::memory_order_release);
	}
	return false;
}

void CTwoNode::Refresh(const unsigned long long P, const unsigned long long O, const unsigned char date)
{
	while (spinlock.test_and_set(std::memory_order_acquire)) ;

	if (P == m_P1 && O == m_O1)
		m_date1 = date;
	else if (P == m_P2 && O == m_O2)
		m_date2 = date;

	spinlock.clear(std::memory_order_release);
}

void CTwoNode::Clear()
{
	while (spinlock.test_and_set(std::memory_order_acquire)) ;

	m_value1 = HashTableValueType();
	m_value2 = HashTableValueType();

	spinlock.clear(std::memory_order_release);
}

void CHashTable::Clear(){
	UpdateCounter = 0;
	LookUpCounter = 0;
	RefreshCounter = 0;
	HitCounter = 0;
	for (std::size_t i = 0; i < (1ULL << Bits); i++) table[i].Clear();
}

void CHashTable::Save(const std::string & filename) const
{
	FILE* file;
	fopen_s(&file, filename.c_str(), "wb");
	if (!file){
		std::cerr << "ERROR: File '" << filename << "' could not be opened!" << std::endl;
		throw "File could not be opened.";
		return;
	}

	fwrite(table, sizeof(NodeType), 1ULL << Bits, file);

	fclose(file);
}

void CHashTable::Load(const std::string & filename)
{
	FILE* file;
	fopen_s(&file, filename.c_str(), "rb");
	if (!file){
		std::cerr << "ERROR: File '" << filename << "' could not be opened!" << std::endl;
		throw "File could not be opened.";
		return;
	}

	fread(table, sizeof(NodeType), 1ULL << Bits, file);

	fclose(file);
}

void CHashTable::print_stats(){
	printf("Updates: %d\nLookUps: %d\nRefreshs: %d\nHits: %d\nHit rate: %2.1f%%\n", UpdateCounter, LookUpCounter, RefreshCounter, HitCounter, static_cast<float>(HitCounter * 100) / static_cast<float>(LookUpCounter));
	uint64_t counter[3] = {0,0,0};
	int num;
	for (std::size_t i = 0; i < (1ULL << Bits); i++){
		num = 0;
		if ((table[i].m_P1 != 0) || (table[i].m_O1 != 0)) num++;
		if ((table[i].m_P2 != 0) || (table[i].m_O2 != 0)) num++;
		counter[num]++;
	}
	float total = counter[0] + counter[1] + counter[2];
	printf("Empty Nodes : %2.1f%%\n1 Entry Nodes : %2.1f%%\n2 Entry Nodes : %2.1f%%\n", counter[0] / total * 100, counter[1] / total * 100, counter[2] / total * 100);
}
