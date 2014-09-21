#pragma once
#include "configfile.h"
#include "datamanipulation.h"
#include "position.h"
#include "utility.h"
#include <functional>
#include <iostream>
#include <vector>
#include <map>

extern const bool PATTERN_L02X  ;
extern const bool PATTERN_L1    ;
extern const bool PATTERN_L2    ;
extern const bool PATTERN_L3    ;
extern const bool PATTERN_D4    ;
extern const bool PATTERN_D5    ;
extern const bool PATTERN_D6    ;
extern const bool PATTERN_D7    ;
extern const bool PATTERN_Comet ;
extern const bool PATTERN_Eplus ;
extern const bool PATTERN_C3p1  ;
extern const bool PATTERN_C3p2  ;
extern const bool PATTERN_Q0    ;
extern const bool PATTERN_B5    ;

namespace Features
{
	template <unsigned long long b>
	class Flip
	{
	public:
		static const unsigned long long          c = ((b >> 1) & 0x5555555555555555ULL) | ((b << 1) & 0xAAAAAAAAAAAAAAAAULL);
		static const unsigned long long          d = ((c >> 2) & 0x3333333333333333ULL) | ((c << 2) & 0xCCCCCCCCCCCCCCCCULL);
		static const unsigned long long horizontal = ((d >> 4) & 0x0F0F0F0F0F0F0F0FULL) | ((d << 4) & 0xF0F0F0F0F0F0F0F0ULL);
		
		static const unsigned long long          e = ((b >>  8) & 0x00FF00FF00FF00FFULL) | ((b <<  8) & 0xFF00FF00FF00FF00ULL);
		static const unsigned long long          f = ((e >> 16) & 0x0000FFFF0000FFFFULL) | ((e << 16) & 0xFFFF0000FFFF0000ULL);
		static const unsigned long long vertical   = ((f >> 32) & 0x00000000FFFFFFFFULL) | ((f << 32) & 0xFFFFFFFF00000000ULL);
		
		static const unsigned long long          g =    (b ^ (b >>  7)) & 0x00AA00AA00AA00AAULL;
		static const unsigned long long          h = b ^ g ^ (g <<  7);
		static const unsigned long long          i =    (h ^ (h >> 14)) & 0x0000CCCC0000CCCCULL;
		static const unsigned long long          j = h ^ i ^ (i << 14);
		static const unsigned long long          k =    (j ^ (j >> 28)) & 0x00000000F0F0F0F0ULL;
		static const unsigned long long diagonal   = j ^ k ^ (k << 28);
		
		static const unsigned long long          l =      b ^ (b << 36);
		static const unsigned long long          m = b ^ (l ^ (b >> 36)) & 0xF0F0F0F00F0F0F0FULL;
		static const unsigned long long          n =     (m ^ (m << 18)) & 0xCCCC0000CCCC0000ULL;
		static const unsigned long long          o = m ^  n ^ (n >> 18);
		static const unsigned long long          p =     (o ^ (o <<  9)) & 0xAA00AA00AA00AA00ULL;
		static const unsigned long long codiagonal = o ^  p ^ (p >>  9);
	};
	
	const int POW_2_0 = 1;
	const int POW_2_1 = 2 * POW_2_0;
	const int POW_2_2 = 2 * POW_2_1;
	const int POW_2_3 = 2 * POW_2_2;
	const int POW_2_4 = 2 * POW_2_3;
	const int POW_2_5 = 2 * POW_2_4;
	const int POW_2_6 = 2 * POW_2_5;
	const int POW_2_7 = 2 * POW_2_6;
	const int POW_2_8 = 2 * POW_2_7;
	const int POW_2_9 = 2 * POW_2_8;
	const int POW_2_10 = 2 * POW_2_9;

	const int POW_3_0 = 1;
	const int POW_3_1 = 3 * POW_3_0;
	const int POW_3_2 = 3 * POW_3_1;
	const int POW_3_3 = 3 * POW_3_2;
	const int POW_3_4 = 3 * POW_3_3;
	const int POW_3_5 = 3 * POW_3_4;
	const int POW_3_6 = 3 * POW_3_5;
	const int POW_3_7 = 3 * POW_3_6;
	const int POW_3_8 = 3 * POW_3_7;
	const int POW_3_9 = 3 * POW_3_8;
	const int POW_3_10 = 3 * POW_3_9;
	
	extern unsigned short SumPow3[1024];
	extern int ReducedSize;
	extern int NumberOfPattern;
	
	inline unsigned short FullPatternIndex(const unsigned long long P, const unsigned long long O, const unsigned long long mask) { return SumPow3[PExt(P, mask)] + (SumPow3[PExt(O, mask)] << 1); }

	void For_each_configuration_in_pattern_do_fkt(const unsigned long long Pattern, std::function<void(const unsigned long long, const unsigned long long)> fkt);
	void Initialize();
	void Finalize();

	class CIndexArray
	{
	public:
		unsigned long long BitBoard;
		std::vector<int> Indices;
	};
	
	
	class CPattern_L02X
	{
		static const int halfSize = POW_3_5;
		static const unsigned long long Pattern = 0x00000000000042FFULL;
		static const unsigned long long PatternH = Flip<Pattern>::horizontal;
		static const unsigned long long PatternV = Flip<Pattern>::vertical;
		static const unsigned long long PatternD = Flip<Pattern>::diagonal;
		static const unsigned long long PatternC = Flip<Pattern>::codiagonal;
		static const unsigned long long PatternHV = Flip<Flip<Pattern>::horizontal>::vertical;
		static const unsigned long long PatternHD = Flip<Flip<Pattern>::horizontal>::diagonal;
		static const unsigned long long PatternHC = Flip<Flip<Pattern>::horizontal>::codiagonal;

		std::vector<std::vector<float>> m_weights; //m_weights[Index][FullIndex]

		static int ReducedPatternIndex0(const unsigned long long P, const unsigned long long O)
		{
			const int indexA = FullPatternIndex(                P ,                 O , 0x000000000000020FULL);
			const int indexB = FullPatternIndex(horizontal_flip(P), horizontal_flip(O), 0x000000000000020FULL);
			return (indexA > indexB) ? (halfSize*indexB + indexA - (((indexB + 1)*indexB) >> 1)) : (halfSize*indexA + indexB - (((indexA + 1)*indexA) >> 1));
		}
		static inline int ReducedPatternIndex1(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(codiagonal_flip(P), codiagonal_flip(O)); }
		static inline int ReducedPatternIndex2(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(vertical_flip(P), vertical_flip(O)); }
		static inline int ReducedPatternIndex3(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(diagonal_flip(P), diagonal_flip(O)); }

	public:
		static const int FullSize = POW_3_10;
		static const int ReducedSize = halfSize * (halfSize + 1) / 2;

