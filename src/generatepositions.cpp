#include "generatepositions.h"

#pragma pack(1)
class CBigNode
{
	unsigned long long m_P, m_O;
	char m_depth;

public:
	CBigNode() : m_P(0), m_O(0), m_depth(-1) {}

	void Update(const unsigned long long P, const unsigned long long O, const int depth){
		if (((P != m_P) || (O != m_O)) && (depth > m_depth)){
			m_P = P;
			m_O = O;
			m_depth = depth;
		}
	}
	bool LookUp(const unsigned long long P, const unsigned long long O, const int depth) { return (P == m_P) && (O == m_O) && (depth == m_depth); }
	void Clear(){
		m_P = 0;
		m_O = 0;
		m_depth = -1;
	}
};
#pragma pack()

class CHashTable
{
	typedef CBigNode NodeType;
public:
	CHashTable(const unsigned int Bits) : Bits(Bits), BitShift(64 - Bits) { m_table = new NodeType[1ULL << Bits]; }
	~CHashTable() { delete[] m_table; }
	void Update(const unsigned long long P, const unsigned long long O, const unsigned long long depth){ m_table[Hash(P, O)].Update(P, O, depth); }
	bool LookUp(const unsigned long long P, const unsigned long long O, const unsigned long long depth){ return m_table[Hash(P, O)].LookUp(P, O, depth); }

	void Clear(){
		for (std::size_t i = 0; i < (1ULL << Bits); i++)
			m_table[i].Clear();
	}
private:
	NodeType* m_table;
	const unsigned int Bits;
	const unsigned int BitShift;

	inline std::size_t Hash(unsigned long long P, unsigned long long O) const
	{
		P ^= P >> 33;
		O ^= O >> 33;
		P *= 0xFF14AFD7ED558CCDULL;
		O *= 0xFF14AFD7ED558CCDULL;
		O ^= O >> 33;
		return (P + O + (O << 41)) >> BitShift;
	}
};

std::set<CPosition> PositionSet;
std::atomic<std::size_t> size;
std::atomic_flag spinlock;
CHashTable * HashTable;

void Work(const unsigned long long N, const int Empties, const bool ETH)
{
	auto local_rnd = std::bind(std::uniform_int_distribution<unsigned int>(0,64), std::mt19937_64(std::chrono::system_clock::now().time_since_epoch().count() + std::this_thread::get_id().hash()));
	unsigned long long P, O, BitBoardPossible;
	int moves;
	
	do
	{
		ResetPosition(P, O, ETH);
		moves = 60 - Empties;
		while (moves > 0)
		{
			BitBoardPossible = PossibleMoves(P, O);
			if (!BitBoardPossible){
				std::swap(P, O);
				BitBoardPossible = PossibleMoves(P, O);
				if (!BitBoardPossible){
					ResetPosition(P, O, ETH);
					moves = 60 - Empties;
					continue;
				}
			}
			for (int k = local_rnd() % PopCount(BitBoardPossible); k > 0; k--)
				RemoveLSB(BitBoardPossible);
			PlayStone(P, O, BitScanLSB(BitBoardPossible));
			moves--;
		}

		while (spinlock.test_and_set(std::memory_order_acquire));
		if (size.load() < N){
			if (PositionSet.count(CPosition(P, O)) == 0){
				PositionSet.insert(CPosition(P, O));
				size.fetch_add(1, std::memory_order_seq_cst);
			}
		}
		spinlock.clear(std::memory_order_release);

	} while (size.load() < N);
}

void GenerateRandomPositions(const std::string & filename, const unsigned long long N, const int Empties, const bool ETH)
{
	std::size_t nthreads = std::thread::hardware_concurrency();
	PositionSet.clear();
	size = 0;
	spinlock.clear(std::memory_order_release);
	std::vector<std::thread> threads(nthreads - 1);
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	startTime = std::chrono::high_resolution_clock::now(); //Start Time

	// Start workers
	for (int i = 0; i < nthreads - 1; ++i)
		threads[i] = std::thread(Work, N, Empties, ETH);
	Work(N, Empties, ETH);

	// Join workers
	for (auto& t : threads)
		t.join();

	endTime = std::chrono::high_resolution_clock::now(); //Stop Time

	std::vector<CDataset_Old> tmp_OLD;
	std::vector<CDataset_Position_Score> tmp_POSITON_SCORE;
	std::vector<CDataset_Position_Score_PV> tmp_POSITON_SCORE_PV;
	std::vector<CDataset_Position_FullScore> tmp_POSITON_FULL_SCORE;
	CDataset_Old data_OLD;
	CDataset_Position_Score data_POSITON_SCORE;
	CDataset_Position_Score_PV data_POSITON_SCORE_PV;
	CDataset_Position_FullScore data_POSITON_FULL_SCORE;
	std::string Ending = filename.substr(filename.rfind(".") + 1, filename.length());

	switch (Ending_to_DataType(Ending))
	{
	case DataType::Old:
		for (auto& it : PositionSet){
			data_OLD.Reset();
			data_OLD.P = it.P;
			data_OLD.O = it.O;
			tmp_OLD.push_back(data_OLD);
		}
		std::random_shuffle(tmp_OLD.begin(), tmp_OLD.end());
		write_to_file(filename, tmp_OLD);
		tmp_OLD.clear();
		break;
	case DataType::Position_Score:
		for (auto& it : PositionSet){
			data_POSITON_SCORE.Reset();
			data_POSITON_SCORE.P = it.P;
			data_POSITON_SCORE.O = it.O;
			tmp_POSITON_SCORE.push_back(data_POSITON_SCORE);
		}
		std::random_shuffle(tmp_POSITON_SCORE.begin(), tmp_POSITON_SCORE.end());
		write_to_file(filename, tmp_POSITON_SCORE);
		tmp_POSITON_SCORE.clear();
		break;
	case DataType::Position_Score_PV:
		for (auto& it : PositionSet){
			data_POSITON_SCORE_PV.Reset();
			data_POSITON_SCORE_PV.P = it.P;
			data_POSITON_SCORE_PV.O = it.O;
			tmp_POSITON_SCORE_PV.push_back(data_POSITON_SCORE_PV);
		}
		std::random_shuffle(tmp_POSITON_SCORE_PV.begin(), tmp_POSITON_SCORE_PV.end());
		write_to_file(filename, tmp_POSITON_SCORE_PV);
		tmp_POSITON_SCORE_PV.clear();
		break;
	case DataType::Position_FullScore:
		for (auto& it : PositionSet){
			data_POSITON_FULL_SCORE.Reset();
			data_POSITON_FULL_SCORE.P = it.P;
			data_POSITON_FULL_SCORE.O = it.O;
			tmp_POSITON_FULL_SCORE.push_back(data_POSITON_FULL_SCORE);
		}
		std::random_shuffle(tmp_POSITON_FULL_SCORE.begin(), tmp_POSITON_FULL_SCORE.end());
		write_to_file(filename, tmp_POSITON_FULL_SCORE);
		tmp_POSITON_FULL_SCORE.clear();
		break;
	}
}

