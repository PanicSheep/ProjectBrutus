#include "search.h"

std::string CSearch::DepthSelectivity(signed char depth, unsigned char selectivity)
{
	char buffer[8];
	int d = (depth == END ? NumberOfEmptyStones(P, O) : depth);

	if (selectivity)
		sprintf(buffer, "%2u@%3s", d, SelectivityTable[selectivity].percentile.c_str());
	else
		sprintf(buffer, "   %2u ", d);

	return std::string(buffer);
}

void CSearch::print_header(const int num_width, const bool verbose)
{
	int width_Nodes = std::stoi(ConfigFile::Configurations["output width nodes"]);
	int width_nps = std::stoi(ConfigFile::Configurations["output width nps"]);
	int width_PV = PV_line.size * 3 + 1;
	int width_PV_front = static_cast<int>((width_PV - 2) / 2.0f + 0.5f);
	std::string L1, L2;

	if (num_width)
	{
		L1.append(std::string(num_width - 2, ' ')).append("# |");
		L2.append(std::string(num_width, '-')).append("+");
	}

	if (verbose)
	{
		L1.append("                            Position                            |");
		L2.append("----------------------------------------------------------------+");
	}

	L1.append(" depth|score|       time (s) |");
	L2.append("------+-----+----------------+");

	L1.append(std::string(width_Nodes - 10 + 2, ' ')).append("nodes (N) |");
	L2.append(std::string(width_Nodes + 2, '-')).append("+");

	L1.append(std::string(width_nps - 8 + 2, ' ')).append("N/s     |");
	L2.append(std::string(width_nps + 2, '-')).append("+");

	L1.append(std::string(width_PV_front, ' ')).append("PV").append(std::string(width_PV - width_PV_front - 2, ' '));
	L2.append(std::string(width_PV, '-'));

	printf("%s\n%s\n", L1.c_str(), L2.c_str());
}

void CSearch::print_stats(CSearch & search, int score, const signed char depth, const unsigned char selectivity)
{
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime);
	unsigned long long NpS;
	if (duration.count() == 0)
		NpS = 0;
	else
		NpS = NodeCounter * 1000LL / duration.count();
	
	printf("%+2.2d  depth: %s   ", score, DepthSelectivity(depth, selectivity), SelectivityTable[selectivity].percentile.c_str());

	printf("%23sN", ThousandsSeparator(NodeCounter).c_str());
	//if (NodeCounter < 1000ULL) // < 1kN
	//	printf("%3u N", NodeCounter);
	//else if (NodeCounter < 1000000ULL) // < 1MN
	//	printf("%3ukN", NodeCounter/1000ULL);
	//else if (NodeCounter < 1000000000ULL) // < 1GN
	//	printf("%3uMN", NodeCounter/1000000ULL);
	//else if (NodeCounter < 1000000000000ULL) // < 1TN
	//	printf("%3uGN", NodeCounter/1000000000ULL);
	//else if (NodeCounter < 1000000000000000ULL) // < 1PN
	//	printf("%3uTN", NodeCounter/1000000000000ULL);
	//else
	//	printf("%3uPN", NodeCounter/1000000000000000ULL);

	printf("  %14s  ", time_format(duration).c_str());
	
	printf("%15sN/s", ThousandsSeparator(NpS).c_str());
	//if (NpS < 1000ULL) // < 1kN
	//	printf("%3u N/s", NpS);
	//else if (NpS < 1000000ULL) // < 1MN
	//	printf("%3ukN/s", NpS/1000ULL);
	//else if (NpS < 1000000000ULL) // < 1GN
	//	printf("%3uMN/s", NpS/1000000ULL);
	//else
	//	printf("%3uGN/s", NpS/1000000000ULL);

	printf("  %s\n", GetPV().c_str());
}