		CPattern_L02X()
		{
			//Initialize weights
			m_weights.resize(4);
			for (int i = 0; i < 4; ++i)
				m_weights[i].resize(FullSize);
		}
		void set_weights(const std::vector<float>& compressed_weights)
		{
			int ReducedIndex;
			int FullIndex;

			For_each_configuration_in_pattern_do_fkt(Pattern,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex0(P, O);
					FullIndex = FullPatternIndex(P, O, Pattern);
					m_weights[0][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternC,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex1(P, O);
					FullIndex = FullPatternIndex(P, O, PatternC);
					m_weights[1][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternV,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex2(P, O);
					FullIndex = FullPatternIndex(P, O, PatternV);
					m_weights[2][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternD,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex3(P, O);
					FullIndex = FullPatternIndex(P, O, PatternD);
					m_weights[3][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
		}
	
		inline float score(const unsigned long long P, const unsigned long long O) const
		{
			return m_weights[0][FullPatternIndex(P, O, Pattern )]
				 + m_weights[1][FullPatternIndex(P, O, PatternC)]
				 + m_weights[2][FullPatternIndex(P, O, PatternV)]
				 + m_weights[3][FullPatternIndex(P, O, PatternD)];
		}
	

		static void FillConfigurationVec(const unsigned long long P, const unsigned long long O, std::vector<int>& vec, int Offset = 0)
		{
			vec.push_back(ReducedPatternIndex0(P, O) + Offset);
			vec.push_back(ReducedPatternIndex1(P, O) + Offset);
			vec.push_back(ReducedPatternIndex2(P, O) + Offset);
			vec.push_back(ReducedPatternIndex3(P, O) + Offset);
		}
	};

	class CPattern_L1
	{
		static const int halfSize = POW_3_4;
		static const unsigned long long Pattern = 0x000000000000FF00ULL;
		static const unsigned long long PatternH = Flip<Pattern>::horizontal;
		static const unsigned long long PatternV = Flip<Pattern>::vertical;
		static const unsigned long long PatternD = Flip<Pattern>::diagonal;
		static const unsigned long long PatternC = Flip<Pattern>::codiagonal;
		static const unsigned long long PatternHV = Flip<Flip<Pattern>::horizontal>::vertical;
		static const unsigned long long PatternHD = Flip<Flip<Pattern>::horizontal>::diagonal;
		static const unsigned long long PatternHC = Flip<Flip<Pattern>::horizontal>::codiagonal;

		std::vector<float> m_weights; //m_weights[FullIndex]

		static int ReducedPatternIndex0(const unsigned long long P, const unsigned long long O)
		{
			const int indexA = FullPatternIndex(                P ,                 O , 0x0000000000000F00ULL);
			const int indexB = FullPatternIndex(horizontal_flip(P), horizontal_flip(O), 0x0000000000000F00ULL);
			return (indexA > indexB) ? (halfSize*indexB + indexA - (((indexB + 1)*indexB) >> 1)) : (halfSize*indexA + indexB - (((indexA + 1)*indexA) >> 1));
		}
		static inline int ReducedPatternIndex1(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(codiagonal_flip(P), codiagonal_flip(O)); }
		static inline int ReducedPatternIndex2(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(vertical_flip(P), vertical_flip(O)); }
		static inline int ReducedPatternIndex3(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(diagonal_flip(P), diagonal_flip(O)); }

	public:
		static const int FullSize = POW_3_8;
		static const int ReducedSize = halfSize * (halfSize + 1) / 2;

		CPattern_L1() { m_weights.resize(FullSize); }
		void set_weights(const std::vector<float>& compressed_weights)
		{
			int ReducedIndex;
			int FullIndex;

			For_each_configuration_in_pattern_do_fkt(Pattern,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex0(P, O);
					FullIndex = FullPatternIndex(P, O, Pattern);
					m_weights[FullIndex] = compressed_weights[ReducedIndex];
				}
			);
		}
	
		inline float score(const unsigned long long P, const unsigned long long O) const
		{
			return m_weights[FullPatternIndex(P, O, Pattern )]
				 + m_weights[FullPatternIndex(P, O, PatternC)]
				 + m_weights[FullPatternIndex(P, O, PatternV)]
				 + m_weights[FullPatternIndex(P, O, PatternD)];
		}
	

		static void FillConfigurationVec(const unsigned long long P, const unsigned long long O, std::vector<int>& vec, int Offset = 0)
		{
			vec.push_back(ReducedPatternIndex0(P, O) + Offset);
			vec.push_back(ReducedPatternIndex1(P, O) + Offset);
			vec.push_back(ReducedPatternIndex2(P, O) + Offset);
			vec.push_back(ReducedPatternIndex3(P, O) + Offset);
		}
	};
	
	class CPattern_L2
	{
		static const int halfSize = POW_3_4;
		static const unsigned long long Pattern = 0x0000000000FF0000ULL;
		static const unsigned long long PatternH = Flip<Pattern>::horizontal;
		static const unsigned long long PatternV = Flip<Pattern>::vertical;
		static const unsigned long long PatternD = Flip<Pattern>::diagonal;
		static const unsigned long long PatternC = Flip<Pattern>::codiagonal;
		static const unsigned long long PatternHV = Flip<Flip<Pattern>::horizontal>::vertical;
		static const unsigned long long PatternHD = Flip<Flip<Pattern>::horizontal>::diagonal;
		static const unsigned long long PatternHC = Flip<Flip<Pattern>::horizontal>::codiagonal;

		std::vector<float> m_weights; //m_weights[FullIndex]
		
		static int ReducedPatternIndex0(const unsigned long long P, const unsigned long long O)
		{
			const int indexA = FullPatternIndex(                P ,                 O , 0x00000000000F0000ULL);
			const int indexB = FullPatternIndex(horizontal_flip(P), horizontal_flip(O), 0x00000000000F0000ULL);
			return (indexA > indexB) ? (halfSize*indexB + indexA - (((indexB + 1)*indexB) >> 1)) : (halfSize*indexA + indexB - (((indexA + 1)*indexA) >> 1));
		}
		static inline int ReducedPatternIndex1(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(codiagonal_flip(P), codiagonal_flip(O)); }
		static inline int ReducedPatternIndex2(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(vertical_flip(P), vertical_flip(O)); }
		static inline int ReducedPatternIndex3(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(diagonal_flip(P), diagonal_flip(O)); }

	public:
		static const int FullSize = POW_3_8;
		static const int ReducedSize = halfSize * (halfSize + 1) / 2;

		CPattern_L2() { m_weights.resize(FullSize); }
		void set_weights(const std::vector<float>& compressed_weights)
		{
			int ReducedIndex;
			int FullIndex;

			For_each_configuration_in_pattern_do_fkt(Pattern,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex0(P, O);
					FullIndex = FullPatternIndex(P, O, Pattern);
					m_weights[FullIndex] = compressed_weights[ReducedIndex];
				}
			);
		}
	
		inline float score(const unsigned long long P, const unsigned long long O) const
		{
			return m_weights[FullPatternIndex(P, O, Pattern )]
				 + m_weights[FullPatternIndex(P, O, PatternC)]
				 + m_weights[FullPatternIndex(P, O, PatternV)]
				 + m_weights[FullPatternIndex(P, O, PatternD)];
		}
	

		static void FillConfigurationVec(const unsigned long long P, const unsigned long long O, std::vector<int>& vec, int Offset = 0)
		{
			vec.push_back(ReducedPatternIndex0(P, O) + Offset);
			vec.push_back(ReducedPatternIndex1(P, O) + Offset);
			vec.push_back(ReducedPatternIndex2(P, O) + Offset);
			vec.push_back(ReducedPatternIndex3(P, O) + Offset);
		}
	};
	
	class CPattern_L3
	{
		static const int halfSize = POW_3_3 * POW_2_1;
		static const unsigned long long Pattern = 0x00000000FF000000ULL;
		static const unsigned long long PatternH = Flip<Pattern>::horizontal;
		static const unsigned long long PatternV = Flip<Pattern>::vertical;
		static const unsigned long long PatternD = Flip<Pattern>::diagonal;
		static const unsigned long long PatternC = Flip<Pattern>::codiagonal;
		static const unsigned long long PatternHV = Flip<Flip<Pattern>::horizontal>::vertical;
		static const unsigned long long PatternHD = Flip<Flip<Pattern>::horizontal>::diagonal;
		static const unsigned long long PatternHC = Flip<Flip<Pattern>::horizontal>::codiagonal;

		std::vector<float> m_weights; //m_weights[FullIndex]

		static int ReducedPatternIndex0(const unsigned long long P, const unsigned long long O)
		{
			const int indexA = FullPatternIndex(                P ,                 O , 0x0000000007000000ULL) + ((                O  & 0x0000000008000000ULL) ? POW_3_3 : 0);
			const int indexB = FullPatternIndex(horizontal_flip(P), horizontal_flip(O), 0x0000000007000000ULL) + ((horizontal_flip(O) & 0x0000000008000000ULL) ? POW_3_3 : 0);
			return (indexA > indexB) ? (halfSize*indexB + indexA - (((indexB + 1)*indexB) >> 1)) : (halfSize*indexA + indexB - (((indexA + 1)*indexA) >> 1));
		}
		static inline int ReducedPatternIndex1(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(codiagonal_flip(P), codiagonal_flip(O)); }
		static inline int ReducedPatternIndex2(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(vertical_flip(P), vertical_flip(O)); }
		static inline int ReducedPatternIndex3(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(diagonal_flip(P), diagonal_flip(O)); }

	public:
		static const int FullSize = POW_3_8;
		static const int ReducedSize = halfSize * (halfSize + 1) / 2;
		
		CPattern_L3() { m_weights.resize(FullSize); }
		void set_weights(const std::vector<float>& compressed_weights)
		{
			int ReducedIndex;
			int FullIndex;

			For_each_configuration_in_pattern_do_fkt(Pattern,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex0(P, O);
					FullIndex = FullPatternIndex(P, O, Pattern);
					m_weights[FullIndex] = compressed_weights[ReducedIndex];
				}
			);
		}
	
		inline float score(const unsigned long long P, const unsigned long long O) const
		{
			return m_weights[FullPatternIndex(P, O, Pattern )]
				 + m_weights[FullPatternIndex(P, O, PatternC)]
				 + m_weights[FullPatternIndex(P, O, PatternV)]
				 + m_weights[FullPatternIndex(P, O, PatternD)];
		}
	


		static void FillConfigurationVec(const unsigned long long P, const unsigned long long O, std::vector<int>& vec, int Offset = 0)
		{
			vec.push_back(ReducedPatternIndex0(P, O) + Offset);
			vec.push_back(ReducedPatternIndex1(P, O) + Offset);
			vec.push_back(ReducedPatternIndex2(P, O) + Offset);
			vec.push_back(ReducedPatternIndex3(P, O) + Offset);
		}
	};
	
	class CPattern_D4
	{
		static const int halfSize = POW_3_2;
		static const unsigned long long Pattern = 0x0000000001020408ULL;
		static const unsigned long long PatternH = Flip<Pattern>::horizontal;
		static const unsigned long long PatternV = Flip<Pattern>::vertical;
		static const unsigned long long PatternD = Flip<Pattern>::diagonal;
		static const unsigned long long PatternC = Flip<Pattern>::codiagonal;
		static const unsigned long long PatternHV = Flip<Flip<Pattern>::horizontal>::vertical;
		static const unsigned long long PatternHD = Flip<Flip<Pattern>::horizontal>::diagonal;
		static const unsigned long long PatternHC = Flip<Flip<Pattern>::horizontal>::codiagonal;

		std::vector<float> m_weights; //m_weights[FullIndex]

		static int ReducedPatternIndex0(const unsigned long long P, const unsigned long long O)
		{
			const int indexA = FullPatternIndex(              P ,               O , 0x0000000000000408ULL);
			const int indexB = FullPatternIndex(diagonal_flip(P), diagonal_flip(O), 0x0000000000000408ULL);
			return (indexA > indexB) ? (halfSize*indexB + indexA - (((indexB + 1)*indexB) >> 1)) : (halfSize*indexA + indexB - (((indexA + 1)*indexA) >> 1));
		}
		static inline int ReducedPatternIndex1(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(horizontal_flip(P), horizontal_flip(O)); }
		static inline int ReducedPatternIndex2(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(codiagonal_flip(P), codiagonal_flip(O)); }
		static inline int ReducedPatternIndex3(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(vertical_flip(P), vertical_flip(O)); }
	public:
		static const int FullSize = POW_3_4;
		static const int ReducedSize = halfSize * (halfSize + 1) / 2;

		CPattern_D4() { m_weights.resize(FullSize); }
		void set_weights(const std::vector<float>& compressed_weights)
		{
			int ReducedIndex;
			int FullIndex;

			For_each_configuration_in_pattern_do_fkt(Pattern,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex0(P, O);
					FullIndex = FullPatternIndex(P, O, Pattern);
					m_weights[FullIndex] = compressed_weights[ReducedIndex];
				}
			);
		}
	
		inline float score(const unsigned long long P, const unsigned long long O) const
		{
			return m_weights[FullPatternIndex(P, O, Pattern )]
				 + m_weights[FullPatternIndex(P, O, PatternH)]
				 + m_weights[FullPatternIndex(P, O, PatternC)]
				 + m_weights[FullPatternIndex(P, O, PatternV)];
		}
	
		static void FillConfigurationVec(const unsigned long long P, const unsigned long long O, std::vector<int>& vec, int Offset = 0)
		{
			vec.push_back(ReducedPatternIndex0(P, O) + Offset);
			vec.push_back(ReducedPatternIndex1(P, O) + Offset);
			vec.push_back(ReducedPatternIndex2(P, O) + Offset);
			vec.push_back(ReducedPatternIndex3(P, O) + Offset);
		}
	};
	
