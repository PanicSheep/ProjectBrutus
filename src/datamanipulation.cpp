#include "datamanipulation.h"

DATASET_POSITON_SCORE::DATASET_POSITON_SCORE(const DATASET_OLD&                that) : P(that.P), O(that.O), depth(that.depth), selectivity(DATASET_DEFAULT_selectivity), score(that.score) { }
DATASET_POSITON_SCORE::DATASET_POSITON_SCORE(const DATASET_POSITON_SCORE_PV&   that) : P(that.P), O(that.O), depth(that.depth), selectivity(that.selectivity), score(that.score) { }
DATASET_POSITON_SCORE::DATASET_POSITON_SCORE(const DATASET_POSITON_FULL_SCORE& that) : P(that.P), O(that.O), depth(that.depth), selectivity(that.selectivity)
{
	score = SCHAR_MIN;
	for (int i = 0; i < 64; i++)
		if (that.score[i] > score)
			score = that.score[i];
}


void DATASET_POSITON_SCORE_PV::Reset()
{
	depth = DATASET_DEFAULT_depth;
	selectivity = DATASET_DEFAULT_selectivity;
	score = DATASET_DEFAULT_score;
	for (int i = 0; i < 5; i++)
		PV[i] = DATASET_DEFAULT_PV;
}

DATASET_POSITON_SCORE_PV::DATASET_POSITON_SCORE_PV() : P(0), O(0) { Reset(); }
DATASET_POSITON_SCORE_PV::DATASET_POSITON_SCORE_PV(const DATASET_OLD&                that) : P(that.P), O(that.O), depth(that.depth), selectivity(DATASET_DEFAULT_selectivity), score(that.score) { for (int i = 0; i < 5; i++) PV[i] = DATASET_DEFAULT_PV; }
DATASET_POSITON_SCORE_PV::DATASET_POSITON_SCORE_PV(const DATASET_POSITON_SCORE&      that) : P(that.P), O(that.O), depth(that.depth), selectivity(that.selectivity), score(that.score) { for (int i = 0; i < 5; i++) PV[i] = DATASET_DEFAULT_PV; }
DATASET_POSITON_SCORE_PV::DATASET_POSITON_SCORE_PV(const DATASET_POSITON_FULL_SCORE& that) : P(that.P), O(that.O), depth(that.depth), selectivity(that.selectivity)
{
	for (int i = 0; i < 5; i++)
		PV[i] = DATASET_DEFAULT_PV;

	score = SCHAR_MIN;
	for (int i = 0; i < 64; i++)
		if (that.score[i] > score){
			score = that.score[i];
			PV[0] = that.PV[i];
		}
}


void DATASET_POSITON_FULL_SCORE::Reset()
{
	depth = DATASET_DEFAULT_depth;
	selectivity = DATASET_DEFAULT_selectivity;
	for (int i = 0; i < 64; ++i){
		score[i] = DATASET_DEFAULT_score;
		PV[i] = DATASET_DEFAULT_PV;
	}
}

DATASET_POSITON_FULL_SCORE::DATASET_POSITON_FULL_SCORE() : P(0), O(0) { Reset(); }

DATASET_POSITON_FULL_SCORE::DATASET_POSITON_FULL_SCORE(const unsigned long long P, const unsigned long long O, const signed char depth, const unsigned char selectivity) : P(P), O(O), depth(depth), selectivity(selectivity)
{
	for (int i = 0; i < 64; ++i){
		score[i] = DATASET_DEFAULT_score;
		PV[i] = DATASET_DEFAULT_PV;
	}
}

DATASET_POSITON_FULL_SCORE::DATASET_POSITON_FULL_SCORE(const DATASET_OLD& that) : P(that.P), O(that.O), depth(that.depth), selectivity(DATASET_DEFAULT_selectivity)
{
	for (int i = 0; i < 64; ++i){
		score[i] = DATASET_DEFAULT_score;
		PV[i] = DATASET_DEFAULT_PV;
	}
	score[36] = that.score;
}
DATASET_POSITON_FULL_SCORE::DATASET_POSITON_FULL_SCORE(const DATASET_POSITON_SCORE& that) : P(that.P), O(that.O), depth(that.depth), selectivity(DATASET_DEFAULT_selectivity)
{
	for (int i = 0; i < 64; ++i){
		score[i] = DATASET_DEFAULT_score;
		PV[i] = DATASET_DEFAULT_PV;
	}
	score[36] = that.score;
}
DATASET_POSITON_FULL_SCORE::DATASET_POSITON_FULL_SCORE(const DATASET_POSITON_SCORE_PV& that) : P(that.P), O(that.O), depth(that.depth), selectivity(that.selectivity)
{
	for (int i = 0; i < 64; ++i){
		score[i] = DATASET_DEFAULT_score;
		PV[i] = DATASET_DEFAULT_PV;
	}

	if (that.PV[0] == DATASET_DEFAULT_PV)
		score[27] = that.score;
	else{
		PV[that.PV[0]] = that.PV[1];
		score[that.PV[0]] = that.score;
	}
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