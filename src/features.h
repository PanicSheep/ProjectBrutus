#pragma once
#include "configfile.h"
#include "datamanipulation.h"
#include "position.h"
#include "utility.h"
#include <functional>
#include <iostream>
#include <vector>
#include <map>

namespace Features
{
	const int BOXES = 15;

	class CIndexArray
	{
	public:
		unsigned long long BitBoard;
		std::vector<int> Indices;
	};

	class FeatureMetaGroup
	{
	public:
		virtual unsigned long long BitPattern() const = 0;
		virtual std::string Name() const = 0;
		virtual int Elements() const = 0;
		virtual int FullSize() const = 0;
		virtual int ReducedSize() const = 0;

		virtual void set_weights(std::vector<float>::const_iterator& weight_iter, const unsigned int BoxIndex) = 0;

		virtual float score(const unsigned long long P, const unsigned long long O, const unsigned int BoxIndex) const = 0;
		virtual float score(const unsigned long long P, const unsigned long long O, const CIndexArray & Player, const CIndexArray & Opponent) const = 0;

		virtual void FillConfigurationArray(const unsigned long long P, const unsigned long long O, int * Array) const = 0;
	};

	template <unsigned long long BITPATTERN>
	class FeatureGroup : public FeatureMetaGroup
	{
	private:
		const unsigned long long m_BitPattern = BITPATTERN;
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
			if (PopCount(m_BitPattern) > 12){
				std::cerr << "Pattern " << Name() << " too big." << std::endl;
				throw "Pattern too big.";
			}

			std::map<unsigned long long, int> Elements;
			for (int sym = 0; sym < 8; ++sym)
			{
				if (Elements.count(sym_flip_inv(m_BitPattern, sym)) == 0) {
					Elements[sym_flip_inv(m_BitPattern, sym)] = 1;
					m_Generators.push_back(sym);
				}
			}
			m_Elements = m_Generators.size();

			for (int sym = 1; sym < 8; ++sym)
				if (sym_flip(m_BitPattern, sym) == m_BitPattern)
					m_Symmetries.push_back(sym);

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

			if (m_Elements == 8)
				m_ReducedSize = pow(3, PopCount(m_BitPattern & ~0x0000001818000000ULL)) 
							  * pow(2, PopCount(m_BitPattern &  0x0000001818000000ULL));
			else if (m_Elements == 4)
			{
				m_ReducedSize = pow(3, PopCount(NonEigenBits & ~0x0000001818000000ULL)) 
							  * pow(2, PopCount(NonEigenBits &  0x0000001818000000ULL));
				m_ReducedSize = m_ReducedSize * (m_ReducedSize + 1) / 2;
				m_ReducedSize *= pow(3, PopCount(EigenBits & ~0x0000001818000000ULL)) 
							   * pow(2, PopCount(EigenBits &  0x0000001818000000ULL));
			}
			else
			{
				std::cerr << "Pattern " << Name() << " not supported." << std::endl;
				throw "Pattern not supported.";
			}

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