	class CPattern_D5
	{
		static const int halfSize = POW_3_2;
		static const int diagSize = POW_3_1;
		static const unsigned long long Pattern = 0x0000000102040810ULL;
		static const unsigned long long PatternH = Flip<Pattern>::horizontal;
		static const unsigned long long PatternV = Flip<Pattern>::vertical;
		static const unsigned long long PatternD = Flip<Pattern>::diagonal;
		static const unsigned long long PatternC = Flip<Pattern>::codiagonal;
		static const unsigned long long PatternHV = Flip<Flip<Pattern>::horizontal>::vertical;
		static const unsigned long long PatternHD = Flip<Flip<Pattern>::horizontal>::diagonal;
		static const unsigned long long PatternHC = Flip<Flip<Pattern>::horizontal>::codiagonal;

		std::vector<float> m_weights; //m_weights[FullIndex]

		static int ReducedPatternIndex0(const unsigned long long P, const unsigned long long O)
		{
			const int indexA = FullPatternIndex(              P ,               O , 0x0000000000000810ULL);
			const int indexB = FullPatternIndex(diagonal_flip(P), diagonal_flip(O), 0x0000000000000810ULL);
			const int indexD = FullPatternIndex(              P ,               O , 0x0000000000040000ULL);
			return (indexA > indexB) ? (diagSize*(halfSize*indexB+indexA - (((indexB+1)*indexB) >> 1)) + indexD) : (diagSize*(halfSize*indexA+indexB - (((indexA+1)*indexA) >> 1)) + indexD);
		}
		static inline int ReducedPatternIndex1(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(horizontal_flip(P), horizontal_flip(O)); }
		static inline int ReducedPatternIndex2(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(codiagonal_flip(P), codiagonal_flip(O)); }
		static inline int ReducedPatternIndex3(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(vertical_flip(P), vertical_flip(O)); }

