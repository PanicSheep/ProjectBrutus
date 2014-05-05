#include "positionmanager.h"

using namespace std;

void DisplayHistogram(const unsigned long long * const frequency)
{
	int min = 0;
	int max = 255;
	bool no_odds = false;

	//Find min
	for (int i = 0; i < 256; i++){
		if (frequency[i]){
			min = i;
			break;
		}
	}

	//Find max
	for (int i = 255; i >= 0; i--){
		if (frequency[i]){
			max = i;
			break;
		}
	}

	//If all values are in [-64, 64] and there are no odd values
	if ((min + SCHAR_MIN >= -64) && (max + SCHAR_MIN <= 64)){
		no_odds = true;
		for (int i = min; i <= max; ++i){
			if (((i + SCHAR_MIN) % 2 == 1) && frequency[i])
				no_odds = false;
		}
	}
	
	printf("Score|Frequency\n");
	printf("-----+---------\n");
	if (no_odds)
	{
		for (int score = -64; score <= 64; score += 2)
			printf(" %+2.2d |%9u\n", score, frequency[score - SCHAR_MIN]);
	}
	else
	{
		for (int score = min; score <= max; score++)
			printf(" %+2.2d |%9u\n", score + SCHAR_MIN, frequency[score]);
	}
}

void DisplayCharacteristics(const CRunningStatistic<signed char>& stats, const unsigned long long * const frequency)
{
	signed char D1, D9, Q1, Q3, median, mode;
	bool b_D1 = true;
	bool b_D9 = true;
	bool b_Q1 = true;
	bool b_Q3 = true;
	bool b_median = true;
	unsigned long long max_value = 0;
	unsigned long long counter;
	unsigned long long sum = 0;

	for (int i = 0; i < 256; ++i)
		sum += frequency[i];

	counter = 0;
	for (int i = 0; i < 256; ++i){
		counter += frequency[i];
		if (frequency[i] > max_value){
			max_value = frequency[i];
			mode = i;
		}
		if (b_D1 && (counter * 10 > sum)){
			D1 = i;
			b_D1 = false;
		}
		if (b_Q1 && (counter * 4 > sum)){
			Q1 = i;
			b_Q1 = false;
		}
		if (b_median && (counter * 2 > sum)){
			median = i;
			b_median = false;
		}
		if (b_Q3 && (counter * 4 > sum * 3)){
			Q3 = i;
			b_Q3 = false;
		}
		if (b_D9 && (counter * 10 > sum * 9)){
			D9 = i;
			b_D9 = false;
		}
	}

	cout << "arithmetic mean: " << stats.mu() << "\n"
		 << "standard deviation: " << stats.sigma() << "\n"
		 << "min: " << static_cast<int>(stats.Min()) << "\n"
		 << "lowest decile: " << static_cast<int>(D1) << "\n"
		 << "lower quartile: " << static_cast<int>(Q1) << "\n"
		 << "median: " << static_cast<int>(median) << "\n"
		 << "upper quartile: " << static_cast<int>(Q3) << "\n"
		 << "uppest decile: " << static_cast<int>(D9) << "\n"
		 << "max: " << static_cast<int>(stats.Max()) << "\n"
		 << "mode: " << static_cast<int>(mode) << "\n";
}

