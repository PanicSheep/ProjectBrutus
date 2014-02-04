#pragma once
#include "datamanipulation.h"
#include "position.h"
#include "utility.h"
#include <functional>
#include <iostream>
#include <vector>

namespace Features
{
	template <unsigned char I>
	inline unsigned short POWER_OF_3()
	{
		unsigned short sum = 0;
		if (I & 0x01) sum += 1;
		if (I & 0x02) sum += 1*3;
		if (I & 0x04) sum += 1*3*3;
		if (I & 0x08) sum += 1*3*3*3;
		if (I & 0x10) sum += 1*3*3*3*3;
		if (I & 0x20) sum += 1*3*3*3*3*3;
		if (I & 0x40) sum += 1*3*3*3*3*3*3;
		if (I & 0x80) sum += 1*3*3*3*3*3*3*3;
		return sum;
	}

	template <unsigned char I>
	inline unsigned short POWER_OF_3_INV()
	{
		unsigned short sum = 0;
		if (I & 0x01) sum += 1*3*3*3*3*3*3*3;
		if (I & 0x02) sum += 1*3*3*3*3*3*3;
		if (I & 0x04) sum += 1*3*3*3*3*3;
		if (I & 0x08) sum += 1*3*3*3*3;
		if (I & 0x10) sum += 1*3*3*3;
		if (I & 0x20) sum += 1*3*3;
		if (I & 0x40) sum += 1*3;
		if (I & 0x80) sum += 1;
		return sum;
	}

	const bool Feature_C = true; 
	const bool Feature_L1 = true; 
	const bool Feature_L2 = true; 
	const bool Feature_L3 = true; 
	const bool Feature_A = true; 
	const bool Feature_Sq = true; 
	const bool Feature_B = true; 
	const bool Feature_D3 = false; 
	const bool Feature_D4 = true; 
	const bool Feature_D5 = true; 
	const bool Feature_D6 = true; 
	const bool Feature_D7 = true; 
	const bool Feature_X = false; 
	const bool Feature_PlayersBoarder = false; 
	const bool Feature_OpponentsBoarder = false; 
	const bool Feature_PlayersExposeds = false; 
	const bool Feature_OpponentsExposeds = false;
	const bool Feature_Possible = false; 
	const bool Feature_Possible_E = false; 
	const bool Feature_Possible_XC = false; 
	const bool Feature_Parity = false; 
	const bool Feature_Quadrants = false; 
	const bool Feature_NumberOfOwnStones = false;
	const int NumberOfFiles = 3;

	const int Size_C = 29646;
	const int Size_L1 = 3321;
	const int Size_L2 = 3321;
	const int Size_L3 = 3321;
	const int Size_A = 131220;
	const int Size_Sq = 10206;
	const int Size_B = 59049;
	const int Size_D3 = 18;
	const int Size_D4 = 45;
	const int Size_D5 = 135;
	const int Size_D6 = 378;
	const int Size_D7 = 1134;
	const int Size_X = 1296;
	const int Size_PlayersBoarder = 65;
	const int Size_OpponentsBoarder = 65;
	const int Size_PlayersExposeds = 65;
	const int Size_OpponentsExposeds = 65;
	const int Size_Possible = 65;
	const int Size_Possible_E = 5;
	const int Size_Possible_XC = 13;
	const int Size_Parity = 5;
	const int Size_Quadrants = 17;
	const int Size_NumberOfOwnStones = 65;

	const int Symmetry_C = 4;
	const int Symmetry_L1 = 4;
	const int Symmetry_L2 = 4;
	const int Symmetry_L3 = 4;
	const int Symmetry_A = 4;
	const int Symmetry_Sq = 4;
	const int Symmetry_B = 8;
	const int Symmetry_D3 = 4;
	const int Symmetry_D4 = 4;
	const int Symmetry_D5 = 4;
	const int Symmetry_D6 = 4;
	const int Symmetry_D7 = 4;
	const int Symmetry_X = 1;
	const int Symmetry_PlayersBoarder = 1;
	const int Symmetry_OpponentsBoarder = 1;
	const int Symmetry_PlayersExposeds = 1;
	const int Symmetry_OpponentsExposeds = 1;
	const int Symmetry_Possible = 1;
	const int Symmetry_Possible_E = 1;
	const int Symmetry_Possible_XC = 1;
	const int Symmetry_Parity = 1;
	const int Symmetry_Quadrants = 4;
	const int Symmetry_NumberOfOwnStones = 1;

