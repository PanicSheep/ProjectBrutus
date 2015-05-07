#include "datamanipulation.h"

const std::string CDataset_Edax::field[] = {
	"H8", "G8", "F8", "E8", "D8", "C8", "B8", "A8",
	"H7", "G7", "F7", "E7", "D7", "C7", "B7", "A7",
	"H6", "G6", "F6", "E6", "D6", "C6", "B6", "A6",
	"H5", "G5", "F5", "E5", "D5", "C5", "B5", "A5",
	"H4", "G4", "F4", "E4", "D4", "C4", "B4", "A4",
	"H3", "G3", "F3", "E3", "D3", "C3", "B3", "A3",
	"H2", "G2", "F2", "E2", "D2", "C2", "B2", "A2",
	"H1", "G1", "F1", "E1", "D1", "C1", "B1", "A1"
};


CPositionScore::CPositionScore(const CDataset_Position_Score&     that) : P(that.P), O(that.O), score(that.score) {}
CPositionScore::CPositionScore(const CDataset_Position_Score_PV&  that) : P(that.P), O(that.O), score(that.score) {}
CPositionScore::CPositionScore(const CDataset_Position_FullScore& that) : P(that.P), O(that.O), score(that.BestScore()) {}
CPositionScore::CPositionScore(const CDataset_Edax&               that) : P(that.P), O(that.O), score(that.BestScore()) {}

CPositionScoreDepthSelectivity::CPositionScoreDepthSelectivity(const CDataset_Position_Score&     that) : P(that.P), O(that.O), score(that.score),       depth(that.depth),        selectivity(that.selectivity) {}
CPositionScoreDepthSelectivity::CPositionScoreDepthSelectivity(const CDataset_Position_Score_PV&  that) : P(that.P), O(that.O), score(that.score),       depth(that.depth),        selectivity(that.selectivity) {}
CPositionScoreDepthSelectivity::CPositionScoreDepthSelectivity(const CDataset_Position_FullScore& that) : P(that.P), O(that.O), score(that.BestScore()), depth(that.depth),        selectivity(that.selectivity) {}
CPositionScoreDepthSelectivity::CPositionScoreDepthSelectivity(const CDataset_Edax&               that) : P(that.P), O(that.O), score(that.BestScore()), depth(DATASET_depth_END), selectivity(DATASET_DEFAULT_selectivity) {}

CDataset_Position_Score::CDataset_Position_Score(const CDataset_Position_Score_PV&  that) : P(that.P), O(that.O), depth(that.depth), selectivity(that.selectivity),            score(that.score) {}
CDataset_Position_Score::CDataset_Position_Score(const CDataset_Position_FullScore& that) : P(that.P), O(that.O), depth(that.depth), selectivity(that.selectivity),            score(that.BestScore()) {}
CDataset_Position_Score::CDataset_Position_Score(const CDataset_Edax&               that) : P(that.P), O(that.O),                    selectivity(DATASET_DEFAULT_selectivity), score(that.BestScore()) { depth = (score == DATASET_DEFAULT_score) ? DATASET_DEFAULT_depth : DATASET_depth_END; }

CDataset_Position_Score_PV::CDataset_Position_Score_PV(const CDataset_Position_Score&     that) : P(that.P), O(that.O), depth(that.depth),                                                                              selectivity(that.selectivity),            score(that.score)       { for (int i = 0; i < 5; i++) PV[i] = DATASET_DEFAULT_PV; }
CDataset_Position_Score_PV::CDataset_Position_Score_PV(const CDataset_Position_FullScore& that) : P(that.P), O(that.O), depth(that.depth),                                                                              selectivity(that.selectivity),            score(that.BestScore()) { for (int i = 0; i < 5; i++) PV[i] = DATASET_DEFAULT_PV; PV[0] = that.BestMove(); }
CDataset_Position_Score_PV::CDataset_Position_Score_PV(const CDataset_Edax&               that) : P(that.P), O(that.O), depth((that.BestScore() == DATASET_DEFAULT_score) ? DATASET_DEFAULT_depth : DATASET_depth_END), selectivity(DATASET_DEFAULT_selectivity), score(that.BestScore()) { for (int i = 0; i < 5; i++) PV[i] = DATASET_DEFAULT_PV; PV[0] = that.BestMove(); }

