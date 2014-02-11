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
	static const unsigned char OldAge = 2;
	while (spinlock.test_and_set(std::memory_order_acquire)) ;

	if (P == m_P1 && O == m_O1){
		Upgrade(m_value1, Value);
		m_date1 = date;
	}
	else if (P == m_P2 && O == m_O2){
		Upgrade(m_value2, Value);
		m_date2 = date;
	}
	else if (Value.cost >= m_value1.cost){
		m_P2 = m_P1;
		m_O2 = m_O1;
		m_value2 = m_value1;
		m_date2 = m_date1;

		m_P1 = P;
		m_O1 = O;
		m_value1 = Value;
		m_date1 = date;
	}
	else if (Value.cost >= m_value2.cost){
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