	const int Size = 0
		+ (Feature_C ? Size_C : 0)
		+ (Feature_L1 ? Size_L1 : 0)
		+ (Feature_L2 ? Size_L2 : 0)
		+ (Feature_L3 ? Size_L3 : 0)
		+ (Feature_A ? Size_A : 0)
		+ (Feature_Sq ? Size_Sq : 0)
		+ (Feature_B ? Size_B : 0)
		+ (Feature_D3 ? Size_D3 : 0)
		+ (Feature_D4 ? Size_D4 : 0)
		+ (Feature_D5 ? Size_D5 : 0)
		+ (Feature_D6 ? Size_D6 : 0)
		+ (Feature_D7 ? Size_D7 : 0)
		+ (Feature_X ? Size_X : 0)
		+ (Feature_PlayersBoarder ? Size_PlayersBoarder : 0)
		+ (Feature_OpponentsBoarder ? Size_OpponentsBoarder : 0)
		+ (Feature_PlayersExposeds ? Size_PlayersExposeds : 0)
		+ (Feature_OpponentsExposeds ? Size_OpponentsExposeds : 0)
		+ (Feature_Possible ? Size_Possible : 0)
		+ (Feature_Possible_E ? Size_Possible_E : 0)
		+ (Feature_Possible_XC ? Size_Possible_XC : 0)
		+ (Feature_Parity ? Size_Parity : 0)
		+ (Feature_Quadrants ? Size_Quadrants : 0)
		+ (Feature_NumberOfOwnStones ? Size_NumberOfOwnStones : 0);

	const int Symmetries = 0
		+ (Feature_C ? Symmetry_C : 0)
		+ (Feature_L1 ? Symmetry_L1 : 0)
		+ (Feature_L2 ? Symmetry_L2 : 0)
		+ (Feature_L3 ? Symmetry_L3 : 0)
		+ (Feature_A ? Symmetry_A : 0)
		+ (Feature_Sq ? Symmetry_Sq : 0)
		+ (Feature_B ? Symmetry_B : 0)
		+ (Feature_D3 ? Symmetry_D3 : 0)
		+ (Feature_D4 ? Symmetry_D4 : 0)
		+ (Feature_D5 ? Symmetry_D5 : 0)
		+ (Feature_D6 ? Symmetry_D6 : 0)
		+ (Feature_D7 ? Symmetry_D7 : 0)
		+ (Feature_X ? Symmetry_X : 0)
		+ (Feature_PlayersBoarder ? Symmetry_PlayersBoarder : 0)
		+ (Feature_OpponentsBoarder ? Symmetry_OpponentsBoarder : 0)
		+ (Feature_PlayersExposeds ? Symmetry_PlayersExposeds : 0)
		+ (Feature_OpponentsExposeds ? Symmetry_OpponentsExposeds : 0)
		+ (Feature_Possible ? Symmetry_Possible : 0)
		+ (Feature_Possible_E ? Symmetry_Possible_E : 0)
		+ (Feature_Possible_XC ? Symmetry_Possible_XC : 0)
		+ (Feature_Parity ? Symmetry_Parity : 0)
		+ (Feature_Quadrants ? Symmetry_Quadrants : 0)
		+ (Feature_NumberOfOwnStones ? Symmetry_NumberOfOwnStones : 0);

	inline void SwapIf(int& A, int& B) { if (A > B) std::swap(A, B); }
	inline void Sort(int& A0, int& A1, int& A2, int& A3)
	{
		SwapIf(A0,A1);
		SwapIf(A2,A3);

		SwapIf(A0,A2);
		SwapIf(A1,A3);

		SwapIf(A1,A2);
	}
	inline void Sort(int& A0, int& A1, int& A2, int& A3, int& A4, int& A5, int& A6, int& A7)
	{
		SwapIf(A0,A4);
		SwapIf(A1,A5);
		SwapIf(A2,A6);
		SwapIf(A3,A7);
			
		SwapIf(A0,A2);
		SwapIf(A1,A3);
		SwapIf(A4,A6);
		SwapIf(A5,A7);
			
		SwapIf(A2,A4);
		SwapIf(A3,A5);
		SwapIf(A0,A1);
		SwapIf(A6,A7);
			
		SwapIf(A2,A3);
		SwapIf(A4,A5);

		SwapIf(A1,A4);
		SwapIf(A3,A6);
			
		SwapIf(A1,A2);
		SwapIf(A3,A4);
		SwapIf(A5,A6);
	}
	inline void Sort4(int* const Array, const int Index) { Sort(Array[Index], Array[Index+1], Array[Index+2], Array[Index+3]); }
	inline void Sort8(int* const Array, const int Index) { Sort(Array[Index], Array[Index+1], Array[Index+2], Array[Index+3], Array[Index+4], Array[Index+5], Array[Index+6], Array[Index+7]); }
}

class CActiveConfigurations
{
private:
	unsigned long long m_P, m_O;
	int m_Array[Features::Symmetries];
public:
	CActiveConfigurations(const unsigned long long P, const unsigned long long O);
	int EvaluateFeatures() const;
	int EvaluateFeatures(const unsigned long long P, const unsigned long long O) const;
};

void LoadFeatureWeights();

void FillConfigurationArray(const unsigned long long P, const unsigned long long O, int* const Array);
void FillConfigurationArraySorted(const unsigned long long P, const unsigned long long O, int* const Array);

int EvaluateFeatures(const unsigned long long P, const unsigned long long O);

bool Test_All_Features();