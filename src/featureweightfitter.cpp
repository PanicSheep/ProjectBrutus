#include "featureweightfitter.h"

using namespace std;

// 136 Bit = 17 Byte
class CPOSITON_SCORE
{
public:
	unsigned long long P, O;
	signed char score;
	CPOSITON_SCORE() : P(0), O(0), score(DATASET_DEFAULT_score) {};
	CPOSITON_SCORE(const CDataset_Old               & data) : P(data.P), O(data.O), score(data.score) {}
	CPOSITON_SCORE(const CDataset_Position_Score     & data) : P(data.P), O(data.O), score(data.score) {}
	CPOSITON_SCORE(const CDataset_Position_Score_PV  & data) : P(data.P), O(data.O), score(data.score) {}
	CPOSITON_SCORE(const CDataset_Position_FullScore& data) : P(data.P), O(data.O)
	{
		score = SCHAR_MIN;
		for (int i = 0; i < 64; i++)
			if (data.score[i] > score)
				score = data.score[i];
	}
};

void Load_Matrix(const vector<string>& file_matrix, vector<CPOSITON_SCORE>& A)
{
	vector<CDataset_Old> tmp_OLD;
	vector<CDataset_Position_Score> tmp_POSITON_SCORE;
	vector<CDataset_Position_Score_PV> tmp_POSITON_SCORE_PV;
	vector<CDataset_Position_FullScore> tmp_POSITON_FULL_SCORE;

	for (auto& file : file_matrix)
	{
		switch (Ending_to_DataType(file.substr(file.rfind(".") + 1, file.length())))
		{
		case DataType::Old:
			read_vector(file, tmp_OLD);
			for (auto& item : tmp_OLD)
				A.push_back(static_cast<CPOSITON_SCORE>(item));
			tmp_OLD.clear();
			break;
		case DataType::Position_Score:
			read_vector(file, tmp_POSITON_SCORE);
			for (auto& item : tmp_POSITON_SCORE)
				A.push_back(static_cast<CPOSITON_SCORE>(item));
			tmp_POSITON_SCORE.clear();
			break;
		case DataType::Position_Score_PV:
			read_vector(file, tmp_POSITON_SCORE_PV);
			for (auto& item : tmp_POSITON_SCORE_PV)
				A.push_back(static_cast<CPOSITON_SCORE>(item));
			tmp_POSITON_SCORE_PV.clear();
			break;
		case DataType::Position_FullScore:
			read_vector(file, tmp_POSITON_FULL_SCORE);
			for (auto& item : tmp_POSITON_FULL_SCORE)
				A.push_back(static_cast<CPOSITON_SCORE>(item));
			tmp_POSITON_FULL_SCORE.clear();
			break;
		}
	}
}

double Matrix_norm(const vector<CPOSITON_SCORE>& A, const vector<bool>& Present, vector<double>& x)
{
	double tmp;
	double sum = 0;
	int Array[Features::Symmetries];
	const int size = A.size();

	for (int i = 0; i < Features::Size; ++i)
		if (!Present[i])
			x[i] = 0;

	#pragma omp parallel private(Array, tmp)
	{
		double local_sum = 0;

		#pragma omp for nowait schedule(static, 1024)
		for (int i = 0; i < size; ++i)
		{
			FillConfigurationArray(A[i].P, A[i].O, Array);

			tmp = 0;
			for (int j = 0; j < Features::Symmetries; ++j) // This equals A*x
				tmp += x[Array[j]];
			local_sum += tmp * tmp;
		}

		#pragma omp critical
		{
			sum += local_sum;
		}
	}

	return sqrt(sum);
}

int Calc_Present_Configurations(const vector<CPOSITON_SCORE>& A, vector<bool>& Present, const int Threshold)
{
	int counter = 0;
	int Array[Features::Symmetries];
	vector<int> Appearances(Features::Size);
	const int size = A.size();

	#pragma omp parallel private(Array)
	{
		vector<int> local_Appearances = vector<int>(Features::Size);
		
		#pragma omp for nowait schedule(static, 1024)
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
	}

	for (int i = 0; i < Features::Size; ++i)
		if (Present[i] = (Appearances[i] >= Threshold))
			counter++;

	return counter;
}

