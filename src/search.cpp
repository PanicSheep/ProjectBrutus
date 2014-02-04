#include "search.h"

std::pair<bool, HashTableValueType> CSearch::HashTableLookUp(const unsigned long long P, const unsigned long long O) const
{
	HashTableValueType HashValue;
	bool tmp = /*(hashTablePV->LookUp(_mm_set_epi64x(P, O), HashValue))
			|| (hashTableL1->LookUp(_mm_set_epi64x(P, O), HashValue))
			|| */(hashTableL2->LookUp(_mm_set_epi64x(P, O), HashValue));
	return std::pair<bool, HashTableValueType>(tmp, HashValue);
}

void CSearch::HashTableUpdate(const unsigned long long P, const unsigned long long O, const unsigned long long NodeCounter, unsigned char empties, signed char depth, unsigned char selectivity, signed char alpha, signed char beta, unsigned char PV, unsigned char AV, bool b_PV, bool b_L1, bool b_L2)
{
	//if (b_PV)
	//	hashTablePV->Update(_mm_set_epi64x(P, O), HashTableValueType(NodeCounter, empties, depth, selectivity, alpha, beta, PV, AV));
	//if (b_L1)
	//	hashTableL1->Update(_mm_set_epi64x(P, O), HashTableValueType(NodeCounter, empties, depth, selectivity, alpha, beta, PV, AV));
	//if (b_L2)
		hashTableL2->Update(_mm_set_epi64x(P, O), HashTableValueType(NodeCounter, empties, depth, selectivity, alpha, beta, PV, AV));
}

const char * const CSearch::GetPV(const int depth) const
{
	if ((PV[depth] >= 0) && (PV[depth] <= 64))
		return FIELD_NAME(PV[depth]);
	else
		return "??";
}

void CSearch::Evaluate()
{
	if (depth == CSearch::END){ // end game
		if (selectivity == 0) // exact
			EvaluateExact(*this);
		else // selective
			EvaluateEndCut(*this);
	}
	else{ // mid game
		if (selectivity == 0) // exact
			EvaluateLimitedDepth(*this);
		else // selective
			EvaluateMPC(*this);
	}
}