#pragma once
#include "configfile.h"
#include "datamanipulation.h"
#include "position.h"
#include "utility.h"
#include <functional>
#include <iostream>
#include <vector>

namespace Features
{
	template <unsigned int power> inline unsigned int Power3() { return 3 * Power3<power-1>(); }
	template <> inline unsigned int Power3<0>() { return 1; }
	
	const bool Feature_C                 = true; 
	const bool Feature_L1                = true; 
	const bool Feature_L2                = true; 
	const bool Feature_L3                = true; 
	const bool Feature_A                 = true; 
	const bool Feature_Sq                = true; 
	const bool Feature_B                 = true; 
	const bool Feature_D3                = false; 
	const bool Feature_D4                = true; 
	const bool Feature_D5                = true; 
	const bool Feature_D6                = true; 
	const bool Feature_D7                = true; 
	const bool Feature_X                 = false; 
	const bool Feature_PlayersBoarder    = false; 
	const bool Feature_OpponentsBoarder  = false; 
	const bool Feature_PlayersExposeds   = false; 
	const bool Feature_OpponentsExposeds = false;
	const bool Feature_Possible          = false; 
	const bool Feature_Possible_E        = false; 
	const bool Feature_Possible_XC       = false; 
	const bool Feature_Parity            = false; 
	const bool Feature_Quadrants         = false; 
	const bool Feature_NumberOfOwnStones = false;

	const int ReducedSize_C = 29646;
	const int ReducedSize_L1 = 3321;
	const int ReducedSize_L2 = 3321;
	const int ReducedSize_L3 = 3321;
	const int ReducedSize_A = 131220;
	const int ReducedSize_Sq = 10206;
	const int ReducedSize_B = 59049;
	const int ReducedSize_D3 = 18;
	const int ReducedSize_D4 = 45;
	const int ReducedSize_D5 = 135;
	const int ReducedSize_D6 = 378;
	const int ReducedSize_D7 = 1134;
	const int ReducedSize_X = 1296;
	//const int ReducedSize_PlayersBoarder = 65;
	//const int ReducedSize_OpponentsBoarder = 65;
	//const int ReducedSize_PlayersExposeds = 65;
	//const int ReducedSize_OpponentsExposeds = 65;
	//const int ReducedSize_Possible = 65;
	//const int ReducedSize_Possible_E = 5;
	//const int ReducedSize_Possible_XC = 13;
	//const int ReducedSize_Parity = 5;
	//const int ReducedSize_Quadrants = 17;
	//const int ReducedSize_NumberOfOwnStones = 65;
	const int FullSize_C = 59049;
	const int FullSize_L1 = 6561;
	const int FullSize_L2 = 6561;
	const int FullSize_L3 = 6561;
	const int FullSize_A = 531441;
	const int FullSize_Sq = 19683;
	const int FullSize_B = 59049;
	const int FullSize_D3 = 27;
	const int FullSize_D4 = 81;
	const int FullSize_D5 = 243;
	const int FullSize_D6 = 729;
	const int FullSize_D7 = 2187;
	const int FullSize_X = 6561;

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

	const int ReducedSize = 0
		+ (Feature_C ? ReducedSize_C : 0)
		+ (Feature_L1 ? ReducedSize_L1 : 0)
		+ (Feature_L2 ? ReducedSize_L2 : 0)
		+ (Feature_L3 ? ReducedSize_L3 : 0)
		+ (Feature_A ? ReducedSize_A : 0)
		+ (Feature_Sq ? ReducedSize_Sq : 0)
		+ (Feature_B ? ReducedSize_B : 0)
		+ (Feature_D3 ? ReducedSize_D3 : 0)
		+ (Feature_D4 ? ReducedSize_D4 : 0)
		+ (Feature_D5 ? ReducedSize_D5 : 0)
		+ (Feature_D6 ? ReducedSize_D6 : 0)
		+ (Feature_D7 ? ReducedSize_D7 : 0)
		+ (Feature_X ? ReducedSize_X : 0);
	
	const int FullSize = 0
		+ (Feature_C ? FullSize_C : 0)
		+ (Feature_L1 ? FullSize_L1 : 0)
		+ (Feature_L2 ? FullSize_L2 : 0)
		+ (Feature_L3 ? FullSize_L3 : 0)
		+ (Feature_A ? FullSize_A : 0)
		+ (Feature_Sq ? FullSize_Sq : 0)
		+ (Feature_B ? FullSize_B : 0)
		+ (Feature_D3 ? FullSize_D3 : 0)
		+ (Feature_D4 ? FullSize_D4 : 0)
		+ (Feature_D5 ? FullSize_D5 : 0)
		+ (Feature_D6 ? FullSize_D6 : 0)
		+ (Feature_D7 ? FullSize_D7 : 0)
		+ (Feature_X ? FullSize_X : 0);

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

	void Initialize();
	void Finalize();
}

class CActiveConfigurations
{
private:
	int m_Array[Features::Symmetries];
	unsigned long long m_P, m_O;
public:
	CActiveConfigurations(const unsigned long long P, const unsigned long long O);
	int EvaluateFeatures() const;
	int EvaluateFeatures(const unsigned long long P, const unsigned long long O) const;
};

void FillReducedConfigurationArray(const unsigned long long P, const unsigned long long O, int* const Array);
void FillReducedConfigurationArraySorted(const unsigned long long P, const unsigned long long O, int* const Array);

int EvaluateFeatures(const unsigned long long P, const unsigned long long O);

bool Test_All_Features();