void Calc_C_r0(const vector<CPOSITON_SCORE>& A, const vector<bool>& Present, const vector<double>& x0, vector<double>& C, vector<double>& r0)
{
	double tmp;
	int Array[Features::Symmetries];
	const int size = A.size();
	
	for (int i = 0; i < Features::Size; ++i)
		if (!Present[i])
			assert(x0[i] == 0);

	#pragma omp parallel private(Array, tmp)
	{
		vector<double> local_C = vector<double>(Features::Size);
		vector<double> local_r0 = vector<double>(Features::Size);

		#pragma omp for nowait schedule(static, 1024)
		for (int i = 0; i < size; ++i)
		{
			FillConfigurationArraySorted(A[i].P, A[i].O, Array);
			
			tmp = static_cast<double>(A[i].score);
			for (int j = 0; j < Features::Symmetries; ++j) // This equals -=A*x0
				tmp -= x0[Array[j]];
			for (int j = 0; j < Features::Symmetries; ++j)
				local_r0[Array[j]] += tmp;

			int j = 0;
			int equals = 1;
			while (j+equals < Features::Symmetries)
			{
				if (Array[j] == Array[j+equals])
					equals++;
				else
				{
					local_C[Array[j]] += equals * equals;
					j += equals;
					equals = 1;
				}
			}
			local_C[Array[j]] += equals * equals;
		}

		#pragma omp critical
		{
			for (int i = 0; i < Features::Size; ++i)
				if (Present[i])
					r0[i] += local_r0[i];
		}
		#pragma omp critical
		{
			for (int i = 0; i < Features::Size; ++i)
				if (Present[i])
					C[i] += local_C[i];
		}
	}
	for (int i = 0; i < Features::Size; ++i){
		if (Present[i])
			C[i] = 1.0 / C[i];
		else
			C[i] = 0.0;		
	}
}
void Calc_C_r0(const vector<CPOSITON_SCORE>& A, const vector<bool>& Present, vector<double>& C, vector<double>& r0)
{
	int Array[Features::Symmetries];
	const int size = A.size();

	#pragma omp parallel private(Array)
	{
		vector<double> local_C = vector<double>(Features::Size);
		vector<double> local_r0 = vector<double>(Features::Size);

		#pragma omp for nowait schedule(static, 1024)
		for (int i = 0; i < size; ++i)
		{
			FillConfigurationArraySorted(A[i].P, A[i].O, Array);
			
			if (A[i].score)
				for (int j = 0; j < Features::Symmetries; ++j)
					local_r0[Array[j]] += static_cast<double>(A[i].score);

			int j = 0;
			int equals = 1;
			while (j+equals < Features::Symmetries)
			{
				if (Array[j] == Array[j+equals])
					equals++;
				else
				{
					local_C[Array[j]] += equals * equals;
					j += equals;
					equals = 1;
				}
			}
			local_C[Array[j]] += equals * equals;
		}

		#pragma omp critical
		{
			for (int i = 0; i < Features::Size; ++i)
				if (Present[i])
					r0[i] += local_r0[i];
		}
		#pragma omp critical
		{
			for (int i = 0; i < Features::Size; ++i)
				if (Present[i])
					C[i] += local_C[i];
		}
	}
	for (int i = 0; i < Features::Size; ++i){
		if (Present[i])
			C[i] = 1.0 / C[i];
		else
			C[i] = 0.0;
	}
}

double relative_residual(const vector<CPOSITON_SCORE>& A, const vector<bool>& Present, const vector<double>& x)
{
	double b = 0;
	double relres = 0;
	double tmp;
	int Array[Features::Symmetries];
	const int size = A.size();

	#pragma omp parallel private(Array, tmp)
	{
		double local_b = 0;
		double local_relres = 0;

		#pragma omp for nowait schedule(static, 1024)
		for (int i = 0; i < size; ++i)
		{
			FillConfigurationArray(A[i].P, A[i].O, Array);
			
			local_b += static_cast<double>(A[i].score) * static_cast<double>(A[i].score);
			tmp = -static_cast<double>(A[i].score);
			for (int j = 0; j < Features::Symmetries; ++j) // This equals A*x
				if (Present[Array[j]])
					tmp += x[Array[j]];
			local_relres += tmp * tmp;
		}
		
		#pragma omp critical
		{
			b += local_b;
			relres += local_relres;
		}
	}

	return sqrt(relres / b);
}

