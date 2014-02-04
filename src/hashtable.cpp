#include "hashtable.h"

bool CTwoNode::LookUp(const __m128i Position, HashTableValueType& Value) const // The return value is stored in 'Value'
{
	if (Equal(Position, m_Position1) || Equal(Position, m_Position2))
	{
		while (spinlock.test_and_set(std::memory_order_acquire)) ;

		if (Equal(Position, m_Position1))
		{
			Value.LogNodeCounter = m_LogNodeCounter1;
			Value.empties = m_empties1;
			Value.depth = m_depth1;
			Value.selectivity = m_selectivity1;
			Value.alpha = m_alpha1;
			Value.beta = m_beta1;
			Value.PV = m_PV1;
			Value.AV = m_AV1;

			spinlock.clear(std::memory_order_release);
			return true;
		}
		if (Equal(Position, m_Position2))
		{
			Value.LogNodeCounter = m_LogNodeCounter2;
			Value.empties = m_empties2;
			Value.depth = m_depth2;
			Value.selectivity = m_selectivity2;
			Value.alpha = m_alpha2;
			Value.beta = m_beta2;
			Value.PV = m_PV2;
			Value.AV = m_AV2;

			spinlock.clear(std::memory_order_release);
			return true;
		}
				
		spinlock.clear(std::memory_order_release);
	}
	return false;
}

void CTwoNode::Clear()
{
	while (spinlock.test_and_set(std::memory_order_acquire)) ;

	m_Position1 = _mm_setzero_si128();
	m_Position2 = _mm_setzero_si128();
	m_LogNodeCounter1 = 0;
	m_LogNodeCounter2 = 0;
	m_empties1 = 0;
	m_empties2 = 0;
	m_depth1 = -1;
	m_depth2 = -1;
	m_selectivity1 = 0;
	m_selectivity2 = 0;
	m_alpha1 = SCHAR_MIN;
	m_alpha2 = SCHAR_MIN;
	m_beta1 = SCHAR_MAX;
	m_beta2 = SCHAR_MAX;
	m_PV1 = 64;
	m_PV2 = 64;
	m_AV1 = 64;
	m_AV2 = 64;

	spinlock.clear(std::memory_order_release);
}

void CTwoNodeBig::Update(const __m128i Position, const HashTableValueType& Value)
{
	while (spinlock.test_and_set(std::memory_order_acquire)) ;
		
	if (Value.LogNodeCounter >= m_LogNodeCounter1)
	{
		m_Position2 = m_Position1;
		m_LogNodeCounter2 = m_LogNodeCounter1;
		m_empties2 = m_empties1;
		m_depth2 = m_depth1;
		m_selectivity2 = m_selectivity1;
		m_alpha2 = m_alpha1;
		m_beta2 = m_beta1;
		m_PV2 = m_PV1;
		m_AV2 = m_AV1;

		m_Position1 = Position;
		m_LogNodeCounter1 = Value.LogNodeCounter;
		m_empties1 = Value.empties;
		m_depth1 = Value.depth;
		m_selectivity1 = Value.selectivity;
		m_alpha1 = Value.alpha;
		m_beta1 = Value.beta;
		m_PV1 = Value.PV;
		m_AV1 = Value.AV;
	}
	else if (Value.LogNodeCounter > m_LogNodeCounter2)
	{
		m_Position2 = Position;
		m_LogNodeCounter2 = Value.LogNodeCounter;
		m_empties2 = Value.empties;
		m_depth2 = Value.depth;
		m_selectivity2 = Value.selectivity;
		m_alpha2 = Value.alpha;
		m_beta2 = Value.beta;
		m_PV2 = Value.PV;
		m_AV2 = Value.AV;
	}

	spinlock.clear(std::memory_order_release);
}

void CTwoNodeDeep::Update(const __m128i Position, const HashTableValueType& Value)
{
	while (spinlock.test_and_set(std::memory_order_acquire)) ;
		
	if ((Value.depth > m_depth1) || 
	   ((Value.depth == m_depth1) && (Value.selectivity < m_selectivity1)))
	{
		m_Position2 = m_Position1;
		m_LogNodeCounter2 = m_LogNodeCounter1;
		m_empties2 = m_empties1;
		m_depth2 = m_depth1;
		m_selectivity2 = m_selectivity1;
		m_alpha2 = m_alpha1;
		m_beta2 = m_beta1;
		m_PV2 = m_PV1;
		m_AV2 = m_AV1;

		m_Position1 = Position;
		m_LogNodeCounter1 = Value.LogNodeCounter;
		m_empties1 = Value.empties;
		m_depth1 = Value.depth;
		m_selectivity1 = Value.selectivity;
		m_alpha1 = Value.alpha;
		m_beta1 = Value.beta;
		m_PV1 = Value.PV;
		m_AV1 = Value.AV;
	}
	else if ((Value.depth > m_depth2) || 
			((Value.depth == m_depth2) && (Value.selectivity < m_selectivity2)))
	{
		m_Position2 = Position;
		m_LogNodeCounter2 = Value.LogNodeCounter;
		m_empties2 = Value.empties;
		m_depth2 = Value.depth;
		m_selectivity2 = Value.selectivity;
		m_alpha2 = Value.alpha;
		m_beta2 = Value.beta;
		m_PV2 = Value.PV;
		m_AV2 = Value.AV;
	}

	spinlock.clear(std::memory_order_release);
}

