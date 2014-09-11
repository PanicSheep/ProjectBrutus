#include "features.h"

void For_each_configuration_in_pattern_do_fkt(const unsigned long long Pattern, std::function<void(const unsigned long long, const unsigned long long)> fkt);

namespace Features
{
	const unsigned char BoxIndex[61] = {
		99,         //  0
		99, 99, 99, //  1,  2,  3
		99, 99,  0, //  4,  5,  6
		 0,  0,  0, //  7,  8,  9
		 1,  1,  1, // 10, 11, 12
		 2,  2,  2, // 13, 14, 15
		 3,  3,  3, // 16, 17, 18
		 4,  4,  4, // 19, 20, 21
		 5,  5,  5, // 22, 23, 24
		 6,  6,  6, // 25, 26, 27
		 7,  7,  7, // 28, 29, 30
		 8,  8,  8, // 31, 32, 33
		 9,  9,  9, // 34, 35, 36
		10, 10, 10, // 37, 38, 39
		11, 11, 11, // 40, 41, 42
		12, 12, 12, // 43, 44, 45
		13, 13, 13, // 46, 47, 48
		14, 14, 14, // 49, 50, 51
		14, 14, 14, // 52, 53, 54
		14, 14, 14, // 55, 56, 57
		14, 14, 14, // 58, 59, 60
	};

	std::vector<FeatureMetaGroup*> FeatureList;
	std::size_t Elements;
	std::size_t FullSize;
	std::size_t ReducedSize;

	unsigned short SumPow3[4096];

	unsigned long long sumpow3(unsigned long long exp)
	{
		long long sum = 0;
		while (exp)	{
			sum += pow(3LL, BitScanLSB(exp));
			RemoveLSB(exp);
		}
		return sum;
	}

	unsigned long long sym_flip(const unsigned long long P, const int sym)
	{
		switch (sym)
		{
		case 1: return horizontal_flip(P);
		case 2: return vertical_flip(P);
		case 3: return diagonal_flip(P);
		case 4: return codiagonal_flip(P);
		case 5: return vertical_flip(horizontal_flip(P));
		case 6: return diagonal_flip(horizontal_flip(P));
		case 7: return codiagonal_flip(horizontal_flip(P));
		default: return P;
		}
	}
	
	unsigned long long sym_flip_inv(const unsigned long long P, const int sym)
	{
		switch (sym)
		{
		case 1: return horizontal_flip(P);
		case 2: return vertical_flip(P);
		case 3: return diagonal_flip(P);
		case 4: return codiagonal_flip(P);
		case 5: return horizontal_flip(vertical_flip(P));
		case 6: return horizontal_flip(diagonal_flip(P));
		case 7: return horizontal_flip(codiagonal_flip(P));
		default: return P;
		}
	}

	unsigned long long Mirror(const unsigned long long B, const std::vector<int> & Generator)
	{
		unsigned long long ret = 0;
		for (int sym : Generator)
			ret |= sym_flip(B, sym);
		return ret;
	}

	void For_each_configuration_in_pattern_do_fkt(const unsigned long long Pattern, std::function<void(const unsigned long long, const unsigned long long)> fkt)
	{
		unsigned long long P, O;
		const unsigned long long PatternSize = 1ULL << PopCount(Pattern);
		const unsigned long long ExtractedCenter = PExt(0x0000001818000000ULL, Pattern);
		for (unsigned long long i = 0; i < PatternSize; i++)
		{
			P = PDep(i, Pattern);
			for (unsigned long long j = 0; j < PatternSize; j++)
			{
				if (i & j) continue;
				if (((i | j) & ExtractedCenter) != ExtractedCenter) continue;
				O = PDep(j, Pattern);

				fkt(P, O);
			}
		}
	}

	std::string Names()
	{
		std::string s;
		for (const auto& it : FeatureList)
			s.append(", " + it->Name());
		return s.substr(2);
	}


	template <>
	class FeatureGroup<0x0102040810204080ULL> : public FeatureMetaGroup
	{
	private:
		const unsigned long long m_BitPattern = 0x0102040810204080ULL;
		const std::string m_Name;
		int m_Elements;
		int m_FullSize;
		int m_ReducedSize;
		unsigned long long EigenBits, NonEigenBits;
		std::vector<int> m_Generators; //All the symmetries to genereate all congruent bit pattern.
		std::vector<int> m_Symmetries; //All the symmetries in m_BitPattern. id is not part of that.
		std::vector<unsigned long long> m_BitBoards;
		std::vector<std::vector<float>> m_weights;
	public:
		inline unsigned long long BitPattern() const { return m_BitPattern; }
		inline std::string Name() const { return m_Name; }
		inline int Elements() const { return m_Elements; }
		inline int FullSize() const { return m_FullSize; }
		inline int ReducedSize() const { return m_ReducedSize; }