void CSearch::print_result(const int num_width, const int num, const bool verbose)
{
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	unsigned long long NpS;
	if (duration.count() == 0)
		NpS = 0;
	else
		NpS = NodeCounter * 1000LL / duration.count();
	int width_Nodes = std::stoi(ConfigFile::Configurations["output width nodes"]);
	int width_nps = std::stoi(ConfigFile::Configurations["output width nps"]);
	//int width_PV = PV_line.size * 3 + 1;
	//int width_PV_front = static_cast<int>((width_PV - 2) / 2.0f + 0.5f);
	//char buffer[256];
	//std::string s(' ', (num_width ? num_width + 1 : 0) + (verbose ? 64 + 1 : 0) + 6 + 1 + 5 + 1 + 16 + 1 + width_Nodes + 2 + 1 + width_nps + 2 + 1 + 3 * PV_line.size + 2);
	
	if (num_width)
	{
		if (verbose)
			printf("%*d|%s|%s| %+2.2d | %s | %*s | %*s | %s \n", num_width, num, board(P, O).c_str(), DepthSelectivity(depth, selectivity).c_str(), score, time_format(duration).c_str(), width_Nodes, ThousandsSeparator(NodeCounter).c_str(), width_nps, (duration.count() == 0 ? " " : ThousandsSeparator(NpS).c_str()), GetPV().c_str());
		else
			printf("%*d|%s| %+2.2d | %s | %*s | %*s | %s \n", num_width, num, DepthSelectivity(depth, selectivity).c_str(), score, time_format(duration).c_str(), width_Nodes, ThousandsSeparator(NodeCounter).c_str(), width_nps, (duration.count() == 0 ? " " : ThousandsSeparator(NpS).c_str()), GetPV().c_str());
	}
	else
	{
		if (verbose)
			printf("%s|%s| %+2.2d | %s | %*s | %*s | %s \n", board(P, O).c_str(), DepthSelectivity(depth, selectivity).c_str(), score, time_format(duration).c_str(), width_Nodes, ThousandsSeparator(NodeCounter).c_str(), width_nps, (duration.count() == 0 ? " " : ThousandsSeparator(NpS).c_str()), GetPV().c_str());
		else
			printf("%s| %+2.2d | %s | %*s | %*s | %s \n", DepthSelectivity(depth, selectivity).c_str(), score, time_format(duration).c_str(), width_Nodes, ThousandsSeparator(NodeCounter).c_str(), width_nps, (duration.count() == 0 ? " " : ThousandsSeparator(NpS).c_str()), GetPV().c_str());
	}

	//// num
	//if (num_width)
	//	s.append(std::string(num_width - std::to_string(num).length(), ' ')).append(std::to_string(num)).append("|");
	//
	//// baord
	//if (verbose)
	//	s.append(board(P, O)).append("|");

	//// depth
	//s.append(DepthSelectivity(depth, selectivity)).append("|");

	//// score
	//sprintf(buffer, " %+2.2d |", score);
	//s.append(buffer);

	//// time
	//s.append(" ").append(time_format(duration)).append(" |");

	//// nodes
	//std::string s_nodes = ThousandsSeparator(NodeCounter);
	//s.append(" ").append(std::string(width_Nodes - s_nodes.length(), ' ')).append(s_nodes).append(" |");

	//// N/s
	//if (duration.count() == 0)
	//	s.append(std::string(width_nps + 2, ' ')).append("|");
	//else
	//{
	//	std::string s_NpS = ThousandsSeparator(NpS);
	//	s.append(" ").append(std::string(width_nps - s_NpS.length(), ' ')).append(s_NpS).append(" |");
	//}

	//// PV
	//s.append(" ").append(GetPV()).append(" ");

	//return buffer;
}

//int CSearch::EvaluateEnd(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const unsigned char selectivity, std::chrono::high_resolution_clock::time_point startTime)
//{
//	if (search.TestTimeOut())
//		return alpha;
//	int score = ::EvaluateEnd(search, P, O, alpha, beta, selectivity);
//	if (startTime != std::chrono::high_resolution_clock::time_point())
//		CSearch::print_stats(search, score, depth, selectivity, startTime);
//	return score;
//}
//int CSearch::EvaluateEnd(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const unsigned char selectivity, CLine & pline, std::chrono::high_resolution_clock::time_point startTime)
//{
//	if (search.TestTimeOut())
//		return alpha;
//	int score = ::EvaluateEnd(search, P, O, alpha, beta, selectivity, pline);
//	if (startTime != std::chrono::high_resolution_clock::time_point())
//		CSearch::print_stats(search, score, depth, selectivity, startTime);
//	return score;
//}
//int CSearch::EvaluateLimitedDepth(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const signed char depth, const unsigned char selectivity, std::chrono::high_resolution_clock::time_point startTime)
//{
//	if (search.TestTimeOut())
//		return alpha;
//	int score = ::EvaluateLimitedDepth(search, P, O, alpha, beta, depth, selectivity);
//	if (startTime != std::chrono::high_resolution_clock::time_point())
//		CSearch::print_stats(search, score, depth, selectivity, startTime);
//	return score;
//}
//int CSearch::EvaluateLimitedDepth(CSearch & search, const unsigned long long P, const unsigned long long O, const int alpha, const int beta, const signed char depth, const unsigned char selectivity, CLine & pline, std::chrono::high_resolution_clock::time_point startTime)
//{
//	if (search.TestTimeOut())
//		return alpha;
//	int score = ::EvaluateLimitedDepth(search, P, O, alpha, beta, depth, selectivity, pline);
//	if (startTime != std::chrono::high_resolution_clock::time_point())
//		CSearch::print_stats(search, score, depth, selectivity, startTime);
//	return score;
//}