vector<double> ATAx(const vector<CPOSITON_SCORE>& A, const vector<bool>& Present, const vector<double>& x)
{
	double tmp;
	int Array[Features::Symmetries];
	const int size = A.size();

	vector<double> result = vector<double>(Features::Size);

	#pragma omp parallel private(Array, tmp)
	{
		vector<double> local_result = vector<double>(Features::Size);

		#pragma omp for nowait schedule(static, 1024)
		for (int i = 0; i < size; ++i)
		{
			FillConfigurationArray(A[i].P, A[i].O, Array);

			tmp = 0;
			for (int j = 0; j < Features::Symmetries; ++j) // This equals A*x
				tmp += x[Array[j]];
			for (int j = 0; j < Features::Symmetries; ++j) // This equals AT*(A*x)
				local_result[Array[j]] += tmp;
		}

		#pragma omp critical
		{
			for (int i = 0; i < Features::Size; ++i)
				if (Present[i])
					result[i] += local_result[i];
		}
	}

	return result;
}

void Test_Restart(const vector<CPOSITON_SCORE>& A, const vector<bool>& Present, const vector<double>& x, const vector<double>& C, vector<double>& r, vector<double>& h, vector<double>& d)
{
	vector<double> r_new = vector<double>(Features::Size);
	vector<double> r_diff = vector<double>(Features::Size);
	double tmp;
	int Array[Features::Symmetries];
	const int size = A.size();

	#pragma omp parallel private(Array, tmp)
	{
		vector<double> local_r = vector<double>(Features::Size);

		#pragma omp for nowait schedule(static, 1024)
		for (int i = 0; i < size; ++i)
		{
			FillConfigurationArray(A[i].P, A[i].O, Array);
			
			tmp = static_cast<double>(A[i].score);
			for (int j = 0; j < Features::Symmetries; ++j) // This equals A*x
				tmp -= x[Array[j]];
			for (int j = 0; j < Features::Symmetries; ++j)
				local_r[Array[j]] += tmp;
		}

		#pragma omp critical
		{
			for (int i = 0; i < r_new.size(); ++i)
				if (Present[i])
					r_new[i] += local_r[i];
		}
	}

	for (int i = 0; i < r_diff.size(); ++i)
		r_diff[i] = r[i] - r_new[i];

	cout << "RELATIVE ERROR: " << sqrt(dot(r_diff) / dot(r)) << endl;
	if (sqrt(dot(r_diff) / dot(r)) > 1e-6)
	{
		cout << "RESTARTING: " << sqrt(dot(r_diff) / dot(r)) << endl;
		for (int i = 0; i < r.size(); ++i)
			r[i] = r_new[i];
		for (int i = 0; i < d.size(); ++i)
			d[i] = h[i] = r[i] * C[i];
	}
}

void DoStatsMuSigma(const vector<CPOSITON_SCORE>& A, const vector<bool>& Present, const vector<double>& x, double& mu, double& sigma, double& avg_abs_err)
{
	CRunningStatistic<double> stats = CRunningStatistic<double>();
	avg_abs_err = 0;
	unsigned long long counter = 0;
	int Array[Features::Symmetries];
	const int size = A.size();

	#pragma omp parallel private(Array)
	{
		CRunningStatistic<double> local_stats = CRunningStatistic<double>();
		double local_avg = 0;
		double local_avg_abs_err = 0;
		unsigned long long local_N = 0;
		double tmp;

		#pragma omp for nowait schedule(static, 1024)
		for (int i = 0; i < size; ++i)
		{
			FillConfigurationArray(A[i].P, A[i].O, Array);

			++local_N;
			tmp = -static_cast<double>(A[i].score);
			for (int j = 0; j < Features::Symmetries; ++j) // This equals A*x
				tmp += x[Array[j]];
			local_stats.Add(tmp);
			local_avg_abs_err += (abs(tmp) - local_avg_abs_err) / static_cast<double>(local_N);
		}

		#pragma omp critical
		{
			stats += local_stats;
			avg_abs_err = avg_abs_err * (static_cast<double>(counter) / static_cast<double>(counter + local_N))
					+ local_avg_abs_err * (static_cast<double>(local_N) / static_cast<double>(counter + local_N));
			counter += local_N;
		}
	}
	mu = stats.mu();
	sigma = stats.sigma();
}

