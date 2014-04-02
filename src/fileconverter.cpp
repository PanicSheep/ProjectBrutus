#include <iostream>
#include <string>
#include "datamanipulation.h"

template <class INPUT, class OUTPUT>
void Convert_2(const std::string & s_input, const std::string & s_output)
{
	std::vector<INPUT > vec_in = read_vector<INPUT>(s_input);
	std::vector<OUTPUT> vec_out;
	for (auto& elem : vec_in)
		vec_out.push_back(static_cast<OUTPUT>(elem));
	write_to_file(s_output, vec_out);
}

template <class INPUT>
void Convert_1(const std::string & s_input, const std::string & s_output)
{
	std::string ending_output = s_output.substr(s_output.rfind(".") + 1, s_output.length());

	switch (Ending_to_DATASET(ending_output))
	{
	//case OLD:                return Convert_2<INPUT, DATASET_OLD               >(s_input, s_output);
	case POSITON_SCORE:      return Convert_2<INPUT, DATASET_POSITON_SCORE     >(s_input, s_output);
	case POSITON_SCORE_PV:   return Convert_2<INPUT, DATASET_POSITON_SCORE_PV  >(s_input, s_output);
	case POSITON_FULL_SCORE: return Convert_2<INPUT, DATASET_POSITON_FULL_SCORE>(s_input, s_output);
	}
}

void Convert(const std::string & s_input, const std::string & s_output)
{
	std::string ending_input = s_input.substr(s_input.rfind(".") + 1, s_input.length());

	switch (Ending_to_DATASET(ending_input))
	{
	case OLD:                return Convert_1<DATASET_OLD               >(s_input, s_output);
	case POSITON_SCORE:      return Convert_1<DATASET_POSITON_SCORE     >(s_input, s_output);
	case POSITON_SCORE_PV:   return Convert_1<DATASET_POSITON_SCORE_PV  >(s_input, s_output);
	case POSITON_FULL_SCORE: return Convert_1<DATASET_POSITON_FULL_SCORE>(s_input, s_output);
	}
}

int main(int argc, char* argv[])
{
	bool b_s_input = false;
	bool b_s_output = false;
	std::string s_input;
	std::string s_output;

	for (int i = 0; i < argc; ++i)
	{
		if (std::string(argv[i]) == "-i"){
			s_input = std::string(argv[++i]);
			b_s_input = true;
		}
		else if (std::string(argv[i]) == "-o"){
			s_output = std::string(argv[++i]);
			b_s_output = true;
		}
		else if (std::string(argv[i]) == "-h")
			std::cout << "Convertes a file from one type to an other." << std::endl <<
			"Arguments:" << std::endl <<
			"-i\tInput filename." << std::endl <<
			"-o\tOutput filename." << std::endl <<
			"-h\tDisplays help." << std::endl;
	}

	//s_input  = "F:\\Reversi\\pos\\rnd_d0_100M.b";
	//s_output = "F:\\Reversi\\pos\\rnd_d0_100M.ps";
	//b_s_input = true;
	//b_s_output = true;

	if (b_s_input && b_s_output)
		Convert(s_input, s_output);

	return 0;
}