	public:
		static const int FullSize = POW_3_5;
		static const int ReducedSize = diagSize * halfSize * (halfSize + 1) / 2;

		CPattern_D5() { m_weights.resize(FullSize); }
		void set_weights(const std::vector<float>& compressed_weights)
		{
			int ReducedIndex;
			int FullIndex;

			For_each_configuration_in_pattern_do_fkt(Pattern,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex0(P, O);
					FullIndex = FullPatternIndex(P, O, Pattern);
					m_weights[FullIndex] = compressed_weights[ReducedIndex];
				}
			);
		}
	
		inline float score(const unsigned long long P, const unsigned long long O) const
		{
			return m_weights[FullPatternIndex(P, O, Pattern )]
				 + m_weights[FullPatternIndex(P, O, PatternH)]
				 + m_weights[FullPatternIndex(P, O, PatternC)]
				 + m_weights[FullPatternIndex(P, O, PatternV)];
		}
	
		static void FillConfigurationVec(const unsigned long long P, const unsigned long long O, std::vector<int>& vec, int Offset = 0)
		{
			vec.push_back(ReducedPatternIndex0(P, O) + Offset);
			vec.push_back(ReducedPatternIndex1(P, O) + Offset);
			vec.push_back(ReducedPatternIndex2(P, O) + Offset);
			vec.push_back(ReducedPatternIndex3(P, O) + Offset);
		}
	};
	
	class CPattern_D6
	{
		static const int halfSize = POW_3_3;
		static const unsigned long long Pattern = 0x0000010204081020ULL;
		static const unsigned long long PatternH = Flip<Pattern>::horizontal;
		static const unsigned long long PatternV = Flip<Pattern>::vertical;
		static const unsigned long long PatternD = Flip<Pattern>::diagonal;
		static const unsigned long long PatternC = Flip<Pattern>::codiagonal;
		static const unsigned long long PatternHV = Flip<Flip<Pattern>::horizontal>::vertical;
		static const unsigned long long PatternHD = Flip<Flip<Pattern>::horizontal>::diagonal;
		static const unsigned long long PatternHC = Flip<Flip<Pattern>::horizontal>::codiagonal;

		std::vector<float> m_weights; //m_weights[FullIndex]
		
		static int ReducedPatternIndex0(const unsigned long long P, const unsigned long long O)
		{
			const int indexA = FullPatternIndex(              P ,               O , 0x0000000000081020ULL);
			const int indexB = FullPatternIndex(diagonal_flip(P), diagonal_flip(O), 0x0000000000081020ULL);
			return (indexA > indexB) ? (halfSize*indexB + indexA - (((indexB + 1)*indexB) >> 1)) : (halfSize*indexA + indexB - (((indexA + 1)*indexA) >> 1));
		}
		static inline int ReducedPatternIndex1(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(horizontal_flip(P), horizontal_flip(O)); }
		static inline int ReducedPatternIndex2(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(codiagonal_flip(P), codiagonal_flip(O)); }
		static inline int ReducedPatternIndex3(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(vertical_flip(P), vertical_flip(O)); }

	public:
		static const int FullSize = POW_3_6;
		static const int ReducedSize = halfSize * (halfSize + 1) / 2;

		CPattern_D6() { m_weights.resize(FullSize); }
		void set_weights(const std::vector<float>& compressed_weights)
		{
			int ReducedIndex;
			int FullIndex;

			For_each_configuration_in_pattern_do_fkt(Pattern,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex0(P, O);
					FullIndex = FullPatternIndex(P, O, Pattern);
					m_weights[FullIndex] = compressed_weights[ReducedIndex];
				}
			);
		}
	
		inline float score(const unsigned long long P, const unsigned long long O) const
		{
			return m_weights[FullPatternIndex(P, O, Pattern )]
				 + m_weights[FullPatternIndex(P, O, PatternH)]
				 + m_weights[FullPatternIndex(P, O, PatternC)]
				 + m_weights[FullPatternIndex(P, O, PatternV)];
		}
	