void SolveInRAM(const vector<string> file_matrix, const char* file_start_x, const char* file_end_x, const int Iterations, const int Threshold)
{
	std::chrono::high_resolution_clock::time_point startTime, endTime;
	vector<double> C(Features::Size);
	vector<double> d(Features::Size);
	vector<double> r(Features::Size);
	vector<double> h(Features::Size);
	vector<double> x;
	vector<double> z(Features::Size);
	vector<bool> Present(Features::Size);
	vector<CPOSITON_SCORE> A;
	double alpha, beta, tmp1, tmp2, tmp_dot2, tol, res, mu, sigma, avg_abs_err;
	tol = 1e-5;

	printf("Matrices:\n");
	for (auto it = file_matrix.cbegin(); it != file_matrix.cend(); ++it)
		printf("\t%s\n", it->c_str());
	if (file_start_x)
		printf("Using starting vector:\n\t%s\n", file_start_x);
	else
		printf("Not using a starting vector.\n");
	printf("Max iterations: %d\n", Iterations);
	printf("Active configuration threshold: %d\n\n", Threshold);

	printf("Loading matrices...");
	startTime = std::chrono::high_resolution_clock::now();
	Load_Matrix(file_matrix, A);
	endTime = std::chrono::high_resolution_clock::now();
	printf("done!\t\t\t %14s\n", time_format(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime)).c_str());
	
	printf("Number of positions: %d\n", A.size());

	printf("Calculating present configurations...");
	startTime = std::chrono::high_resolution_clock::now();
	const int Number_of_Present_Configurations = Calc_Present_Configurations(A, Present, Threshold);
	endTime = std::chrono::high_resolution_clock::now();
	printf("done!\t %14s\n", time_format(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime)).c_str());

	printf("Configurations: %d\n", Features::Size);
	printf("Active configurations: %d\n", Number_of_Present_Configurations);
	printf("Percentage: %.2f%%\n\n", Number_of_Present_Configurations * 100.0 / Features::Size);

	if (file_start_x)
	{
		printf("Loading vector x_0...");
		startTime = std::chrono::high_resolution_clock::now();
		read_vector(file_start_x, x);
		for (int i = 0; i < Features::Size; ++i)
			if (!Present[i])
				x[i] = 0;
		endTime = std::chrono::high_resolution_clock::now();
		printf("done!\t\t\t %14s\n", time_format(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime)).c_str());

		printf("Loading matrix C and vectors r_0...");
		startTime = std::chrono::high_resolution_clock::now();
		Calc_C_r0(A, Present, x, C, r);
		endTime = std::chrono::high_resolution_clock::now();
		printf("done!\t %14s\n", time_format(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime)).c_str());
	}
	else
	{
		x = vector<double>(Features::Size);
		printf("Loading matrix C and vectors r_0...");
		startTime = std::chrono::high_resolution_clock::now();
		Calc_C_r0(A, Present, C, r);
		endTime = std::chrono::high_resolution_clock::now();
		printf("done!\t %14s\n", time_format(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime)).c_str());
	}

	printf("Calculating vectors h_0 and d_0...");
	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < Features::Size; ++i)
		h[i] = C[i] * r[i];
	for (int i = 0; i < Features::Size; ++i)
		d[i] = h[i];
	endTime = std::chrono::high_resolution_clock::now();
	printf("done!\t\t %14s\n\n", time_format(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime)).c_str());

	printf(" it | norm(res) | avg(abs(err)) |  avg(err)  |   std(err)   |      time     \n");
	printf("----+-----------+---------------+------------+--------------+---------------\n");
	for (int k = 1; k <= Iterations; ++k)
	{
		startTime = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < Features::Size; ++i)
			z[i] = 0;

		z = ATAx(A, Present, d);
		tmp1 = dot(r, h);
		alpha = tmp1 / dot(d, z);
		beta = 1.0 / tmp1;
		//tmp2 = dot(d, z);
		//alpha = tmp1 / tmp2;
		//beta = 1.0 / tmp1;
		//tmp_dot2 = 0.0;

		for (int i = 0; i < Features::Size; ++i)
			x[i] += alpha * d[i];

		write_to_file(file_end_x, x);

		//tmp1 = 0.0;
		for (int i = 0; i < Features::Size; ++i)
			r[i] -= alpha * z[i];
		for (int i = 0; i < Features::Size; ++i)
			h[i] = C[i] * r[i];
			//tmp1 += r[i] * h[i];
			//tmp_dot2 += r[i] * r[i];

		beta *= dot(r, h);
		//beta *= tmp1;

		for (int i = 0; i < Features::Size; ++i)
			d[i] = h[i] + beta * d[i];

		res = sqrt(dot(r));
		//res = sqrt(tmp_dot2);

		if (k % 10 == 0)
		{
			//Test_Restart(A, Present, x, C, r, h, d);
			//res = sqrt(dot(r));
			DoStatsMuSigma(A, Present, x, mu, sigma, avg_abs_err);
			endTime = std::chrono::high_resolution_clock::now();
			//printf(" it | norm(res) | avg(abs(err)) |  avg(err)  |   std(err)   |      time     \n");
			printf("%4d| %.2e |  %.4e  | % .2e | % .4e | %14s\n", k, res, avg_abs_err, mu, sigma, time_format(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime)).c_str());
			//printf("%4d| %.2e |  %.4e  | % .2e | % .4e | %14s\n", k, Matrix_norm(A, Present, r), avg_abs_err, mu, sigma, time_format(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime)).c_str());
		}
		else
		{
			endTime = std::chrono::high_resolution_clock::now();
			//printf(" it | norm(res) | avg(abs(err)) |  avg(err)  |   std(err)   |      time     \n");
			printf("%4d| %.2e |               |            |              | %14s\n", k, res, time_format(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime)).c_str());
			//printf("%4d| %.2e |               |            |              | %14s\n", k, Matrix_norm(A, Present, r), time_format(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime)).c_str());
		}

		if (res < tol) break;
	}
	printf("\nConjugate gradient method terminated!\n");

	printf("Saving vector...");
	write_to_file(file_end_x, x);
	printf("done!\n");
}

