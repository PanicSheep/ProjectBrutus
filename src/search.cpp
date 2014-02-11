#include "search.h"

std::pair<bool, HashTableValueType> CSearch::HashTableLookUp(const unsigned long long P, const unsigned long long O) const
{
	HashTableValueType HashValue;
	bool tmp = hashTable->LookUp(P, O, HashValue);
	return std::pair<bool, HashTableValueType>(tmp, HashValue);
}

void CSearch::HashTableUpdate(const unsigned long long P, const unsigned long long O, const unsigned long long NodeCounter, signed char depth, unsigned char selectivity, signed char alpha, signed char beta, unsigned char PV, unsigned char AV)
{
	hashTable->Update(P, O, HashTableValueType(NodeCounter,depth, selectivity, alpha, beta, PV, AV));
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