CDataset_Position_FullScore::CDataset_Position_FullScore(const CDataset_Position_Score&    that) : P(that.P), O(that.O), depth(that.depth),                                                                              selectivity(that.selectivity)            { for (int i = 0; i < 64; ++i) score[i] = DATASET_DEFAULT_score; score[36] = that.score; }
CDataset_Position_FullScore::CDataset_Position_FullScore(const CDataset_Position_Score_PV& that) : P(that.P), O(that.O), depth(that.depth),                                                                              selectivity(that.selectivity)            { for (int i = 0; i < 64; ++i) score[i] = DATASET_DEFAULT_score; score[36] = that.score; }
CDataset_Position_FullScore::CDataset_Position_FullScore(const CDataset_Edax&              that) : P(that.P), O(that.O), depth((that.BestScore() == DATASET_DEFAULT_score) ? DATASET_DEFAULT_depth : DATASET_depth_END), selectivity(DATASET_DEFAULT_selectivity) { for (int i = 0; i < 64; ++i) score[i] = that.score[i]; }

CDataset_Edax::CDataset_Edax(std::string line)
{
	std::string delimiter = ";";
	std::string token;
	size_t pos = 0;

	Reset();
	P = 0;
	O = 0;

	for (unsigned int i = 0; i < 64; ++i)
	{
		if (line.substr(i, 1) == "X") P |= 0x8000000000000000ULL >> i;
		if (line.substr(i, 1) == "O") O |= 0x8000000000000000ULL >> i;
	}
	if (line.substr(65, 1) == "O")
		std::swap(P, O);

	while ((pos = line.find(delimiter)) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + delimiter.length());

		for (int i = 0; i < 64; ++i)
			if (token.substr(1, 2) == field[i])
				score[i] = std::stoi(token.substr(4, 3));
	}
}

signed char CDataset_Edax::BestScore() const
{
	signed char ret = DATASET_DEFAULT_score;
	for (int i = 0; i < 64; ++i)
		if (score[i] > ret)
			ret = score[i];
	return ret;
}

unsigned char CDataset_Edax::BestMove() const
{
	signed char bestScore = BestScore();
	for (unsigned char i = 0; i < 64; ++i)
		if (score[i] == bestScore)
			return i;
	return DATASET_DEFAULT_PV;
}

std::string CDataset_Edax::to_string() const
{
	std::string s;
	s.append(board1D(P, O));
	s.append(" X;");
	for (int i = 0; i < 64; ++i)
		if (score[i] != DATASET_DEFAULT_score)
			s.append(" " + field[i] + ":" + ((score[i] >= 0) ? "+" : "-") + std::to_string(score[i]) + ";");
	return s;
}

namespace DataManipulation
{
	template<> std::vector<CDataset_Edax> read_vector2(const std::string & filename, std::size_t size)
	{
		FILE* file;
		fopen_s(&file, filename.c_str(), "r");
		if (!file){
			std::cerr << "ERROR: File '" << filename << "' could not be opened!" << std::endl;
			throw "File could not be opened.";
			return std::vector<CDataset_Edax>();
		}

		const std::size_t N = 4 * 1024;
		std::vector<CDataset_Edax> vector;
		char DataArray[N];
		while (size-- && fgets(DataArray, N, file))
			vector.push_back(CDataset_Edax(std::string(DataArray)));

		fclose(file);
		return vector;
	}

	template<> std::vector<CPosition> read_vector2(const std::string & filename, std::size_t size)
	{
		std::vector<CPosition> ret;
		std::string ending = filename.substr(filename.rfind(".") + 1, filename.length());
		switch (Ending_to_DataType(ending))
		{
			case DataType::Position_Score:
			{
				std::vector<CDataset_Position_Score    > vec = read_vector<CDataset_Position_Score    >(filename, size);
				for (const auto& pos : vec) ret.push_back(CPosition(pos.P, pos.O));
				break;
			}
			case DataType::Position_Score_PV:
			{
				std::vector<CDataset_Position_Score_PV > vec = read_vector<CDataset_Position_Score_PV >(filename, size);
				for (const auto& pos : vec) ret.push_back(CPosition(pos.P, pos.O));
				break;
			}
			case DataType::Position_FullScore:
			{
				std::vector<CDataset_Position_FullScore> vec = read_vector<CDataset_Position_FullScore>(filename, size);
				for (const auto& pos : vec) ret.push_back(CPosition(pos.P, pos.O));
				break;
			}
			case DataType::Edax:
			{
				std::vector<CDataset_Edax              > vec = read_vector<CDataset_Edax              >(filename, size);
				for (const auto& pos : vec) ret.push_back(CPosition(pos.P, pos.O));
				break;
			}
		}
		return ret;
	}