void CSearch::Evaluate(const bool verbose)
{
	startTime = std::chrono::high_resolution_clock::now();
	const int Empties = NumberOfEmptyStones(P, O);

	if (depth >= Empties)
	{
		if (Empties < 11)
		{
			score = EvaluateEnd(*this, P, O, alpha, beta, selectivity, PV_line);
			if (verbose) print_stats(*this, score, depth, selectivity);
			if (TestTimeOut()) return;
		}
		else
		{
			for (int d = (Empties % 2 ? 3 : 4); d <= Empties - 10; d += 2){
				score = EvaluateLimitedDepth(*this, P, O, alpha, beta, d, 6, PV_line);
				if (verbose) print_stats(*this, score, d, 6);
				if (TestTimeOut()) return;
			}
			for (int s = 6; s >= selectivity; s -= 2){
				score = EvaluateEnd(*this, P, O, alpha, beta, s, PV_line);
				if (verbose) print_stats(*this, score, depth, s);
				if (TestTimeOut()) return;
			}
		}
	}
	else
	{
		for (int d = ((depth % 2) ? 3 : 4); d < depth; d += 2){
			score = EvaluateLimitedDepth(*this, P, O, alpha, beta, d, 6, PV_line);
			if (verbose) print_stats(*this, score, d, 6);
			if (TestTimeOut()) return;
		}
		for (int s = 6; s >= selectivity; s -= 2){
			score = EvaluateLimitedDepth(*this, P, O, alpha, beta, depth, s, PV_line);
			if (verbose) print_stats(*this, score, depth, s);
			if (TestTimeOut()) return;
		}
	}

	endTime = std::chrono::high_resolution_clock::now();
	if (verbose) print_stats(*this, score, depth, selectivity);
}

void CSearch::EvaluateDirect(const bool verbose)
{
	startTime = std::chrono::high_resolution_clock::now();
	const int Empties = NumberOfEmptyStones(P, O);

	if (depth >= Empties)
	{
		score = EvaluateEnd(*this, P, O, alpha, beta, selectivity, PV_line);
		if (verbose) print_stats(*this, score, depth, selectivity);
		if (TestTimeOut()) return;
	}
	else
	{
		score = EvaluateLimitedDepth(*this, P, O, alpha, beta, depth, selectivity, PV_line);
		if (verbose) print_stats(*this, score, depth, selectivity);
		if (TestTimeOut()) return;
	}

	endTime = std::chrono::high_resolution_clock::now();
	if (verbose) print_stats(*this, score, depth, selectivity);
}

void CSearch::EvaluateIterativeDeepening(const bool verbose)
{
	startTime = std::chrono::high_resolution_clock::now();
	const int Empties = NumberOfEmptyStones(P, O);

	if (depth >= Empties)
	{
		for (int d = (Empties % 2 ? 3 : 4); d <= Empties - 10; d += 2){
			score = EvaluateLimitedDepth(*this, P, O, alpha, beta, d, selectivity, PV_line);
			if (verbose) print_stats(*this, score, d, selectivity);
			if (TestTimeOut()) return;
		}
		score = EvaluateEnd(*this, P, O, alpha, beta, selectivity, PV_line);
	}
	else
	{
		for (int d = ((depth % 2) ? 3 : 4); d <= depth; d += 2){
			score = EvaluateLimitedDepth(*this, P, O, alpha, beta, d, selectivity, PV_line);
			if (verbose) print_stats(*this, score, d, selectivity);
			if (TestTimeOut()) return;
		}
	}

	endTime = std::chrono::high_resolution_clock::now();
	if (verbose) print_stats(*this, score, depth, selectivity);
}