void Stats(const vector<string>& FileNames, bool b_Progress, bool b_Histogram, bool b_Characteristics)
{
	std::vector<CDataset_Old> tmp_OLD;
	std::vector<CDataset_Position_Score> tmp_POSITON_SCORE;
	std::vector<CDataset_Position_Score_PV> tmp_POSITON_SCORE_PV;
	std::vector<CDataset_Position_FullScore> tmp_POSITON_FULL_SCORE;
	CRunningStatistic<signed char> stats = CRunningStatistic<signed char>();
	unsigned long long depth[256];
	unsigned long long score[256];
	memset(depth, 0, 256 * sizeof(unsigned long long));
	memset(score, 0, 256 * sizeof(unsigned long long));

	for (auto& filename : FileNames)
	{
		std::string Ending = filename.substr(filename.rfind(".") + 1, filename.length());
		switch (Ending_to_DataType(Ending))
		{
		case DataType::Old:
			read_vector(filename, tmp_OLD);
			for (auto& item : tmp_OLD){
				depth[item.depth+1]++;
				score[item.score - SCHAR_MIN]++;
				stats.Add(item.score);
			}
			tmp_OLD.clear();
			break;
		case DataType::Position_Score:
			read_vector(filename, tmp_POSITON_SCORE);
			for (auto& item : tmp_POSITON_SCORE){
				depth[item.depth+1]++;
				score[item.score - SCHAR_MIN]++;
				stats.Add(item.score);
			}
			tmp_POSITON_SCORE.clear();
			break;
		case DataType::Position_Score_PV:
			read_vector(filename, tmp_POSITON_SCORE_PV);
			for (auto& item : tmp_POSITON_SCORE_PV){
				depth[item.depth+1]++;
				score[item.score - SCHAR_MIN]++;
				stats.Add(item.score);
			}
			tmp_POSITON_SCORE_PV.clear();
			break;
		case DataType::Position_FullScore:
			read_vector(filename, tmp_POSITON_FULL_SCORE);
			int maxscore = SCHAR_MIN;
			for (auto& item : tmp_POSITON_FULL_SCORE){
				for (int i = 0; i < 64; i++)
					maxscore = ((item.score[i] > maxscore) ? item.score[i] : maxscore);
				depth[item.depth+1]++;
				score[maxscore - SCHAR_MIN]++;
				stats.Add(maxscore);
			}
			tmp_POSITON_FULL_SCORE.clear();
			break;
		}
	}
	
	if (b_Progress){
		cout << "Progress:\n";
		for (int i = 0; i < 255; ++i){
			if (depth[i])
				cout << i-1 << "\t" << depth[i] << "\n";
		}
		if (depth[SCHAR_MAX+1])
			cout << "Exact\t" << depth[SCHAR_MAX+1] << "\n";
	}
	
	if (b_Histogram){
		DisplayHistogram(score);
		if (b_Characteristics) cout << "\n";
	}

	if (b_Characteristics)
		DisplayCharacteristics(stats, score);
}

void Progress(const string & filename)
{
	std::vector<CDataset_Old> tmp_OLD;
	std::vector<CDataset_Position_Score> tmp_POSITON_SCORE;
	std::vector<CDataset_Position_Score_PV> tmp_POSITON_SCORE_PV;
	std::vector<CDataset_Position_FullScore> tmp_POSITON_FULL_SCORE;
	std::string Ending = filename.substr(filename.rfind(".") + 1, filename.length());
	unsigned long long depth[256];
	memset(depth, 0, 256 * sizeof(unsigned long long));

	switch (Ending_to_DataType(Ending))
	{
	case DataType::Old:
		read_vector(filename, tmp_OLD);
		for (auto& item : tmp_OLD)
			depth[item.depth+1]++;
		tmp_OLD.clear();
		break;
	case DataType::Position_Score:
		read_vector(filename, tmp_POSITON_SCORE);
		for (auto& item : tmp_POSITON_SCORE)
			depth[item.depth+1]++;
		tmp_POSITON_SCORE.clear();
		break;
	case DataType::Position_Score_PV:
		read_vector(filename, tmp_POSITON_SCORE_PV);
		for (auto& item : tmp_POSITON_SCORE_PV)
			depth[item.depth+1]++;
		tmp_POSITON_SCORE_PV.clear();
		break;
	case DataType::Position_FullScore:
		read_vector(filename, tmp_POSITON_FULL_SCORE);
		for (auto& item : tmp_POSITON_FULL_SCORE)
			depth[item.depth+1]++;
		tmp_POSITON_FULL_SCORE.clear();
		break;
	}

	cout << "Progress of " << filename << ":\n";
	for (int i = 0; i <= SCHAR_MAX; ++i){
		if (depth[i])
			cout << i-1 << "\t" << depth[i] << "\n";
	}
	if (depth[SCHAR_MAX+1])
		cout << "Exact\t" << depth[SCHAR_MAX+1] << "\n";
}