//void Compare(const char* file_x, list<string> file)
//{
//	RunningStatistic<double> stats = RunningStatistic<double>();
//	TinyVector<double, Features::Size> x = TinyVector<double, Features::Size>();
//	unsigned long long dist[129][129];
//	unsigned long long P, O;
//	short score;
//	array<int, Features::Symmetries> Array;
//	fstream myfile;
//	ReversiPosition Position;
//	double tmp;
//
//	for (int i = 0; i < 129; ++i){
//		for (int j = 0; j < 129; ++j)
//			dist[i][j] = 0;
//	}
//
//	read_vector(x, file_x);
//
//	//Calculate statistics for e := Ax - b
//	const list<string>::iterator end = file.end();
//	for (list<string>::iterator it = file.begin(); it != end; ++it){
//		myfile = fstream(it->c_str(), ios::in | ios::binary);
//		if(!myfile){
//			cerr << "ERROR: File '" << it->c_str() << "' could not be opened!" << endl;
//			return;
//		}
//		while (!myfile.eof())
//		{
//			myfile.read((char*)&P,sizeof(unsigned long long));
//			myfile.read((char*)&O,sizeof(unsigned long long));
//			myfile.read((char*)&score,sizeof(short));
//			Position = ReversiPosition(P, O);
//			LoadAttributesIntoArray(Position, Array);
//			tmp = 0;
//			for (int j = 0; j < Features::Symmetries; ++j)
//				tmp += x[Array[j]];
//			++dist[BIND(static_cast<int>(tmp + 0.5) + 64,0,128)][score + 64];
//			stats.Add(tmp - score);
//		}
//		myfile.close();
//	}
//
//	//tmp = 0;
//	//for (int i = 0; i < 129; ++i){
//	//	for (int j = 0; j < 129; ++j)
//	//		tmp = MAX(tmp, (double)dist[i][j]);
//	//}
//
//	cout << "avg: " << stats.mu() << "\n";
//	cout << "std: " << stats.sigma() << "\n\n";
//
//	for (int i = 0; i < 129; ++i){
//		for (int j = 0; j < 129; ++j){
//			if (dist[i][j])
//				cout << "#";
//			else
//				cout << " ";
//		}
//		cout << "\n";
//	}
//}