void CTwoNodeUpdate::Update(const __m128i Position, const HashTableValueType& Value)
{
	while (spinlock.test_and_set(std::memory_order_acquire)) ;
		
	if ((Value.depth > m_depth1) || 
	   ((Value.depth == m_depth1) && (Value.selectivity < m_selectivity1)))
	{
		if (Equal(Position, m_Position1))
		{
			m_LogNodeCounter1 = Value.LogNodeCounter;
			m_empties1 = Value.empties;
			m_depth1 = Value.depth;
			m_selectivity1 = Value.selectivity;
			m_alpha1 = Value.alpha;
			m_beta1 = Value.beta;
			m_PV1 = Value.PV;
			m_AV1 = Value.AV;
		}
		else if (Equal(Position, m_Position2))
		{
			m_LogNodeCounter2 = m_LogNodeCounter1;
			m_empties2 = m_empties1;
			m_depth2 = m_depth1;
			m_selectivity2 = m_selectivity1;
			m_alpha2 = m_alpha1;
			m_beta2 = m_beta1;
			m_PV2 = m_PV1;
			m_AV2 = m_AV1;

			m_LogNodeCounter2 = Value.LogNodeCounter;
			m_empties2 = Value.empties;
			m_depth2 = Value.depth;
			m_selectivity2 = Value.selectivity;
			m_alpha2 = Value.alpha;
			m_beta2 = Value.beta;
			m_PV2 = Value.PV;
			m_AV2 = Value.AV;
		}
		else
		{
			m_Position2 = m_Position1;
			m_LogNodeCounter2 = m_LogNodeCounter1;
			m_empties2 = m_empties1;
			m_depth2 = m_depth1;
			m_selectivity2 = m_selectivity1;
			m_alpha2 = m_alpha1;
			m_beta2 = m_beta1;
			m_PV2 = m_PV1;
			m_AV2 = m_AV1;

			m_Position1 = Position;
			m_LogNodeCounter1 = Value.LogNodeCounter;
			m_empties1 = Value.empties;
			m_depth1 = Value.depth;
			m_selectivity1 = Value.selectivity;
			m_alpha1 = Value.alpha;
			m_beta1 = Value.beta;
			m_PV1 = Value.PV;
			m_AV1 = Value.AV;
		}
	}
	else if ((Value.depth > m_depth2) || 
			((Value.depth == m_depth2) && (Value.selectivity < m_selectivity2)))
	{
		m_Position2 = Position;
		m_LogNodeCounter2 = Value.LogNodeCounter;
		m_empties2 = Value.empties;
		m_depth2 = Value.depth;
		m_selectivity2 = Value.selectivity;
		m_alpha2 = Value.alpha;
		m_beta2 = Value.beta;
		m_PV2 = Value.PV;
		m_AV2 = Value.AV;
	}

	spinlock.clear(std::memory_order_release);
}

//void CTwoNode::Update(const __m128i Position, const HashTableValueType& Value)
//{
//	while (spinlock.test_and_set(std::memory_order_acquire)) ;
//		
//	if ((Value.empties > m_empties1) || 
//		((Value.empties == m_empties1) && (Value.depth > m_depth1)) || 
//		((Value.empties == m_empties1) && (Value.depth == m_depth1) && (Value.selectivity < m_selectivity1)))
//	{
//		m_LogNodeCounter2 = m_LogNodeCounter1;
//		m_empties2 = m_empties1;
//		m_depth2 = m_depth1;
//		m_selectivity2 = m_selectivity1;
//		m_alpha2 = m_alpha1;
//		m_beta2 = m_beta1;
//		m_PV2 = m_PV1;
//		m_AV2 = m_AV1;
//
//		m_LogNodeCounter1 = Value.LogNodeCounter;
//		m_empties1 = Value.empties;
//		m_depth1 = Value.depth;
//		m_selectivity1 = Value.selectivity;
//		m_alpha1 = Value.alpha;
//		m_beta1 = Value.beta;
//		m_PV1 = Value.PV;
//		m_AV1 = Value.AV;
//	}
//	else if ((Value.empties > m_empties2) || 
//			((Value.empties == m_empties2) && (Value.depth > m_depth2)) || 
//			((Value.empties == m_empties2) && (Value.depth == m_depth2) && (Value.selectivity < m_selectivity2)))
//	{
//		m_LogNodeCounter2 = Value.LogNodeCounter;
//		m_empties2 = Value.empties;
//		m_depth2 = Value.depth;
//		m_selectivity2 = Value.selectivity;
//		m_alpha2 = Value.alpha;
//		m_beta2 = Value.beta;
//		m_PV2 = Value.PV;
//		m_AV2 = Value.AV;
//	}
//
//	spinlock.clear(std::memory_order_release);
//}