		FeatureGroup(const std::string & name) : m_Name(name), m_FullSize(pow(3, PopCount(m_BitPattern)))
		{
			assert(PopCount(m_BitPattern) <= 12);
			
			m_Generators.push_back(0);
			m_Generators.push_back(1);
			m_Elements = m_Generators.size();

			m_Symmetries.push_back(3);

			EigenBits = 0ULL;
			NonEigenBits = 0ULL;
			unsigned long long tmpPattern = m_BitPattern;
			unsigned long long LSB, tmp;
			while (tmpPattern)
			{
				LSB = GetLSB(tmpPattern);
				RemoveLSB(tmpPattern);
				tmp = Mirror(LSB, m_Symmetries);
				if (LSB & tmp)
					EigenBits |= LSB;
				else{
					NonEigenBits |= LSB;
					tmpPattern ^= tmp;
				}
			}

			assert(EigenBits == 0);

			m_ReducedSize = pow(3, PopCount(NonEigenBits & ~0x0000001818000000ULL)) 
						  * pow(2, PopCount(NonEigenBits &  0x0000001818000000ULL));
			m_ReducedSize = m_ReducedSize * (m_ReducedSize + 1) / 2;
			m_ReducedSize *= pow(3, PopCount(EigenBits & ~0x0000001818000000ULL)) 
						   * pow(2, PopCount(EigenBits &  0x0000001818000000ULL));
			
			//Initialize BitBoards
			for (const auto& sym : m_Generators)
				m_BitBoards.push_back(sym_flip(m_BitPattern, sym));

			//Initialize weights
			m_weights.resize(m_Elements * BOXES);
			for (int i = 0; i < m_Elements * BOXES; ++i)
				m_weights[i].resize(m_FullSize);
			
		}

		void set_weights(std::vector<float>::const_iterator& weight_iter, const unsigned int BoxIndex)
		{
			unsigned long long tmpP, tmpO, tmpBitBoard;
			int FullIndex;
			int ReducedIndex;
			float w;
			int counter;
			int * Array = new int[m_Elements];
			
			For_each_configuration_in_pattern_do_fkt(m_BitPattern,
				[&](unsigned long long P, unsigned long long O)
				{
					FillConfigurationArray(P, O, Array);
					ReducedIndex = Array[0];
					w = *(weight_iter + ReducedIndex);
					counter = 0;
					for (const int sym : m_Generators)
					{
						tmpP = sym_flip(P, sym);
						tmpO = sym_flip(O, sym);
						tmpBitBoard = sym_flip(m_BitPattern, sym);
						FullIndex = SumPow3[PExt(tmpP, tmpBitBoard)] + 2 * SumPow3[PExt(tmpO, tmpBitBoard)];
						m_weights[BoxIndex * m_Elements + counter++][FullIndex] = w;
					}
				}
			);

			delete[] Array;
			weight_iter += m_ReducedSize;
		}

		float score(const unsigned long long P, const unsigned long long O, const unsigned int BoxIndex) const
		{
			unsigned long long tmpP, tmpO, tmpBitBoard;
			float result = 0;
			int index;

			for (int sym = 0; sym < m_Elements; ++sym)
			{
				tmpBitBoard = m_BitBoards[sym];
				tmpP = P & tmpBitBoard;
				tmpO = O & tmpBitBoard;
				index = SumPow3[PExt(tmpP, tmpBitBoard)] + 2 * SumPow3[PExt(tmpO, tmpBitBoard)];
				result += m_weights[BoxIndex * m_Elements + sym][index];
			}
			return result;
		}

		void FillConfigurationArray(const unsigned long long P, const unsigned long long O, int * Array) const
		{
			int index;

			// RoomForOptimization: pow(*, i) has to cast i from unsigned int to unsigned long long.
			const unsigned int size1 = PopCount(NonEigenBits & ~0x0000001818000000ULL);
			const unsigned int size2 = PopCount(NonEigenBits &  0x0000001818000000ULL);
			const unsigned int size3 = PopCount(   EigenBits & ~0x0000001818000000ULL);
			const unsigned int size4 = PopCount(   EigenBits &  0x0000001818000000ULL);
			const int diagtmp = pow(3, PopCount(   EigenBits & ~0x0000001818000000ULL)) * pow(2, PopCount(   EigenBits & 0x0000001818000000ULL));
			const int sidetmp = pow(3, PopCount(NonEigenBits & ~0x0000001818000000ULL)) * pow(2, PopCount(NonEigenBits & 0x0000001818000000ULL));
			unsigned long long tmpP, tmpO, tmpPattern, LSB;
			int indexA, indexB, indexD;
			for (int sym : m_Generators)
			{
				tmpP = sym_flip(P, sym);
				tmpO = sym_flip(O, sym);
										
				// Side A of non-eigen fields
				indexA = 0;
				tmpPattern = NonEigenBits & ~0x0000001818000000ULL;
				for (int i = 0; i < size1; ++i)
				{
					LSB = GetLSB(tmpPattern);
					RemoveLSB(tmpPattern);
					if (tmpP & LSB) indexA += 1 * pow(3, i);
					if (tmpO & LSB) indexA += 2 * pow(3, i);
				}
				tmpPattern = NonEigenBits & 0x0000001818000000ULL;
				for (int i = 0; i < size2; ++i)
				{
					LSB = GetLSB(tmpPattern);
					RemoveLSB(tmpPattern);
					if (tmpO & LSB) indexA += 1 * pow(2, i) * pow(3, size1); // RoomForOptimization: Move *pow(3, size1) out of loop.
				}
					
				// Side B of non-eigen fields
				indexB = 0;
				tmpPattern = NonEigenBits & ~0x0000001818000000ULL;
				for (int i = 0; i < size1; ++i)
				{
					LSB = Mirror(GetLSB(tmpPattern), m_Symmetries);
					RemoveLSB(tmpPattern);
					if (tmpP & LSB) indexB += 1 * pow(3, i);
					if (tmpO & LSB) indexB += 2 * pow(3, i);
				}
				tmpPattern = NonEigenBits & 0x0000001818000000ULL;
				for (int i = 0; i < size2; ++i)
				{
					LSB = Mirror(GetLSB(tmpPattern), m_Symmetries);
					RemoveLSB(tmpPattern);
					if (tmpO & LSB) indexB += 1 * pow(2, i) * pow(3, size1); // RoomForOptimization: Move *pow(3, size1) out of loop.
				}

				// Eigen fields
				indexD = 0;
				tmpPattern = EigenBits & ~0x0000001818000000ULL;
				for (unsigned int i = 0; i < size3; ++i)
				{
					LSB = GetLSB(tmpPattern);
					RemoveLSB(tmpPattern);
					if (tmpP & LSB) indexD += 1 * pow(3, i);
					if (tmpO & LSB) indexD += 2 * pow(3, i);
				}
				tmpPattern = EigenBits & 0x0000001818000000ULL;
				for (unsigned int i = 0; i < size4; ++i)
				{
					LSB = GetLSB(tmpPattern);
					RemoveLSB(tmpPattern);
					if (tmpO & LSB) indexD += 1 * pow(2, i) * pow(3, size3); // RoomForOptimization: Move *pow(3, size3) out of loop.
				}

				index = indexA > indexB ? diagtmp*(sidetmp*indexB+indexA - (((indexB+1)*indexB) >> 1)) + indexD : diagtmp*(sidetmp*indexA+indexB - (((indexA+1)*indexA) >> 1)) + indexD;
					
				*(Array++) = index;
			}
		}
	};