		static void FillConfigurationVec(const unsigned long long P, const unsigned long long O, std::vector<int>& vec, int Offset = 0)
		{
			vec.push_back(ReducedPatternIndex0(P, O) + Offset);
			vec.push_back(ReducedPatternIndex1(P, O) + Offset);
			vec.push_back(ReducedPatternIndex2(P, O) + Offset);
			vec.push_back(ReducedPatternIndex3(P, O) + Offset);
		}
	};
	
	class CPattern_D7
	{
		static const int halfSize = POW_3_3;
		static const int diagSize = POW_2_1;
		static const unsigned long long Pattern = 0x0001020408102040ULL;
		static const unsigned long long PatternH = Flip<Pattern>::horizontal;
		static const unsigned long long PatternV = Flip<Pattern>::vertical;
		static const unsigned long long PatternD = Flip<Pattern>::diagonal;
		static const unsigned long long PatternC = Flip<Pattern>::codiagonal;
		static const unsigned long long PatternHV = Flip<Flip<Pattern>::horizontal>::vertical;
		static const unsigned long long PatternHD = Flip<Flip<Pattern>::horizontal>::diagonal;
		static const unsigned long long PatternHC = Flip<Flip<Pattern>::horizontal>::codiagonal;

		std::vector<float> m_weights; //m_weights[FullIndex]
		
		static int ReducedPatternIndex0(const unsigned long long P, const unsigned long long O)
		{
			const int indexA = FullPatternIndex(              P ,               O , 0x0000000000102040ULL);
			const int indexB = FullPatternIndex(diagonal_flip(P), diagonal_flip(O), 0x0000000000102040ULL);
			const int indexD = (O & 0x0000000008000000ULL) ? 1 : 0;
			return (indexA > indexB) ? (diagSize*(halfSize*indexB+indexA - (((indexB+1)*indexB) >> 1)) + indexD) : (diagSize*(halfSize*indexA+indexB - (((indexA+1)*indexA) >> 1)) + indexD);
		}
		static inline int ReducedPatternIndex1(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(horizontal_flip(P), horizontal_flip(O)); }
		static inline int ReducedPatternIndex2(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(codiagonal_flip(P), codiagonal_flip(O)); }
		static inline int ReducedPatternIndex3(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(vertical_flip(P), vertical_flip(O)); }

	public:
		static const int FullSize = POW_3_7;
		static const int ReducedSize = diagSize * halfSize * (halfSize + 1) / 2;
		
		CPattern_D7() { m_weights.resize(FullSize); }
		void set_weights(const std::vector<float>& compressed_weights)
		{
			int ReducedIndex;
			int FullIndex;

			For_each_configuration_in_pattern_do_fkt(Pattern,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex0(P, O);
					FullIndex = FullPatternIndex(P, O, Pattern);
					m_weights[FullIndex] = compressed_weights[ReducedIndex];
				}
			);
		}
	
		inline float score(const unsigned long long P, const unsigned long long O) const
		{
			return m_weights[FullPatternIndex(P, O, Pattern )]
				 + m_weights[FullPatternIndex(P, O, PatternH)]
				 + m_weights[FullPatternIndex(P, O, PatternC)]
				 + m_weights[FullPatternIndex(P, O, PatternV)];
		}

		static void FillConfigurationVec(const unsigned long long P, const unsigned long long O, std::vector<int>& vec, int Offset = 0)
		{
			vec.push_back(ReducedPatternIndex0(P, O) + Offset);
			vec.push_back(ReducedPatternIndex1(P, O) + Offset);
			vec.push_back(ReducedPatternIndex2(P, O) + Offset);
			vec.push_back(ReducedPatternIndex3(P, O) + Offset);
		}
	};
	
	class CPattern_Comet
	{
		static const int halfSize = POW_3_1;
		static const int diagSize = POW_3_6 * POW_2_2;
		static const unsigned long long Pattern = 0x8040201008040303ULL;
		static const unsigned long long PatternH = Flip<Pattern>::horizontal;
		static const unsigned long long PatternV = Flip<Pattern>::vertical;
		static const unsigned long long PatternD = Flip<Pattern>::diagonal;
		static const unsigned long long PatternC = Flip<Pattern>::codiagonal;
		static const unsigned long long PatternHV = Flip<Flip<Pattern>::horizontal>::vertical;
		static const unsigned long long PatternHD = Flip<Flip<Pattern>::horizontal>::diagonal;
		static const unsigned long long PatternHC = Flip<Flip<Pattern>::horizontal>::codiagonal;

		std::vector<std::vector<float>> m_weights; //m_weights[FullIndex]
	
		static int ReducedPatternIndex0(const unsigned long long P, const unsigned long long O)
		{
			const int indexA = FullPatternIndex(              P ,               O , 0x0000000000000002ULL);
			const int indexB = FullPatternIndex(diagonal_flip(P), diagonal_flip(O), 0x0000000000000002ULL);
			const int indexD = FullPatternIndex(              P ,               O , 0x8040200000040201ULL) * 4 + PExt(O, 0x0000001008000000ULL);
			return (indexA > indexB) ? (diagSize*(halfSize*indexB+indexA - (((indexB+1)*indexB) >> 1)) + indexD) : (diagSize*(halfSize*indexA+indexB - (((indexA+1)*indexA) >> 1)) + indexD);
		}
		static inline int ReducedPatternIndex1(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(horizontal_flip(P), horizontal_flip(O)); }
		static inline int ReducedPatternIndex2(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(codiagonal_flip(P), codiagonal_flip(O)); }
		static inline int ReducedPatternIndex3(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(vertical_flip(P), vertical_flip(O)); }

	public:
		static const int FullSize = POW_3_10;
		static const int ReducedSize = diagSize * halfSize * (halfSize + 1) / 2;

		CPattern_Comet()
		{
			//Initialize weights
			m_weights.resize(4);
			for (int i = 0; i < 4; ++i)
				m_weights[i].resize(FullSize);
		}
		void set_weights(const std::vector<float>& compressed_weights)
		{
			int ReducedIndex;
			int FullIndex;

			For_each_configuration_in_pattern_do_fkt(Pattern,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex0(P, O);
					FullIndex = FullPatternIndex(P, O, Pattern);
					m_weights[0][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternH,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex1(P, O);
					FullIndex = FullPatternIndex(P, O, PatternH);
					m_weights[1][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternC,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex2(P, O);
					FullIndex = FullPatternIndex(P, O, PatternC);
					m_weights[2][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternV,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex3(P, O);
					FullIndex = FullPatternIndex(P, O, PatternV);
					m_weights[3][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
		}
	
		inline float score(const unsigned long long P, const unsigned long long O) const
		{
			return m_weights[0][FullPatternIndex(P, O, Pattern )]
				 + m_weights[1][FullPatternIndex(P, O, PatternH)]
				 + m_weights[2][FullPatternIndex(P, O, PatternC)]
				 + m_weights[3][FullPatternIndex(P, O, PatternV)];
		}

		static void FillConfigurationVec(const unsigned long long P, const unsigned long long O, std::vector<int>& vec, int Offset = 0)
		{
			vec.push_back(ReducedPatternIndex0(P, O) + Offset);
			vec.push_back(ReducedPatternIndex1(P, O) + Offset);
			vec.push_back(ReducedPatternIndex2(P, O) + Offset);
			vec.push_back(ReducedPatternIndex3(P, O) + Offset);
		}
	};
	
	class CPattern_Eplus
	{
		static const int halfSize = POW_3_5;
		static const unsigned long long Pattern = 0x0000000000003CBDULL;
		static const unsigned long long PatternH = Flip<Pattern>::horizontal;
		static const unsigned long long PatternV = Flip<Pattern>::vertical;
		static const unsigned long long PatternD = Flip<Pattern>::diagonal;
		static const unsigned long long PatternC = Flip<Pattern>::codiagonal;
		static const unsigned long long PatternHV = Flip<Flip<Pattern>::horizontal>::vertical;
		static const unsigned long long PatternHD = Flip<Flip<Pattern>::horizontal>::diagonal;
		static const unsigned long long PatternHC = Flip<Flip<Pattern>::horizontal>::codiagonal;

		std::vector<std::vector<float>> m_weights; //m_weights[Index][FullIndex]

		static int ReducedPatternIndex0(const unsigned long long P, const unsigned long long O)
		{
			const int indexA = FullPatternIndex(                P ,                 O , 0x0000000000000C0DULL);
			const int indexB = FullPatternIndex(horizontal_flip(P), horizontal_flip(O), 0x0000000000000C0DULL);
			return (indexA > indexB) ? (halfSize*indexB + indexA - (((indexB + 1)*indexB) >> 1)) : (halfSize*indexA + indexB - (((indexA + 1)*indexA) >> 1));
		}
		static inline int ReducedPatternIndex1(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(codiagonal_flip(P), codiagonal_flip(O)); }
		static inline int ReducedPatternIndex2(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(vertical_flip(P), vertical_flip(O)); }
		static inline int ReducedPatternIndex3(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(diagonal_flip(P), diagonal_flip(O)); }

	public:
		static const int FullSize = POW_3_10;
		static const int ReducedSize = halfSize * (halfSize + 1) / 2;

		CPattern_Eplus()
		{
			//Initialize weights
			m_weights.resize(4);
			for (int i = 0; i < 4; ++i)
				m_weights[i].resize(FullSize);
		}
		void set_weights(const std::vector<float>& compressed_weights)
		{
			int ReducedIndex;
			int FullIndex;

			For_each_configuration_in_pattern_do_fkt(Pattern,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex0(P, O);
					FullIndex = FullPatternIndex(P, O, Pattern);
					m_weights[0][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternC,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex1(P, O);
					FullIndex = FullPatternIndex(P, O, PatternC);
					m_weights[1][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternV,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex2(P, O);
					FullIndex = FullPatternIndex(P, O, PatternV);
					m_weights[2][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternD,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex3(P, O);
					FullIndex = FullPatternIndex(P, O, PatternD);
					m_weights[3][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
		}
	
		inline float score(const unsigned long long P, const unsigned long long O) const
		{
			return m_weights[0][FullPatternIndex(P, O, Pattern )]
				 + m_weights[1][FullPatternIndex(P, O, PatternC)]
				 + m_weights[2][FullPatternIndex(P, O, PatternV)]
				 + m_weights[3][FullPatternIndex(P, O, PatternD)];
		}
	

		static void FillConfigurationVec(const unsigned long long P, const unsigned long long O, std::vector<int>& vec, int Offset = 0)
		{
			vec.push_back(ReducedPatternIndex0(P, O) + Offset);
			vec.push_back(ReducedPatternIndex1(P, O) + Offset);
			vec.push_back(ReducedPatternIndex2(P, O) + Offset);
			vec.push_back(ReducedPatternIndex3(P, O) + Offset);
		}
	};
	
	class CPattern_C3p1
	{
		static const int halfSize = POW_3_3;
		static const int diagSize = POW_3_2;
		static const unsigned long long Pattern = 0x000000000101030FULL;
		static const unsigned long long PatternH = Flip<Pattern>::horizontal;
		static const unsigned long long PatternV = Flip<Pattern>::vertical;
		static const unsigned long long PatternD = Flip<Pattern>::diagonal;
		static const unsigned long long PatternC = Flip<Pattern>::codiagonal;
		static const unsigned long long PatternHV = Flip<Flip<Pattern>::horizontal>::vertical;
		static const unsigned long long PatternHD = Flip<Flip<Pattern>::horizontal>::diagonal;
		static const unsigned long long PatternHC = Flip<Flip<Pattern>::horizontal>::codiagonal;

		std::vector<std::vector<float>> m_weights; //m_weights[Index][FullIndex]
	
		static int ReducedPatternIndex0(const unsigned long long P, const unsigned long long O)
		{
			const int indexA = FullPatternIndex(              P ,               O , 0x0000000001010100ULL);
			const int indexB = FullPatternIndex(diagonal_flip(P), diagonal_flip(O), 0x0000000001010100ULL);
			const int indexD = FullPatternIndex(              P ,               O , 0x0000000000000201ULL);
			return (indexA > indexB) ? (diagSize*(halfSize*indexB+indexA - (((indexB+1)*indexB) >> 1)) + indexD) : (diagSize*(halfSize*indexA+indexB - (((indexA+1)*indexA) >> 1)) + indexD);
		}
		static inline int ReducedPatternIndex1(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(horizontal_flip(P), horizontal_flip(O)); }
		static inline int ReducedPatternIndex2(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(codiagonal_flip(P), codiagonal_flip(O)); }
		static inline int ReducedPatternIndex3(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(vertical_flip(P), vertical_flip(O)); }

	public:
		static const int FullSize = POW_3_8;
		static const int ReducedSize = diagSize * halfSize * (halfSize + 1) / 2;

		CPattern_C3p1()
		{
			//Initialize weights
			m_weights.resize(4);
			for (int i = 0; i < 4; ++i)
				m_weights[i].resize(FullSize);
		}
		void set_weights(const std::vector<float>& compressed_weights)
		{
			int ReducedIndex;
			int FullIndex;

			For_each_configuration_in_pattern_do_fkt(Pattern,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex0(P, O);
					FullIndex = FullPatternIndex(P, O, Pattern);
					m_weights[0][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternH,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex1(P, O);
					FullIndex = FullPatternIndex(P, O, PatternH);
					m_weights[1][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternC,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex2(P, O);
					FullIndex = FullPatternIndex(P, O, PatternC);
					m_weights[2][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternV,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex3(P, O);
					FullIndex = FullPatternIndex(P, O, PatternV);
					m_weights[3][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
		}
	
		inline float score(const unsigned long long P, const unsigned long long O) const
		{
			return m_weights[0][FullPatternIndex(P, O, Pattern )]
				 + m_weights[1][FullPatternIndex(P, O, PatternH)]
				 + m_weights[2][FullPatternIndex(P, O, PatternC)]
				 + m_weights[3][FullPatternIndex(P, O, PatternV)];
		}

		static void FillConfigurationVec(const unsigned long long P, const unsigned long long O, std::vector<int>& vec, int Offset = 0)
		{
			vec.push_back(ReducedPatternIndex0(P, O) + Offset);
			vec.push_back(ReducedPatternIndex1(P, O) + Offset);
			vec.push_back(ReducedPatternIndex2(P, O) + Offset);
			vec.push_back(ReducedPatternIndex3(P, O) + Offset);
		}
	};
	
	class CPattern_C3p2
	{
		static const int halfSize = POW_3_4;
		static const int diagSize = POW_3_2;
		static const unsigned long long Pattern = 0x000000010101031FULL;
		static const unsigned long long PatternH = Flip<Pattern>::horizontal;
		static const unsigned long long PatternV = Flip<Pattern>::vertical;
		static const unsigned long long PatternD = Flip<Pattern>::diagonal;
		static const unsigned long long PatternC = Flip<Pattern>::codiagonal;
		static const unsigned long long PatternHV = Flip<Flip<Pattern>::horizontal>::vertical;
		static const unsigned long long PatternHD = Flip<Flip<Pattern>::horizontal>::diagonal;
		static const unsigned long long PatternHC = Flip<Flip<Pattern>::horizontal>::codiagonal;

		std::vector<std::vector<float>> m_weights; //m_weights[Index][FullIndex]
	
		static int ReducedPatternIndex0(const unsigned long long P, const unsigned long long O)
		{
			const int indexA = FullPatternIndex(              P ,               O , 0x0000000101010100ULL);
			const int indexB = FullPatternIndex(diagonal_flip(P), diagonal_flip(O), 0x0000000101010100ULL);
			const int indexD = FullPatternIndex(              P ,               O , 0x0000000000000201ULL);
			return (indexA > indexB) ? (diagSize*(halfSize*indexB+indexA - (((indexB+1)*indexB) >> 1)) + indexD) : (diagSize*(halfSize*indexA+indexB - (((indexA+1)*indexA) >> 1)) + indexD);
		}
		static inline int ReducedPatternIndex1(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(horizontal_flip(P), horizontal_flip(O)); }
		static inline int ReducedPatternIndex2(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(codiagonal_flip(P), codiagonal_flip(O)); }
		static inline int ReducedPatternIndex3(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(vertical_flip(P), vertical_flip(O)); }

	public:
		static const int FullSize = POW_3_10;
		static const int ReducedSize = diagSize * halfSize * (halfSize + 1) / 2;

		CPattern_C3p2()
		{
			//Initialize weights
			m_weights.resize(4);
			for (int i = 0; i < 4; ++i)
				m_weights[i].resize(FullSize);
		}
		void set_weights(const std::vector<float>& compressed_weights)
		{
			int ReducedIndex;
			int FullIndex;

			For_each_configuration_in_pattern_do_fkt(Pattern,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex0(P, O);
					FullIndex = FullPatternIndex(P, O, Pattern);
					m_weights[0][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternH,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex1(P, O);
					FullIndex = FullPatternIndex(P, O, PatternH);
					m_weights[1][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternC,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex2(P, O);
					FullIndex = FullPatternIndex(P, O, PatternC);
					m_weights[2][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternV,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex3(P, O);
					FullIndex = FullPatternIndex(P, O, PatternV);
					m_weights[3][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
		}
	
		inline float score(const unsigned long long P, const unsigned long long O) const
		{
			return m_weights[0][FullPatternIndex(P, O, Pattern )]
				 + m_weights[1][FullPatternIndex(P, O, PatternH)]
				 + m_weights[2][FullPatternIndex(P, O, PatternC)]
				 + m_weights[3][FullPatternIndex(P, O, PatternV)];
		}

		static void FillConfigurationVec(const unsigned long long P, const unsigned long long O, std::vector<int>& vec, int Offset = 0)
		{
			vec.push_back(ReducedPatternIndex0(P, O) + Offset);
			vec.push_back(ReducedPatternIndex1(P, O) + Offset);
			vec.push_back(ReducedPatternIndex2(P, O) + Offset);
			vec.push_back(ReducedPatternIndex3(P, O) + Offset);
		}
	};
	
	class CPattern_Q0
	{
		static const int halfSize = POW_3_3;
		static const int diagSize = POW_3_3;
		static const unsigned long long Pattern = 0x0000000000070707ULL;
		static const unsigned long long PatternH = Flip<Pattern>::horizontal;
		static const unsigned long long PatternV = Flip<Pattern>::vertical;
		static const unsigned long long PatternD = Flip<Pattern>::diagonal;
		static const unsigned long long PatternC = Flip<Pattern>::codiagonal;
		static const unsigned long long PatternHV = Flip<Flip<Pattern>::horizontal>::vertical;
		static const unsigned long long PatternHD = Flip<Flip<Pattern>::horizontal>::diagonal;
		static const unsigned long long PatternHC = Flip<Flip<Pattern>::horizontal>::codiagonal;

		std::vector<std::vector<float>> m_weights; //m_weights[FullIndex]
	
		static int ReducedPatternIndex0(const unsigned long long P, const unsigned long long O)
		{
			const int indexA = FullPatternIndex(              P ,               O , 0x0000000000030100ULL);
			const int indexB = FullPatternIndex(diagonal_flip(P), diagonal_flip(O), 0x0000000000030100ULL);
			const int indexD = FullPatternIndex(              P ,               O , 0x0000000000040201ULL);
			return (indexA > indexB) ? (diagSize*(halfSize*indexB+indexA - (((indexB+1)*indexB) >> 1)) + indexD) : (diagSize*(halfSize*indexA+indexB - (((indexA+1)*indexA) >> 1)) + indexD);
		}
		static inline int ReducedPatternIndex1(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(horizontal_flip(P), horizontal_flip(O)); }
		static inline int ReducedPatternIndex2(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(codiagonal_flip(P), codiagonal_flip(O)); }
		static inline int ReducedPatternIndex3(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(vertical_flip(P), vertical_flip(O)); }

	public:
		static const int FullSize = POW_3_9;
		static const int ReducedSize = diagSize * halfSize * (halfSize + 1) / 2;

		CPattern_Q0()
		{
			//Initialize weights
			m_weights.resize(4);
			for (int i = 0; i < 4; ++i)
				m_weights[i].resize(FullSize);
		}
		void set_weights(const std::vector<float>& compressed_weights)
		{
			int ReducedIndex;
			int FullIndex;

			For_each_configuration_in_pattern_do_fkt(Pattern,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex0(P, O);
					FullIndex = FullPatternIndex(P, O, Pattern);
					m_weights[0][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternH,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex1(P, O);
					FullIndex = FullPatternIndex(P, O, PatternH);
					m_weights[1][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternC,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex2(P, O);
					FullIndex = FullPatternIndex(P, O, PatternC);
					m_weights[2][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternV,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex3(P, O);
					FullIndex = FullPatternIndex(P, O, PatternV);
					m_weights[3][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
		}
	
		inline float score(const unsigned long long P, const unsigned long long O) const
		{
			return m_weights[0][FullPatternIndex(P, O, Pattern )]
				 + m_weights[1][FullPatternIndex(P, O, PatternH)]
				 + m_weights[2][FullPatternIndex(P, O, PatternC)]
				 + m_weights[3][FullPatternIndex(P, O, PatternV)];
		}

		static void FillConfigurationVec(const unsigned long long P, const unsigned long long O, std::vector<int>& vec, int Offset = 0)
		{
			vec.push_back(ReducedPatternIndex0(P, O) + Offset);
			vec.push_back(ReducedPatternIndex1(P, O) + Offset);
			vec.push_back(ReducedPatternIndex2(P, O) + Offset);
			vec.push_back(ReducedPatternIndex3(P, O) + Offset);
		}
	};
	
	class CPattern_B5
	{
		static const unsigned long long Pattern = 0x0000000000001F1FULL;
		static const unsigned long long PatternH = Flip<Pattern>::horizontal;
		static const unsigned long long PatternV = Flip<Pattern>::vertical;
		static const unsigned long long PatternD = Flip<Pattern>::diagonal;
		static const unsigned long long PatternC = Flip<Pattern>::codiagonal;
		static const unsigned long long PatternHV = Flip<Flip<Pattern>::horizontal>::vertical;
		static const unsigned long long PatternHD = Flip<Flip<Pattern>::horizontal>::diagonal;
		static const unsigned long long PatternHC = Flip<Flip<Pattern>::horizontal>::codiagonal;

		std::vector<std::vector<float>> m_weights; //m_weights[Index][FullIndex]

		static inline int ReducedPatternIndex0(const unsigned long long P, const unsigned long long O) { return FullPatternIndex(P, O, 0x0000000000001F1FULL); }
		static inline int ReducedPatternIndex1(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(horizontal_flip(P), horizontal_flip(O)); }
		static inline int ReducedPatternIndex2(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(vertical_flip(P), vertical_flip(O)); }
		static inline int ReducedPatternIndex3(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(diagonal_flip(P), diagonal_flip(O)); }
		static inline int ReducedPatternIndex4(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(codiagonal_flip(P), codiagonal_flip(O)); }
		static inline int ReducedPatternIndex5(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(horizontal_flip(vertical_flip(P)), horizontal_flip(vertical_flip(O))); }
		static inline int ReducedPatternIndex6(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(horizontal_flip(diagonal_flip(P)), horizontal_flip(diagonal_flip(O))); }
		static inline int ReducedPatternIndex7(const unsigned long long P, const unsigned long long O) { return ReducedPatternIndex0(horizontal_flip(codiagonal_flip(P)), horizontal_flip(codiagonal_flip(O))); }

	public:
		static const int FullSize = POW_3_10;
		static const int ReducedSize = POW_3_10;

		CPattern_B5()
		{
			//Initialize weights
			m_weights.resize(8);
			for (int i = 0; i < 8; ++i)
				m_weights[i].resize(FullSize);
		}
		void set_weights(const std::vector<float>& compressed_weights)
		{
			int ReducedIndex;
			int FullIndex;

			For_each_configuration_in_pattern_do_fkt(Pattern,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex0(P, O);
					FullIndex = FullPatternIndex(P, O, Pattern);
					m_weights[0][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternH,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex1(P, O);
					FullIndex = FullPatternIndex(P, O, PatternH);
					m_weights[1][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternV,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex2(P, O);
					FullIndex = FullPatternIndex(P, O, PatternV);
					m_weights[2][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternD,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex3(P, O);
					FullIndex = FullPatternIndex(P, O, PatternD);
					m_weights[3][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternC,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex4(P, O);
					FullIndex = FullPatternIndex(P, O, PatternC);
					m_weights[4][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternHV,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex5(P, O);
					FullIndex = FullPatternIndex(P, O, PatternHV);
					m_weights[5][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternHD,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex6(P, O);
					FullIndex = FullPatternIndex(P, O, PatternHD);
					m_weights[6][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
			For_each_configuration_in_pattern_do_fkt(PatternHC,
				[&](unsigned long long P, unsigned long long O)
				{
					ReducedIndex = ReducedPatternIndex7(P, O);
					FullIndex = FullPatternIndex(P, O, PatternHC);
					m_weights[7][FullIndex] = compressed_weights[ReducedIndex];
				}
			);
		}
	
		inline float score(const unsigned long long P, const unsigned long long O) const
		{
			return m_weights[0][FullPatternIndex(P, O, Pattern  )]
				 + m_weights[1][FullPatternIndex(P, O, PatternH )]
				 + m_weights[2][FullPatternIndex(P, O, PatternV )]
				 + m_weights[3][FullPatternIndex(P, O, PatternD )]
				 + m_weights[4][FullPatternIndex(P, O, PatternC )]
				 + m_weights[5][FullPatternIndex(P, O, PatternHV)]
				 + m_weights[6][FullPatternIndex(P, O, PatternHD)]
				 + m_weights[7][FullPatternIndex(P, O, PatternHC)];
		}
	
		static void FillConfigurationVec(const unsigned long long P, const unsigned long long O, std::vector<int>& vec, int Offset = 0)
		{
			vec.push_back(ReducedPatternIndex0(P, O) + Offset);
			vec.push_back(ReducedPatternIndex1(P, O) + Offset);
			vec.push_back(ReducedPatternIndex2(P, O) + Offset);
			vec.push_back(ReducedPatternIndex3(P, O) + Offset);
			vec.push_back(ReducedPatternIndex4(P, O) + Offset);
			vec.push_back(ReducedPatternIndex5(P, O) + Offset);
			vec.push_back(ReducedPatternIndex6(P, O) + Offset);
			vec.push_back(ReducedPatternIndex7(P, O) + Offset);
		}
	};
}

void FillConfigurationVec(const unsigned long long P, const unsigned long long O, std::vector<int>& vec);

int EvaluateFeatures(const unsigned long long P, const unsigned long long O);
int EvaluateFeatures(const unsigned long long P, const unsigned long long O, std::vector<float>& scores);
int EvaluateFeatures(const unsigned long long P, const unsigned long long O, Features::CIndexArray indexArray);
void FillConfigurationVecOffsetted(const unsigned long long P, const unsigned long long O, std::vector<int>& vec);

//bool Test_All_Features();