	template<> std::vector<CPositionScore> read_vector2(const std::string & filename, std::size_t size)
	{
		std::vector<CPositionScore> ret;
		std::string ending = filename.substr(filename.rfind(".") + 1, filename.length());
		switch (Ending_to_DataType(ending))
		{
			case DataType::Position_Score:
			{
				std::vector<CDataset_Position_Score    > vec = read_vector<CDataset_Position_Score    >(filename, size);
				for (const auto& pos : vec) ret.push_back(CPositionScore(pos));
				break;
			}
			case DataType::Position_Score_PV:
			{
				std::vector<CDataset_Position_Score_PV > vec = read_vector<CDataset_Position_Score_PV >(filename, size);
				for (const auto& pos : vec) ret.push_back(CPositionScore(pos));
				break;
			}
			case DataType::Position_FullScore:
			{
				std::vector<CDataset_Position_FullScore> vec = read_vector<CDataset_Position_FullScore>(filename, size);
				for (const auto& pos : vec) ret.push_back(CPositionScore(pos));
				break;
			}
			case DataType::Edax:
			{
				std::vector<CDataset_Edax              > vec = read_vector<CDataset_Edax              >(filename, size);
				for (const auto& pos : vec) ret.push_back(CPositionScore(pos));
				break;
			}
		}
		return ret;
	}
	
	template<> std::vector<CPositionScoreDepthSelectivity> read_vector2(const std::string & filename, std::size_t size)
	{
		std::vector<CPositionScoreDepthSelectivity> ret;
		std::string ending = filename.substr(filename.rfind(".") + 1, filename.length());
		switch (Ending_to_DataType(ending))
		{
			case DataType::Position_Score:
			{
				std::vector<CDataset_Position_Score    > vec = read_vector<CDataset_Position_Score    >(filename, size);
				for (const auto& pos : vec) ret.push_back(CPositionScoreDepthSelectivity(pos));
				break;
			}
			case DataType::Position_Score_PV:
			{
				std::vector<CDataset_Position_Score_PV > vec = read_vector<CDataset_Position_Score_PV >(filename, size);
				for (const auto& pos : vec) ret.push_back(CPositionScoreDepthSelectivity(pos));
				break;
			}
			case DataType::Position_FullScore:
			{
				std::vector<CDataset_Position_FullScore> vec = read_vector<CDataset_Position_FullScore>(filename, size);
				for (const auto& pos : vec) ret.push_back(CPositionScoreDepthSelectivity(pos));
				break;
			}
			case DataType::Edax:
			{
				std::vector<CDataset_Edax              > vec = read_vector<CDataset_Edax              >(filename, size);
				for (const auto& pos : vec) ret.push_back(CPositionScoreDepthSelectivity(pos));
				break;
			}
		}
		return ret;
	}
}

template<>
void write_to_file(const std::string & filename, const std::vector<CDataset_Edax>& vector)
{
	FILE* file;
	fopen_s(&file, filename.c_str(), "wb");
	if (!file){
		std::cerr << "ERROR: File '" << filename << "' could not be opened!" << std::endl;
		throw "File could not be opened.";
		return;
	}

	for (const CDataset_Edax& data : vector) fputs((data.to_string() + "\n").c_str(), file);

	fclose(file);
}


bool Test_datamanipulation()
{
	std::string filename(std::tmpnam(NULL)); // create temporary file name

	// create temporary vector
	const int N = 10000;
	std::vector<double> data(N);
	data[0] = 1;
	data[1] = 1;
	for (int i = 2; i < N; ++i)
		data[i] = data[i - 1] + data[i - 2];

	write_to_file(filename, data);
	data.clear();
	data = read_vector<double>(filename);

	// check data
	if (data[0] != 1) return false;
	if (data[1] != 1) return false;
	for (int i = 2; i < N; ++i)
		if (data[i] != data[i - 1] + data[i - 2]) return false;

	data.clear();
	write_to_file(filename, data);

	// check data
	if (data[0] != 1) return false;
	if (data[1] != 1) return false;
	for (int i = 2; i < N; ++i)
		if (data[i] != data[i - 1] + data[i - 2]) return false;

	data.clear();
	data = read_vector<double>(filename, N - 1);

	// check data
	if (data[0] != 1) return false;
	if (data[1] != 1) return false;
	for (int i = 2; i < N - 1; ++i)
		if (data[i] != data[i - 1] + data[i - 2]) return false;

	if (std::remove(filename.c_str())) // delete temporary file
		std::cerr << "ERROR: could not remove temporary file: '" << filename << "'." << std::endl;

	return true;
}