void CountUnique(const string & filename)
{
	typedef pair<unsigned long long, unsigned long long> ULLULL;
	struct comp { inline bool operator()(const ULLULL & lhs, const ULLULL & rhs) { return (lhs.first < rhs.first) || ((lhs.first == rhs.first) && (lhs.second < rhs.second)); } };
	set<ULLULL, comp> PositionSet;
	std::vector<CDataset_Old> tmp_OLD;
	std::vector<CDataset_Position_Score> tmp_POSITON_SCORE;
	std::vector<CDataset_Position_Score_PV> tmp_POSITON_SCORE_PV;
	std::vector<CDataset_Position_FullScore> tmp_POSITON_FULL_SCORE;
	std::string Ending = filename.substr(filename.rfind(".") + 1, filename.length());

	switch (Ending_to_DataType(Ending))
	{
	case DataType::Old:
		read_vector(filename, tmp_OLD);
		for (auto& item : tmp_OLD)
			PositionSet.insert(ULLULL(item.P, item.O));
		tmp_OLD.clear();
		break;
	case DataType::Position_Score:
		read_vector(filename, tmp_POSITON_SCORE);
		for (auto& item : tmp_POSITON_SCORE)
			PositionSet.insert(ULLULL(item.P, item.O));
		tmp_POSITON_SCORE.clear();
		break;
	case DataType::Position_Score_PV:
		read_vector(filename, tmp_POSITON_SCORE_PV);
		for (auto& item : tmp_POSITON_SCORE_PV)
			PositionSet.insert(ULLULL(item.P, item.O));
		tmp_POSITON_SCORE_PV.clear();
		break;
	case DataType::Position_FullScore:
		read_vector(filename, tmp_POSITON_FULL_SCORE);
		for (auto& item : tmp_POSITON_FULL_SCORE)
			PositionSet.insert(ULLULL(item.P, item.O));
		tmp_POSITON_FULL_SCORE.clear();
		break;
	}
	cout << "Number of unique positions in " << filename << ":\t" << PositionSet.size() << endl;
}

void Reset(const string & filename)
{
	std::vector<CDataset_Old> tmp_OLD;
	std::vector<CDataset_Position_Score> tmp_POSITON_SCORE;
	std::vector<CDataset_Position_Score_PV> tmp_POSITON_SCORE_PV;
	std::vector<CDataset_Position_FullScore> tmp_POSITON_FULL_SCORE;
	std::string Ending = filename.substr(filename.rfind(".") + 1, filename.length());

	switch (Ending_to_DataType(Ending))
	{
	case DataType::Old:
		read_vector(filename, tmp_OLD);
		for (auto& item : tmp_OLD)
			item.Reset();
		write_to_file(filename, tmp_OLD);
		tmp_OLD.clear();
		break;
	case DataType::Position_Score:
		read_vector(filename, tmp_POSITON_SCORE);
		for (auto& item : tmp_POSITON_SCORE)
			item.Reset();
		write_to_file(filename, tmp_POSITON_SCORE);
		tmp_POSITON_SCORE.clear();
		break;
	case DataType::Position_Score_PV:
		read_vector(filename, tmp_POSITON_SCORE_PV);
		for (auto& item : tmp_POSITON_SCORE_PV)
			item.Reset();
		write_to_file(filename, tmp_POSITON_SCORE_PV);
		tmp_POSITON_SCORE_PV.clear();
		break;
	case DataType::Position_FullScore:
		read_vector(filename, tmp_POSITON_FULL_SCORE);
		for (auto& item : tmp_POSITON_FULL_SCORE)
			item.Reset();
		write_to_file(filename, tmp_POSITON_FULL_SCORE);
		tmp_POSITON_FULL_SCORE.clear();
		break;
	}
	cout << "Positions of " << filename << " resetted.\n";
}

void Mix(const string & filename)
{
	std::vector<CDataset_Old> tmp_OLD;
	std::vector<CDataset_Position_Score> tmp_POSITON_SCORE;
	std::vector<CDataset_Position_Score_PV> tmp_POSITON_SCORE_PV;
	std::vector<CDataset_Position_FullScore> tmp_POSITON_FULL_SCORE;
	std::string Ending = filename.substr(filename.rfind(".") + 1, filename.length());

	switch (Ending_to_DataType(Ending))
	{
	case DataType::Old:
		read_vector(filename, tmp_OLD);
		std::random_shuffle(tmp_OLD.begin(), tmp_OLD.end());
		write_to_file(filename, tmp_OLD);
		tmp_OLD.clear();
		break;
	case DataType::Position_Score:
		read_vector(filename, tmp_POSITON_SCORE);
		std::random_shuffle(tmp_POSITON_SCORE.begin(), tmp_POSITON_SCORE.end());
		write_to_file(filename, tmp_POSITON_SCORE);
		tmp_POSITON_SCORE.clear();
		break;
	case DataType::Position_Score_PV:
		read_vector(filename, tmp_POSITON_SCORE_PV);
		std::random_shuffle(tmp_POSITON_SCORE_PV.begin(), tmp_POSITON_SCORE_PV.end());
		write_to_file(filename, tmp_POSITON_SCORE_PV);
		tmp_POSITON_SCORE_PV.clear();
		break;
	case DataType::Position_FullScore:
		read_vector(filename, tmp_POSITON_FULL_SCORE);
		std::random_shuffle(tmp_POSITON_FULL_SCORE.begin(), tmp_POSITON_FULL_SCORE.end());
		write_to_file(filename, tmp_POSITON_FULL_SCORE);
		tmp_POSITON_FULL_SCORE.clear();
		break;
	}
	cout << "Positions of " << filename << " mixed.\n";
}