	void Initialize(const int conf)
	{
		for (int i = 0; i < 4096; i++) SumPow3[i] = sumpow3(i);

		FeatureList.push_back(new FeatureGroup<0x00000000000042FFULL>("L0+2X"));
		//FeatureList.push_back(new FeatureGroup<0x00000000000000FFULL>("L0"));
		FeatureList.push_back(new FeatureGroup<0x000000000000FF00ULL>("L1"));
		FeatureList.push_back(new FeatureGroup<0x0000000000FF0000ULL>("L2"));
		FeatureList.push_back(new FeatureGroup<0x00000000FF000000ULL>("L3"));

		if (conf & 0x001) FeatureList.push_back(new FeatureGroup<0x0000000001020408ULL>("D4"));
		if (conf & 0x002) FeatureList.push_back(new FeatureGroup<0x0000000102040810ULL>("D5"));
		FeatureList.push_back(new FeatureGroup<0x0000010204081020ULL>("D6"));
		FeatureList.push_back(new FeatureGroup<0x0001020408102040ULL>("D7"));
		//FeatureList.push_back(new FeatureGroup<0x0102040810204080ULL>("D8"));
		//FeatureList.push_back(new FeatureGroup<0x0000000103060C18ULL>("D4+D5"));
		
		if (  conf & 0x004 ) FeatureList.push_back(new FeatureGroup<0x000000000101030FULL>("C3+1"));
		if (!(conf & 0x004)) FeatureList.push_back(new FeatureGroup<0x000000010101031FULL>("C3+2"));
		//FeatureList.push_back(new FeatureGroup<0x000001010101033FULL>("C3+3"));
		//FeatureList.push_back(new FeatureGroup<0x000000000103070FULL>("C4"));
		//FeatureList.push_back(new FeatureGroup<0x000000010103071FULL>("C4+1"));
		
		//FeatureList.push_back(new FeatureGroup<0x000000000000C3FFULL>("Barbell"));
		if (conf & 0x008) FeatureList.push_back(new FeatureGroup<0x81000000000000FFULL>(":|"));
		//FeatureList.push_back(new FeatureGroup<0x81000000000042FFULL>("L0+2X+2C"));
		FeatureList.push_back(new FeatureGroup<0x00000000000003CBDLL>("E+"));
		
		FeatureList.push_back(new FeatureGroup<0x0000000000070707ULL>("Q0"));
		//FeatureList.push_back(new FeatureGroup<0x000000000E0E0E00ULL>("Q1"));
		//FeatureList.push_back(new FeatureGroup<0x0000001C1C1C0000ULL>("Q2"));
		//FeatureList.push_back(new FeatureGroup<0x0000000000247E00ULL>("TT"));
		//FeatureList.push_back(new FeatureGroup<0x00000000003C7E00ULL>("Py"));
		
		//FeatureList.push_back(new FeatureGroup<0x03010000000081C3ULL>("3L"));
		if (conf & 0x010) FeatureList.push_back(new FeatureGroup<0x03010000000083C3ULL>("3L+"));
		//FeatureList.push_back(new FeatureGroup<0x030300000000C3C3ULL>("3B"));
		
		//FeatureList.push_back(new FeatureGroup<0x8100000000010387ULL>("C3+Cs"));
		//FeatureList.push_back(new FeatureGroup<0x810000000101038FULL>("C3+1+Cs"));
		
		if (  conf & 0x020 ) FeatureList.push_back(new FeatureGroup<0x8040201008040303ULL>("Comet"));
		if (!(conf & 0x020)) FeatureList.push_back(new FeatureGroup<0x8040201008050307ULL>("Arrow"));
		//FeatureList.push_back(new FeatureGroup<0x804020100905030FULL>("Anchor"));

		//FeatureList.push_back(new FeatureGroup<0x0000000000000F0FULL>("B4"));
		if (conf & 0x040) FeatureList.push_back(new FeatureGroup<0x0000000000001F1FULL>("B5"));
		//FeatureList.push_back(new FeatureGroup<0x0000000000003F3FULL>("B6"));

		Elements = 0;
		FullSize = 0;
		ReducedSize = 0;
		for (const FeatureMetaGroup * const it : FeatureList) Elements += it->Elements();
		for (const FeatureMetaGroup * const it : FeatureList) FullSize += it->FullSize();
		for (const FeatureMetaGroup * const it : FeatureList) ReducedSize += it->ReducedSize();

		std::vector<std::string> Filenames;
		std::string s;
		for (int i = 0; i < BOXES; ++i)
		{
			s = "weights range " + std::to_string(i);
			if (ConfigFile::Configurations.count(s) > 0)
				Filenames.push_back(ConfigFile::Configurations[s]);
		}

		std::vector<float>::const_iterator v_float_citer;
		std::vector<double> v_double;
		for (std::size_t j = 0; j < Filenames.size(); ++j)
		{
			v_double = read_vector<double>(Filenames[j]);
			std::vector<float> v_float(v_double.begin(), v_double.end());
			if (v_float.size() != ReducedSize){
				std::cerr << "ERROR: Weight's file " << Filenames[j] << " size (" << v_float.size() << ") does not match expected size (" << ReducedSize << ")." << std::endl;
				continue;
			}
			v_float_citer = v_float.cbegin();
			for (FeatureMetaGroup * const it : FeatureList)
				it->set_weights(v_float_citer, j);
		}
	}

