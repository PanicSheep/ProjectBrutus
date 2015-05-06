#include <iostream>
#include <string>
#include "datamanipulation.h"

template <class INPUT, class OUTPUT>
void Convert_2(const std::string & s_input, const std::string & s_output)
{
	std::vector<INPUT > vec_in = read_vector<INPUT>(s_input);
	std::vector<OUTPUT> vec_out;
	for (const auto& elem : vec_in)
		vec_out.push_back(static_cast<OUTPUT>(elem));
	write_to_file(s_output, vec_out);
}

template <class INPUT>
void Convert_1(const std::string & s_input, const std::string & s_output)
{
	std::string ending_output = s_output.substr(s_output.rfind(".") + 1, s_output.length());

	switch (Ending_to_DataType(ending_output))
	{
	//case DataType::Old:              return Convert_2<INPUT, CDataset_Old               >(s_input, s_output);
	case DataType::Position_Score:     return Convert_2<INPUT, CDataset_Position_Score    >(s_input, s_output);
	case DataType::Position_Score_PV:  return Convert_2<INPUT, CDataset_Position_Score_PV >(s_input, s_output);
	case DataType::Position_FullScore: return Convert_2<INPUT, CDataset_Position_FullScore>(s_input, s_output);
	case DataType::Edax:               return Convert_2<INPUT, CDataset_Edax              >(s_input, s_output);
	//case DataType::Position:           return Convert_2<INPUT, CPosition                  >(s_input, s_output);
	//case DataType::PositionScore:      return Convert_2<INPUT, CPositionScore             >(s_input, s_output);
	}
}

void Convert(const std::string & s_input, const std::string & s_output)
{
	std::string ending_input = s_input.substr(s_input.rfind(".") + 1, s_input.length());

	switch (Ending_to_DataType(ending_input))
	{
	case DataType::Position_Score:     return Convert_1<CDataset_Position_Score    >(s_input, s_output);
	case DataType::Position_Score_PV:  return Convert_1<CDataset_Position_Score_PV >(s_input, s_output);
	case DataType::Position_FullScore: return Convert_1<CDataset_Position_FullScore>(s_input, s_output);
	case DataType::Edax:               return Convert_1<CDataset_Edax              >(s_input, s_output);
	//case DataType::Position:           return Convert_1<CPosition                  >(s_input, s_output);
	//case DataType::PositionScore:      return Convert_1<CPositionScore             >(s_input, s_output);
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

	if (b_s_input && b_s_output)
		Convert(s_input, s_output);
	else
		std::cerr << "Not enough parameters provided." << std::endl;

	return 0;
}