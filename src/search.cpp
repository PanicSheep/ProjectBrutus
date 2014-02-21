#include "search.h"

const std::string CSearch::GetPV(const int depth) const
{
	if ((PV[depth] >= 0) && (PV[depth] <= 64))
		return FIELD_NAME(PV[depth]);
	else
		return "??";
}

void CSearch::Evaluate()
{
	if (depth >= NumberOfEmptyStones(P, O))
		EvaluateExact(*this);
	else
		EvaluateLimitedDepth(*this);
}