	void Finalize()
	{
		
	}
}

void FillConfigurationArray(const unsigned long long P, const unsigned long long O, int * Array)
{
	using namespace Features;
	int Offset = 0;
	for (const FeatureMetaGroup * const it : FeatureList)
	{
		it->FillConfigurationArray(P, O, Array);
		for (int i = 0; i < it->Elements(); ++i)
			Array[i] += Offset;
		Array += it->Elements();
		Offset += it->ReducedSize();
	}
}

int EvaluateFeatures(const unsigned long long P, const unsigned long long O)
{
	const int BoxIndex = Features::BoxIndex[NumberOfEmptyStones(P, O)];
	float sum = 0;

	for (const Features::FeatureMetaGroup * const it : Features::FeatureList)
		sum += it->score(P, O, BoxIndex);

	return RoundInt(sum);
}
int EvaluateFeatures(const unsigned long long P, const unsigned long long O, float * Array)
{
	const int BoxIndex = Features::BoxIndex[NumberOfEmptyStones(P, O)];
	float sum = 0;

	for (const Features::FeatureMetaGroup * const it : Features::FeatureList)
		sum += *Array++ = it->score(P, O, BoxIndex);

	return RoundInt(sum);
}


//bool Congruent(const unsigned long long P1, const unsigned long long P2)
//{
//	if (P1 == P2) return true;
//	if (P1 == horizontal_flip(P2)) return true;
//	if (P1 == vertical_flip(P2)) return true;
//	if (P1 == diagonal_flip(P2)) return true;
//	if (P1 == codiagonal_flip(P2)) return true;
//	if (P1 == vertical_flip(horizontal_flip(P2))) return true;
//	if (P1 == diagonal_flip(horizontal_flip(P2))) return true;
//	if (P1 == codiagonal_flip(horizontal_flip(P2))) return true;
//	return false;
//}
//
//bool Test_Patterns_PopCount(const int symmetries, const unsigned long long * const Pattern)
//{
//	bool Success = true;
//	unsigned long long * POPCOUNT = new unsigned long long[symmetries];
//
//	for (int i = 0; i < symmetries; i++)
//		POPCOUNT[i] = PopCount(Pattern[i]);
//	for (int i = 0; i < symmetries; i++)
//	{
//		if (POPCOUNT[0] != POPCOUNT[i]){
//			std::cerr << "ERROR: Pattern are inconsistent!" << std::endl;
//			Success = false;
//		}
//	}
//
//	delete[] POPCOUNT;
//	return Success;
//}
//
//bool Test_Pattern_Congruence(const int symmetries, const unsigned long long * const Pattern)
//{
//	bool Success = true;
//
//	for (int i = 0; i < symmetries; i++)
//	{
//		if (!Congruent(Pattern[0], Pattern[i])){
//			std::cerr << "ERROR: Pattern are not congruent!" << std::endl;
//			Success = false;
//		}
//	}
//
//	return Success;
//}
//
//void For_each_configuration_in_pattern_do_fkt(const unsigned long long Pattern, std::function<void(const unsigned long long, const unsigned long long)> fkt)
//{
//	unsigned long long P, O;
//	const unsigned long long PatternSize = 1ULL << PopCount(Pattern);
//	const unsigned long long ExtractedCenter = PExt(0x0000001818000000ULL, Pattern);
//	for (unsigned long long i = 0; i < PatternSize; i++)
//	{
//		P = PDep(i, Pattern);
//		for (unsigned long long j = 0; j < PatternSize; j++)
//		{
//			if (i & j) continue;
//			if (((i | j) & ExtractedCenter) != ExtractedCenter) continue;
//			O = PDep(j, Pattern);
//
//			fkt(P, O);
//		}
//	}
//
//}
//
//bool Test_Index_Range(const int symmetries, const int size, unsigned long long * Pattern, int(*Feature[])(const unsigned long long P, const unsigned long long O))
//{
//	bool Success = true;
//
//	for (int k = 0; k < symmetries; k++)
//	{
//		For_each_configuration_in_pattern_do_fkt(Pattern[k],
//			[Feature, k, size, &Success](const unsigned long long P, const unsigned long long O)
//				{
//					int index = Feature[k](P, O);
//					if (index < 0){
//						std::cerr << "ERROR: Index is too small!" << std::endl;
//						Success = false;
//					}
//					if (index > size){
//						std::cerr << "ERROR: Index is too big!" << std::endl;
//						Success = false;
//					}
//					return ;
//				}
//		);
//	}
//
//	return Success;
//}
//
//bool Test_Feature_symmetrie(const int symmetries, unsigned long long * Pattern, int(*Feature[])(const unsigned long long P, const unsigned long long O))
//{
//	bool Success = true;
//
//	For_each_configuration_in_pattern_do_fkt(Pattern[0],
//		[Feature, symmetries, Pattern, &Success](const unsigned long long P, const unsigned long long O)
//			{
//				int indexA = Feature[0](P, O);
//				int indexB;
//				for (int k = 0; k < symmetries; ++k)
//				{
//					if (Pattern[k] == horizontal_flip(Pattern[0])){
//						indexB = Feature[k](horizontal_flip(P), horizontal_flip(O));
//						if (indexA != indexB){
//							std::cerr << "ERROR: Indices don't match!" << std::endl;
//							Success = false;
//						}
//					}
//					if (Pattern[k] == vertical_flip(Pattern[0])){
//						indexB = Feature[k](vertical_flip(P), vertical_flip(O));
//						if (indexA != indexB){
//							std::cerr << "ERROR: Indices don't match!" << std::endl;
//							Success = false;
//						}
//					}
//					if (Pattern[k] == diagonal_flip(Pattern[0])){
//						indexB = Feature[k](diagonal_flip(P), diagonal_flip(O));
//						if (indexA != indexB){
//							std::cerr << "ERROR: Indices don't match!" << std::endl;
//							Success = false;
//						}
//					}
//					if (Pattern[k] == codiagonal_flip(Pattern[0])){
//						indexB = Feature[k](codiagonal_flip(P), codiagonal_flip(O));
//						if (indexA != indexB){
//							std::cerr << "ERROR: Indices don't match!" << std::endl;
//							Success = false;
//						}
//					}
//					if (Pattern[k] == vertical_flip(horizontal_flip(Pattern[0]))){
//						indexB = Feature[k](vertical_flip(horizontal_flip(P)), vertical_flip(horizontal_flip(O)));
//						if (indexA != indexB){
//							std::cerr << "ERROR: Indices don't match!" << std::endl;
//							Success = false;
//						}
//					}
//					if (Pattern[k] == diagonal_flip(horizontal_flip(Pattern[0]))){
//						indexB = Feature[k](diagonal_flip(horizontal_flip(P)), diagonal_flip(horizontal_flip(O)));
//						if (indexA != indexB){
//							std::cerr << "ERROR: Indices don't match!" << std::endl;
//							Success = false;
//						}
//					}
//					if (Pattern[k] == codiagonal_flip(horizontal_flip(Pattern[0]))){
//						indexB = Feature[k](codiagonal_flip(horizontal_flip(P)), codiagonal_flip(horizontal_flip(O)));
//						if (indexA != indexB){
//							std::cerr << "ERROR: Indices don't match!" << std::endl;
//							Success = false;
//						}
//					}
//				}
//			}
//	);
//
//	return Success;
//}
//
//bool Test_Feature_symmetrie_FullIndex(int * tmpWeights, const int symmetries, unsigned long long * Pattern, int(*Feature[])(const unsigned long long P, const unsigned long long O), int Offset)
//{
//	bool Success = true;
//
//	For_each_configuration_in_pattern_do_fkt(Pattern[0],
//		[&](const unsigned long long P, const unsigned long long O)
//			{
//				auto err = [&]{
//					std::cerr << "ERROR: Indices don't match!" << std::endl;
//					std::cerr << board2D(P, O) << "\n";
//					Success = false;
//				};
//				int indexA, indexB;
//				int weightA = tmpWeights[indexA = Offset + Feature[0](P, O)];
//				int weightB;
//				for (int k = 0; k < symmetries; ++k)
//				{
//					if (Pattern[k] == horizontal_flip(Pattern[0])){
//						weightB = tmpWeights[indexB = Offset + Feature[k](horizontal_flip(P), horizontal_flip(O))];
//						if (weightA != weightB) err();
//					}
//					if (Pattern[k] == vertical_flip(Pattern[0])){
//						weightB = tmpWeights[indexB = Offset + Feature[k](vertical_flip(P), vertical_flip(O))];
//						if (weightA != weightB) err();
//					}
//					if (Pattern[k] == diagonal_flip(Pattern[0])){
//						weightB = tmpWeights[indexB = Offset + Feature[k](diagonal_flip(P), diagonal_flip(O))];
//						if (weightA != weightB) err();
//					}
//					if (Pattern[k] == codiagonal_flip(Pattern[0])){
//						weightB = tmpWeights[indexB = Offset + Feature[k](codiagonal_flip(P), codiagonal_flip(O))];
//						if (weightA != weightB) err();
//					}
//					if (Pattern[k] == vertical_flip(horizontal_flip(Pattern[0]))){
//						weightB = tmpWeights[indexB = Offset + Feature[k](vertical_flip(horizontal_flip(P)), vertical_flip(horizontal_flip(O)))];
//						if (weightA != weightB) err();
//					}
//					if (Pattern[k] == diagonal_flip(horizontal_flip(Pattern[0]))){
//						weightB = tmpWeights[indexB = Offset + Feature[k](diagonal_flip(horizontal_flip(P)), diagonal_flip(horizontal_flip(O)))];
//						if (weightA != weightB) err();
//					}
//					if (Pattern[k] == codiagonal_flip(horizontal_flip(Pattern[0]))){
//						weightB = tmpWeights[indexB = Offset + Feature[k](codiagonal_flip(horizontal_flip(P)), codiagonal_flip(horizontal_flip(O)))];
//						if (weightA != weightB) err();
//					}
//				}
//			}
//	);
//
//	return Success;
//}
//
//bool Test_Index_Covering(const int symmetries, const int size, unsigned long long * Pattern, int(*Feature[])(const unsigned long long P, const unsigned long long O))
//{
//	bool Success = true;
//	int * taken = new int[size];
//
//	for (int k = 0; k < symmetries; k++)
//	{
//		for (int i = 0; i < size; i++)
//			taken[i] = 0;
//
//		For_each_configuration_in_pattern_do_fkt(Pattern[k],
//			[Feature, k, &taken](const unsigned long long P, const unsigned long long O){ taken[Feature[k](P, O)]++; }
//		);
//
//		for (int i = 0; i < size; i++)
//		{
//			if (taken[i] == 0){
//				std::cerr << "ERROR: Not every index is covered! Index " << i << " is never taken." << std::endl;
//				Success = false;
//			}
//		}
//	}
//	return Success;
//}
//
//bool Test(const int symmetries, const int size, unsigned long long * Pattern, int(*Feature[])(const unsigned long long P, const unsigned long long O), bool IsPattern)
//{
//	using namespace Features;
//	bool Success = true;
//	const unsigned long long POPCOUNT = PopCount(Pattern[0]);
//	unsigned long long * BitConfig = new unsigned long long[POPCOUNT];
//
//	// Test patern's population count to be equal
//	Success &= Test_Patterns_PopCount(symmetries, Pattern);
//
//	// Test congruence of patterns
//	Success &= Test_Pattern_Congruence(symmetries, Pattern);
//
//	// Test the range of all the features
//	Success &= Test_Index_Range(symmetries, size, Pattern, Feature);
//
//	// Test if all the symmetries of all configurations of all the features result in the same index
//	Success &= Test_Feature_symmetrie(symmetries, Pattern, Feature);
//
//	// Test if all possible indices are taken at least once
//	if (IsPattern)
//		Success &= Test_Index_Covering(symmetries, size, Pattern, Feature);
//
//	return Success;
//}
//
//bool Test_All_Features()
//{
//	using namespace Features;
//	bool Success = true;
//	int * tmpWeights = new int[FullSize];
//	int ReducedOffset = 0;
//	int FullOffset = 0;
//	For_each_configuration_in_pattern_do_fkt(Pattern_LowerC      , [&](unsigned long long P, unsigned long long O){	tmpWeights[FullIndex_LowerC      (P, O) + FullOffset] = ReducedIndex_LowerC      (P, O) + ReducedOffset; }); ReducedOffset += ReducedSize_C ; FullOffset += FullSize_C ;
//	For_each_configuration_in_pattern_do_fkt(Pattern_LowerL1     , [&](unsigned long long P, unsigned long long O){ tmpWeights[FullIndex_LowerL1     (P, O) + FullOffset] = ReducedIndex_LowerL1     (P, O) + ReducedOffset; }); ReducedOffset += ReducedSize_L1; FullOffset += FullSize_L1;
//	For_each_configuration_in_pattern_do_fkt(Pattern_LowerL2     , [&](unsigned long long P, unsigned long long O){ tmpWeights[FullIndex_LowerL2     (P, O) + FullOffset] = ReducedIndex_LowerL2     (P, O) + ReducedOffset; }); ReducedOffset += ReducedSize_L2; FullOffset += FullSize_L2;
//	For_each_configuration_in_pattern_do_fkt(Pattern_LowerL3     , [&](unsigned long long P, unsigned long long O){ tmpWeights[FullIndex_LowerL3     (P, O) + FullOffset] = ReducedIndex_LowerL3     (P, O) + ReducedOffset; }); ReducedOffset += ReducedSize_L3; FullOffset += FullSize_L3;
//	For_each_configuration_in_pattern_do_fkt(Pattern_RightLowerA , [&](unsigned long long P, unsigned long long O){ tmpWeights[FullIndex_RightLowerA (P, O) + FullOffset] = ReducedIndex_RightLowerA (P, O) + ReducedOffset; }); ReducedOffset += ReducedSize_A ; FullOffset += FullSize_A ;
//	For_each_configuration_in_pattern_do_fkt(Pattern_RightLowerSq, [&](unsigned long long P, unsigned long long O){ tmpWeights[FullIndex_RightLowerSq(P, O) + FullOffset] = ReducedIndex_RightLowerSq(P, O) + ReducedOffset; }); ReducedOffset += ReducedSize_Sq; FullOffset += FullSize_Sq;
//	For_each_configuration_in_pattern_do_fkt(Pattern_LowerRightB , [&](unsigned long long P, unsigned long long O){ tmpWeights[FullIndex_LowerRightB (P, O) + FullOffset] = ReducedIndex_LowerRightB (P, O) + ReducedOffset; }); ReducedOffset += ReducedSize_B ; FullOffset += FullSize_B ;
//	For_each_configuration_in_pattern_do_fkt(Pattern_RightLowerD4, [&](unsigned long long P, unsigned long long O){ tmpWeights[FullIndex_RightLowerD4(P, O) + FullOffset] = ReducedIndex_RightLowerD4(P, O) + ReducedOffset; }); ReducedOffset += ReducedSize_D4; FullOffset += FullSize_D4;
//	For_each_configuration_in_pattern_do_fkt(Pattern_RightLowerD5, [&](unsigned long long P, unsigned long long O){ tmpWeights[FullIndex_RightLowerD5(P, O) + FullOffset] = ReducedIndex_RightLowerD5(P, O) + ReducedOffset; }); ReducedOffset += ReducedSize_D5; FullOffset += FullSize_D5;
//	For_each_configuration_in_pattern_do_fkt(Pattern_RightLowerD6, [&](unsigned long long P, unsigned long long O){ tmpWeights[FullIndex_RightLowerD6(P, O) + FullOffset] = ReducedIndex_RightLowerD6(P, O) + ReducedOffset; }); ReducedOffset += ReducedSize_D6; FullOffset += FullSize_D6;
//	For_each_configuration_in_pattern_do_fkt(Pattern_RightLowerD7, [&](unsigned long long P, unsigned long long O){ tmpWeights[FullIndex_RightLowerD7(P, O) + FullOffset] = ReducedIndex_RightLowerD7(P, O) + ReducedOffset; }); ReducedOffset += ReducedSize_D7; FullOffset += FullSize_D7;
//		
//	unsigned long long Pattern_A[] = {Pattern_RightLowerA, Pattern_LeftUpperA, Pattern_LeftLowerA, Pattern_RightUpperA};
//	unsigned long long Pattern_B[] = {Pattern_LowerRightB, Pattern_LowerLeftB, Pattern_LeftLowerB, Pattern_LeftUpperB, Pattern_UpperLeftB, Pattern_UpperRightB, Pattern_RightUpperB, Pattern_RightLowerB};
//	unsigned long long Pattern_C[] = {Pattern_LowerC, Pattern_UpperC, Pattern_LeftC, Pattern_RightC};
//	unsigned long long Pattern_D3[] = {Pattern_RightLowerD3, Pattern_LeftUpperD3, Pattern_LeftLowerD3, Pattern_RightUpperD3};
//	unsigned long long Pattern_D4[] = {Pattern_RightLowerD4, Pattern_LeftUpperD4, Pattern_LeftLowerD4, Pattern_RightUpperD4};
//	unsigned long long Pattern_D5[] = {Pattern_RightLowerD5, Pattern_LeftUpperD5, Pattern_LeftLowerD5, Pattern_RightUpperD5};
//	unsigned long long Pattern_D6[] = {Pattern_RightLowerD6, Pattern_LeftUpperD6, Pattern_LeftLowerD6, Pattern_RightUpperD6};
//	unsigned long long Pattern_D7[] = {Pattern_RightLowerD7, Pattern_LeftUpperD7, Pattern_LeftLowerD7, Pattern_RightUpperD7};
//	unsigned long long Pattern_L1[] = {Pattern_LowerL1, Pattern_UpperL1, Pattern_LeftL1, Pattern_RightL1};
//	unsigned long long Pattern_L2[] = {Pattern_LowerL2, Pattern_UpperL2, Pattern_LeftL2, Pattern_RightL2};
//	unsigned long long Pattern_L3[] = {Pattern_LowerL3, Pattern_UpperL3, Pattern_LeftL3, Pattern_RightL3};
//	unsigned long long Pattern_Sq[] = {Pattern_RightLowerSq, Pattern_LeftUpperSq, Pattern_LeftLowerSq, Pattern_RightUpperSq};
//	int(*ReducedIndex_A[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_RightLowerA, ReducedIndex_LeftUpperA, ReducedIndex_LeftLowerA, ReducedIndex_RightUpperA};
//	int(*ReducedIndex_B[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_LowerRightB, ReducedIndex_LowerLeftB, ReducedIndex_LeftLowerB, ReducedIndex_LeftUpperB, ReducedIndex_UpperLeftB, ReducedIndex_UpperRightB, ReducedIndex_RightUpperB, ReducedIndex_RightLowerB};
//	int(*ReducedIndex_C[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_LowerC, ReducedIndex_UpperC, ReducedIndex_LeftC, ReducedIndex_RightC};
//	int(*ReducedIndex_D3[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_RightLowerD3, ReducedIndex_LeftUpperD3, ReducedIndex_LeftLowerD3, ReducedIndex_RightUpperD3};
//	int(*ReducedIndex_D4[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_RightLowerD4, ReducedIndex_LeftUpperD4, ReducedIndex_LeftLowerD4, ReducedIndex_RightUpperD4};
//	int(*ReducedIndex_D5[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_RightLowerD5, ReducedIndex_LeftUpperD5, ReducedIndex_LeftLowerD5, ReducedIndex_RightUpperD5};
//	int(*ReducedIndex_D6[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_RightLowerD6, ReducedIndex_LeftUpperD6, ReducedIndex_LeftLowerD6, ReducedIndex_RightUpperD6};
//	int(*ReducedIndex_D7[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_RightLowerD7, ReducedIndex_LeftUpperD7, ReducedIndex_LeftLowerD7, ReducedIndex_RightUpperD7};
//	int(*ReducedIndex_L1[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_LowerL1, ReducedIndex_UpperL1, ReducedIndex_LeftL1, ReducedIndex_RightL1};
//	int(*ReducedIndex_L2[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_LowerL2, ReducedIndex_UpperL2, ReducedIndex_LeftL2, ReducedIndex_RightL2};
//	int(*ReducedIndex_L3[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_LowerL3, ReducedIndex_UpperL3, ReducedIndex_LeftL3, ReducedIndex_RightL3};
//	int(*ReducedIndex_Sq[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_RightLowerSq, ReducedIndex_LeftUpperSq, ReducedIndex_LeftLowerSq, ReducedIndex_RightUpperSq};
//	int(*FullIndex_A[])(const unsigned long long P, const unsigned long long O) = {FullIndex_RightLowerA, FullIndex_LeftUpperA, FullIndex_LeftLowerA, FullIndex_RightUpperA};
//	int(*FullIndex_B[])(const unsigned long long P, const unsigned long long O) = {FullIndex_LowerRightB, FullIndex_LowerLeftB, FullIndex_LeftLowerB, FullIndex_LeftUpperB, FullIndex_UpperLeftB, FullIndex_UpperRightB, FullIndex_RightUpperB, FullIndex_RightLowerB};
//	int(*FullIndex_C[])(const unsigned long long P, const unsigned long long O) = {FullIndex_LowerC, FullIndex_UpperC, FullIndex_LeftC, FullIndex_RightC};
//	//int(*FullIndex_D3[])(const unsigned long long P, const unsigned long long O) = {FullIndex_RightLowerD3, FullIndex_LeftUpperD3, FullIndex_LeftLowerD3, FullIndex_RightUpperD3};
//	int(*FullIndex_D4[])(const unsigned long long P, const unsigned long long O) = {FullIndex_RightLowerD4, FullIndex_LeftUpperD4, FullIndex_LeftLowerD4, FullIndex_RightUpperD4};
//	int(*FullIndex_D5[])(const unsigned long long P, const unsigned long long O) = {FullIndex_RightLowerD5, FullIndex_LeftUpperD5, FullIndex_LeftLowerD5, FullIndex_RightUpperD5};
//	int(*FullIndex_D6[])(const unsigned long long P, const unsigned long long O) = {FullIndex_RightLowerD6, FullIndex_LeftUpperD6, FullIndex_LeftLowerD6, FullIndex_RightUpperD6};
//	int(*FullIndex_D7[])(const unsigned long long P, const unsigned long long O) = {FullIndex_RightLowerD7, FullIndex_LeftUpperD7, FullIndex_LeftLowerD7, FullIndex_RightUpperD7};
//	int(*FullIndex_L1[])(const unsigned long long P, const unsigned long long O) = {FullIndex_LowerL1, FullIndex_UpperL1, FullIndex_LeftL1, FullIndex_RightL1};
//	int(*FullIndex_L2[])(const unsigned long long P, const unsigned long long O) = {FullIndex_LowerL2, FullIndex_UpperL2, FullIndex_LeftL2, FullIndex_RightL2};
//	int(*FullIndex_L3[])(const unsigned long long P, const unsigned long long O) = {FullIndex_LowerL3, FullIndex_UpperL3, FullIndex_LeftL3, FullIndex_RightL3};
//	int(*FullIndex_Sq[])(const unsigned long long P, const unsigned long long O) = {FullIndex_RightLowerSq, FullIndex_LeftUpperSq, FullIndex_LeftLowerSq, FullIndex_RightUpperSq};
//
//	// Reduced Index
//	Success &= Test(Symmetry_A,  ReducedSize_A,  Pattern_A,  ReducedIndex_A, true);
//	Success &= Test(Symmetry_B,  ReducedSize_B,  Pattern_B,  ReducedIndex_B, true);
//	Success &= Test(Symmetry_C,  ReducedSize_C,  Pattern_C,  ReducedIndex_C, true);
//	Success &= Test(Symmetry_D3, ReducedSize_D3, Pattern_D3, ReducedIndex_D3, true);
//	Success &= Test(Symmetry_D4, ReducedSize_D4, Pattern_D4, ReducedIndex_D4, true);
//	Success &= Test(Symmetry_D5, ReducedSize_D5, Pattern_D5, ReducedIndex_D5, true);
//	Success &= Test(Symmetry_D6, ReducedSize_D6, Pattern_D6, ReducedIndex_D6, true);
//	Success &= Test(Symmetry_D7, ReducedSize_D7, Pattern_D7, ReducedIndex_D7, false);
//	Success &= Test(Symmetry_L1, ReducedSize_L1, Pattern_L1, ReducedIndex_L1, true);
//	Success &= Test(Symmetry_L2, ReducedSize_L2, Pattern_L2, ReducedIndex_L2, true);
//	Success &= Test(Symmetry_L3, ReducedSize_L3, Pattern_L3, ReducedIndex_L3, false);
//	Success &= Test(Symmetry_Sq, ReducedSize_Sq, Pattern_Sq, ReducedIndex_Sq, true);
//
//	//FullIndex
//	FullOffset = 0;
//	Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_C,  Pattern_C,  FullIndex_C , FullOffset); FullOffset += FullSize_C ;
//	Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_L1, Pattern_L1, FullIndex_L1, FullOffset); FullOffset += FullSize_L1;
//	Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_L2, Pattern_L2, FullIndex_L2, FullOffset); FullOffset += FullSize_L2;
//	Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_L3, Pattern_L3, FullIndex_L3, FullOffset); FullOffset += FullSize_L3;
//	Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_A,  Pattern_A,  FullIndex_A , FullOffset); FullOffset += FullSize_A ;
//	Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_Sq, Pattern_Sq, FullIndex_Sq, FullOffset); FullOffset += FullSize_Sq;
//	Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_B,  Pattern_B,  FullIndex_B , FullOffset); FullOffset += FullSize_B ;
//	//Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_D3, Pattern_D3, FullIndex_D3, FullOffset); FullOffset += FullSize_D3;
//	Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_D4, Pattern_D4, FullIndex_D4, FullOffset); FullOffset += FullSize_D4;
//	Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_D5, Pattern_D5, FullIndex_D5, FullOffset); FullOffset += FullSize_D5;
//	Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_D6, Pattern_D6, FullIndex_D6, FullOffset); FullOffset += FullSize_D6;
//	Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_D7, Pattern_D7, FullIndex_D7, FullOffset); FullOffset += FullSize_D7;
//	
//	delete[] tmpWeights;
//	return Success;
//}