void Peek(const string & filename, const std::size_t start, const std::size_t end)
{
	std::vector<CDataset_Old> tmp_OLD;
	std::vector<CDataset_Position_Score> tmp_POSITON_SCORE;
	std::vector<CDataset_Position_Score_PV> tmp_POSITON_SCORE_PV;
	std::vector<CDataset_Position_FullScore> tmp_POSITON_FULL_SCORE;
	std::string Ending = filename.substr(filename.rfind(".") + 1, filename.length());

	cout << "Peek on " << filename << ":\n";
	switch (Ending_to_DataType(Ending))
	{
	case DataType::Old:
		printf("    #    |                            Position                            |depth|score\n");
		printf("---------+----------------------------------------------------------------+-----+-----\n");
		read_vector(filename, tmp_OLD);
		for (std::size_t i = start; i < end; i++){
			if (tmp_OLD[i].depth == SCHAR_MAX)
				printf("%9u|%64s| END | %+2.2d \n", i, board(tmp_OLD[i].P, tmp_OLD[i].O).c_str(), tmp_OLD[i].score);
			else
				printf("%9u|%64s| %3.3d | %+2.2d \n", i, board(tmp_OLD[i].P, tmp_OLD[i].O).c_str(), tmp_OLD[i].depth, tmp_OLD[i].score);
		}
		tmp_OLD.clear();
		break;
	case DataType::Position_Score:
		printf("    #    |                            Position                            |depth|selectivity|score\n");
		printf("---------+----------------------------------------------------------------+-----+-----------+-----\n");
		read_vector(filename, tmp_POSITON_SCORE);
		for (std::size_t i = start; i < end; i++){
			if (tmp_POSITON_SCORE[i].depth == SCHAR_MAX)
				printf("%9u|%64s| END |   %5s%%  | %+2.2d \n", i, board(tmp_POSITON_SCORE[i].P, tmp_POSITON_SCORE[i].O).c_str(), SelectivityTable[tmp_POSITON_SCORE[i].selectivity], tmp_POSITON_SCORE[i].score);
			else
				printf("%9u|%64s| %3.3d |   %5s%%  | %+2.2d \n", i, board(tmp_POSITON_SCORE[i].P, tmp_POSITON_SCORE[i].O).c_str(), tmp_POSITON_SCORE[i].depth, SelectivityTable[tmp_POSITON_SCORE[i].selectivity], tmp_POSITON_SCORE[i].score);
		}
		tmp_POSITON_SCORE.clear();
		break;
	case DataType::Position_Score_PV:
		printf("    #    |                            Position                            |depth|selectivity|score|       PV       \n");
		printf("---------+----------------------------------------------------------------+-----+-----------+-----+----------------\n");
		read_vector(filename, tmp_POSITON_SCORE_PV);
		for (std::size_t i = start; i < end; i++){
			if (tmp_POSITON_SCORE_PV[i].depth == SCHAR_MAX)
				printf("%9u|%64s| END |   %5s%%  | %+2.2d | %2s %2s %2s %2s %2s \n", i,
					board(tmp_POSITON_SCORE_PV[i].P, tmp_POSITON_SCORE_PV[i].O).c_str(),
					SelectivityTable[tmp_POSITON_SCORE_PV[i].selectivity],
					tmp_POSITON_SCORE_PV[i].score,
					FIELD_NAME(tmp_POSITON_SCORE_PV[i].PV[0]),
					FIELD_NAME(tmp_POSITON_SCORE_PV[i].PV[1]),
					FIELD_NAME(tmp_POSITON_SCORE_PV[i].PV[2]),
					FIELD_NAME(tmp_POSITON_SCORE_PV[i].PV[3]),
					FIELD_NAME(tmp_POSITON_SCORE_PV[i].PV[4]));
			else
				printf("%9u|%64s| %3.3d |   %5s%%  | %+2.2d | %2s %2s %2s %2s %2s \n", i, 
					board(tmp_POSITON_SCORE_PV[i].P, tmp_POSITON_SCORE_PV[i].O).c_str(),
					tmp_POSITON_SCORE_PV[i].depth, 
					SelectivityTable[tmp_POSITON_SCORE_PV[i].selectivity], 
					tmp_POSITON_SCORE_PV[i].score, 
					FIELD_NAME(tmp_POSITON_SCORE_PV[i].PV[0]),
					FIELD_NAME(tmp_POSITON_SCORE_PV[i].PV[1]), 
					FIELD_NAME(tmp_POSITON_SCORE_PV[i].PV[2]), 
					FIELD_NAME(tmp_POSITON_SCORE_PV[i].PV[3]), 
					FIELD_NAME(tmp_POSITON_SCORE_PV[i].PV[4]));
		}
		tmp_POSITON_SCORE_PV.clear();
		break;
	case DataType::Position_FullScore:
		printf("    #    |                            Position                            |depth|selectivity| move:score:PV \n");
		printf("---------+----------------------------------------------------------------+-----+-----------+---------------\n");
		read_vector(filename, tmp_POSITON_FULL_SCORE);
		for (std::size_t i = start; i < end; i++){
			if (tmp_POSITON_FULL_SCORE[i].depth == SCHAR_MAX)
				printf("%9u|%64s| END |   %5s%%  |", i, board(tmp_POSITON_FULL_SCORE[i].P, tmp_POSITON_FULL_SCORE[i].O).c_str(), SelectivityTable[tmp_POSITON_FULL_SCORE[i].selectivity]);
			else
				printf("%9u|%64s| %3.3d |   %5s%%  |", i, board(tmp_POSITON_FULL_SCORE[i].P, tmp_POSITON_FULL_SCORE[i].O).c_str(), tmp_POSITON_FULL_SCORE[i].depth, SelectivityTable[tmp_POSITON_FULL_SCORE[i].selectivity]);
			for (int j = 0; j < 64; j++)
				if (tmp_POSITON_FULL_SCORE[i].score[j] != SCHAR_MIN)
					printf(" %2s:%2.2d:%2s", FIELD_NAME(j), tmp_POSITON_FULL_SCORE[i].score[j], FIELD_NAME(tmp_POSITON_FULL_SCORE[i].PV[j]));
			printf("\n");
		}
		tmp_POSITON_FULL_SCORE.clear();
		break;
	}
}