		//float score(const unsigned long long P, const unsigned long long O, const float * const weights) const
		//{
		//	// RoomForOptimization: pow(3, i) has to cast i from unsigned int to unsigned long long.
		//	const unsigned int size1 = PopCount(m_BitPattern & ~0x0000001818000000ULL);
		//	const unsigned int size2 = PopCount(m_BitPattern &  0x0000001818000000ULL);
		//	float result = 0;
		//	int index;
		//	
		//	if (m_Elements == 8)
		//	{
		//		unsigned long long tmpP, tmpO, tmpPattern, LSB;
		//		for (int sym : m_Generators)
		//		{
		//			tmpP = sym_flip(P, sym);
		//			tmpO = sym_flip(O, sym);
		//
		//			index = 0;
		//			tmpPattern = m_BitPattern & ~0x0000001818000000ULL;
		//			for (unsigned int i = 0; i < size1; ++i)
		//			{
		//				LSB = GetLSB(tmpPattern);
		//				RemoveLSB(tmpPattern);
		//				if (tmpP & LSB) index += 1 * pow(3, i);
		//				if (tmpO & LSB) index += 2 * pow(3, i);
		//			}
		//			tmpPattern = m_BitPattern & 0x0000001818000000ULL;
		//			for (unsigned int i = 0; i < size2; ++i)
		//			{
		//				LSB = GetLSB(tmpPattern);
		//				RemoveLSB(tmpPattern);
		//				if (tmpO & LSB) index += 1 * pow(2, i) * pow(3, size1); // RoomForOptimization: Move *pow(3, size1) out of loop.
		//			}
		//			result += weights[index];
		//		}
		//	}
		//	else if (m_Elements == 4)
		//	{
		//		// RoomForOptimization: pow(*, i) has to cast i from unsigned int to unsigned long long.
		//		const unsigned int size1 = PopCount(NonEigenBits & ~0x0000001818000000ULL);
		//		const unsigned int size2 = PopCount(NonEigenBits &  0x0000001818000000ULL);
		//		const unsigned int size3 = PopCount(   EigenBits & ~0x0000001818000000ULL);
		//		const unsigned int size4 = PopCount(   EigenBits &  0x0000001818000000ULL);
		//		const int diagtmp = pow(3, PopCount(   EigenBits & ~0x0000001818000000ULL)) * pow(2, PopCount(   EigenBits & 0x0000001818000000ULL));
		//		const int sidetmp = pow(3, PopCount(NonEigenBits & ~0x0000001818000000ULL)) * pow(2, PopCount(NonEigenBits & 0x0000001818000000ULL));
		//		unsigned long long tmpP, tmpO, tmpPattern, LSB;
		//		int indexA, indexB, indexD;
		//		for (int sym : m_Generators)
		//		{
		//			tmpP = sym_flip(P, sym);
		//			tmpO = sym_flip(O, sym);
		//								
		//			// Side A of non-eigen fields
		//			indexA = 0;
		//			tmpPattern = NonEigenBits & ~0x0000001818000000ULL;
		//			for (int i = 0; i < size1; ++i)
		//			{
		//				LSB = GetLSB(tmpPattern);
		//				RemoveLSB(tmpPattern);
		//				if (tmpP & LSB) indexA += 1 * pow(3, i);
		//				if (tmpO & LSB) indexA += 2 * pow(3, i);
		//			}
		//			tmpPattern = NonEigenBits & 0x0000001818000000ULL;
		//			for (int i = 0; i < size2; ++i)
		//			{
		//				LSB = GetLSB(tmpPattern);
		//				RemoveLSB(tmpPattern);
		//				if (tmpO & LSB) indexA += 1 * pow(2, i) * pow(3, size1); // RoomForOptimization: Move *pow(3, size1) out of loop.
		//			}
		//			
		//			// Side B of non-eigen fields
		//			indexB = 0;
		//			tmpPattern = NonEigenBits & ~0x0000001818000000ULL;
		//			for (int i = 0; i < size1; ++i)
		//			{
		//				LSB = Mirror(GetLSB(tmpPattern), m_Symmetries);
		//				RemoveLSB(tmpPattern);
		//				if (tmpP & LSB) indexB += 1 * pow(3, i);
		//				if (tmpO & LSB) indexB += 2 * pow(3, i);
		//			}
		//			tmpPattern = NonEigenBits & 0x0000001818000000ULL;
		//			for (int i = 0; i < size2; ++i)
		//			{
		//				LSB = Mirror(GetLSB(tmpPattern), m_Symmetries);
		//				RemoveLSB(tmpPattern);
		//				if (tmpO & LSB) indexB += 1 * pow(2, i) * pow(3, size1); // RoomForOptimization: Move *pow(3, size1) out of loop.
		//			}
		//
		//			// Eigen fields
		//			indexD = 0;
		//			tmpPattern = EigenBits & ~0x0000001818000000ULL;
		//			for (unsigned int i = 0; i < size3; ++i)
		//			{
		//				LSB = GetLSB(tmpPattern);
		//				RemoveLSB(tmpPattern);
		//				if (tmpP & LSB) indexD += 1 * pow(3, i);
		//				if (tmpO & LSB) indexD += 2 * pow(3, i);
		//			}
		//			tmpPattern = EigenBits & 0x0000001818000000ULL;
		//			for (unsigned int i = 0; i < size4; ++i)
		//			{
		//				LSB = GetLSB(tmpPattern);
		//				RemoveLSB(tmpPattern);
		//				if (tmpO & LSB) indexD += 1 * pow(2, i) * pow(3, size3); // RoomForOptimization: Move *pow(3, size3) out of loop.
		//			}
		//
		//			index = indexA > indexB ? diagtmp*(sidetmp*indexB+indexA - (((indexB+1)*indexB) >> 1)) + indexD : diagtmp*(sidetmp*indexA+indexB - (((indexA+1)*indexA) >> 1)) + indexD;
		//			
		//			result += weights[index];
		//		}
		//	}
		//	else
		//	{
		//		std::cerr << "Pattern " << Name() << " not supported." << std::endl;
		//		throw "Pattern not supported.";
		//	}
		//
		//	return result;
		//}
		float score(const unsigned long long P, const unsigned long long O, const unsigned int BoxIndex) const
		{
			unsigned long long tmpBitBoard;
			float result = 0;
			int index;

			for (int sym = 0; sym < m_Elements; ++sym)
			{
				tmpBitBoard = m_BitBoards[sym];
				index = SumPow3[PExt(P, tmpBitBoard)] + 2 * SumPow3[PExt(O, tmpBitBoard)];
				result += m_weights[BoxIndex * m_Elements + sym][index];
			}
			return result;
		}
		float score(const unsigned long long P, const unsigned long long O, const CIndexArray & Player, const CIndexArray & Opponent) const
		{
			return 0;
		}

