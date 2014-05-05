#include "search.h"

void CSearch::print_header(const bool verbose)
{
	if (verbose)
	{
		printf("                            Position                            |  depth |score|       nodes       | PV\n");
		printf("----------------------------------------------------------------+--------+-----+-------------------|----\n");
	}
	else
	{
		printf("                            Position                            |  depth |score| PV\n");
		printf("----------------------------------------------------------------+--------+-----+----\n");
	}
}

void CSearch::print_stats(const signed char depth, const unsigned char selectivity, std::chrono::high_resolution_clock::time_point startTime)
{
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime);
	unsigned long long NpS;
	if (duration.count() == 0)
		NpS = 0;
	else
		NpS = NodeCounter * 1000LL / duration.count();
	
	printf("%+2.2d  depth: %2u@%5s%%   ", score, depth, SelectivityTable[selectivity].percentile.c_str());

	if (NodeCounter < 1000ULL) // < 1kN
		printf("%3u N", NodeCounter);
	else if (NodeCounter < 1000000ULL) // < 1MN
		printf("%3ukN", NodeCounter/1000ULL);
	else if (NodeCounter < 1000000000ULL) // < 1GN
		printf("%3uMN", NodeCounter/1000000ULL);
	else if (NodeCounter < 1000000000000ULL) // < 1TN
		printf("%3uGN", NodeCounter/1000000000ULL);
	else if (NodeCounter < 1000000000000000ULL) // < 1PN
		printf("%3uTN", NodeCounter/1000000000000ULL);
	else
		printf("%3uPN", NodeCounter/1000000000000000ULL);

	printf("  %14s  ", time_format(duration).c_str());
	
	if (NpS < 1000ULL) // < 1kN
		printf("%3u N/s", NpS);
	else if (NpS < 1000000ULL) // < 1MN
		printf("%3ukN/s", NpS/1000ULL);
	else if (NpS < 1000000000ULL) // < 1GN
		printf("%3uMN/s", NpS/1000000ULL);
	else
		printf("%3uGN/s", NpS/1000000000ULL);

	printf("  %s\n", GetPV().c_str());
}

void CSearch::print_result(const bool verbose)
{
	if (verbose)
		printf("%64s|%2u@%5s%%| %+2.2d |%19llu %s\n", board(P, O), depth, SelectivityTable[selectivity].percentile.c_str(), score, NodeCounter, GetPV().c_str());
	else
		printf("%64s|%2u@%5s%%| %+2.2d | %s\n", board(P, O), depth, SelectivityTable[selectivity].percentile.c_str(), score, GetPV().c_str());
}

int CSearch::EvaluateEnd(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const unsigned char selectivity, std::chrono::high_resolution_clock::time_point startTime)
{
	int score = ::EvaluateEnd(search, P, O, alpha, beta, selectivity);
	if (startTime != std::chrono::high_resolution_clock::time_point())
		CSearch::print_stats(depth, selectivity, startTime);
	return score;
}
int CSearch::EvaluateEnd(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const unsigned char selectivity, CLine & pline, std::chrono::high_resolution_clock::time_point startTime)
{
	int score = ::EvaluateEnd(search, P, O, alpha, beta, selectivity, pline);
	if (startTime != std::chrono::high_resolution_clock::time_point())
		CSearch::print_stats(depth, selectivity, startTime);
	return score;
}
int CSearch::EvaluateLimitedDepth(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const signed char depth, const unsigned char selectivity, std::chrono::high_resolution_clock::time_point startTime)
{
	int score = ::EvaluateLimitedDepth(search, P, O, alpha, beta, depth, selectivity);
	if (startTime != std::chrono::high_resolution_clock::time_point())
		CSearch::print_stats(depth, selectivity, startTime);
	return score;
}
int CSearch::EvaluateLimitedDepth(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const signed char depth, const unsigned char selectivity, CLine & pline, std::chrono::high_resolution_clock::time_point startTime)
{
	int score = ::EvaluateLimitedDepth(search, P, O, alpha, beta, depth, selectivity, pline);
	if (startTime != std::chrono::high_resolution_clock::time_point())
		CSearch::print_stats(depth, selectivity, startTime);
	return score;
}

void CSearch::Evaluate(std::chrono::high_resolution_clock::time_point startTime)
{
	const int Empties = NumberOfEmptyStones(P, O);

	if (depth >= Empties)
	{
		if (Empties < 11)
		{
			score = EvaluateEnd(*this, P, O, alpha, beta, selectivity, PV_line, startTime);
		}
		else
		{
			for (int d = (Empties % 2 ? 3 : 4); d <= Empties - 10; d+=2)
				score = EvaluateLimitedDepth(*this, P, O, alpha, beta, d, 6, PV_line, startTime);
			if (selectivity < 6)
				score = EvaluateEnd(*this, P, O, alpha, beta, 6, PV_line, startTime);
			score = EvaluateEnd(*this, P, O, alpha, beta, selectivity, PV_line, startTime);
		}
	}
	else
	{
		//EvaluateLimitedDepth(*this, depth, selectivity);
		for (int d = ((depth % 2) ? 3 : 4); d < depth; d+=2)
			score = EvaluateLimitedDepth(*this, P, O, alpha, beta, d, 6, PV_line, startTime);
		for (int s = 6; s >= selectivity; s-=2)
			score = EvaluateLimitedDepth(*this, P, O, alpha, beta, depth, s, PV_line, startTime);
	}
}