void DepthFix(const string & filename)
{
	std::vector<CDataset_Old> tmp_OLD;
	std::vector<CDataset_Position_Score> tmp_POSITON_SCORE;
	std::vector<CDataset_Position_Score_PV> tmp_POSITON_SCORE_PV;
	std::vector<CDataset_Position_FullScore> tmp_POSITON_FULL_SCORE;
	std::string Ending = filename.substr(filename.rfind(".") + 1, filename.length());

	switch (Ending_to_DataType(Ending))
	{
	case DataType::Old:
		read_vector(filename, tmp_OLD);
		for (auto& item : tmp_OLD)
			item.depth = SCHAR_MAX;
		write_to_file(filename, tmp_OLD);
		tmp_OLD.clear();
		break;
	case DataType::Position_Score:
		read_vector(filename, tmp_POSITON_SCORE);
		for (auto& item : tmp_POSITON_SCORE)
			item.depth = SCHAR_MAX;
		write_to_file(filename, tmp_POSITON_SCORE);
		tmp_POSITON_SCORE.clear();
		break;
	case DataType::Position_Score_PV:
		read_vector(filename, tmp_POSITON_SCORE_PV);
		for (auto& item : tmp_POSITON_SCORE_PV)
			item.depth = SCHAR_MAX;
		write_to_file(filename, tmp_POSITON_SCORE_PV);
		tmp_POSITON_SCORE_PV.clear();
		break;
	case DataType::Position_FullScore:
		read_vector(filename, tmp_POSITON_FULL_SCORE);
		for (auto& item : tmp_POSITON_FULL_SCORE)
			item.depth = SCHAR_MAX;
		write_to_file(filename, tmp_POSITON_FULL_SCORE);
		tmp_POSITON_FULL_SCORE.clear();
		break;
	}
	cout << "Positions of " << filename << " fixed.\n";
}

