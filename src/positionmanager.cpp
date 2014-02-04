#include "positionmanager.h"

using namespace std;

void DisplayDistribution(const unsigned long long * const value)
{
	int min = 0;
	int max = 255;
	bool odd_zero = false;

	//Find min
	for (int i = 0; i < 256; ++i){
		if (value[i]){
			min = i;
			break;
		}
	}

	//Find max
	for (int i = 255; i >= 0; --i){
		if (value[i]){
			max = i;
			break;
		}
	}

	//If all values are in [-64, 64] and there are no odd values
	if ((min+SCHAR_MIN >= -64) && (max+SCHAR_MIN <= 64)){
		odd_zero = true;
		for (int i = min; i <= max; ++i){
			if (((i+SCHAR_MIN) % 2 == 1) && value[i])
				odd_zero = false;
		}
	}
	
	if (odd_zero)
	{
		cout << "Value distribution:" << endl;
		for (int i = -64; i <= 64; i+=2)
			cout << i << "\t" << value[i-SCHAR_MIN] << "\n";
	}
	else
	{
		cout << "Value distribution:" << endl;
		for (int i = min; i <= max; ++i)
			cout << i+SCHAR_MIN << "\t" << value[i] << "\n";
	}
}

void DisplayCharacteristics(const CRunningStatistic<signed char>& stats, const unsigned long long * const value)
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
		sum += value[i];

	counter = 0;
	for (int i = 0; i < 256; ++i){
		counter += value[i];
		if (value[i] > max_value){
			max_value = value[i];
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
		 << "min: " << static_cast<int>(stats.min()) << "\n"
		 << "lowest decile: " << static_cast<int>(D1) << "\n"
		 << "lower quartile: " << static_cast<int>(Q1) << "\n"
		 << "median: " << static_cast<int>(median) << "\n"
		 << "upper quartile: " << static_cast<int>(Q3) << "\n"
		 << "uppest decile: " << static_cast<int>(D9) << "\n"
		 << "max: " << static_cast<int>(stats.max()) << "\n"
		 << "mode: " << static_cast<int>(mode) << "\n";
}

void Stats(const vector<string>& FileNames, bool b_Progress, bool b_Distribution, bool b_Characteristics)
{
	const int N = 10000;
	DATASET_POSITON_SCORE DataArray[N];
	CRunningStatistic<signed char> stats = CRunningStatistic<signed char>();
	unsigned long long depth[256];
	unsigned long long value[256];
	memset(depth, 0, 256 * sizeof(unsigned long long));
	memset(value, 0, 256 * sizeof(unsigned long long));
	std::size_t ValidData;
	FILE* file;

	const vector<string>::const_iterator end = FileNames.cend();
	for (vector<string>::const_iterator it = FileNames.cbegin(); it != end; ++it)
	{
		fopen_s(&file, it->c_str(), "rb");
		while (ValidData = fread(DataArray, sizeof(DATASET_POSITON_SCORE), N, file))
		{
			for (int i = 0; i < ValidData; ++i){
				//Position = ReversiPosition(DataArray[i].BitBoardPlayer, DataArray[i].BitBoardOpponent);
				//if (!Position.PossibleMoves()){
				//	Position.PlayPass();
				//	if (Position.PossibleMoves()){
						/*if (b_Progress)*/ ++depth[DataArray[i].depth];
						/*if (b_Distribution || b_Characteristics)*/ ++value[DataArray[i].score - SCHAR_MIN];
						/*if (b_Characteristics)*/ stats.Add(DataArray[i].score);
				//	}
				//}
			}
		}
		fclose(file);
	}
	
	if (b_Progress){
		cout << "Progress:\n";
		for (int i = 0; i < 255; ++i){
			if (depth[i])
				cout << i << "\t" << depth[i] << "\n";
		}
		if (depth[255])
			cout << "Exact\t" << depth[255] << "\n";
		cout << "\n";
	}
	
	if (b_Distribution){
		DisplayDistribution(value);
		if (b_Characteristics) cout << "\n";
	}

	if (b_Characteristics)
		DisplayCharacteristics(stats, value);
}

void Progress(const char * const Filename)
{
	const int N = 10000;
	DATASET_POSITON_SCORE DataArray[N];
	unsigned long long depth[256];
	memset(depth, 0, 256 * sizeof(unsigned long long));
	std::size_t ValidData;
	FILE* file;
	fopen_s(&file, Filename, "rb");

	while (ValidData = fread(DataArray, sizeof(DATASET_POSITON_SCORE), N, file)){
		for (int i = 0; i < ValidData; ++i)
			++depth[DataArray[i].depth];
	}

	cout << "Progress of " << Filename << ":\n";
	for (int i = 0; i < 255; ++i){
		if (depth[i])
			cout << i << "\t" << depth[i] << "\n";
	}
	if (depth[255])
		cout << "Exact\t" << depth[255] << "\n";

	fclose(file);
}

void CountUnique(const char * const Filename)
{
	typedef pair<unsigned long long, unsigned long long> ULLULL;
	const int N = 10000;
	DATASET_POSITON_SCORE DataArray[N];
	std::size_t ValidData;
	struct comp{
		inline bool operator()(const ULLULL & lhs, const ULLULL & rhs){
			//return (lhs.first <= rhs.first) && (lhs.second < rhs.second);
			if (lhs.first < rhs.first)
				return true;
			else if (lhs.first == rhs.first)
				return lhs.second < rhs.second;
			else
				return false;
		}
	};
	set<ULLULL, comp> PositionSet;
	FILE* file;
	fopen_s(&file, Filename, "rb");

	while (ValidData = fread(DataArray, sizeof(DATASET_POSITON_SCORE), N, file)){
		for (int i = 0; i < ValidData; ++i)
			PositionSet.insert(ULLULL(DataArray[i].P, DataArray[i].O));
	}

	cout << "Unique positions in " << Filename << ":\t" << PositionSet.size() << endl;

	fclose(file);
}

void Reset(const char * const Filename)
{
	const int N = 10000;
	DATASET_POSITON_SCORE DataArray[N];
	std::size_t ValidData;
	fpos_t pos1, pos2;
	FILE* file;
	fopen_s(&file, Filename, "r+b");

	fgetpos(file, &pos1);
	while (ValidData = fread(DataArray, sizeof(DATASET_POSITON_SCORE), N, file))
	{
		for (int i = 0; i < ValidData; ++i)
			DataArray[i].Reset();
		fgetpos(file, &pos2);
		fsetpos(file, &pos1);
		fwrite(DataArray, sizeof(DATASET_POSITON_SCORE), ValidData, file);
		fsetpos(file, &pos2);
		fgetpos(file, &pos1);
	}

	cout << "Positions of " << Filename << " resetted.\n";

	fclose(file);
}

int main(int argc, char* argv[])
{
	bool b_FileName = false;
	bool b_Progress = false;
	bool b_Distribution = false;
	bool b_Unique = false;
	bool b_Reset = false;
	bool b_Characteristics = false;
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
		else if (string(argv[i]) == "-p")
			b_Progress = true;
		else if (string(argv[i]) == "-d")
			b_Distribution = true;
		else if (string(argv[i]) == "-u")
			b_Unique = true;
		else if (string(argv[i]) == "-r")
			b_Reset = true;
		else if (string(argv[i]) == "-c")
			b_Characteristics = true;
		else if (string(argv[i]) == "-all"){
			b_Progress = true;
			b_Distribution = true;
			b_Characteristics = true;
		}
		else if (string(argv[i]) == "-h"){
			cout << "Calculates statistics for given file of positions.\n" <<
					"Arguments:\n" <<
					"-f\tFilenames.\n" <<
					"-p\tShows the progress of solving.\n" <<
					"-d\tShows the histographic distribution.\n" <<
					"-u\tShows the number of unique positions.\n" <<
					"-r\tResets the score and depth of all positions.\n" <<
					"-c\tShows the characteristic parameters of the distribution.\n" << 
					"-all\tShows all stats.\n" <<
					"-h\tDisplays help." << endl;
			return 0;
		}
	}

	if (!b_FileName){
		char buffer[1024];
		while (cin >> buffer)
			FileNames.push_back(string(buffer));
		b_FileName = (FileNames.size() != 0);
	}

	if (b_FileName){
		if (b_Reset){
			const vector<string>::const_iterator end = FileNames.cend();
			for (vector<string>::const_iterator it = FileNames.cbegin(); it != end; ++it)
				Reset(it->c_str());
			return 0;
		}

		if (b_Unique){
			const vector<string>::const_iterator end = FileNames.cend();
			for (vector<string>::const_iterator it = FileNames.cbegin(); it != end; ++it)
				CountUnique(it->c_str());
			return 0;
		}
		if (b_Progress && !b_Distribution && !b_Characteristics) //Only progress
		{
			const vector<string>::const_iterator end = FileNames.cend();
			for (vector<string>::const_iterator it = FileNames.cbegin(); it != end; ++it)
				Progress(it->c_str());
		}
		else
			Stats(FileNames, b_Progress, b_Distribution, b_Characteristics);
	}

	return 0;
}