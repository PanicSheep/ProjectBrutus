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
	bool v = false;
	bool SkipSolved = true;
	bool Save = true;
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
		else if (std::string(argv[i]) == "-v")
			v = true;
		else if (std::string(argv[i]) == "-test"){
			SkipSolved = false;
			Save = false;
		}
		else if (std::string(argv[i]) == "-noskip")
			SkipSolved = false;
		else if (std::string(argv[i]) == "-nosave")
			Save = false;
		else if (std::string(argv[i]) == "-h")
			std::cout << "Solves a file of given reversi positions." << std::endl <<
					"Arguments:" << std::endl <<
					"-f\tFilename." << std::endl <<
					"-n\tNumber of positions to solve (default: 100)" << std::endl <<
					"-d\tDepth to solve for (default: Exact)" << std::endl <<
					"-t\tNumber of threads" << std::endl <<
					"-test\tRun as test." << std::endl <<
					"-noskip\tDon't skip solved positions." << std::endl <<
					"-nosave\tDon't save results." << std::endl <<
					"-h\tDisplays help." << std::endl;
	}

	SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);

	ConfigFile::Initialize(argv[0], std::string("ProjectBrutus.ini"));
	Features::Initialize();

	//Filename = std::string("F:\\Reversi\\pos\\rnd_d31_1M.psp");
	//n = 1000;
	//d = 13;
	//s = 6;
	//t = 4;
	//v = true;
	//test = true;
	
	std::string ending_input = Filename.substr(Filename.rfind(".") + 1, Filename.length());

	if(b_file)
	{
		if (FILE *file = fopen(Filename.c_str(), "r"))
			fclose(file);
		else
		{
			std::cerr << "ERROR: File '" << Filename << "' could not be opened." << std::endl;
			return 1;
		}

		if (!SkipSolved && !Save)
			std::cout << "Running as a TEST!" << std::endl;
		if (!SkipSolved)
			std::cout << "Don't skip solved positions!" << std::endl;
		if (!Save)
			std::cout << "Don't save results!" << std::endl;

		switch (Ending_to_DataType(ending_input))
		{
		//case DataType::Old:                Solve<CDataset_Old               >(Filename, n, d, s, t, v, test); break;
		case DataType::Position_Score:     Solve<CDataset_Position_Score    >(Filename, n, d, s, t, v, SkipSolved, Save); break;
		case DataType::Position_Score_PV:  Solve<CDataset_Position_Score_PV >(Filename, n, d, s, t, v, SkipSolved, Save); break;
		case DataType::Position_FullScore: Solve<CDataset_Position_FullScore>(Filename, n, d, s, t, v, SkipSolved, Save); break;
		}
	}

	Features::Finalize();
	return 0;
}