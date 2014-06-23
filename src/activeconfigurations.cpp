#include "activeconfigurations.h"

using namespace std;

void Load_Matrix(const vector<string>& file_matrix, vector<CDataset_Position_Score>& A)
{
	const vector<string>::const_iterator end = file_matrix.cend();
	for (vector<string>::const_iterator it = file_matrix.cbegin(); it != end; ++it)
		read_vector(it->c_str(), A);
}

void CalculateActiveConfigurations(const vector<string> file_matrix)
{
	std::chrono::high_resolution_clock::time_point startTime, endTime;
	std::chrono::high_resolution_clock::time_point OverallstartTime, OverallendTime;
	vector<CDataset_Position_Score> A;
	vector<int> Appearances(Features::Size);
	vector<int> local_Appearances;
	int Array[Features::Symmetries];
	int max = 0;

	printf("Matrices:\n");
	for (auto it = file_matrix.cbegin(); it != file_matrix.cend(); ++it)
		printf("\t%s\n", it->c_str());

	printf("Loading matrices...");
	startTime = std::chrono::high_resolution_clock::now();
	Load_Matrix(file_matrix, A);
	endTime = std::chrono::high_resolution_clock::now();
	printf("done!\t\t\t %14s\n", time_format(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime)).c_str());
	
	const int size = A.size();

	OverallstartTime = std::chrono::high_resolution_clock::now();
	#pragma omp parallel private(Array, local_Appearances)
	{
		int local_max = 0;
		local_Appearances = vector<int>(Features::Size);

		#pragma omp for nowait schedule(static, 1000)
		for (int i = 0; i < size; ++i)
		{
			FillConfigurationArray(A[i].P, A[i].O, Array);

			for (int j = 0; j < Features::Symmetries; ++j)
				local_Appearances[Array[j]]++;
		}

		#pragma omp critical
		{
			for (int i = 0; i < Features::Size; ++i)
				Appearances[i] += local_Appearances[i];
		}

		#pragma omp barrier

		#pragma omp for nowait schedule(static, 1000)
		for (int i = 0; i < Features::Size; ++i)
			if (Appearances[i] > local_max)
				local_max = Appearances[i];

		#pragma omp critical
		{
			if (local_max > max)
				max = local_max;
		}
	}

	int * counter = new int[max+1];
	memset(counter, 0, sizeof(int) * (max+1));
	for (int i = 0; i < Features::Size; ++i)
		counter[Appearances[i]]++;

	printf("Configurations: %d\n", Features::Size);
	printf("Max: %d\n\n", max);
	printf(" Threshold | Active Configs | Percentage \n");
	printf("-----------+----------------+------------\n");
	int sum = 0;
	for (int i = max; i >= 0; --i)
	{
		if ((static_cast<int>(100.0 * sum / Features::Size) != static_cast<int>(100.0 * (sum + counter[i]) / Features::Size)) || (i <= 50))
			printf(" % 9d | % 14d | % 9.2f%%\n", i, sum, 100.0 * (sum + counter[i]) / Features::Size);
		sum += counter[i];
	}
	OverallendTime = std::chrono::high_resolution_clock::now();
	printf("\nOverall time: %14s\n", time_format(std::chrono::duration_cast<std::chrono::milliseconds>(OverallendTime - OverallstartTime)).c_str());

	delete[] counter;
}

void Print_help()
{
	cout << endl << "Calculates the numer of active configurations for all thresholds." << endl
				 << "Attributes:" << endl
				 << "-f\tsolved position files" << endl
				 << "-h\thelp" << endl << endl;
}

int main(int argc, char* argv[])
{
	bool b_FileName = false;
	vector<string> FileNames;

	for(int i = 0; i < argc; ++i) 
	{
		if (string(argv[i]) == "-f"){
			++i;
			while ((i < argc) && ((char)(*argv[i]) != '-'))
				FileNames.push_back(string(argv[i++]));
			--i;
			b_FileName = true;
		}
		else if (string(argv[i]) == "-h"){
			Print_help();
			return 0;
		}
	}

	//if (b_c){
	//	Compare(file_end_x, FileNames);
	//	return 0;
	//}

	//FileNames.push_back(string("F:\\Reversi\\pos\\rnd_d19_100M.b"));
	//FileNames.push_back(string("F:\\Reversi\\pos\\rnd_d7_1M.b"));
	//FileNames.push_back(string("F:\\Reversi\\pos\\rnd_d8_1M.b"));
	//FileNames.push_back(string("F:\\Reversi\\pos\\rnd_d9_1M.b"));
	//FileNames.push_back(string("F:\\Reversi\\pos\\rnd_d10_1M.b"));
	//CalculateActiveConfigurations(FileNames);
	//return 0;

	if (b_FileName)
		CalculateActiveConfigurations(FileNames);

	return 0;
}