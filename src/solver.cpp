#include "solver.h"

int main(int argc, char* argv[])
{
	PositionCounter.store(0, std::memory_order_release);
	Spinlock.clear(std::memory_order_release);
	bool b_file = false;
	int d = CSearch::END;
	int s = 0;
	int n = 10;
	int t = 4;
	std::string Filename;

	for(int i = 0; i < argc; ++i)
	{
		if (std::string(argv[i]) == "-f"){
			Filename = std::string(argv[++i]);
			b_file = true;
		}
		else if (std::string(argv[i]) == "-n")
			n = atoi(argv[++i]);
		else if (std::string(argv[i]) == "-d")
			d = atoi(argv[++i]);
		else if (std::string(argv[i]) == "-s")
			s = atoi(argv[++i]);
		else if (std::string(argv[i]) == "-t")
			t = atoi(argv[++i]);
		else if (std::string(argv[i]) == "-h")
			std::cout << "Solves a file of given reversi positions." << std::endl <<
					"Arguments:" << std::endl <<
					"-f\tFilename." << std::endl <<
					"-n\tNumber of positions to solve (default: 100)" << std::endl <<
					"-d\tDepth to solve for (default: Exact)" << std::endl <<
					"-t\tNumber of threads" << std::endl <<
					"-h\tDisplays help." << std::endl;
	}

	SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);

	ConfigFile::Initialize(std::string("F:\\Reversi\\ProjectBrutus.ini"));
	Features::Initialize();

	//Filename = std::string("F:\\Reversi\\pos\\rnd_d23_1M.psp");
	//n = 1000;
	//d = 13;
	//s = 6;
	//t = 4;
	
	std::string ending_input = Filename.substr(Filename.rfind(".") + 1, Filename.length());

	if (FILE *file = fopen(Filename.c_str(), "r"))
		fclose(file);
	else
	{
		std::cerr << "ERROR: File '" << Filename << "' could not be opened." << std::endl;
		return 1;
	}

	switch (Ending_to_DATASET(ending_input))
	{
	//case OLD:                Solve<DATASET_OLD               >(Filename, n, d, s, t); break;
	case POSITON_SCORE:      Solve<DATASET_POSITON_SCORE     >(Filename, n, d, s, t); break;
	case POSITON_SCORE_PV:   Solve<DATASET_POSITON_SCORE_PV  >(Filename, n, d, s, t); break;
	case POSITON_FULL_SCORE: Solve<DATASET_POSITON_FULL_SCORE>(Filename, n, d, s, t); break;
	}

	Features::Finalize();
	return 0;
}