		void FillConfigurationArray(const unsigned long long P, const unsigned long long O, int * Array) const
		{
			// RoomForOptimization: pow(*, i) has to cast i from unsigned int to unsigned long long.
			const unsigned int size1 = PopCount(m_BitPattern & ~0x0000001818000000ULL);
			const unsigned int size2 = PopCount(m_BitPattern &  0x0000001818000000ULL);
			float result = 0;
			int index;
			
			if (m_Elements == 8)
			{
				unsigned long long tmpP, tmpO, tmpPattern, LSB;
				for (int sym : m_Generators)
				{
					tmpP = sym_flip(P, sym);
					tmpO = sym_flip(O, sym);

					index = 0;
					tmpPattern = m_BitPattern & ~0x0000001818000000ULL;
					for (unsigned int i = 0; i < size1; ++i)
					{
						LSB = GetLSB(tmpPattern);
						RemoveLSB(tmpPattern);
						if (tmpP & LSB) index += 1 * pow(3, i);
						if (tmpO & LSB) index += 2 * pow(3, i);
					}
					tmpPattern = m_BitPattern & 0x0000001818000000ULL;
					for (unsigned int i = 0; i < size2; ++i)
					{
						LSB = GetLSB(tmpPattern);
						RemoveLSB(tmpPattern);
						if (tmpO & LSB) index += 1 * pow(2, i) * pow(3, size1); // RoomForOptimization: Move *pow(3, size1) out of loop.
					}
					*(Array++) = index;
				}
			}
			else
			{
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
		}
		
	};
	
	extern std::vector<FeatureMetaGroup*> FeatureList;
	extern std::size_t Elements;
	extern std::size_t FullSize;
	extern std::size_t ReducedSize;
	extern std::string Names();

	
	unsigned long long sym_flip(const unsigned long long P, const int sym);
	unsigned long long sym_flip_inv(const unsigned long long P, const int sym);
	unsigned long long Mirror(const unsigned long long B, const std::vector<int> & Generator);
	void For_each_configuration_in_pattern_do_fkt(const unsigned long long Pattern, std::function<void(const unsigned long long, const unsigned long long)> fkt);
	
	//inline void SwapIf(int& A, int& B) { if (A > B) std::swap(A, B); }
	//inline void Sort(int& A0, int& A1, int& A2, int& A3)
	//{
	//	// Sorting network
	//	SwapIf(A0,A1);
	//	SwapIf(A2,A3);

	//	SwapIf(A0,A2);
	//	SwapIf(A1,A3);

	//	SwapIf(A1,A2);
	//}
	//inline void Sort(int& A0, int& A1, int& A2, int& A3, int& A4, int& A5, int& A6, int& A7)
	//{
	//	// Sorting network
	//	SwapIf(A0,A4);
	//	SwapIf(A1,A5);
	//	SwapIf(A2,A6);
	//	SwapIf(A3,A7);
	//		
	//	SwapIf(A0,A2);
	//	SwapIf(A1,A3);
	//	SwapIf(A4,A6);
	//	SwapIf(A5,A7);
	//		
	//	SwapIf(A2,A4);
	//	SwapIf(A3,A5);
	//	SwapIf(A0,A1);
	//	SwapIf(A6,A7);
	//		
	//	SwapIf(A2,A3);
	//	SwapIf(A4,A5);

	//	SwapIf(A1,A4);
	//	SwapIf(A3,A6);
	//		
	//	SwapIf(A1,A2);
	//	SwapIf(A3,A4);
	//	SwapIf(A5,A6);
	//}
	//inline void Sort4(int * const Array) { Sort(Array[0], Array[1], Array[2], Array[3]); }
	//inline void Sort8(int * const Array) { Sort(Array[0], Array[1], Array[2], Array[3], Array[4], Array[5], Array[6], Array[7]); }

	void Initialize(const int conf);
	void Finalize();
}

void FillConfigurationArray(const unsigned long long P, const unsigned long long O, int * Array);

int EvaluateFeatures(const unsigned long long P, const unsigned long long O);
int EvaluateFeatures(const unsigned long long P, const unsigned long long O, float * const Array);

//bool Test_All_Features();