void Print_help()
{
	cout << endl << "Calculates the feature weights." << endl
				 << "Attributes:" << endl
				 << "-f\tsolved position files" << endl
				 << "-initial\tinitial coefficient file" << endl
				 << "-x\toutput coefficient file" << endl 
				 << "-iterations\tnumber of iterations of the CG-method" << endl 
				 << "-t\tnumber of threats to use" << endl 
				 << "-n\tlower treshold for number of active configurations" << endl 
				 << "-c\tcompares the true scores to the evaluation function" << endl 
				 << "-h\thelp" << endl << endl;
}

int main(int argc, char* argv[])
{
	bool b_FileName = false;
	bool b_Initial = false;
	bool b_x = false;
	bool b_Iterations = false;
	bool b_c = false;
	int t = 1;
	int Threshold = 0;

	vector<string> FileNames;
	char* file_start_x = NULL;
	char* file_end_x = NULL;
	int Iterations = 100;

	for(int i = 0; i < argc; ++i) 
	{
		if (string(argv[i]) == "-f"){
			++i;
			while ((i < argc) && ((char)(*argv[i]) != '-'))
				FileNames.push_back(string(argv[i++]));
			--i;
			b_FileName = true;
		}
		else if (string(argv[i]) == "-initial"){
			file_start_x = argv[++i];
			b_Initial = true;
		}
		else if (string(argv[i]) == "-x"){
			file_end_x = argv[++i];
			b_x = true;
		}
		else if (string(argv[i]) == "-iterations"){
			Iterations = atoi(argv[++i]);
			b_Iterations = true;
		}
		else if (std::string(argv[i]) == "-t")
			t = atoi(argv[++i]);
		else if (std::string(argv[i]) == "-n")
			Threshold = atoi(argv[++i]);
		else if (string(argv[i]) == "-c")
			b_c = true;
		else if (string(argv[i]) == "-h"){
			Print_help();
			return 0;
		}
	}

	SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);

	//if (b_c){
	//	Compare(file_end_x, FileNames);
	//	return 0;
	//}
	
	//FileNames.push_back(string("F:\\Reversi\\pos\\rnd_d5_1M.b"));
	////FileNames.push_back(string("F:\\Reversi\\pos\\rnd_d9_1M.b"));
	////FileNames.push_back(string("F:\\Reversi\\pos\\rnd_d10_1M.b"));
	////FileNames.push_back(string("F:\\Reversi\\pos\\rnd_d11_1M.b"));
	////FileNames.push_back(string("F:\\Reversi\\pos\\rnd_d12_1M.b"));
	////FileNames.push_back(string("F:\\Reversi\\pos\\rnd_d13_1M.b"));
	////SolveInRAM(FileNames, string("F:\\Penis.b").c_str(), string("F:\\Penis.b").c_str(), Iterations, 10);
	//SolveInRAM(FileNames, 0, string("F:\\Penis.b").c_str(), 50, 10);
	//return 0;

	if (b_FileName)
		SolveInRAM(FileNames, file_start_x, file_end_x, Iterations, Threshold);

	return 0;
}