void perft(unsigned long long P, unsigned long long O, const char depth)
{
	if (depth == 0)
	{
		if (PositionSet.count(CPosition(P, O)) == 0)
			PositionSet.insert(CPosition(P, O));
		return;
	}

	unsigned char LSB;
	unsigned long long BitBoardPossible = PossibleMoves(P, O);
	unsigned long long flipped;

	if (BitBoardPossible == 0)
	{
		if (PossibleMoves(O, P))
			perft(O, P, depth);
		return;
	}

	if (HashTable->LookUp(P, O, depth))
		return;

	while (BitBoardPossible)
	{
		LSB = BitScanLSB(BitBoardPossible);
		RemoveLSB(BitBoardPossible);
		flipped = flip(P, O, LSB);
		perft(O ^ flipped, P ^ flipped ^ (1ULL << LSB), depth - 1);
	}

	HashTable->Update(P, O, depth);
}

void GeneratePerftPositions(const std::string & filename, const char depth, const bool ETH)
{
	unsigned long long P1, O1, P2, O2, PosMov;
	ResetPosition(P1, O1, ETH);
	ResetPosition(P2, O2, ETH);
	PosMov = PossibleMoves(P1, O1);
	PlayStone(P1, O1, BitScanLSB(PosMov));
	RemoveLSB(PosMov);
	PlayStone(P2, O2, BitScanLSB(PosMov));
	HashTable = new CHashTable(24);
	PositionSet.clear();
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	startTime = std::chrono::high_resolution_clock::now(); //Start Time

	perft(P1, O1, depth - 1);
	perft(P2, O2, depth - 1);

	endTime = std::chrono::high_resolution_clock::now(); //Stop Time

	delete HashTable;

	std::cout << "Perft(" << (int)depth << "): " << PositionSet.size() << std::endl;
	std::cout << time_format(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime)) << std::endl;

	std::vector<CDataset_Old> tmp_OLD;
	std::vector<CDataset_Position_Score> tmp_POSITON_SCORE;
	std::vector<CDataset_Position_Score_PV> tmp_POSITON_SCORE_PV;
	std::vector<CDataset_Position_FullScore> tmp_POSITON_FULL_SCORE;
	CDataset_Old data_OLD;
	CDataset_Position_Score data_POSITON_SCORE;
	CDataset_Position_Score_PV data_POSITON_SCORE_PV;
	CDataset_Position_FullScore data_POSITON_FULL_SCORE;
	std::string Ending = filename.substr(filename.rfind(".") + 1, filename.length());

	switch (Ending_to_DataType(Ending))
	{
	case DataType::Old:
		for (auto& it : PositionSet){
			data_OLD.Reset();
			data_OLD.P = it.P;
			data_OLD.O = it.O;
			tmp_OLD.push_back(data_OLD);
		}
		write_to_file(filename, tmp_OLD);
		tmp_OLD.clear();
		break;
	case DataType::Position_Score:
		for (auto& it : PositionSet){
			data_POSITON_SCORE.Reset();
			data_POSITON_SCORE.P = it.P;
			data_POSITON_SCORE.O = it.O;
			tmp_POSITON_SCORE.push_back(data_POSITON_SCORE);
		}
		write_to_file(filename, tmp_POSITON_SCORE);
		tmp_POSITON_SCORE.clear();
		break;
	case DataType::Position_Score_PV:
		for (auto& it : PositionSet){
			data_POSITON_SCORE_PV.Reset();
			data_POSITON_SCORE_PV.P = it.P;
			data_POSITON_SCORE_PV.O = it.O;
			tmp_POSITON_SCORE_PV.push_back(data_POSITON_SCORE_PV);
		}
		write_to_file(filename, tmp_POSITON_SCORE_PV);
		tmp_POSITON_SCORE_PV.clear();
		break;
	case DataType::Position_FullScore:
		for (auto& it : PositionSet){
			data_POSITON_FULL_SCORE.Reset();
			data_POSITON_FULL_SCORE.P = it.P;
			data_POSITON_FULL_SCORE.O = it.O;
			tmp_POSITON_FULL_SCORE.push_back(data_POSITON_FULL_SCORE);
		}
		write_to_file(filename, tmp_POSITON_FULL_SCORE);
		tmp_POSITON_FULL_SCORE.clear();
		break;
	}
}