int main(int argc, char* argv[])
{
	bool b_FileName = false;
	bool b_Progress = false;
	bool b_Histogram = false;
	bool b_Characteristics = false;
	bool b_Unique = false;
	bool b_Reset = false;
	bool b_Mix = false;
	bool b_Fix = false;
	bool b_Peek = false;
	bool b_New = false;
	bool b_Perft = false;
	int Empties;
	int N;
	int perft_depth;
	int peek_start, peek_num;
	vector<string> FileNames;

	for(int i = 0; i < argc; ++i) 
	{
		if (string(argv[i]) == "-f"){
			++i;
			while ((i < argc) && (static_cast<char>(*argv[i]) != '-'))
				FileNames.push_back(string(argv[i++]));
			--i;
			b_FileName = true;
		}
		else if (string(argv[i]) == "-progress")
			b_Progress = true;
		else if (string(argv[i]) == "-histogram")
			b_Histogram = true;
		else if (string(argv[i]) == "-char")
			b_Characteristics = true;
		else if (string(argv[i]) == "-uniques")
			b_Unique = true;
		else if (string(argv[i]) == "-mix")
			b_Mix = true;
		else if (string(argv[i]) == "-fix")
			b_Fix = true;
		else if (string(argv[i]) == "-peek"){
			b_Peek = true;
			peek_start = atoi(argv[++i]);
			peek_num = atoi(argv[++i]);
		}
		else if (string(argv[i]) == "-reset")
			b_Reset = true;
		else if (string(argv[i]) == "-perft"){
			b_Perft = true;
			perft_depth = atoi(argv[++i]);
		}
		else if (string(argv[i]) == "-new")
			b_New = true;
		else if (string(argv[i]) == "-e")
			Empties = atoi(argv[++i]);
		else if (string(argv[i]) == "-N")
			N = atoi(argv[++i]);
		else if (string(argv[i]) == "-h"){
			cout << "Calculates statistics for given file of positions.\n" <<
					"Arguments:\n" <<
					"-f\tFilenames.\n" <<
					"-progress\tShows the progress of solving.\n" <<
					"-histogram\tShows the histographic distribution.\n" <<
					"-char\tShows the characteristic parameters of the distribution.\n" <<
					"-uniques\tShows the number of unique positions.\n" <<
					"-mix\tMix the positions.\n" <<
					"-peek\tPeek on positions. -peek [start] [num]\n" <<
					"-reset\tResets the score and depth of all positions.\n" <<
					"-new\tGenerate new random positions using -e and -N.\n" <<
					"-e\tNumber of empty fields in each position.\n" <<
					"-N\tNumber of positions to generate.\n" <<
					"-h\tDisplays this help." << endl;
			return 0;
		}
	}

	if (!b_FileName){
		char buffer[1024];
		while (cin >> buffer)
			FileNames.push_back(string(buffer));
		b_FileName = (FileNames.size() != 0);
	}

	if (b_FileName)
	{
		if (b_Fix){
			for (const string & filename : FileNames)
				DepthFix(filename);
			return 0;
		}

		if (b_Mix){
			for (const string & filename : FileNames)
				Mix(filename);
			return 0;
		}

		if (b_Peek){
			for (const string & filename : FileNames)
				Peek(filename, peek_start, peek_num);
			return 0;
		}

		if (b_Reset){
			for (const string & filename : FileNames)
				Reset(filename);
			return 0;
		}

		if (b_New){
			for (const string & filename : FileNames)
				GenerateRandomPositions(filename, N, Empties, true);
			return 0;
		}

		if (b_Perft){
			for (const string & filename : FileNames)
				GeneratePerftPositions(filename, perft_depth, true);
			return 0;
		}

		if (b_Unique){
			for (const string & filename : FileNames)
				CountUnique(filename);
			return 0;
		}
		if (b_Progress && !b_Histogram && !b_Characteristics) //Only progress
		{
			for (const string & filename : FileNames)
				Progress(filename);
		}
		else
			Stats(FileNames, b_Progress, b_Histogram, b_Characteristics);
	}

	return 0;
}