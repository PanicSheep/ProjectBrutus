#include "features.h"

/**
	DiagLookUp2 =     8 Byte
	BoxIndex    =    61 Byte
	PowerOf3    =   512 Byte
	PowerOf3inv =   512 Byte
	Weights     = NumberOfFiles * Size * sizeof(float) Byte
	----------------------
	              1'093 Byte
**/

void For_each_configuration_in_pattern_do_fkt(const unsigned long long Pattern, std::function<void(const unsigned long long, const unsigned long long)> fkt);

namespace Features
{
	//const unsigned short DiagLookUp[4][4] = {
	//	{0xFFFF, 0xFFFF, 0xFFFF,      0},
	//	{0xFFFF, 0xFFFF,      1, 0xFFFF},
	//	{0xFFFF,      2, 0xFFFF, 0xFFFF},
	//	{     3, 0xFFFF, 0xFFFF, 0xFFFF}
	//};

	const unsigned short DiagLookUp2[4] = {0, 729, 1458, 2187};

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

	unsigned short PowerOf3[1024];
	unsigned short PowerOf3inv_2[1024];
	unsigned short PowerOf3inv[256];

	float (*Weights)[FullSize];

	const unsigned long long Pattern_LowerC       = 0x00000000000042FFULL;
	const unsigned long long Pattern_UpperC       = 0xFF42000000000000ULL;
	const unsigned long long Pattern_LeftC        = 0x80C080808080C080ULL;
	const unsigned long long Pattern_RightC       = 0x0103010101010301ULL;
	const unsigned long long Pattern_LowerL1      = 0x000000000000FF00ULL;
	const unsigned long long Pattern_UpperL1      = 0x00FF000000000000ULL;
	const unsigned long long Pattern_LeftL1       = 0x4040404040404040ULL;
	const unsigned long long Pattern_RightL1      = 0x0202020202020202ULL;
	const unsigned long long Pattern_LowerL2      = 0x0000000000FF0000ULL;
	const unsigned long long Pattern_UpperL2      = 0x0000FF0000000000ULL;
	const unsigned long long Pattern_LeftL2       = 0x2020202020202020ULL;
	const unsigned long long Pattern_RightL2      = 0x0404040404040404ULL;
	const unsigned long long Pattern_LowerL3      = 0x00000000FF000000ULL;
	const unsigned long long Pattern_UpperL3      = 0x000000FF00000000ULL;
	const unsigned long long Pattern_LeftL3       = 0x1010101010101010ULL;
	const unsigned long long Pattern_RightL3      = 0x0808080808080808ULL;
	const unsigned long long Pattern_RightLowerA  = 0x8040201008050307ULL;
	const unsigned long long Pattern_LeftUpperA   = 0xE0C0A01008040201ULL;
	const unsigned long long Pattern_LeftLowerA   = 0x0102040810A0C0E0ULL;
	const unsigned long long Pattern_RightUpperA  = 0x0703050810204080ULL;
	const unsigned long long Pattern_RightLowerSq = 0x0000000000070707ULL;
	const unsigned long long Pattern_LeftUpperSq  = 0xE0E0E00000000000ULL;
	const unsigned long long Pattern_LeftLowerSq  = 0x0000000000E0E0E0ULL;
	const unsigned long long Pattern_RightUpperSq = 0x0707070000000000ULL;
	const unsigned long long Pattern_LowerRightB  = 0x0000000000001F1FULL;
	const unsigned long long Pattern_LowerLeftB   = 0x000000000000F8F8ULL;
	const unsigned long long Pattern_LeftLowerB   = 0x000000C0C0C0C0C0ULL;
	const unsigned long long Pattern_LeftUpperB   = 0xC0C0C0C0C0000000ULL;
	const unsigned long long Pattern_UpperLeftB   = 0xF8F8000000000000ULL;
	const unsigned long long Pattern_UpperRightB  = 0x1F1F000000000000ULL;
	const unsigned long long Pattern_RightUpperB  = 0x0303030303000000ULL;
	const unsigned long long Pattern_RightLowerB  = 0x0000000303030303ULL;
	const unsigned long long Pattern_RightLowerD3 = 0x0000000000010204ULL;
	const unsigned long long Pattern_LeftUpperD3  = 0x2040800000000000ULL;
	const unsigned long long Pattern_LeftLowerD3  = 0x0000000000804020ULL;
	const unsigned long long Pattern_RightUpperD3 = 0x0402010000000000ULL;
	const unsigned long long Pattern_RightLowerD4 = 0x0000000001020408ULL;
	const unsigned long long Pattern_LeftUpperD4  = 0x1020408000000000ULL;
	const unsigned long long Pattern_LeftLowerD4  = 0x0000000080402010ULL;
	const unsigned long long Pattern_RightUpperD4 = 0x0804020100000000ULL;
	const unsigned long long Pattern_RightLowerD5 = 0x0000000102040810ULL;
	const unsigned long long Pattern_LeftUpperD5  = 0x0810204080000000ULL;
	const unsigned long long Pattern_LeftLowerD5  = 0x0000008040201008ULL;
	const unsigned long long Pattern_RightUpperD5 = 0x1008040201000000ULL;
	const unsigned long long Pattern_RightLowerD6 = 0x0000010204081020ULL;
	const unsigned long long Pattern_LeftUpperD6  = 0x0408102040800000ULL;
	const unsigned long long Pattern_LeftLowerD6  = 0x0000804020100804ULL;
	const unsigned long long Pattern_RightUpperD6 = 0x2010080402010000ULL;
	const unsigned long long Pattern_RightLowerD7 = 0x0001020408102040ULL;
	const unsigned long long Pattern_LeftUpperD7  = 0x0204081020408000ULL;
	const unsigned long long Pattern_LeftLowerD7  = 0x0080402010080402ULL;
	const unsigned long long Pattern_RightUpperD7 = 0x4020100804020100ULL;
	const unsigned long long Pattern_CenterX      = 0x0000241818240000ULL;

	int ReducedIndex_LowerC(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[((P & 0x00000000000040F0ULL) * 0x0082000000000000ULL) >> 59] * 2
			 + PowerOf3[((O & 0x00000000000040F0ULL) * 0x0082000000000000ULL) >> 59];
		sym2 = PowerOf3inv[((P & 0x000000000000020FULL) * 0x1004000000000000ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x000000000000020FULL) * 0x1004000000000000ULL) >> 56];
		//if (sym1 > sym2)
		//	return 243*sym2+sym1 - (((sym2+1)*sym2) >> 1);
		//else
		//	return 243*sym1+sym2 - (((sym1+1)*sym1) >> 1);
		return sym1 > sym2 ? 243*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 243*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_UpperC(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[((P & 0xF000000000000000ULL) >> 60) | ((P & 0x0040000000000000ULL) >> 50)] * 2
			 + PowerOf3[((O & 0xF000000000000000ULL) >> 60) | ((O & 0x0040000000000000ULL) >> 50)];
		//sym2 = PowerOf3inv[((P & 0x0F00000000000000ULL) >> 52) | ((P & 0x0002000000000000ULL) >> 41)] * 2
		//	 + PowerOf3inv[((O & 0x0F00000000000000ULL) >> 52) | ((O & 0x0002000000000000ULL) >> 41)];

		//sym1 = PowerOf3[(((P & 0xF040000000000000ULL) >> 1) * 0x0000000000000401ULL) >> 59] * 2
		//	 + PowerOf3[(((O & 0xF040000000000000ULL) >> 1) * 0x0000000000000401ULL) >> 59];
		sym2 = PowerOf3inv[((P & 0x0F02000000000000ULL) * 0x0000000000000410ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x0F02000000000000ULL) * 0x0000000000000410ULL) >> 56];

		//if (sym1 > sym2)
		//	return 243*sym2+sym1 - (((sym2+1)*sym2) >> 1);
		//else
		//	return 243*sym1+sym2 - (((sym1+1)*sym1) >> 1);
		return sym1 > sym2 ? 243*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 243*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_LeftC(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		//sym1 = PowerOf3inv[(((P & 0xC080808000000000ULL) >> 3) * 0x0000000008040201ULL) >> 56] * 2
		//	 + PowerOf3inv[(((O & 0xC080808000000000ULL) >> 3) * 0x0000000008040201ULL) >> 56];
		//sym2 = PowerOf3inv[((P & 0x00000000808080C0ULL) * 0x0020408100000000ULL) >> 56] * 2
		//	 + PowerOf3inv[((O & 0x00000000808080C0ULL) * 0x0020408100000000ULL) >> 56];
		sym1 = PowerOf3inv[(((P & 0x8080808000000000ULL) >> 3) * 0x0000000008040201ULL) >> 56 | (P & 0x0040000000000000ULL) >> 51] * 2
			 + PowerOf3inv[(((O & 0x8080808000000000ULL) >> 3) * 0x0000000008040201ULL) >> 56 | (O & 0x0040000000000000ULL) >> 51];
		sym2 = PowerOf3inv[((P & 0x0000000080808080ULL) * 0x0020408100000000ULL) >> 56 | (P & 0x0000000000004000ULL) >> 11] * 2
			 + PowerOf3inv[((O & 0x0000000080808080ULL) * 0x0020408100000000ULL) >> 56 | (O & 0x0000000000004000ULL) >> 11];
		//if (sym1 > sym2)
		//	return 243*sym2+sym1 - (((sym2+1)*sym2) >> 1);
		//else
		//	return 243*sym1+sym2 - (((sym1+1)*sym1) >> 1);
		return sym1 > sym2 ? 243*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 243*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_RightC(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		//sym1 = PowerOf3[((P & 0x0301010100000000ULL) * 0x0000000008102040ULL) >> 59] * 2
		//	 + PowerOf3[((O & 0x0301010100000000ULL) * 0x0000000008102040ULL) >> 59];
		//sym2 = PowerOf3[((P & 0x0000000001010103ULL) * 0x4020100800000000ULL) >> 59] * 2
		//	 + PowerOf3[((O & 0x0000000001010103ULL) * 0x4020100800000000ULL) >> 59];
		sym1 = PowerOf3[((P & 0x0101010100000000ULL) * 0x0000000008102040ULL) >> 59 | (P & 0x0002000000000000ULL) >> 45] * 2
			 + PowerOf3[((O & 0x0101010100000000ULL) * 0x0000000008102040ULL) >> 59 | (O & 0x0002000000000000ULL) >> 45];
		sym2 = PowerOf3[((P & 0x0000000001010101ULL) * 0x4020100800000000ULL) >> 59 | (P & 0x0000000000000200ULL) >> 5] * 2
			 + PowerOf3[((O & 0x0000000001010101ULL) * 0x4020100800000000ULL) >> 59 | (O & 0x0000000000000200ULL) >> 5];
		//if (sym1 > sym2)
		//	return 243*sym2+sym1 - (((sym2+1)*sym2) >> 1);
		//else
		//	return 243*sym1+sym2 - (((sym1+1)*sym1) >> 1);
		return sym1 > sym2 ? 243*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 243*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_LowerL1(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[(P & 0x0000000000000F00ULL) >> 8] * 2
			 + PowerOf3[(O & 0x0000000000000F00ULL) >> 8];
		sym2 = PowerOf3inv[(P & 0x000000000000F000ULL) >> 8] * 2
			 + PowerOf3inv[(O & 0x000000000000F000ULL) >> 8];
		//if (sym1 > sym2)
		//	return 81*sym2+sym1 - (((sym2+1)*sym2) >> 1);
		//else
		//	return 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
		return sym1 > sym2 ? 81*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_LowerL2(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[(P & 0x00000000000F0000ULL) >> 16] * 2
			 + PowerOf3[(O & 0x00000000000F0000ULL) >> 16];
		sym2 = PowerOf3inv[(P & 0x0000000000F00000ULL) >> 16] * 2
			 + PowerOf3inv[(O & 0x0000000000F00000ULL) >> 16];
		//if (sym1 > sym2)
		//	return 81*sym2+sym1 - (((sym2+1)*sym2) >> 1);
		//else
		//	return 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
		return sym1 > sym2 ? 81*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_LowerL3(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[(P & 0x000000000F000000ULL) >> 24] * 2
			 + PowerOf3[(O & 0x000000000F000000ULL) >> 24];
		sym2 = PowerOf3inv[(P & 0x00000000F0000000ULL) >> 24] * 2
			 + PowerOf3inv[(O & 0x00000000F0000000ULL) >> 24];
		//if (sym1 > sym2)
		//	return 81*sym2+sym1 - (((sym2+1)*sym2) >> 1);
		//else
		//	return 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
		return sym1 > sym2 ? 81*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_UpperL3(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[(P & 0x0000000F00000000ULL) >> 32] * 2
			 + PowerOf3[(O & 0x0000000F00000000ULL) >> 32];
		sym2 = PowerOf3inv[(P & 0x000000F000000000ULL) >> 32] * 2
			 + PowerOf3inv[(O & 0x000000F000000000ULL) >> 32];
		//if (sym1 > sym2)
		//	return 81*sym2+sym1 - (((sym2+1)*sym2) >> 1);
		//else
		//	return 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
		return sym1 > sym2 ? 81*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_UpperL2(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[(P & 0x00000F0000000000ULL) >> 40] * 2
			 + PowerOf3[(O & 0x00000F0000000000ULL) >> 40];
		sym2 = PowerOf3inv[(P & 0x0000F00000000000ULL) >> 40] * 2
			 + PowerOf3inv[(O & 0x0000F00000000000ULL) >> 40];
		//if (sym1 > sym2)
		//	return 81*sym2+sym1 - (((sym2+1)*sym2) >> 1);
		//else
		//	return 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
		return sym1 > sym2 ? 81*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_UpperL1(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[(P & 0x000F000000000000ULL) >> 48] * 2
			 + PowerOf3[(O & 0x000F000000000000ULL) >> 48];
		sym2 = PowerOf3inv[(P & 0x00F0000000000000ULL) >> 48] * 2
			 + PowerOf3inv[(O & 0x00F0000000000000ULL) >> 48];
		//if (sym1 > sym2)
		//	return 81*sym2+sym1 - (((sym2+1)*sym2) >> 1);
		//else
		//	return 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
		return sym1 > sym2 ? 81*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_LeftL1(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3inv[((P & 0x4040404000000000ULL) * 0x0000000000408102ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x4040404000000000ULL) * 0x0000000000408102ULL) >> 56];
		sym2 = PowerOf3inv[((P & 0x0000000040404040ULL) * 0x0201008040000000ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x0000000040404040ULL) * 0x0201008040000000ULL) >> 56];
		//if (sym1 > sym2)
		//	return 81*sym2+sym1 - (((sym2+1)*sym2) >> 1);
		//else
		//	return 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
		return sym1 > sym2 ? 81*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_LeftL2(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3inv[((P & 0x2020202000000000ULL) * 0x0000000000810204ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x2020202000000000ULL) * 0x0000000000810204ULL) >> 56];
		sym2 = PowerOf3inv[((P & 0x0000000020202020ULL) * 0x0402010080000000ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x0000000020202020ULL) * 0x0402010080000000ULL) >> 56];
		//if (sym1 > sym2)
		//	return 81*sym2+sym1 - (((sym2+1)*sym2) >> 1);
		//else
		//	return 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
		return sym1 > sym2 ? 81*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_LeftL3(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3inv[((P & 0x1010101000000000ULL) * 0x0000000001020408ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x1010101000000000ULL) * 0x0000000001020408ULL) >> 56];
		sym2 = PowerOf3inv[((P & 0x0000000010101010ULL) * 0x0804020100000000ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x0000000010101010ULL) * 0x0804020100000000ULL) >> 56];
		//if (sym1 > sym2)
		//	return 81*sym2+sym1 - (((sym2+1)*sym2) >> 1);
		//else
		//	return 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
		return sym1 > sym2 ? 81*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_RightL3(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3inv[((P & 0x0808080800000000ULL) * 0x0000000002040810ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x0808080800000000ULL) * 0x0000000002040810ULL) >> 56];
		sym2 = PowerOf3inv[((P & 0x0000000008080808ULL) * 0x1008040200000000ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x0000000008080808ULL) * 0x1008040200000000ULL) >> 56];
		//if (sym1 > sym2)
		//	return 81*sym2+sym1 - (((sym2+1)*sym2) >> 1);
		//else
		//	return 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
		return sym1 > sym2 ? 81*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_RightL2(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3inv[((P & 0x0404040400000000ULL) * 0x0000000004081020ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x0404040400000000ULL) * 0x0000000004081020ULL) >> 56];
		sym2 = PowerOf3inv[((P & 0x0000000004040404ULL) * 0x2010080400000000ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x0000000004040404ULL) * 0x2010080400000000ULL) >> 56];
		//if (sym1 > sym2)
		//	return 81*sym2+sym1 - (((sym2+1)*sym2) >> 1);
		//else
		//	return 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
		return sym1 > sym2 ? 81*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_RightL1(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3inv[((P & 0x0202020200000000ULL) * 0x0000000008102040ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x0202020200000000ULL) * 0x0000000008102040ULL) >> 56];
		sym2 = PowerOf3inv[((P & 0x0000000002020202ULL) * 0x4020100800000000ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x0000000002020202ULL) * 0x4020100800000000ULL) >> 56];
		//if (sym1 > sym2)
		//	return 81*sym2+sym1 - (((sym2+1)*sym2) >> 1);
		//else
		//	return 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
		return sym1 > sym2 ? 81*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 81*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_RightLowerA(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2, diag;
		sym1 = PowerOf3[(P & 0x0000000000000006ULL) >> 1] * 2
			 + PowerOf3[(O & 0x0000000000000006ULL) >> 1];
		sym2 = PowerOf3[((P & 0x0000000000010100ULL) * 0x0040800000000000ULL) >> 62] * 2
			 + PowerOf3[((O & 0x0000000000010100ULL) * 0x0040800000000000ULL) >> 62];
		diag = PowerOf3[((P & 0x8040200000040201ULL) * 0x0404040000010101ULL) >> 58] * 2
			 + PowerOf3[((O & 0x8040200000040201ULL) * 0x0404040000010101ULL) >> 58]
			 //+ DiagLookUp[((P & 0x0000001008000000ULL) * 0x0000000101000000ULL) >> 59]
				//		 [((O & 0x0000001008000000ULL) * 0x0000000101000000ULL) >> 59] * 729;
			 + DiagLookUp2[((P & 0x0000001008000000ULL) * 0x0000000101000000ULL) >> 59];
		//if (sym1 > sym2)
		//	return 2916*(9*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag;
		//else
		//	return 2916*(9*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
		return sym1 > sym2 ? 2916*(9*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag : 2916*(9*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
	}
	int ReducedIndex_LeftUpperA(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2, diag;
		sym1 = PowerOf3inv[(P & 0x6000000000000000ULL) >> 55] * 2
			 + PowerOf3inv[(O & 0x6000000000000000ULL) >> 55];
		sym2 = PowerOf3inv[((P & 0x0080800000000000ULL) * 0x0000000000008100ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x0080800000000000ULL) * 0x0000000000008100ULL) >> 56];
		diag = PowerOf3inv[((P & 0x8040200000040201ULL) * 0x0404040000010101ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x8040200000040201ULL) * 0x0404040000010101ULL) >> 56]
			 //+ DiagLookUp[((P & 0x0000001008000000ULL) * 0x0000000200800000ULL) >> 59]
				//		 [((O & 0x0000001008000000ULL) * 0x0000000200800000ULL) >> 59] * 729;
			 + DiagLookUp2[((P & 0x0000001008000000ULL) * 0x0000000200800000ULL) >> 59];
		//if (sym1 > sym2)
		//	return 2916*(9*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag;
		//else
		//	return 2916*(9*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
		return sym1 > sym2 ? 2916*(9*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag : 2916*(9*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
	}
	int ReducedIndex_LeftLowerA(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2, diag;
		sym1 = PowerOf3inv[(P & 0x0000000000000060ULL) << 1] * 2
			 + PowerOf3inv[(O & 0x0000000000000060ULL) << 1];
		sym2 = PowerOf3[((P & 0x0000000000808000ULL) * 0x0000810000000000ULL) >> 62] * 2
			 + PowerOf3[((O & 0x0000000000808000ULL) * 0x0000810000000000ULL) >> 62];
		diag = PowerOf3inv[((P & 0x0102040000204080ULL) * 0x0101010000040404ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x0102040000204080ULL) * 0x0101010000040404ULL) >> 56]
			 //+ DiagLookUp[((P & 0x0000000810000000ULL) * 0x0000000082000000ULL) >> 59]
				//		 [((O & 0x0000000810000000ULL) * 0x0000000082000000ULL) >> 59] * 729;
			 + DiagLookUp2[((P & 0x0000000810000000ULL) * 0x0000000082000000ULL) >> 59];
		//if (sym1 > sym2)
		//	return 2916*(9*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag;
		//else
		//	return 2916*(9*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
		return sym1 > sym2 ? 2916*(9*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag : 2916*(9*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
	}
	int ReducedIndex_RightUpperA(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2, diag;
		sym1 = PowerOf3[(P & 0x0600000000000000ULL) >> 57] * 2
			 + PowerOf3[(O & 0x0600000000000000ULL) >> 57];
		sym2 = PowerOf3[((P & 0x0001010000000000ULL) * 0x0000000000100800ULL) >> 59] * 2
			 + PowerOf3[((O & 0x0001010000000000ULL) * 0x0000000000100800ULL) >> 59];
		diag = PowerOf3[((P & 0x0102040000204080ULL) * 0x0101010000040404ULL) >> 58] * 2
			 + PowerOf3[((O & 0x0102040000204080ULL) * 0x0101010000040404ULL) >> 58]
			 //+ DiagLookUp[((P & 0x0000000810000000ULL) * 0x0000000101000000ULL) >> 59]
				//		 [((O & 0x0000000810000000ULL) * 0x0000000101000000ULL) >> 59] * 729;
			 + DiagLookUp2[((P & 0x0000000810000000ULL) * 0x0000000101000000ULL) >> 59];
		//if (sym1 > sym2)
		//	return 2916*(9*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag;
		//else
		//	return 2916*(9*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
		return sym1 > sym2 ? 2916*(9*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag : 2916*(9*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
	}
	int ReducedIndex_RightLowerSq(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2, diag;
		sym1 = PowerOf3[((P & 0x0000000000000406ULL) * 0x1020000000000000ULL) >> 61] * 2
			 + PowerOf3[((O & 0x0000000000000406ULL) * 0x1020000000000000ULL) >> 61];
		sym2 = PowerOf3[((P & 0x0000000000030100ULL) * 0x0020400000000000ULL) >> 61] * 2
			 + PowerOf3[((O & 0x0000000000030100ULL) * 0x0020400000000000ULL) >> 61];
		diag = PowerOf3[((P & 0x0000000000040201ULL) * 0x0101010101010101ULL) >> 56] * 2
			 + PowerOf3[((O & 0x0000000000040201ULL) * 0x0101010101010101ULL) >> 56];
		//if (sym1 > sym2)
		//	return 27*(27*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag;
		//else
		//	return 27*(27*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
		return sym1 > sym2 ? 27*(27*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag : 27*(27*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
	}
	int ReducedIndex_LeftUpperSq(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2, diag;
		sym1 = PowerOf3inv[((P & 0x0080C00000000000ULL) * 0x0000000000008100ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x0080C00000000000ULL) * 0x0000000000008100ULL) >> 56];
		sym2 = PowerOf3inv[((P & 0x6020000000000000ULL) * 0x0000000000000102ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x6020000000000000ULL) * 0x0000000000000102ULL) >> 56];
		diag = PowerOf3inv[((P & 0x8040200000000000ULL) * 0x0101010101010101ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x8040200000000000ULL) * 0x0101010101010101ULL) >> 56];
		//if (sym1 > sym2)
		//	return 27*(27*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag;
		//else
		//	return 27*(27*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
		return sym1 > sym2 ? 27*(27*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag : 27*(27*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
	}
	int ReducedIndex_LeftLowerSq(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2, diag;
		sym1 = PowerOf3inv[((P & 0x0000000000002060ULL) * 0x0201000000000000ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x0000000000002060ULL) * 0x0201000000000000ULL) >> 56];
		sym2 = PowerOf3inv[((P & 0x0000000000C08000ULL) * 0x0001008000000000ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x0000000000C08000ULL) * 0x0001008000000000ULL) >> 56];
		diag = PowerOf3inv[((P & 0x0000000000204080ULL) * 0x0101010101010101ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x0000000000204080ULL) * 0x0101010101010101ULL) >> 56];
		//if (sym1 > sym2)
		//	return 27*(27*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag;
		//else
		//	return 27*(27*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
		return sym1 > sym2 ? 27*(27*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag : 27*(27*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
	}
	int ReducedIndex_RightUpperSq(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2, diag;
		sym1 = PowerOf3[((P & 0x0604000000000000ULL) * 0x0000000000002010ULL) >> 61] * 2
			 + PowerOf3[((O & 0x0604000000000000ULL) * 0x0000000000002010ULL) >> 61];
		sym2 = PowerOf3[((P & 0x0001030000000000ULL) * 0x0000000000402000ULL) >> 61] * 2
			 + PowerOf3[((O & 0x0001030000000000ULL) * 0x0000000000402000ULL) >> 61];
		diag = PowerOf3[((P & 0x0102040000000000ULL) * 0x0101010101010101ULL) >> 56] * 2
			 + PowerOf3[((O & 0x0102040000000000ULL) * 0x0101010101010101ULL) >> 56];
		//if (sym1 > sym2)
		//	return 27*(27*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag;
		//else
		//	return 27*(27*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
		return sym1 > sym2 ? 27*(27*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag : 27*(27*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
	}
	int ReducedIndex_LowerRightB(const unsigned long long P, const unsigned long long O)
	{
		return PowerOf3[P & 0x000000000000001FULL] * 486
			 + PowerOf3[O & 0x000000000000001FULL] * 243
			 + PowerOf3[(P & 0x0000000000001F00ULL) >> 8] * 2
			 + PowerOf3[(O & 0x0000000000001F00ULL) >> 8];
	}
	int ReducedIndex_LowerLeftB(const unsigned long long P, const unsigned long long O)
	{
		return PowerOf3inv[P & 0x00000000000000F8ULL] * 486
			 + PowerOf3inv[O & 0x00000000000000F8ULL] * 243
			 + PowerOf3inv[(P & 0x000000000000F800ULL) >> 8] * 2
			 + PowerOf3inv[(O & 0x000000000000F800ULL) >> 8];
	}
	int ReducedIndex_LeftLowerB(const unsigned long long P, const unsigned long long O)
	{
		return PowerOf3[((P & 0x0000008080808080ULL) * 0x0010204081000000ULL) >> 59] * 486
			 + PowerOf3[((O & 0x0000008080808080ULL) * 0x0010204081000000ULL) >> 59] * 243
			 + PowerOf3[((P & 0x0000004040404040ULL) * 0x0020408102000000ULL) >> 59] * 2
			 + PowerOf3[((O & 0x0000004040404040ULL) * 0x0020408102000000ULL) >> 59];
	}
	int ReducedIndex_LeftUpperB(const unsigned long long P, const unsigned long long O)
	{
		return PowerOf3inv[((P & 0x8080808080000000ULL) * 0x0000000010204081ULL) >> 56] * 486
			 + PowerOf3inv[((O & 0x8080808080000000ULL) * 0x0000000010204081ULL) >> 56] * 243
			 + PowerOf3inv[((P & 0x4040404040000000ULL) * 0x0000000010204081ULL) >> 55] * 2
			 + PowerOf3inv[((O & 0x4040404040000000ULL) * 0x0000000010204081ULL) >> 55];
	}
	int ReducedIndex_UpperLeftB(const unsigned long long P, const unsigned long long O)
	{
		return PowerOf3inv[(P & 0xF800000000000000ULL) >> 56] * 486
			 + PowerOf3inv[(O & 0xF800000000000000ULL) >> 56] * 243
			 + PowerOf3inv[(P & 0x00F8000000000000ULL) >> 48] * 2
			 + PowerOf3inv[(O & 0x00F8000000000000ULL) >> 48];
	}
	int ReducedIndex_UpperRightB(const unsigned long long P, const unsigned long long O)
	{
		return PowerOf3[(P & 0x1F00000000000000ULL) >> 56] * 486
			 + PowerOf3[(O & 0x1F00000000000000ULL) >> 56] * 243
			 + PowerOf3[(P & 0x001F000000000000ULL) >> 48] * 2
			 + PowerOf3[(O & 0x001F000000000000ULL) >> 48];
	}
	int ReducedIndex_RightUpperB(const unsigned long long P, const unsigned long long O)
	{
		return PowerOf3[((P & 0x0101010101000000ULL) * 0x0000001008040201ULL) >> 56] * 486
			 + PowerOf3[((O & 0x0101010101000000ULL) * 0x0000001008040201ULL) >> 56] * 243
			 + PowerOf3[((P & 0x0202020202000000ULL) * 0x0000001008040201ULL) >> 57] * 2
			 + PowerOf3[((O & 0x0202020202000000ULL) * 0x0000001008040201ULL) >> 57];
	}
	int ReducedIndex_RightLowerB(const unsigned long long P, const unsigned long long O)
	{
		return PowerOf3inv[((P & 0x0000000101010101ULL) * 0x8040201008000000ULL) >> 56] * 486
			 + PowerOf3inv[((O & 0x0000000101010101ULL) * 0x8040201008000000ULL) >> 56] * 243
			 + PowerOf3inv[((P & 0x0000000202020202ULL) * 0x4020100804000000ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x0000000202020202ULL) * 0x4020100804000000ULL) >> 56];
	}
	int ReducedIndex_RightLowerD3(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2, diag;
		sym1 = ((P & 0x0000000000000004ULL) >>  1)
			 + ((O & 0x0000000000000004ULL) >>  2);
		sym2 = ((P & 0x0000000000010000ULL) >> 15)
			 + ((O & 0x0000000000010000ULL) >> 16);
		diag = ((P & 0x0000000000000200ULL) >>  8)
			 + ((O & 0x0000000000000200ULL) >>  9);
		return sym1 > sym2 ? 3*(3*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag : 3*(3*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
	}
	int ReducedIndex_LeftUpperD3(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2, diag;
		sym1 = ((P & 0x2000000000000000ULL) >> 60)
			 + ((O & 0x2000000000000000ULL) >> 61);
		sym2 = ((P & 0x0000800000000000ULL) >> 46)
			 + ((O & 0x0000800000000000ULL) >> 47);
		diag = ((P & 0x0040000000000000ULL) >> 53)
			 + ((O & 0x0040000000000000ULL) >> 54);
		return sym1 > sym2 ? 3*(3*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag : 3*(3*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
	}
	int ReducedIndex_LeftLowerD3(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2, diag;
		sym1 = ((P & 0x0000000000000020ULL) >>  4)
			 + ((O & 0x0000000000000020ULL) >>  5);
		sym2 = ((P & 0x0000000000800000ULL) >> 22)
			 + ((O & 0x0000000000800000ULL) >> 23);
		diag = ((P & 0x0000000000004000ULL) >> 13)
			 + ((O & 0x0000000000004000ULL) >> 14);
		return sym1 > sym2 ? 3*(3*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag : 3*(3*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
	}
	int ReducedIndex_RightUpperD3(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2, diag;
		sym1 = ((P & 0x0400000000000000ULL) >> 57)
			 + ((O & 0x0400000000000000ULL) >> 58);
		sym2 = ((P & 0x0000010000000000ULL) >> 39)
			 + ((O & 0x0000010000000000ULL) >> 40);
		diag = ((P & 0x0002000000000000ULL) >> 48)
			 + ((O & 0x0002000000000000ULL) >> 49);
		return sym1 > sym2 ? 3*(3*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag : 3*(3*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
	}
	int ReducedIndex_RightLowerD4(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[((P & 0x0000000000000408ULL) * 0x0101010101010101ULL) >> 58] * 2
			 + PowerOf3[((O & 0x0000000000000408ULL) * 0x0101010101010101ULL) >> 58];
		sym2 = PowerOf3inv[((P & 0x0000000001020000ULL) * 0x0101010101010101ULL) >> 50] * 2
			 + PowerOf3inv[((O & 0x0000000001020000ULL) * 0x0101010101010101ULL) >> 50];
		return sym1 > sym2 ? 9*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 9*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_LeftUpperD4(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[((P & 0x0000408000000000ULL) * 0x0101010101010101ULL) >> 62] * 2
			 + PowerOf3[((O & 0x0000408000000000ULL) * 0x0101010101010101ULL) >> 62];
		sym2 = PowerOf3inv[((P & 0x1020000000000000ULL) * 0x0101010101010101ULL) >> 54] * 2
			 + PowerOf3inv[((O & 0x1020000000000000ULL) * 0x0101010101010101ULL) >> 54];
		return sym1 > sym2 ? 9*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 9*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_LeftLowerD4(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[((P & 0x0000000080400000ULL) * 0x0101010101010101ULL) >> 62] * 2
			 + PowerOf3[((O & 0x0000000080400000ULL) * 0x0101010101010101ULL) >> 62];
		sym2 = PowerOf3inv[((P & 0x0000000000002010ULL) * 0x0101010101010101ULL) >> 54] * 2
			 + PowerOf3inv[((O & 0x0000000000002010ULL) * 0x0101010101010101ULL) >> 54];
		return sym1 > sym2 ? 9*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 9*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_RightUpperD4(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[((P & 0x0804000000000000ULL) * 0x0101010101010101ULL) >> 58] * 2
			 + PowerOf3[((O & 0x0804000000000000ULL) * 0x0101010101010101ULL) >> 58];
		sym2 = PowerOf3inv[((P & 0x0000020100000000ULL) * 0x0101010101010101ULL) >> 50] * 2
			 + PowerOf3inv[((O & 0x0000020100000000ULL) * 0x0101010101010101ULL) >> 50];
		return sym1 > sym2 ? 9*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 9*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_RightLowerD5(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2, diag;
		sym1 = PowerOf3[((P & 0x0000000000000810ULL) * 0x0101010101010101ULL) >> 59] * 2
			 + PowerOf3[((O & 0x0000000000000810ULL) * 0x0101010101010101ULL) >> 59];
		sym2 = PowerOf3inv[((P & 0x0000000102000000ULL) * 0x0101010101010101ULL) >> 50] * 2
			 + PowerOf3inv[((O & 0x0000000102000000ULL) * 0x0101010101010101ULL) >> 50];
		diag = ((P & 0x0000000000040000ULL) >> 17)
			 + ((O & 0x0000000000040000ULL) >> 18);
		return sym1 > sym2 ? 3*(9*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag : 3*(9*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
	}
	int ReducedIndex_LeftUpperD5(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2, diag;
		sym1 = PowerOf3[((P & 0x0000004080000000ULL) * 0x0101010101010101ULL) >> 62] * 2
			 + PowerOf3[((O & 0x0000004080000000ULL) * 0x0101010101010101ULL) >> 62];
		sym2 = PowerOf3inv[((P & 0x0810000000000000ULL) * 0x0101010101010101ULL) >> 53] * 2
			 + PowerOf3inv[((O & 0x0810000000000000ULL) * 0x0101010101010101ULL) >> 53];
		diag = ((P & 0x0000200000000000ULL) >> 44)
			 + ((O & 0x0000200000000000ULL) >> 45);
		return sym1 > sym2 ? 3*(9*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag : 3*(9*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
	}
	int ReducedIndex_LeftLowerD5(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2, diag;
		sym1 = PowerOf3[((P & 0x0000008040000000ULL) * 0x0101010101010101ULL) >> 62] * 2
			 + PowerOf3[((O & 0x0000008040000000ULL) * 0x0101010101010101ULL) >> 62];
		sym2 = PowerOf3inv[((P & 0x0000000000001008ULL) * 0x0101010101010101ULL) >> 53] * 2
			 + PowerOf3inv[((O & 0x0000000000001008ULL) * 0x0101010101010101ULL) >> 53];
		diag = ((P & 0x0000000000200000ULL) >> 20)
			 + ((O & 0x0000000000200000ULL) >> 21);
		return sym1 > sym2 ? 3*(9*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag : 3*(9*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
	}
	int ReducedIndex_RightUpperD5(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2, diag;
		sym1 = PowerOf3[((P & 0x1008000000000000ULL) * 0x0101010101010101ULL) >> 59] * 2
			 + PowerOf3[((O & 0x1008000000000000ULL) * 0x0101010101010101ULL) >> 59];
		sym2 = PowerOf3inv[((P & 0x0000000201000000ULL) * 0x0101010101010101ULL) >> 50] * 2
			 + PowerOf3inv[((O & 0x0000000201000000ULL) * 0x0101010101010101ULL) >> 50];
		diag = ((P & 0x0000040000000000ULL) >> 41)
			 + ((O & 0x0000040000000000ULL) >> 42);
		return sym1 > sym2 ? 3*(9*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag : 3*(9*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
	}
	int ReducedIndex_RightLowerD6(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[((P & 0x0000000000081020ULL) * 0x0101010101010101ULL) >> 59] * 2
			 + PowerOf3[((O & 0x0000000000081020ULL) * 0x0101010101010101ULL) >> 59];
		sym2 = PowerOf3inv[((P & 0x0000010204000000ULL) * 0x0101010101010101ULL) >> 51] * 2
			 + PowerOf3inv[((O & 0x0000010204000000ULL) * 0x0101010101010101ULL) >> 51];
		return sym1 > sym2 ? 27*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 27*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_LeftUpperD6(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[((P & 0x0000002040800000ULL) * 0x0101010101010101ULL) >> 61] * 2
			 + PowerOf3[((O & 0x0000002040800000ULL) * 0x0101010101010101ULL) >> 61];
		sym2 = PowerOf3inv[((P & 0x0408100000000000ULL) * 0x0101010101010101ULL) >> 53] * 2
			 + PowerOf3inv[((O & 0x0408100000000000ULL) * 0x0101010101010101ULL) >> 53];
		return sym1 > sym2 ? 27*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 27*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_LeftLowerD6(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[((P & 0x0000804020000000ULL) * 0x0101010101010101ULL) >> 61] * 2
			 + PowerOf3[((O & 0x0000804020000000ULL) * 0x0101010101010101ULL) >> 61];
		sym2 = PowerOf3inv[((P & 0x0000000000100804ULL) * 0x0101010101010101ULL) >> 53] * 2
			 + PowerOf3inv[((O & 0x0000000000100804ULL) * 0x0101010101010101ULL) >> 53];
		return sym1 > sym2 ? 27*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 27*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_RightUpperD6(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[((P & 0x2010080000000000ULL) * 0x0101010101010101ULL) >> 59] * 2
			 + PowerOf3[((O & 0x2010080000000000ULL) * 0x0101010101010101ULL) >> 59];
		sym2 = PowerOf3inv[((P & 0x0000000402010000ULL) * 0x0101010101010101ULL) >> 51] * 2
			 + PowerOf3inv[((O & 0x0000000402010000ULL) * 0x0101010101010101ULL) >> 51];
		return sym1 > sym2 ? 27*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 27*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int ReducedIndex_RightLowerD7(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2, diag;
		sym1 = PowerOf3[((P & 0x0000000000102040ULL) * 0x0101010101010101ULL) >> 60] * 2
			 + PowerOf3[((O & 0x0000000000102040ULL) * 0x0101010101010101ULL) >> 60];
		sym2 = PowerOf3inv[((P & 0x0001020400000000ULL) * 0x0101010101010101ULL) >> 51] * 2
			 + PowerOf3inv[((O & 0x0001020400000000ULL) * 0x0101010101010101ULL) >> 51];
		diag = ((P & 0x0000000008000000ULL) >> 26)
			 + ((O & 0x0000000008000000ULL) >> 27);
		return sym1 > sym2 ? 3*(27*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag : 3*(27*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
	}
	int ReducedIndex_LeftUpperD7(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2, diag;
		sym1 = PowerOf3[((P & 0x0000000020408000ULL) * 0x0101010101010101ULL) >> 61] * 2
			 + PowerOf3[((O & 0x0000000020408000ULL) * 0x0101010101010101ULL) >> 61];
		sym2 = PowerOf3inv[((P & 0x0204080000000000ULL) * 0x0101010101010101ULL) >> 52] * 2
			 + PowerOf3inv[((O & 0x0204080000000000ULL) * 0x0101010101010101ULL) >> 52];
		diag = ((P & 0x0000001000000000ULL) >> 35)
			 + ((O & 0x0000001000000000ULL) >> 36);
		return sym1 > sym2 ? 3*(27*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag : 3*(27*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
	}
	int ReducedIndex_LeftLowerD7(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2, diag;
		sym1 = PowerOf3[((P & 0x0080402000000000ULL) * 0x0101010101010101ULL) >> 61] * 2
			 + PowerOf3[((O & 0x0080402000000000ULL) * 0x0101010101010101ULL) >> 61];
		sym2 = PowerOf3inv[((P & 0x0000000000080402ULL) * 0x0101010101010101ULL) >> 52] * 2
			 + PowerOf3inv[((O & 0x0000000000080402ULL) * 0x0101010101010101ULL) >> 52];
		diag = ((P & 0x0000000010000000ULL) >> 27)
			 + ((O & 0x0000000010000000ULL) >> 28);
		return sym1 > sym2 ? 3*(27*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag : 3*(27*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
	}
	int ReducedIndex_RightUpperD7(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2, diag;
		sym1 = PowerOf3[((P & 0x4020100000000000ULL) * 0x0101010101010101ULL) >> 60] * 2
			 + PowerOf3[((O & 0x4020100000000000ULL) * 0x0101010101010101ULL) >> 60];
		sym2 = PowerOf3inv[((P & 0x0000000004020100ULL) * 0x0101010101010101ULL) >> 51] * 2
			 + PowerOf3inv[((O & 0x0000000004020100ULL) * 0x0101010101010101ULL) >> 51];
		diag = ((P & 0x0000000800000000ULL) >> 34)
			 + ((O & 0x0000000800000000ULL) >> 35);
		return sym1 > sym2 ? 3*(27*sym2+sym1 - (((sym2+1)*sym2) >> 1)) + diag : 3*(27*sym1+sym2 - (((sym1+1)*sym1) >> 1)) + diag;
	}
	int ReducedIndex_CenterX(const unsigned long long P, const unsigned long long O)
	{
		return PowerOf3[((P & 0x0000240000240000ULL) * 0x00000A0000040000ULL) >> 60] * 32
			 + PowerOf3[((O & 0x0000240000240000ULL) * 0x00000A0000040000ULL) >> 60] * 16
			 + (((P & 0x0000001818000000ULL) * 0x0000000802000000ULL) >> 60);
	}
	
	int FullIndex_LowerC(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_LowerC)] * 2 
			 + PowerOf3[PExt(O, Pattern_LowerC)];
	#else
		return PowerOf3[(P & 0x00000000000000FFULL) | (P & 0x0000000000000200ULL) >> 1 | (P & 0x0000000000004000ULL) >> 5] * 2
			 + PowerOf3[(O & 0x00000000000000FFULL) | (O & 0x0000000000000200ULL) >> 1 | (O & 0x0000000000004000ULL) >> 5];
	#endif
	}
	int FullIndex_UpperC(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3inv_2[PExt(P, Pattern_UpperC)] * 2 
			 + PowerOf3inv_2[PExt(O, Pattern_UpperC)];
	#else
		return PowerOf3inv_2[(P & 0xFF00000000000000ULL) >> 54 | (P & 0x0002000000000000ULL) >> 49 | (P & 0x0040000000000000ULL) >> 53] * 2
			 + PowerOf3inv_2[(O & 0xFF00000000000000ULL) >> 54 | (O & 0x0002000000000000ULL) >> 49 | (O & 0x0040000000000000ULL) >> 53];
	#endif
	}
	int FullIndex_LeftC(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, 0x8080808080808080ULL) | PExt(P, 0x0040000000004000ULL) << 8] * 2
			 + PowerOf3[PExt(O, 0x8080808080808080ULL) | PExt(O, 0x0040000000004000ULL) << 8];
	#else
		return PowerOf3[((P & 0x8080808080808080ULL) * 0x0002040810204081ULL) >> 56 | (P & 0x0000000000004000ULL) >> 6 | (P & 0x0040000000000000ULL) >> 45] * 2
			 + PowerOf3[((O & 0x8080808080808080ULL) * 0x0002040810204081ULL) >> 56 | (O & 0x0000000000004000ULL) >> 6 | (O & 0x0040000000000000ULL) >> 45];
	#endif
	}
	int FullIndex_RightC(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, 0x0101010101010101ULL) | PExt(P, 0x0002000000000200ULL) << 8] * 2
			 + PowerOf3[PExt(O, 0x0101010101010101ULL) | PExt(O, 0x0002000000000200ULL) << 8];
	#else
		return PowerOf3[((P & 0x0101010101010101ULL) * 0x0102040810204080ULL) >> 56 | (P & 0x0002000000000000ULL) >> 40 | (P & 0x0000000000000200ULL) >> 1] * 2
			 + PowerOf3[((O & 0x0101010101010101ULL) * 0x0102040810204080ULL) >> 56 | (O & 0x0002000000000000ULL) >> 40 | (O & 0x0000000000000200ULL) >> 1];
	#endif
	}
	int FullIndex_LowerL1(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_BEXTR
		return PowerOf3[BExtr(P, 8, 8)] * 2
			 + PowerOf3[BExtr(O, 8, 8)];
	#else
		return PowerOf3[(P & Pattern_LowerL1) >> 8] * 2
			 + PowerOf3[(O & Pattern_LowerL1) >> 8];
	#endif
	}
	int FullIndex_LowerL2(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_BEXTR
		return PowerOf3[BExtr(P, 16, 8)] * 2
			 + PowerOf3[BExtr(O, 16, 8)];
	#else
		return PowerOf3[(P & Pattern_LowerL2) >> 16] * 2
			 + PowerOf3[(O & Pattern_LowerL2) >> 16];
	#endif
	}
	int FullIndex_LowerL3(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_BEXTR
		return PowerOf3[BExtr(P, 24, 8)] * 2
			 + PowerOf3[BExtr(O, 24, 8)];
	#else
		return PowerOf3[(P & Pattern_LowerL3) >> 24] * 2
			 + PowerOf3[(O & Pattern_LowerL3) >> 24];
	#endif
	}
	int FullIndex_UpperL3(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_BEXTR
		return PowerOf3[BExtr(P, 32, 8)] * 2
			 + PowerOf3[BExtr(O, 32, 8)];
	#else
		return PowerOf3[(P & Pattern_UpperL3) >> 32] * 2
			 + PowerOf3[(O & Pattern_UpperL3) >> 32];
	#endif
	}
	int FullIndex_UpperL2(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_BEXTR
		return PowerOf3[BExtr(P, 40, 8)] * 2
			 + PowerOf3[BExtr(O, 40, 8)];
	#else
		return PowerOf3[(P & Pattern_UpperL2) >> 40] * 2
			 + PowerOf3[(O & Pattern_UpperL2) >> 40];
	#endif
	}
	int FullIndex_UpperL1(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_BEXTR
		return PowerOf3[BExtr(P, 48, 8)] * 2
			 + PowerOf3[BExtr(O, 48, 8)];
	#else
		return PowerOf3[(P & Pattern_UpperL1) >> 48] * 2
			 + PowerOf3[(O & Pattern_UpperL1) >> 48];
	#endif
	}
	int FullIndex_LeftL1(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_LeftL1)] * 2
		     + PowerOf3[PExt(O, Pattern_LeftL1)];
	#else
		return PowerOf3[((P & Pattern_LeftL1) * 0x0004081020408102ULL) >> 56] * 2
			 + PowerOf3[((O & Pattern_LeftL1) * 0x0004081020408102ULL) >> 56];
	#endif
	}
	int FullIndex_LeftL2(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_LeftL2)] * 2
		     + PowerOf3[PExt(O, Pattern_LeftL2)];
	#else
		return PowerOf3[((P & Pattern_LeftL2) * 0x0008102040810204ULL) >> 56] * 2
			 + PowerOf3[((O & Pattern_LeftL2) * 0x0008102040810204ULL) >> 56];
	#endif
	}
	int FullIndex_LeftL3(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_LeftL3)] * 2
		     + PowerOf3[PExt(O, Pattern_LeftL3)];
	#else
		return PowerOf3[((P & Pattern_LeftL3) * 0x0010204081020408ULL) >> 56] * 2
			 + PowerOf3[((O & Pattern_LeftL3) * 0x0010204081020408ULL) >> 56];
	#endif
	}
	int FullIndex_RightL3(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_RightL3)] * 2
		     + PowerOf3[PExt(O, Pattern_RightL3)];
	#else
		return PowerOf3[((P & Pattern_RightL3) * 0x0020408102040810ULL) >> 56] * 2
			 + PowerOf3[((O & Pattern_RightL3) * 0x0020408102040810ULL) >> 56];
	#endif
	}
	int FullIndex_RightL2(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_RightL2)] * 2
		     + PowerOf3[PExt(O, Pattern_RightL2)];
	#else
		return PowerOf3[((P & Pattern_RightL2) * 0x0040810204081020ULL) >> 56] * 2
			 + PowerOf3[((O & Pattern_RightL2) * 0x0040810204081020ULL) >> 56];
	#endif
	}
	int FullIndex_RightL1(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_RightL1)] * 2
		     + PowerOf3[PExt(O, Pattern_RightL1)];
	#else
		return PowerOf3[((P & Pattern_RightL1) * 0x0081020408102040ULL) >> 56] * 2
			 + PowerOf3[((O & Pattern_RightL1) * 0x0081020408102040ULL) >> 56];
	#endif
	}
	int FullIndex_RightLowerA(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, 0x8040201008040201ULL)] * 2
			 + PowerOf3[PExt(O, 0x8040201008040201ULL)]
		     + (PowerOf3[PExt(P, 0x0000000000010106ULL)] * 2
			 +  PowerOf3[PExt(O, 0x0000000000010106ULL)]) * 6561;
	#else
		return PowerOf3[((P & 0x8040201008040201ULL) * 0x0101010101010101ULL) >> 56] * 2
			 + PowerOf3[((O & 0x8040201008040201ULL) * 0x0101010101010101ULL) >> 56]
			 + (PowerOf3[((P & 0x0000000000010106ULL) * 0x0108100000000000ULL) >> 57] * 2
			 +  PowerOf3[((O & 0x0000000000010106ULL) * 0x0108100000000000ULL) >> 57]) * 6561;
	#endif
	}
	int FullIndex_LeftUpperA(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3inv_2[PExt(P, 0x8040201008040201ULL) << 2] * 2
			 + PowerOf3inv_2[PExt(O, 0x8040201008040201ULL) << 2]
		     + (PowerOf3inv_2[PExt(P, 0x6080800000000000ULL) << 6] * 2
			 +  PowerOf3inv_2[PExt(O, 0x6080800000000000ULL) << 6]) * 6561;
	#else
		return PowerOf3inv[((P & 0x8040201008040201ULL) * 0x0101010101010101ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x8040201008040201ULL) * 0x0101010101010101ULL) >> 56]
			 + (PowerOf3inv[(((P & 0x6080800000000000ULL) * 0x0000000000001021ULL) & 0xFF00000000000000ULL) >> 55] * 2
			 +  PowerOf3inv[(((O & 0x6080800000000000ULL) * 0x0000000000001021ULL) & 0xFF00000000000000ULL) >> 55]) * 6561;
	#endif
	}
	int FullIndex_LeftLowerA(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, 0x0102040810204080ULL) | PExt(P, 0x0000000000808000ULL) << 8] * 2
			 + PowerOf3[PExt(O, 0x0102040810204080ULL) | PExt(O, 0x0000000000808000ULL) << 8]
		     + (PowerOf3inv_2[(P & 0x0000000000000060ULL) << 3] * 2
			 +  PowerOf3inv_2[(O & 0x0000000000000060ULL) << 3]) * Power3<10>();
	#else
		return PowerOf3inv[((P & 0x0102040810204080ULL) * 0x0101010101010101ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x0102040810204080ULL) * 0x0101010101010101ULL) >> 56]
			 + (PowerOf3inv[((P & 0x0000000000808060ULL) * 0x0080100800000000ULL) >> 54] * 2
			 +  PowerOf3inv[((O & 0x0000000000808060ULL) * 0x0080100800000000ULL) >> 54]) * Power3<8>();
	#endif
	}
	int FullIndex_RightUpperA(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3inv_2[PExt(P, 0x0102040810204080ULL) << 2 | PExt(P, 0x0001010000000000ULL)] * 2
			 + PowerOf3inv_2[PExt(O, 0x0102040810204080ULL) << 2 | PExt(O, 0x0001010000000000ULL)]
		     + (PowerOf3[(P & 0x0600000000000000ULL) >> 57] * 2
			 +  PowerOf3[(O & 0x0600000000000000ULL) >> 57]) * Power3<10>();
	#else
		return PowerOf3[((P & 0x0102040810204080ULL) * 0x0101010101010101ULL) >> 56] * 2
			 + PowerOf3[((O & 0x0102040810204080ULL) * 0x0101010101010101ULL) >> 56]
			 + (PowerOf3[((P & 0x0601010000000000ULL) * 0x0000000000020102ULL) >> 56] * 2
			 +  PowerOf3[((O & 0x0601010000000000ULL) * 0x0000000000020102ULL) >> 56]) * Power3<8>();
	#endif
	}
	int FullIndex_RightLowerSq(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_RightLowerSq)] * 2
			 + PowerOf3[PExt(O, Pattern_RightLowerSq)];
	#else
		return PowerOf3[(P & 0x0000000000000007ULL) | (P & 0x0000000000000700ULL) >> 5 | (P & 0x0000000000070000ULL) >> 10] * 2
			 + PowerOf3[(O & 0x0000000000000007ULL) | (O & 0x0000000000000700ULL) >> 5 | (O & 0x0000000000070000ULL) >> 10];
	#endif
	}
	int FullIndex_LeftUpperSq(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3inv_2[PExt(P, Pattern_LeftUpperSq) << 1] * 2
			 + PowerOf3inv_2[PExt(O, Pattern_LeftUpperSq) << 1];
	#else
		return PowerOf3inv_2[(P & 0xE000000000000000ULL) >> 54 | (P & 0x00E0000000000000ULL) >> 49 | (P & 0x0000E00000000000ULL) >> 44] * 2
			 + PowerOf3inv_2[(O & 0xE000000000000000ULL) >> 54 | (O & 0x00E0000000000000ULL) >> 49 | (O & 0x0000E00000000000ULL) >> 44];
	#endif
	}
	int FullIndex_LeftLowerSq(const unsigned long long P, const unsigned long long O)
	{
		return PowerOf3inv_2[((P & Pattern_LeftLowerSq) * 0x0100200400000000ULL) >> 54] * 2
			 + PowerOf3inv_2[((O & Pattern_LeftLowerSq) * 0x0100200400000000ULL) >> 54];
	}
	int FullIndex_RightUpperSq(const unsigned long long P, const unsigned long long O)
	{
		return PowerOf3[(((P & Pattern_RightUpperSq) >> 1) * 0x0000000000400801ULL) >> 55] * 2
			 + PowerOf3[(((O & Pattern_RightUpperSq) >> 1) * 0x0000000000400801ULL) >> 55];
	}
	int FullIndex_LowerRightB(const unsigned long long P, const unsigned long long O)
	{
		// - - - - - - - -
		// - - - - - - - -
		// - - - - - - - -
		// - - - - - - - -
		// - - - - - - - -
		// - - - - - - - -
		// - - - # # # # #
		// - - - # # # # #

	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_LowerRightB)] * 2
		     + PowerOf3[PExt(O, Pattern_LowerRightB)];
	#else
		return PowerOf3[(P & 0x000000000000001FULL) | (P & 0x0000000000001F00ULL) >> 3] * 2
			 + PowerOf3[(O & 0x000000000000001FULL) | (O & 0x0000000000001F00ULL) >> 3];
	#endif
	}
	int FullIndex_LowerLeftB(const unsigned long long P, const unsigned long long O)
	{
		// - - - - - - - -
		// - - - - - - - -
		// - - - - - - - -
		// - - - - - - - -
		// - - - - - - - -
		// - - - - - - - -
		// # # # # # - - -
		// # # # # # - - -
		return PowerOf3inv_2[(P & 0x00000000000000F8ULL) << 2 | (P & 0x000000000000F800ULL) >> 11] * 2
			 + PowerOf3inv_2[(O & 0x00000000000000F8ULL) << 2 | (O & 0x000000000000F800ULL) >> 11];
	}
	int FullIndex_LeftLowerB(const unsigned long long P, const unsigned long long O)
	{
		// - - - - - - - -
		// - - - - - - - -
		// - - - - - - - -
		// # # - - - - - -
		// # # - - - - - -
		// # # - - - - - -
		// # # - - - - - -
		// # # - - - - - -

	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, 0x0000008080808080ULL) | PExt(P, 0x0000004040404040ULL) << 5] * 2
		     + PowerOf3[PExt(O, 0x0000008080808080ULL) | PExt(O, 0x0000004040404040ULL) << 5];
	#else
		return PowerOf3[((P & 0x0000008080808080ULL) * 0x0010204081000000ULL) >> 59 | (((P & 0x0000004040404040ULL) * 0x0010204081000000ULL) & 0xFF00000000000000ULL) >> 53] * 2
			 + PowerOf3[((O & 0x0000008080808080ULL) * 0x0010204081000000ULL) >> 59 | (((O & 0x0000004040404040ULL) * 0x0010204081000000ULL) & 0xFF00000000000000ULL) >> 53];
	#endif
	}
	int FullIndex_LeftUpperB(const unsigned long long P, const unsigned long long O)
	{
		// # # - - - - - -
		// # # - - - - - -
		// # # - - - - - -
		// # # - - - - - -
		// # # - - - - - -
		// - - - - - - - -
		// - - - - - - - -
		// - - - - - - - -

	#ifdef HAS_PEXT
		return PowerOf3inv_2[PExt(P, 0x8080808080000000ULL) << 5 | PExt(P, 0x4040404040000000ULL)] * 2
		     + PowerOf3inv_2[PExt(O, 0x8080808080000000ULL) << 5 | PExt(O, 0x4040404040000000ULL)];
	#else
		return PowerOf3inv_2[(((P & 0x8080808080000000ULL) * 0x0000000010204081ULL) & 0xFF00000000000000ULL) >> 54 | ((P & 0x4040404040000000ULL) * 0x0000000010204081ULL) >> 58] * 2
			 + PowerOf3inv_2[(((O & 0x8080808080000000ULL) * 0x0000000010204081ULL) & 0xFF00000000000000ULL) >> 54 | ((O & 0x4040404040000000ULL) * 0x0000000010204081ULL) >> 58];
	#endif
	}
	int FullIndex_UpperLeftB(const unsigned long long P, const unsigned long long O)
	{
		// # # # # # - - -
		// # # # # # - - -
		// - - - - - - - -
		// - - - - - - - -
		// - - - - - - - -
		// - - - - - - - -
		// - - - - - - - -
		// - - - - - - - -

	#ifdef HAS_PEXT
		return PowerOf3inv_2[PExt(P, Pattern_UpperLeftB)] * 2
		     + PowerOf3inv_2[PExt(O, Pattern_UpperLeftB)];
	#else
		return PowerOf3inv_2[(P & 0xF800000000000000ULL) >> 54 | (P & 0x00F8000000000000ULL) >> 51] * 2
			 + PowerOf3inv_2[(O & 0xF800000000000000ULL) >> 54 | (O & 0x00F8000000000000ULL) >> 51];
	#endif
	}
	int FullIndex_UpperRightB(const unsigned long long P, const unsigned long long O)
	{
		// - - - # # # # #
		// - - - # # # # #
		// - - - - - - - -
		// - - - - - - - -
		// - - - - - - - -
		// - - - - - - - -
		// - - - - - - - -
		// - - - - - - - -

		return PowerOf3[(P & 0x1F00000000000000ULL) >> 56 | (P & 0x001F000000000000ULL) >> 43] * 2
			 + PowerOf3[(O & 0x1F00000000000000ULL) >> 56 | (O & 0x001F000000000000ULL) >> 43];
	}
	int FullIndex_RightUpperB(const unsigned long long P, const unsigned long long O)
	{
		// - - - - - - # #
		// - - - - - - # #
		// - - - - - - # #
		// - - - - - - # #
		// - - - - - - # #
		// - - - - - - - -
		// - - - - - - - -
		// - - - - - - - -

	#ifdef HAS_PEXT
		return PowerOf3inv_2[PExt(P, 0x0101010101000000ULL) << 5 | PExt(P, 0x0202020202000000ULL)] * 2
		     + PowerOf3inv_2[PExt(O, 0x0101010101000000ULL) << 5 | PExt(O, 0x0202020202000000ULL)];
	#else
		return PowerOf3inv_2[(((P & 0x0101010101000000ULL) * 0x0000000102040810ULL) & 0xFF00000000000000ULL) >> 51 | ((P & 0x0202020202000000ULL) * 0x0000000102040810ULL) >> 57] * 2
			 + PowerOf3inv_2[(((O & 0x0101010101000000ULL) * 0x0000000102040810ULL) & 0xFF00000000000000ULL) >> 51 | ((O & 0x0202020202000000ULL) * 0x0000000102040810ULL) >> 57];
	#endif
	}
	int FullIndex_RightLowerB(const unsigned long long P, const unsigned long long O)
	{
		// - - - - - - - -
		// - - - - - - - -
		// - - - - - - - -
		// - - - - - - # #
		// - - - - - - # #
		// - - - - - - # #
		// - - - - - - # #
		// - - - - - - # #

	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, 0x0000000101010101ULL) | PExt(P, 0x0000000202020202ULL) << 5] * 2
		     + PowerOf3[PExt(O, 0x0000000101010101ULL) | PExt(O, 0x0000000202020202ULL) << 5];
	#else
		return PowerOf3[((P & 0x0000000101010101ULL) * 0x0102040810000000ULL) >> 56 | (((P & 0x0000000202020202ULL) * 0x0102040810000000ULL) & 0xFF00000000000000ULL) >> 52] * 2
			 + PowerOf3[((O & 0x0000000101010101ULL) * 0x0102040810000000ULL) >> 56 | (((O & 0x0000000202020202ULL) * 0x0102040810000000ULL) & 0xFF00000000000000ULL) >> 52];
	#endif
	}
	int FullIndex_RightLowerD4(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_RightLowerD4)] * 2 
			 + PowerOf3[PExt(O, Pattern_RightLowerD4)];
	#else
		return PowerOf3[((P & Pattern_RightLowerD4) * 0x0101010101010101ULL) >> 56] * 2 
			 + PowerOf3[((O & Pattern_RightLowerD4) * 0x0101010101010101ULL) >> 56];
	#endif
	}
	int FullIndex_LeftUpperD4(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_LeftUpperD4)] * 2 
			 + PowerOf3[PExt(O, Pattern_LeftUpperD4)];
	#else
		return PowerOf3[((P & Pattern_LeftUpperD4) * 0x0101010101010101ULL) >> 60] * 2 
			 + PowerOf3[((O & Pattern_LeftUpperD4) * 0x0101010101010101ULL) >> 60];
	#endif
	}
	int FullIndex_LeftLowerD4(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_LeftLowerD4)] * 2 
			 + PowerOf3[PExt(O, Pattern_LeftLowerD4)];
	#else
		return PowerOf3[((P & Pattern_LeftLowerD4) * 0x0101010101010101ULL) >> 60] * 2 
			 + PowerOf3[((O & Pattern_LeftLowerD4) * 0x0101010101010101ULL) >> 60];
	#endif
	}
	int FullIndex_RightUpperD4(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_RightUpperD4)] * 2 
			 + PowerOf3[PExt(O, Pattern_RightUpperD4)];
	#else
		return PowerOf3[((P & Pattern_RightUpperD4) * 0x0101010101010101ULL) >> 56] * 2 
			 + PowerOf3[((O & Pattern_RightUpperD4) * 0x0101010101010101ULL) >> 56];
	#endif
	}
	int FullIndex_RightLowerD5(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_RightLowerD5)] * 2 
			 + PowerOf3[PExt(O, Pattern_RightLowerD5)];
	#else
		return PowerOf3[((P & Pattern_RightLowerD5) * 0x0101010101010101ULL) >> 56] * 2 
			 + PowerOf3[((O & Pattern_RightLowerD5) * 0x0101010101010101ULL) >> 56];
	#endif
	}
	int FullIndex_LeftUpperD5(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_LeftUpperD5)] * 2 
			 + PowerOf3[PExt(O, Pattern_LeftUpperD5)];
	#else
		return PowerOf3[((P & Pattern_LeftUpperD5) * 0x0101010101010101ULL) >> 59] * 2 
			 + PowerOf3[((O & Pattern_LeftUpperD5) * 0x0101010101010101ULL) >> 59];
	#endif
	}
	int FullIndex_LeftLowerD5(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_LeftLowerD5)] * 2 
			 + PowerOf3[PExt(O, Pattern_LeftLowerD5)];
	#else
		return PowerOf3[((P & Pattern_LeftLowerD5) * 0x0101010101010101ULL) >> 59] * 2 
			 + PowerOf3[((O & Pattern_LeftLowerD5) * 0x0101010101010101ULL) >> 59];
	#endif
	}
	int FullIndex_RightUpperD5(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_RightUpperD5)] * 2 
			 + PowerOf3[PExt(O, Pattern_RightUpperD5)];
	#else
		return PowerOf3[((P & Pattern_RightUpperD5) * 0x0101010101010101ULL) >> 56] * 2 
			 + PowerOf3[((O & Pattern_RightUpperD5) * 0x0101010101010101ULL) >> 56];
	#endif
	}
	int FullIndex_RightLowerD6(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_RightLowerD6)] * 2 
			 + PowerOf3[PExt(O, Pattern_RightLowerD6)];
	#else
		return PowerOf3[((P & Pattern_RightLowerD6) * 0x0101010101010101ULL) >> 56] * 2 
			 + PowerOf3[((O & Pattern_RightLowerD6) * 0x0101010101010101ULL) >> 56];
	#endif
	}
	int FullIndex_LeftUpperD6(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_LeftUpperD6)] * 2 
			 + PowerOf3[PExt(O, Pattern_LeftUpperD6)];
	#else
		return PowerOf3[((P & Pattern_LeftUpperD6) * 0x0101010101010101ULL) >> 58] * 2 
			 + PowerOf3[((O & Pattern_LeftUpperD6) * 0x0101010101010101ULL) >> 58];
	#endif
	}
	int FullIndex_LeftLowerD6(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_LeftLowerD6)] * 2 
			 + PowerOf3[PExt(O, Pattern_LeftLowerD6)];
	#else
		return PowerOf3[((P & Pattern_LeftLowerD6) * 0x0101010101010101ULL) >> 58] * 2 
			 + PowerOf3[((O & Pattern_LeftLowerD6) * 0x0101010101010101ULL) >> 58];
	#endif
	}
	int FullIndex_RightUpperD6(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_RightUpperD6)] * 2 
			 + PowerOf3[PExt(O, Pattern_RightUpperD6)];
	#else
		return PowerOf3[((P & Pattern_RightUpperD6) * 0x0101010101010101ULL) >> 56] * 2 
			 + PowerOf3[((O & Pattern_RightUpperD6) * 0x0101010101010101ULL) >> 56];
	#endif
	}
	int FullIndex_RightLowerD7(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_RightLowerD7)] * 2 
			 + PowerOf3[PExt(O, Pattern_RightLowerD7)];
	#else
		return PowerOf3[((P & Pattern_RightLowerD7) * 0x0101010101010101ULL) >> 56] * 2 
			 + PowerOf3[((O & Pattern_RightLowerD7) * 0x0101010101010101ULL) >> 56];
	#endif
	}
	int FullIndex_LeftUpperD7(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_LeftUpperD7)] * 2 
			 + PowerOf3[PExt(O, Pattern_LeftUpperD7)];
	#else
		return PowerOf3[((P & Pattern_LeftUpperD7) * 0x0101010101010101ULL) >> 57] * 2 
			 + PowerOf3[((O & Pattern_LeftUpperD7) * 0x0101010101010101ULL) >> 57];
	#endif
	}
	int FullIndex_LeftLowerD7(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_LeftLowerD7)] * 2 
			 + PowerOf3[PExt(O, Pattern_LeftLowerD7)];
	#else
		return PowerOf3[((P & Pattern_LeftLowerD7) * 0x0101010101010101ULL) >> 57] * 2 
			 + PowerOf3[((O & Pattern_LeftLowerD7) * 0x0101010101010101ULL) >> 57];
	#endif
	}
	int FullIndex_RightUpperD7(const unsigned long long P, const unsigned long long O)
	{
	#ifdef HAS_PEXT
		return PowerOf3[PExt(P, Pattern_RightUpperD7)] * 2 
			 + PowerOf3[PExt(O, Pattern_RightUpperD7)];
	#else
		return PowerOf3[((P & Pattern_RightUpperD7) * 0x0101010101010101ULL) >> 56] * 2 
			 + PowerOf3[((O & Pattern_RightUpperD7) * 0x0101010101010101ULL) >> 56];
	#endif
	}

	unsigned short POWER_OF_3(const unsigned short I)
	{
		unsigned short sum = 0;
		if (I & 0x001) sum += 1;
		if (I & 0x002) sum += 1*3;
		if (I & 0x004) sum += 1*3*3;
		if (I & 0x008) sum += 1*3*3*3;
		if (I & 0x010) sum += 1*3*3*3*3;
		if (I & 0x020) sum += 1*3*3*3*3*3;
		if (I & 0x040) sum += 1*3*3*3*3*3*3;
		if (I & 0x080) sum += 1*3*3*3*3*3*3*3;
		if (I & 0x100) sum += 1*3*3*3*3*3*3*3*3;
		if (I & 0x200) sum += 1*3*3*3*3*3*3*3*3*3;
		return sum;
	}
	unsigned short POWER_OF_3_INV_2(const unsigned short I)
	{
		unsigned short sum = 0;
		if (I & 0x001) sum += 1*3*3*3*3*3*3*3*3*3;
		if (I & 0x002) sum += 1*3*3*3*3*3*3*3*3;
		if (I & 0x004) sum += 1*3*3*3*3*3*3*3;
		if (I & 0x008) sum += 1*3*3*3*3*3*3;
		if (I & 0x010) sum += 1*3*3*3*3*3;
		if (I & 0x020) sum += 1*3*3*3*3;
		if (I & 0x040) sum += 1*3*3*3;
		if (I & 0x080) sum += 1*3*3;
		if (I & 0x100) sum += 1*3;
		if (I & 0x200) sum += 1;
		return sum;
	}
	unsigned short POWER_OF_3_INV(const unsigned short I)
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

	
	void For_each_pattern_do_fkt(std::function<void(const unsigned long long)> fkt)
	{
		if (Feature_C){
			fkt(Pattern_LowerC);
			fkt(Pattern_UpperC);
			fkt(Pattern_LeftC);
			fkt(Pattern_RightC);
		}
		if (Feature_L1){
			fkt(Pattern_LowerL1);
			fkt(Pattern_UpperL1);
			fkt(Pattern_LeftL1);
			fkt(Pattern_RightL1);
		}
		if (Feature_L2){
			fkt(Pattern_LowerL2);
			fkt(Pattern_UpperL2);
			fkt(Pattern_LeftL2);
			fkt(Pattern_RightL2);
		}
		if (Feature_L3){
			fkt(Pattern_LowerL3);
			fkt(Pattern_UpperL3);
			fkt(Pattern_LeftL3);
			fkt(Pattern_RightL3);
		}
		if (Feature_A){
			fkt(Pattern_RightLowerA);
			fkt(Pattern_LeftUpperA);
			fkt(Pattern_LeftLowerA);
			fkt(Pattern_RightUpperA);
		}
		if (Feature_Sq){
			fkt(Pattern_RightLowerSq);
			fkt(Pattern_LeftUpperSq);
			fkt(Pattern_LeftLowerSq);
			fkt(Pattern_RightUpperSq);
		}
		if (Feature_B){
			fkt(Pattern_LowerRightB);
			fkt(Pattern_LowerLeftB);
			fkt(Pattern_LeftLowerB);
			fkt(Pattern_LeftUpperB);
			fkt(Pattern_UpperLeftB);
			fkt(Pattern_UpperRightB);
			fkt(Pattern_RightUpperB);
			fkt(Pattern_RightLowerB);
		}
		if (Feature_D3){
			fkt(Pattern_RightLowerD3);
			fkt(Pattern_LeftUpperD3);
			fkt(Pattern_LeftLowerD3);
			fkt(Pattern_RightUpperD3);
		}
		if (Feature_D4){
			fkt(Pattern_RightLowerD4);
			fkt(Pattern_LeftUpperD4);
			fkt(Pattern_LeftLowerD4);
			fkt(Pattern_RightUpperD4);
		}
		if (Feature_D5){
			fkt(Pattern_RightLowerD5);
			fkt(Pattern_LeftUpperD5);
			fkt(Pattern_LeftLowerD5);
			fkt(Pattern_RightUpperD5);
		}
		if (Feature_D6){
			fkt(Pattern_RightLowerD6);
			fkt(Pattern_LeftUpperD6);
			fkt(Pattern_LeftLowerD6);
			fkt(Pattern_RightUpperD6);
		}
		if (Feature_D7){
			fkt(Pattern_RightLowerD7);
			fkt(Pattern_LeftUpperD7);
			fkt(Pattern_LeftLowerD7);
			fkt(Pattern_RightUpperD7);
		}
		if (Feature_X)
			fkt(Pattern_CenterX);

		//if (Feature_PlayersBoarder   ){
		//if (Feature_OpponentsBoarder ){
		//if (Feature_PlayersExposeds  ){
		//if (Feature_OpponentsExposeds){
		//if (Feature_Possible         ){
		//if (Feature_Possible_E       ){
		//if (Feature_Possible_XC      ){
		//if (Feature_Parity           ){
		//if (Feature_Quadrants        ){
		//if (Feature_NumberOfOwnStones){
	}

	void Initialize()
	{
		for (int i = 0; i < 1024; i++) PowerOf3[i] = POWER_OF_3(i);
		for (int i = 0; i < 1024; i++) PowerOf3inv_2[i] = POWER_OF_3_INV_2(i);
		for (int i = 0; i <  256; i++) PowerOf3inv[i] = POWER_OF_3_INV(i);

		std::vector<std::string> Filenames;
		std::string s;
		for (int i = 0; i < 15; ++i)
		{
			s = "weights range " + std::to_string(i);
			if (ConfigFile::Configurations.count(s))
				Filenames.push_back(ConfigFile::Configurations[s]);
		}

		Weights = new float[Filenames.size()][FullSize]();
		std::vector<double> weights;
		for (int j = 0; j < Filenames.size(); ++j)
		{
			read_vector(Filenames[j], weights);
			//for (int i = 0; i < ReducedSize; ++i)
			//	Weights[j][i] = static_cast<float>(weights[i]);
			int ReducedOffset = 0;
			int FullOffset = 0;
			For_each_configuration_in_pattern_do_fkt(Pattern_LowerC      , [&](unsigned long long P, unsigned long long O){ Weights[j][FullIndex_LowerC      (P, O) + FullOffset] = weights[ReducedIndex_LowerC      (P, O) + ReducedOffset]; }); ReducedOffset += ReducedSize_C ; FullOffset += FullSize_C ;
			For_each_configuration_in_pattern_do_fkt(Pattern_LowerL1     , [&](unsigned long long P, unsigned long long O){ Weights[j][FullIndex_LowerL1     (P, O) + FullOffset] = weights[ReducedIndex_LowerL1     (P, O) + ReducedOffset]; }); ReducedOffset += ReducedSize_L1; FullOffset += FullSize_L1;
			For_each_configuration_in_pattern_do_fkt(Pattern_LowerL2     , [&](unsigned long long P, unsigned long long O){ Weights[j][FullIndex_LowerL2     (P, O) + FullOffset] = weights[ReducedIndex_LowerL2     (P, O) + ReducedOffset]; }); ReducedOffset += ReducedSize_L2; FullOffset += FullSize_L2;
			For_each_configuration_in_pattern_do_fkt(Pattern_LowerL3     , [&](unsigned long long P, unsigned long long O){ Weights[j][FullIndex_LowerL3     (P, O) + FullOffset] = weights[ReducedIndex_LowerL3     (P, O) + ReducedOffset]; }); ReducedOffset += ReducedSize_L3; FullOffset += FullSize_L3;
			For_each_configuration_in_pattern_do_fkt(Pattern_RightLowerA , [&](unsigned long long P, unsigned long long O){ Weights[j][FullIndex_RightLowerA (P, O) + FullOffset] = weights[ReducedIndex_RightLowerA (P, O) + ReducedOffset]; }); ReducedOffset += ReducedSize_A ; FullOffset += FullSize_A ;
			For_each_configuration_in_pattern_do_fkt(Pattern_RightLowerSq, [&](unsigned long long P, unsigned long long O){ Weights[j][FullIndex_RightLowerSq(P, O) + FullOffset] = weights[ReducedIndex_RightLowerSq(P, O) + ReducedOffset]; }); ReducedOffset += ReducedSize_Sq; FullOffset += FullSize_Sq;
			For_each_configuration_in_pattern_do_fkt(Pattern_LowerRightB , [&](unsigned long long P, unsigned long long O){ Weights[j][FullIndex_LowerRightB (P, O) + FullOffset] = weights[ReducedIndex_LowerRightB (P, O) + ReducedOffset]; }); ReducedOffset += ReducedSize_B ; FullOffset += FullSize_B ;
			For_each_configuration_in_pattern_do_fkt(Pattern_RightLowerD4, [&](unsigned long long P, unsigned long long O){ Weights[j][FullIndex_RightLowerD4(P, O) + FullOffset] = weights[ReducedIndex_RightLowerD4(P, O) + ReducedOffset]; }); ReducedOffset += ReducedSize_D4; FullOffset += FullSize_D4;
			For_each_configuration_in_pattern_do_fkt(Pattern_RightLowerD5, [&](unsigned long long P, unsigned long long O){ Weights[j][FullIndex_RightLowerD5(P, O) + FullOffset] = weights[ReducedIndex_RightLowerD5(P, O) + ReducedOffset]; }); ReducedOffset += ReducedSize_D5; FullOffset += FullSize_D5;
			For_each_configuration_in_pattern_do_fkt(Pattern_RightLowerD6, [&](unsigned long long P, unsigned long long O){ Weights[j][FullIndex_RightLowerD6(P, O) + FullOffset] = weights[ReducedIndex_RightLowerD6(P, O) + ReducedOffset]; }); ReducedOffset += ReducedSize_D6; FullOffset += FullSize_D6;
			For_each_configuration_in_pattern_do_fkt(Pattern_RightLowerD7, [&](unsigned long long P, unsigned long long O){ Weights[j][FullIndex_RightLowerD7(P, O) + FullOffset] = weights[ReducedIndex_RightLowerD7(P, O) + ReducedOffset]; }); ReducedOffset += ReducedSize_D7; FullOffset += FullSize_D7;
			weights.clear();
		}

	}

	void Finalize()
	{
		delete[] Weights;
	}
}

void FillFullConfigurationArray(const unsigned long long P, const unsigned long long O, int* const Array)
{
	using namespace Features;
	unsigned long long BitBoardPossible;
	int Index = 0;
	int Offset = 0;

	if (Feature_C){
		Array[Index++] = FullIndex_LowerC(P, O);
		Array[Index++] = FullIndex_UpperC(P, O);
		Array[Index++] = FullIndex_LeftC(P, O);
		Array[Index++] = FullIndex_RightC(P, O);
		Offset += FullSize_C;
	}
	if (Feature_L1){
		Array[Index++] = Offset + FullIndex_LowerL1(P, O);
		Array[Index++] = Offset + FullIndex_UpperL1(P, O);
		Array[Index++] = Offset + FullIndex_LeftL1(P, O);
		Array[Index++] = Offset + FullIndex_RightL1(P, O);
		Offset += FullSize_L1;
	}
	if (Feature_L2){
		Array[Index++] = Offset + FullIndex_LowerL2(P, O);
		Array[Index++] = Offset + FullIndex_UpperL2(P, O);
		Array[Index++] = Offset + FullIndex_LeftL2(P, O);
		Array[Index++] = Offset + FullIndex_RightL2(P, O);
		Offset += FullSize_L2;
	}
	if (Feature_L3){
		Array[Index++] = Offset + FullIndex_LowerL3(P, O);
		Array[Index++] = Offset + FullIndex_UpperL3(P, O);
		Array[Index++] = Offset + FullIndex_LeftL3(P, O);
		Array[Index++] = Offset + FullIndex_RightL3(P, O);
		Offset += FullSize_L3;
	}
	if (Feature_A){
		Array[Index++] = Offset + FullIndex_RightLowerA(P, O);
		Array[Index++] = Offset + FullIndex_LeftUpperA(P, O);
		Array[Index++] = Offset + FullIndex_LeftLowerA(P, O);
		Array[Index++] = Offset + FullIndex_RightUpperA(P, O);
		Offset += FullSize_A;
	}
	if (Feature_Sq){
		Array[Index++] = Offset + FullIndex_RightLowerSq(P, O);
		Array[Index++] = Offset + FullIndex_LeftUpperSq(P, O);
		Array[Index++] = Offset + FullIndex_LeftLowerSq(P, O);
		Array[Index++] = Offset + FullIndex_RightUpperSq(P, O);
		Offset += FullSize_Sq;
	}
	if (Feature_B){
		Array[Index++] = Offset + FullIndex_LowerRightB(P, O);
		Array[Index++] = Offset + FullIndex_LowerLeftB(P, O);
		Array[Index++] = Offset + FullIndex_LeftLowerB(P, O);
		Array[Index++] = Offset + FullIndex_LeftUpperB(P, O);
		Array[Index++] = Offset + FullIndex_UpperLeftB(P, O);
		Array[Index++] = Offset + FullIndex_UpperRightB(P, O);
		Array[Index++] = Offset + FullIndex_RightUpperB(P, O);
		Array[Index++] = Offset + FullIndex_RightLowerB(P, O);
		Offset += FullSize_B;
	}
	//if (Feature_D3){
	//	Array[Index++] = Offset + FullIndex_RightLowerD3(P, O);
	//	Array[Index++] = Offset + FullIndex_LeftUpperD3(P, O);
	//	Array[Index++] = Offset + FullIndex_LeftLowerD3(P, O);
	//	Array[Index++] = Offset + FullIndex_RightUpperD3(P, O);
	//	Offset += FullSize_D3;
	//}
	if (Feature_D4){
		Array[Index++] = Offset + FullIndex_RightLowerD4(P, O);
		Array[Index++] = Offset + FullIndex_LeftUpperD4(P, O);
		Array[Index++] = Offset + FullIndex_LeftLowerD4(P, O);
		Array[Index++] = Offset + FullIndex_RightUpperD4(P, O);
		Offset += FullSize_D4;
	}
	if (Feature_D5){
		Array[Index++] = Offset + FullIndex_RightLowerD5(P, O);
		Array[Index++] = Offset + FullIndex_LeftUpperD5(P, O);
		Array[Index++] = Offset + FullIndex_LeftLowerD5(P, O);
		Array[Index++] = Offset + FullIndex_RightUpperD5(P, O);
		Offset += FullSize_D5;
	}
	if (Feature_D6){
		Array[Index++] = Offset + FullIndex_RightLowerD6(P, O);
		Array[Index++] = Offset + FullIndex_LeftUpperD6(P, O);
		Array[Index++] = Offset + FullIndex_LeftLowerD6(P, O);
		Array[Index++] = Offset + FullIndex_RightUpperD6(P, O);
		Offset += FullSize_D6;
	}
	if (Feature_D7){
		Array[Index++] = Offset + FullIndex_RightLowerD7(P, O);
		Array[Index++] = Offset + FullIndex_LeftUpperD7(P, O);
		Array[Index++] = Offset + FullIndex_LeftLowerD7(P, O);
		Array[Index++] = Offset + FullIndex_RightUpperD7(P, O);
		Offset += FullSize_D7;
	}
	//if (Feature_X){
	//	Array[Index++] = Offset + FullIndex_CenterX(P, O);
	//	Offset += FullSize_X;
	//}
}
void FillReducedConfigurationArray(const unsigned long long P, const unsigned long long O, int* const Array)
{
	using namespace Features;
	unsigned long long BitBoardPossible;
	int Index = 0;
	int Offset = 0;

	if (Feature_C){
		Array[Index++] = ReducedIndex_LowerC(P, O);
		Array[Index++] = ReducedIndex_UpperC(P, O);
		Array[Index++] = ReducedIndex_LeftC(P, O);
		Array[Index++] = ReducedIndex_RightC(P, O);
		Offset += ReducedSize_C;
	}
	if (Feature_L1){
		Array[Index++] = Offset + ReducedIndex_LowerL1(P, O);
		Array[Index++] = Offset + ReducedIndex_UpperL1(P, O);
		Array[Index++] = Offset + ReducedIndex_LeftL1(P, O);
		Array[Index++] = Offset + ReducedIndex_RightL1(P, O);
		Offset += ReducedSize_L1;
	}
	if (Feature_L2){
		Array[Index++] = Offset + ReducedIndex_LowerL2(P, O);
		Array[Index++] = Offset + ReducedIndex_UpperL2(P, O);
		Array[Index++] = Offset + ReducedIndex_LeftL2(P, O);
		Array[Index++] = Offset + ReducedIndex_RightL2(P, O);
		Offset += ReducedSize_L2;
	}
	if (Feature_L3){
		Array[Index++] = Offset + ReducedIndex_LowerL3(P, O);
		Array[Index++] = Offset + ReducedIndex_UpperL3(P, O);
		Array[Index++] = Offset + ReducedIndex_LeftL3(P, O);
		Array[Index++] = Offset + ReducedIndex_RightL3(P, O);
		Offset += ReducedSize_L3;
	}
	if (Feature_A){
		Array[Index++] = Offset + ReducedIndex_RightLowerA(P, O);
		Array[Index++] = Offset + ReducedIndex_LeftUpperA(P, O);
		Array[Index++] = Offset + ReducedIndex_LeftLowerA(P, O);
		Array[Index++] = Offset + ReducedIndex_RightUpperA(P, O);
		Offset += ReducedSize_A;
	}
	if (Feature_Sq){
		Array[Index++] = Offset + ReducedIndex_RightLowerSq(P, O);
		Array[Index++] = Offset + ReducedIndex_LeftUpperSq(P, O);
		Array[Index++] = Offset + ReducedIndex_LeftLowerSq(P, O);
		Array[Index++] = Offset + ReducedIndex_RightUpperSq(P, O);
		Offset += ReducedSize_Sq;
	}
	if (Feature_B){
		Array[Index++] = Offset + ReducedIndex_LowerRightB(P, O);
		Array[Index++] = Offset + ReducedIndex_LowerLeftB(P, O);
		Array[Index++] = Offset + ReducedIndex_LeftLowerB(P, O);
		Array[Index++] = Offset + ReducedIndex_LeftUpperB(P, O);
		Array[Index++] = Offset + ReducedIndex_UpperLeftB(P, O);
		Array[Index++] = Offset + ReducedIndex_UpperRightB(P, O);
		Array[Index++] = Offset + ReducedIndex_RightUpperB(P, O);
		Array[Index++] = Offset + ReducedIndex_RightLowerB(P, O);
		Offset += ReducedSize_B;
	}
	if (Feature_D3){
		Array[Index++] = Offset + ReducedIndex_RightLowerD3(P, O);
		Array[Index++] = Offset + ReducedIndex_LeftUpperD3(P, O);
		Array[Index++] = Offset + ReducedIndex_LeftLowerD3(P, O);
		Array[Index++] = Offset + ReducedIndex_RightUpperD3(P, O);
		Offset += ReducedSize_D3;
	}
	if (Feature_D4){
		Array[Index++] = Offset + ReducedIndex_RightLowerD4(P, O);
		Array[Index++] = Offset + ReducedIndex_LeftUpperD4(P, O);
		Array[Index++] = Offset + ReducedIndex_LeftLowerD4(P, O);
		Array[Index++] = Offset + ReducedIndex_RightUpperD4(P, O);
		Offset += ReducedSize_D4;
	}
	if (Feature_D5){
		Array[Index++] = Offset + ReducedIndex_RightLowerD5(P, O);
		Array[Index++] = Offset + ReducedIndex_LeftUpperD5(P, O);
		Array[Index++] = Offset + ReducedIndex_LeftLowerD5(P, O);
		Array[Index++] = Offset + ReducedIndex_RightUpperD5(P, O);
		Offset += ReducedSize_D5;
	}
	if (Feature_D6){
		Array[Index++] = Offset + ReducedIndex_RightLowerD6(P, O);
		Array[Index++] = Offset + ReducedIndex_LeftUpperD6(P, O);
		Array[Index++] = Offset + ReducedIndex_LeftLowerD6(P, O);
		Array[Index++] = Offset + ReducedIndex_RightUpperD6(P, O);
		Offset += ReducedSize_D6;
	}
	if (Feature_D7){
		Array[Index++] = Offset + ReducedIndex_RightLowerD7(P, O);
		Array[Index++] = Offset + ReducedIndex_LeftUpperD7(P, O);
		Array[Index++] = Offset + ReducedIndex_LeftLowerD7(P, O);
		Array[Index++] = Offset + ReducedIndex_RightUpperD7(P, O);
		Offset += ReducedSize_D7;
	}
	if (Feature_X){
		Array[Index++] = Offset + ReducedIndex_CenterX(P, O);
		Offset += ReducedSize_X;
	}
	//if (Feature_PlayersBoarder){
	//	Array[Index++] = Offset + (int)PopCount(PlayersBoarder(P, O));
	//	Offset += ReducedSize_PlayersBoarder;
	//}
	//if (Feature_OpponentsBoarder){
	//	Array[Index++] = Offset + (int)PopCount(OpponentsBoarder(P, O));
	//	Offset += ReducedSize_OpponentsBoarder;
	//}
	//if (Feature_PlayersExposeds){
	//	Array[Index++] = Offset + (int)PopCount(PlayersExposed(P, O));
	//	Offset += ReducedSize_PlayersExposeds;
	//}
	//if (Feature_OpponentsExposeds){
	//	Array[Index++] = Offset + (int)PopCount(OpponentsExposed(P, O));
	//	Offset += ReducedSize_OpponentsExposeds;
	//}

	//if (Feature_Possible || Feature_Possible_E || Feature_Possible_XC)
	//	BitBoardPossible = PossibleMoves(P, O);

	//if (Feature_Possible){
	//	Array[Index++] = Offset + (int)PopCount(BitBoardPossible);
	//	Offset += ReducedSize_Possible;
	//}
	//if (Feature_Possible_E){
	//	Array[Index++] = Offset + (int)PopCount(BitBoardPossible & 0x8100000000000081UL);
	//	Offset += ReducedSize_Possible_E;
	//}
	//if (Feature_Possible_XC){
	//	Array[Index++] = Offset + (int)PopCount(BitBoardPossible & 0x42C300000000C342UL);
	//	Offset += ReducedSize_Possible_XC;
	//}
	//if (Feature_Parity){
	//	const unsigned long long E = ~(P | O);
	//	const unsigned long long parity = (PopCount(E & 0xF0F0F0F000000000ULL) & 1) << 3 
	//									| (PopCount(E & 0x0F0F0F0F00000000ULL) & 1) << 2 
	//									| (PopCount(E & 0x00000000F0F0F0F0ULL) & 1) << 1 
	//									|  PopCount(E & 0x000000000F0F0F0FULL) & 1;
	//	Array[Index++] = Offset + (int)PopCount(parity);
	//	Offset += ReducedSize_Parity;
	//}
	//if (Feature_Quadrants){
	//	const unsigned long long E = ~(P | O);
	//	Array[Index++] = Offset + (int)PopCount(E & 0xF0F0F0F000000000ULL);
	//	Array[Index++] = Offset + (int)PopCount(E & 0x0F0F0F0F00000000ULL);
	//	Array[Index++] = Offset + (int)PopCount(E & 0xF0F0F0F000000000ULL);
	//	Array[Index++] = Offset + (int)PopCount(E & 0x000000000F0F0F0FULL);
	//	Offset += ReducedSize_Quadrants;
	//}
	//if (Feature_NumberOfOwnStones){
	//	Array[Index++] = Offset + (int)PopCount(P);
	//	Offset += ReducedSize_NumberOfOwnStones;
	//}
}

void FillReducedConfigurationArraySorted(const unsigned long long P, const unsigned long long O, int* const Array)
{
	using namespace Features;
	unsigned long long BitBoardPossible;
	int Index = 0;
	int Offset = 0;

	if (Feature_C){
		Array[Index+0] = ReducedIndex_LowerC(P, O);
		Array[Index+1] = ReducedIndex_UpperC(P, O);
		Array[Index+2] = ReducedIndex_LeftC(P, O);
		Array[Index+3] = ReducedIndex_RightC(P, O);
		Sort4(Array, Index);
		Index += Symmetry_C;
		Offset += ReducedSize_C;
	}
	if (Feature_L1){
		Array[Index+0] = Offset + ReducedIndex_LowerL1(P, O);
		Array[Index+1] = Offset + ReducedIndex_UpperL1(P, O);
		Array[Index+2] = Offset + ReducedIndex_LeftL1(P, O);
		Array[Index+3] = Offset + ReducedIndex_RightL1(P, O);
		Sort4(Array, Index);
		Index += Symmetry_L1;
		Offset += ReducedSize_L1;
	}
	if (Feature_L2){
		Array[Index+0] = Offset + ReducedIndex_LowerL2(P, O);
		Array[Index+1] = Offset + ReducedIndex_UpperL2(P, O);
		Array[Index+2] = Offset + ReducedIndex_LeftL2(P, O);
		Array[Index+3] = Offset + ReducedIndex_RightL2(P, O);
		Sort4(Array, Index);
		Index += Symmetry_L2;
		Offset += ReducedSize_L2;
	}
	if (Feature_L3){
		Array[Index+0] = Offset + ReducedIndex_LowerL3(P, O);
		Array[Index+1] = Offset + ReducedIndex_UpperL3(P, O);
		Array[Index+2] = Offset + ReducedIndex_LeftL3(P, O);
		Array[Index+3] = Offset + ReducedIndex_RightL3(P, O);
		Sort4(Array, Index);
		Index += Symmetry_L3;
		Offset += ReducedSize_L3;
	}
	if (Feature_A){
		Array[Index+0] = Offset + ReducedIndex_RightLowerA(P, O);
		Array[Index+1] = Offset + ReducedIndex_LeftUpperA(P, O);
		Array[Index+2] = Offset + ReducedIndex_LeftLowerA(P, O);
		Array[Index+3] = Offset + ReducedIndex_RightUpperA(P, O);
		Sort4(Array, Index);
		Index += Symmetry_A;
		Offset += ReducedSize_A;
	}
	if (Feature_Sq){
		Array[Index+0] = Offset + ReducedIndex_RightLowerSq(P, O);
		Array[Index+1] = Offset + ReducedIndex_LeftUpperSq(P, O);
		Array[Index+2] = Offset + ReducedIndex_LeftLowerSq(P, O);
		Array[Index+3] = Offset + ReducedIndex_RightUpperSq(P, O);
		Sort4(Array, Index);
		Index += Symmetry_Sq;
		Offset += ReducedSize_Sq;
	}
	if (Feature_B){
		Array[Index+0] = Offset + ReducedIndex_LowerRightB(P, O);
		Array[Index+1] = Offset + ReducedIndex_LowerLeftB(P, O);
		Array[Index+2] = Offset + ReducedIndex_LeftLowerB(P, O);
		Array[Index+3] = Offset + ReducedIndex_LeftUpperB(P, O);
		Array[Index+4] = Offset + ReducedIndex_UpperLeftB(P, O);
		Array[Index+5] = Offset + ReducedIndex_UpperRightB(P, O);
		Array[Index+6] = Offset + ReducedIndex_RightUpperB(P, O);
		Array[Index+7] = Offset + ReducedIndex_RightLowerB(P, O);
		Sort8(Array, Index);
		Index += Symmetry_B;
		Offset += ReducedSize_B;
	}
	if (Feature_D3){
		Array[Index+0] = Offset + ReducedIndex_RightLowerD3(P, O);
		Array[Index+1] = Offset + ReducedIndex_LeftUpperD3(P, O);
		Array[Index+2] = Offset + ReducedIndex_LeftLowerD3(P, O);
		Array[Index+3] = Offset + ReducedIndex_RightUpperD3(P, O);
		Sort4(Array, Index);
		Index += Symmetry_D3;
		Offset += ReducedSize_D3;
	}
	if (Feature_D4){
		Array[Index+0] = Offset + ReducedIndex_RightLowerD4(P, O);
		Array[Index+1] = Offset + ReducedIndex_LeftUpperD4(P, O);
		Array[Index+2] = Offset + ReducedIndex_LeftLowerD4(P, O);
		Array[Index+3] = Offset + ReducedIndex_RightUpperD4(P, O);
		Sort4(Array, Index);
		Index += Symmetry_D4;
		Offset += ReducedSize_D4;
	}
	if (Feature_D5){
		Array[Index+0] = Offset + ReducedIndex_RightLowerD5(P, O);
		Array[Index+1] = Offset + ReducedIndex_LeftUpperD5(P, O);
		Array[Index+2] = Offset + ReducedIndex_LeftLowerD5(P, O);
		Array[Index+3] = Offset + ReducedIndex_RightUpperD5(P, O);
		Sort4(Array, Index);
		Index += Symmetry_D5;
		Offset += ReducedSize_D5;
	}
	if (Feature_D6){
		Array[Index+0] = Offset + ReducedIndex_RightLowerD6(P, O);
		Array[Index+1] = Offset + ReducedIndex_LeftUpperD6(P, O);
		Array[Index+2] = Offset + ReducedIndex_LeftLowerD6(P, O);
		Array[Index+3] = Offset + ReducedIndex_RightUpperD6(P, O);
		Sort4(Array, Index);
		Index += Symmetry_D6;
		Offset += ReducedSize_D6;
	}
	if (Feature_D7){
		Array[Index+0] = Offset + ReducedIndex_RightLowerD7(P, O);
		Array[Index+1] = Offset + ReducedIndex_LeftUpperD7(P, O);
		Array[Index+2] = Offset + ReducedIndex_LeftLowerD7(P, O);
		Array[Index+3] = Offset + ReducedIndex_RightUpperD7(P, O);
		Sort4(Array, Index);
		Index += Symmetry_D7;
		Offset += ReducedSize_D7;
	}
	if (Feature_X){
		Array[Index++] = Offset + ReducedIndex_CenterX(P, O);
		Offset += ReducedSize_X;
	}
	//if (Feature_PlayersBoarder){
	//	Array[Index++] = Offset + (int)PopCount(PlayersBoarder(P, O));
	//	Offset += ReducedSize_PlayersBoarder;
	//}
	//if (Feature_OpponentsBoarder){
	//	Array[Index++] = Offset + (int)PopCount(OpponentsBoarder(P, O));
	//	Offset += ReducedSize_OpponentsBoarder;
	//}
	//if (Feature_PlayersExposeds){
	//	Array[Index++] = Offset + (int)PopCount(PlayersExposed(P, O));
	//	Offset += ReducedSize_PlayersExposeds;
	//}
	//if (Feature_OpponentsExposeds){
	//	Array[Index++] = Offset + (int)PopCount(OpponentsExposed(P, O));
	//	Offset += ReducedSize_OpponentsExposeds;
	//}

	//if (Feature_Possible || Feature_Possible_E || Feature_Possible_XC)
	//	BitBoardPossible = PossibleMoves(P, O);

	//if (Feature_Possible){
	//	Array[Index++] = Offset + (int)PopCount(BitBoardPossible);
	//	Offset += ReducedSize_Possible;
	//}
	//if (Feature_Possible_E){
	//	Array[Index++] = Offset + (int)PopCount(BitBoardPossible & 0x8100000000000081UL);
	//	Offset += ReducedSize_Possible_E;
	//}
	//if (Feature_Possible_XC){
	//	Array[Index++] = Offset + (int)PopCount(BitBoardPossible & 0x42C300000000C342UL);
	//	Offset += ReducedSize_Possible_XC;
	//}
	//if (Feature_Parity){
	//	unsigned long long Parity = (PopCount(~(P | O) & 0xF0F0F0F000000000ULL) & 1) << 3 
	//							  | (PopCount(~(P | O) & 0x0F0F0F0F00000000ULL) & 1) << 2 
	//							  | (PopCount(~(P | O) & 0x00000000F0F0F0F0ULL) & 1) << 1 
	//							  |  PopCount(~(P | O) & 0x000000000F0F0F0FULL) & 1;
	//	Array[Index++] = Offset + (int)PopCount(Parity);
	//	Offset += ReducedSize_Parity;
	//}
	//if (Feature_Quadrants){
	//	Array[Index+0] = Offset + (int)PopCount(~(P | O) & 0xF0F0F0F000000000ULL);
	//	Array[Index+1] = Offset + (int)PopCount(~(P | O) & 0x0F0F0F0F00000000ULL);
	//	Array[Index+2] = Offset + (int)PopCount(~(P | O) & 0xF0F0F0F000000000ULL);
	//	Array[Index+3] = Offset + (int)PopCount(~(P | O) & 0x000000000F0F0F0FULL);
	//	Sort4(Array, Index);
	//	Index += Symmetry_Quadrants;
	//	Offset += ReducedSize_Quadrants;
	//}
	//if (Feature_NumberOfOwnStones){
	//	Array[Index++] = Offset + (int)PopCount(P);
	//	Offset += ReducedSize_NumberOfOwnStones;
	//}
}

int EvaluateFeatures(const unsigned long long P, const unsigned long long O)
{
	float sum = 0.0f;
	unsigned char BoxIndex = Features::BoxIndex[NumberOfEmptyStones(P, O)];

	int Array[Features::Symmetries];
	FillFullConfigurationArray(P, O, Array);

	for (int i = 0; i < Features::Symmetries; ++i)
		sum += Features::Weights[BoxIndex][Array[i]];
	
	return static_cast<int>(std::floorf(sum + 0.5f));
}

CActiveConfigurations::CActiveConfigurations(const unsigned long long P, const unsigned long long O) : m_P(O), m_O(P)
{
	FillFullConfigurationArray(m_P, m_O, m_Array);
}

int CActiveConfigurations::EvaluateFeatures() const
{
	float sum = 0.0f;
	//for (int i = 0; i < Features::Symmetries; ++i)
	//	sum += m_weights[i];
	return static_cast<int>(std::floorf(sum + 0.5f));
}

int CActiveConfigurations::EvaluateFeatures(const unsigned long long P, const unsigned long long O) const
{
	using namespace Features;
	const unsigned long long diff = (m_P ^ P) | (m_O ^ O);
	unsigned long long BitBoardPossible;
	int Index = 0;
	int Offset = 0;

	int BoxIndex = Features::BoxIndex[NumberOfEmptyStones(P, O)];
	//// ############ Remove safety
	//if (BoxIndex >= Features::NumberOfFiles)
	//{
	//	BoxIndex = Features::NumberOfFiles - 1;
	//	//std::cerr << "ERROR: Feature index out of range!" << std::endl;
	//	//throw std::errc(0);
	//}
	//// ############ Remove safety

	float * weights = Features::Weights[BoxIndex];


	float sum = 0.0f;

	if (Feature_C){
		sum += weights[(diff & Pattern_LowerC) ? FullIndex_LowerC(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_UpperC) ? FullIndex_UpperC(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftC ) ? FullIndex_LeftC (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightC) ? FullIndex_RightC(P, O) : m_Array[Index]]; Index++;
		Offset += FullSize_C;
	}
	if (Feature_L1){
		sum += weights[(diff & Pattern_LowerL1) ? Offset + FullIndex_LowerL1(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_UpperL1) ? Offset + FullIndex_UpperL1(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftL1 ) ? Offset + FullIndex_LeftL1 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightL1) ? Offset + FullIndex_RightL1(P, O) : m_Array[Index]]; Index++;
		Offset += FullSize_L1;
	}
	if (Feature_L2){
		sum += weights[(diff & Pattern_LowerL2) ? Offset + FullIndex_LowerL2(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_UpperL2) ? Offset + FullIndex_UpperL2(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftL2 ) ? Offset + FullIndex_LeftL2 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightL2) ? Offset + FullIndex_RightL2(P, O) : m_Array[Index]]; Index++;
		Offset += FullSize_L2;
	}
	if (Feature_L3){
		sum += weights[(diff & Pattern_LowerL3) ? Offset + FullIndex_LowerL3(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_UpperL3) ? Offset + FullIndex_UpperL3(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftL3 ) ? Offset + FullIndex_LeftL3 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightL3) ? Offset + FullIndex_RightL3(P, O) : m_Array[Index]]; Index++;
		Offset += FullSize_L3;
	}
	if (Feature_A){
		sum += weights[(diff & Pattern_RightLowerA) ? Offset + FullIndex_RightLowerA(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftUpperA ) ? Offset + FullIndex_LeftUpperA (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftLowerA ) ? Offset + FullIndex_LeftLowerA (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightUpperA) ? Offset + FullIndex_RightUpperA(P, O) : m_Array[Index]]; Index++;
		Offset += FullSize_A;
	}
	if (Feature_Sq){
		sum += weights[(diff & Pattern_RightLowerSq) ? Offset + FullIndex_RightLowerSq(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftUpperSq ) ? Offset + FullIndex_LeftUpperSq (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftLowerSq ) ? Offset + FullIndex_LeftLowerSq (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightUpperSq) ? Offset + FullIndex_RightUpperSq(P, O) : m_Array[Index]]; Index++;
		Offset += FullSize_Sq;
	}
	if (Feature_B){
		sum += weights[(diff & Pattern_LowerRightB) ? Offset + FullIndex_LowerRightB(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LowerLeftB ) ? Offset + FullIndex_LowerLeftB (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftLowerB ) ? Offset + FullIndex_LeftLowerB (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftUpperB ) ? Offset + FullIndex_LeftUpperB (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_UpperLeftB ) ? Offset + FullIndex_UpperLeftB (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_UpperRightB) ? Offset + FullIndex_UpperRightB(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightUpperB) ? Offset + FullIndex_RightUpperB(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightLowerB) ? Offset + FullIndex_RightLowerB(P, O) : m_Array[Index]]; Index++;
		Offset += FullSize_B;
	}
	//if (Feature_D3){
	//	sum += weights[(diff & Pattern_RightLowerD3) ? Offset + FullIndex_RightLowerD3(P, O) : m_Array[Index]]; Index++;
	//	sum += weights[(diff & Pattern_LeftUpperD3 ) ? Offset + FullIndex_LeftUpperD3 (P, O) : m_Array[Index]]; Index++;
	//	sum += weights[(diff & Pattern_LeftLowerD3 ) ? Offset + FullIndex_LeftLowerD3 (P, O) : m_Array[Index]]; Index++;
	//	sum += weights[(diff & Pattern_RightUpperD3) ? Offset + FullIndex_RightUpperD3(P, O) : m_Array[Index]]; Index++;
	//	Offset += FullSize_D3;
	//}
	if (Feature_D4){
		sum += weights[(diff & Pattern_RightLowerD4) ? Offset + FullIndex_RightLowerD4(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftUpperD4 ) ? Offset + FullIndex_LeftUpperD4 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftLowerD4 ) ? Offset + FullIndex_LeftLowerD4 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightUpperD4) ? Offset + FullIndex_RightUpperD4(P, O) : m_Array[Index]]; Index++;
		Offset += FullSize_D4;
	}
	if (Feature_D5){
		sum += weights[(diff & Pattern_RightLowerD5) ? Offset + FullIndex_RightLowerD5(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftUpperD5 ) ? Offset + FullIndex_LeftUpperD5 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftLowerD5 ) ? Offset + FullIndex_LeftLowerD5 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightUpperD5) ? Offset + FullIndex_RightUpperD5(P, O) : m_Array[Index]]; Index++;
		Offset += FullSize_D5;
	}
	if (Feature_D6){
		sum += weights[(diff & Pattern_RightLowerD6) ? Offset + FullIndex_RightLowerD6(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftUpperD6 ) ? Offset + FullIndex_LeftUpperD6 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftLowerD6 ) ? Offset + FullIndex_LeftLowerD6 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightUpperD6) ? Offset + FullIndex_RightUpperD6(P, O) : m_Array[Index]]; Index++;
		Offset += FullSize_D6;
	}
	if (Feature_D7){
		sum += weights[(diff & Pattern_RightLowerD7) ? Offset + FullIndex_RightLowerD7(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftUpperD7 ) ? Offset + FullIndex_LeftUpperD7 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftLowerD7 ) ? Offset + FullIndex_LeftLowerD7 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightUpperD7) ? Offset + FullIndex_RightUpperD7(P, O) : m_Array[Index]]; Index++;
		Offset += FullSize_D7;
	}
	//if (Feature_X){
	//	sum += weights[(diff & Pattern_CenterX) ? Offset + FullIndex_CenterX(P, O) : m_Array[Index]]; Index++;
	//	Offset += FullSize_X;
	//}
	//if (Feature_PlayersBoarder){
	//	sum += weights[Offset + (int)PopCount(PlayersBoarder(P, O))];
	//	Offset += FullSize_PlayersBoarder;
	//}
	//if (Feature_OpponentsBoarder){
	//	sum += weights[Offset + (int)PopCount(OpponentsBoarder(P, O))];
	//	Offset += FullSize_OpponentsBoarder;
	//}
	//if (Feature_PlayersExposeds){
	//	sum += weights[Offset + (int)PopCount(PlayersExposed(P, O))];
	//	Offset += FullSize_PlayersExposeds;
	//}
	//if (Feature_OpponentsExposeds){
	//	sum += weights[Offset + (int)PopCount(OpponentsExposed(P, O))];
	//	Offset += FullSize_OpponentsExposeds;
	//}

	//if (Feature_Possible || Feature_Possible_E || Feature_Possible_XC)
	//	BitBoardPossible = PossibleMoves(P, O);

	//if (Feature_Possible){
	//	sum += weights[Offset + (int)PopCount(BitBoardPossible)];
	//	Offset += FullSize_Possible;
	//}
	//if (Feature_Possible_E){
	//	sum += weights[Offset + (int)PopCount(BitBoardPossible & 0x8100000000000081UL)];
	//	Offset += FullSize_Possible_E;
	//}
	//if (Feature_Possible_XC){
	//	sum += weights[Offset + (int)PopCount(BitBoardPossible & 0x42C300000000C342UL)];
	//	Offset += FullSize_Possible_XC;
	//}
	//if (Feature_Parity){
	//	const unsigned long long E = ~(P | O);
	//	const unsigned long long parity = (PopCount(E & 0xF0F0F0F000000000ULL) & 1) << 3 
	//									| (PopCount(E & 0x0F0F0F0F00000000ULL) & 1) << 2 
	//									| (PopCount(E & 0x00000000F0F0F0F0ULL) & 1) << 1 
	//									|  PopCount(E & 0x000000000F0F0F0FULL) & 1;
	//	sum += weights[Offset + (int)PopCount(parity)];
	//	Offset += FullSize_Parity;
	//}
	//if (Feature_Quadrants){
	//	const unsigned long long E = ~(P | O);
	//	sum += weights[Offset + (int)PopCount(E & 0xF0F0F0F000000000ULL)];
	//	sum += weights[Offset + (int)PopCount(E & 0x0F0F0F0F00000000ULL)];
	//	sum += weights[Offset + (int)PopCount(E & 0xF0F0F0F000000000ULL)];
	//	sum += weights[Offset + (int)PopCount(E & 0x000000000F0F0F0FULL)];
	//	Offset += FullSize_Quadrants;
	//}
	//if (Feature_NumberOfOwnStones){
	//	sum += weights[Offset + (int)PopCount(P)];
	//	Offset += FullSize_NumberOfOwnStones;
	//}

	return static_cast<int>(std::floorf(sum + 0.5f));
}


bool Congruent(const unsigned long long P1, const unsigned long long P2)
{
	if (P1 == P2) return true;
	if (P1 == horizontal_flip(P2)) return true;
	if (P1 == vertical_flip(P2)) return true;
	if (P1 == diagonal_flip(P2)) return true;
	if (P1 == codiagonal_flip(P2)) return true;
	if (P1 == vertical_flip(horizontal_flip(P2))) return true;
	if (P1 == diagonal_flip(horizontal_flip(P2))) return true;
	if (P1 == codiagonal_flip(horizontal_flip(P2))) return true;
	return false;
}

bool Test_Patterns_PopCount(const int symmeties, const unsigned long long * const Pattern)
{
	bool Success = true;
	unsigned long long * POPCOUNT = new unsigned long long[symmeties];

	for (int i = 0; i < symmeties; i++)
		POPCOUNT[i] = PopCount(Pattern[i]);
	for (int i = 0; i < symmeties; i++)
	{
		if (POPCOUNT[0] != POPCOUNT[i]){
			std::cerr << "ERROR: Pattern are inconsistent!" << std::endl;
			Success = false;
		}
	}

	delete[] POPCOUNT;
	return Success;
}

bool Test_Pattern_Congruence(const int symmeties, const unsigned long long * const Pattern)
{
	bool Success = true;

	for (int i = 0; i < symmeties; i++)
	{
		if (!Congruent(Pattern[0], Pattern[i])){
			std::cerr << "ERROR: Pattern are not congruent!" << std::endl;
			Success = false;
		}
	}

	return Success;
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

bool Test_Index_Range(const int symmeties, const int size, unsigned long long * Pattern, int(*Feature[])(const unsigned long long P, const unsigned long long O))
{
	bool Success = true;

	for (int k = 0; k < symmeties; k++)
	{
		For_each_configuration_in_pattern_do_fkt(Pattern[k],
			[Feature, k, size, &Success](const unsigned long long P, const unsigned long long O)
				{
					int index = Feature[k](P, O);
					if (index < 0){
						std::cerr << "ERROR: Index is too small!" << std::endl;
						Success = false;
					}
					if (index > size){
						std::cerr << "ERROR: Index is too big!" << std::endl;
						Success = false;
					}
					return ;
				}
		);
	}

	return Success;
}

bool Test_Feature_symmetrie(const int symmeties, unsigned long long * Pattern, int(*Feature[])(const unsigned long long P, const unsigned long long O))
{
	bool Success = true;

	For_each_configuration_in_pattern_do_fkt(Pattern[0],
		[Feature, symmeties, Pattern, &Success](const unsigned long long P, const unsigned long long O)
			{
				int indexA = Feature[0](P, O);
				int indexB;
				for (int k = 0; k < symmeties; ++k)
				{
					if (Pattern[k] == horizontal_flip(Pattern[0])){
						indexB = Feature[k](horizontal_flip(P), horizontal_flip(O));
						if (indexA != indexB){
							std::cerr << "ERROR: Indices don't match!" << std::endl;
							Success = false;
						}
					}
					if (Pattern[k] == vertical_flip(Pattern[0])){
						indexB = Feature[k](vertical_flip(P), vertical_flip(O));
						if (indexA != indexB){
							std::cerr << "ERROR: Indices don't match!" << std::endl;
							Success = false;
						}
					}
					if (Pattern[k] == diagonal_flip(Pattern[0])){
						indexB = Feature[k](diagonal_flip(P), diagonal_flip(O));
						if (indexA != indexB){
							std::cerr << "ERROR: Indices don't match!" << std::endl;
							Success = false;
						}
					}
					if (Pattern[k] == codiagonal_flip(Pattern[0])){
						indexB = Feature[k](codiagonal_flip(P), codiagonal_flip(O));
						if (indexA != indexB){
							std::cerr << "ERROR: Indices don't match!" << std::endl;
							Success = false;
						}
					}
					if (Pattern[k] == vertical_flip(horizontal_flip(Pattern[0]))){
						indexB = Feature[k](vertical_flip(horizontal_flip(P)), vertical_flip(horizontal_flip(O)));
						if (indexA != indexB){
							std::cerr << "ERROR: Indices don't match!" << std::endl;
							Success = false;
						}
					}
					if (Pattern[k] == diagonal_flip(horizontal_flip(Pattern[0]))){
						indexB = Feature[k](diagonal_flip(horizontal_flip(P)), diagonal_flip(horizontal_flip(O)));
						if (indexA != indexB){
							std::cerr << "ERROR: Indices don't match!" << std::endl;
							Success = false;
						}
					}
					if (Pattern[k] == codiagonal_flip(horizontal_flip(Pattern[0]))){
						indexB = Feature[k](codiagonal_flip(horizontal_flip(P)), codiagonal_flip(horizontal_flip(O)));
						if (indexA != indexB){
							std::cerr << "ERROR: Indices don't match!" << std::endl;
							Success = false;
						}
					}
				}
			}
	);

	return Success;
}

bool Test_Feature_symmetrie_FullIndex(int * tmpWeights, const int symmeties, unsigned long long * Pattern, int(*Feature[])(const unsigned long long P, const unsigned long long O), int Offset)
{
	bool Success = true;

	For_each_configuration_in_pattern_do_fkt(Pattern[0],
		[&](const unsigned long long P, const unsigned long long O)
			{
				auto err = [&]{
					std::cerr << "ERROR: Indices don't match!" << std::endl;
					std::cerr << board2D(P, O) << "";
					Success = false;
				};
				int indexA, indexB;
				int weightA = tmpWeights[indexA = Offset + Feature[0](P, O)];
				int weightB;
				for (int k = 0; k < symmeties; ++k)
				{
					if (Pattern[k] == horizontal_flip(Pattern[0])){
						weightB = tmpWeights[indexB = Offset + Feature[k](horizontal_flip(P), horizontal_flip(O))];
						if (weightA != weightB) err();
					}
					if (Pattern[k] == vertical_flip(Pattern[0])){
						weightB = tmpWeights[indexB = Offset + Feature[k](vertical_flip(P), vertical_flip(O))];
						if (weightA != weightB) err();
					}
					if (Pattern[k] == diagonal_flip(Pattern[0])){
						weightB = tmpWeights[indexB = Offset + Feature[k](diagonal_flip(P), diagonal_flip(O))];
						if (weightA != weightB) err();
					}
					if (Pattern[k] == codiagonal_flip(Pattern[0])){
						weightB = tmpWeights[indexB = Offset + Feature[k](codiagonal_flip(P), codiagonal_flip(O))];
						if (weightA != weightB) err();
					}
					if (Pattern[k] == vertical_flip(horizontal_flip(Pattern[0]))){
						weightB = tmpWeights[indexB = Offset + Feature[k](vertical_flip(horizontal_flip(P)), vertical_flip(horizontal_flip(O)))];
						if (weightA != weightB) err();
					}
					if (Pattern[k] == diagonal_flip(horizontal_flip(Pattern[0]))){
						weightB = tmpWeights[indexB = Offset + Feature[k](diagonal_flip(horizontal_flip(P)), diagonal_flip(horizontal_flip(O)))];
						if (weightA != weightB) err();
					}
					if (Pattern[k] == codiagonal_flip(horizontal_flip(Pattern[0]))){
						weightB = tmpWeights[indexB = Offset + Feature[k](codiagonal_flip(horizontal_flip(P)), codiagonal_flip(horizontal_flip(O)))];
						if (weightA != weightB) err();
					}
				}
			}
	);

	return Success;
}

bool Test_Index_Covering(const int symmeties, const int size, unsigned long long * Pattern, int(*Feature[])(const unsigned long long P, const unsigned long long O))
{
	bool Success = true;
	int * taken = new int[size];

	for (int k = 0; k < symmeties; k++)
	{
		for (int i = 0; i < size; i++)
			taken[i] = 0;

		For_each_configuration_in_pattern_do_fkt(Pattern[k],
			[Feature, k, &taken](const unsigned long long P, const unsigned long long O){ taken[Feature[k](P, O)]++; }
		);

		for (int i = 0; i < size; i++)
		{
			if (taken[i] == 0){
				std::cerr << "ERROR: Not every index is covered! Index " << i << " is never taken." << std::endl;
				Success = false;
			}
		}
	}
	return Success;
}

bool Test(const int symmeties, const int size, unsigned long long * Pattern, int(*Feature[])(const unsigned long long P, const unsigned long long O), bool IsPattern)
{
	using namespace Features;
	bool Success = true;
	const unsigned long long POPCOUNT = PopCount(Pattern[0]);
	unsigned long long * BitConfig = new unsigned long long[POPCOUNT];

	// Test patern's population count to be equal
	Success &= Test_Patterns_PopCount(symmeties, Pattern);

	// Test congruence of patterns
	Success &= Test_Pattern_Congruence(symmeties, Pattern);

	// Test the range of all the features
	Success &= Test_Index_Range(symmeties, size, Pattern, Feature);

	// Test if all the symmetries of all configurations of all the features result in the same index
	Success &= Test_Feature_symmetrie(symmeties, Pattern, Feature);

	// Test if all possible indices are taken at least once
	if (IsPattern)
		Success &= Test_Index_Covering(symmeties, size, Pattern, Feature);

	return Success;

	int UniqueCount;
	bool HasSymmetry;
	unsigned long long P, O, tmp;
	int * taken = new int[size];
	for (int k = 0; k < symmeties; k++)
	{
		for (int i = 0; i < size; i++)
			taken[i] = 0;

		For_each_configuration_in_pattern_do_fkt(Pattern[k],
			[Feature, k, &taken](const unsigned long long P, const unsigned long long O){ taken[Feature[k](P, O)]++; }
		);

		UniqueCount = 0;
		for (unsigned long long i = 0; i < (1ULL << POPCOUNT); i++)
		{
			P = 0;
			tmp = i;
			while (tmp)
			{
				P |= BitConfig[BitScanLSB(tmp)];
				RemoveLSB(tmp);
			}

			HasSymmetry = false;
			if (P == horizontal_flip(P)) HasSymmetry = true;
			if (P == vertical_flip(P)) HasSymmetry = true;
			if (P == diagonal_flip(P)) HasSymmetry = true;
			if (P == codiagonal_flip(P)) HasSymmetry = true;
			if (P == vertical_flip(horizontal_flip(P))) HasSymmetry = true;
			if (P == diagonal_flip(horizontal_flip(P))) HasSymmetry = true;
			if (P == codiagonal_flip(horizontal_flip(P))) HasSymmetry = true;
			if (HasSymmetry == false)
				continue;

			for (unsigned long long j = 0; j < (1ULL << POPCOUNT); j++)
			{
				if ((i & j) != 0)
					continue;

				O = 0;
				tmp = j;
				while (tmp)
				{
					O |= BitConfig[BitScanLSB(tmp)];
					RemoveLSB(tmp);
				}
				
				HasSymmetry = false;
				if ((P == horizontal_flip(P)) && (O == horizontal_flip(O)) && (Pattern[k] == horizontal_flip(Pattern[k]))) HasSymmetry = true;
				else if ((P == vertical_flip(P)) && (O == vertical_flip(O)) && (Pattern[k] == vertical_flip(Pattern[k]))) HasSymmetry = true;
				else if ((P == diagonal_flip(P)) && (O == diagonal_flip(O)) && (Pattern[k] == diagonal_flip(Pattern[k]))) HasSymmetry = true;
				else if ((P == codiagonal_flip(P)) && (O == codiagonal_flip(O)) && (Pattern[k] == codiagonal_flip(Pattern[k]))) HasSymmetry = true;
				else if ((P == vertical_flip(horizontal_flip(P))) && (O == vertical_flip(horizontal_flip(O))) && (Pattern[k] == vertical_flip(horizontal_flip(Pattern[k])))) HasSymmetry = true;
				else if ((P == diagonal_flip(horizontal_flip(P))) && (O == diagonal_flip(horizontal_flip(O))) && (Pattern[k] == diagonal_flip(horizontal_flip(Pattern[k])))) HasSymmetry = true;
				else if ((P == codiagonal_flip(horizontal_flip(P))) && (O == codiagonal_flip(horizontal_flip(O))) && (Pattern[k] == codiagonal_flip(horizontal_flip(Pattern[k])))) HasSymmetry = true;
				if (HasSymmetry){
					if (taken[Feature[k](P, O)] != 1){
						std::cerr << "ERROR: Symmetric position has non unique index! " << Feature[k](P, O) << " : " << taken[Feature[k](P, O)] << " : " << std::hex << P << " : " << std::hex << O << std::endl;
						Success = false;
					}
					UniqueCount++;
				}
			}
		}
		for (int i = 0; i < size; i++)
		{
			if (taken[i] == 1)
				UniqueCount--;
		}
		if (UniqueCount != 0){
			std::cerr << "ERROR: Numer of unique indices is wrong!" << UniqueCount << std::endl;
			Success = false;
		}
	}

	return Success;
}

bool Test_All_Features()
{
	using namespace Features;
	bool Success = true;
	int * tmpWeights = new int[FullSize];
	int ReducedOffset = 0;
	int FullOffset = 0;
	For_each_configuration_in_pattern_do_fkt(Pattern_LowerC      , [&](unsigned long long P, unsigned long long O){	tmpWeights[FullIndex_LowerC      (P, O) + FullOffset] = ReducedIndex_LowerC      (P, O) + ReducedOffset; }); ReducedOffset += ReducedSize_C ; FullOffset += FullSize_C ;
	For_each_configuration_in_pattern_do_fkt(Pattern_LowerL1     , [&](unsigned long long P, unsigned long long O){ tmpWeights[FullIndex_LowerL1     (P, O) + FullOffset] = ReducedIndex_LowerL1     (P, O) + ReducedOffset; }); ReducedOffset += ReducedSize_L1; FullOffset += FullSize_L1;
	For_each_configuration_in_pattern_do_fkt(Pattern_LowerL2     , [&](unsigned long long P, unsigned long long O){ tmpWeights[FullIndex_LowerL2     (P, O) + FullOffset] = ReducedIndex_LowerL2     (P, O) + ReducedOffset; }); ReducedOffset += ReducedSize_L2; FullOffset += FullSize_L2;
	For_each_configuration_in_pattern_do_fkt(Pattern_LowerL3     , [&](unsigned long long P, unsigned long long O){ tmpWeights[FullIndex_LowerL3     (P, O) + FullOffset] = ReducedIndex_LowerL3     (P, O) + ReducedOffset; }); ReducedOffset += ReducedSize_L3; FullOffset += FullSize_L3;
	For_each_configuration_in_pattern_do_fkt(Pattern_RightLowerA , [&](unsigned long long P, unsigned long long O){ tmpWeights[FullIndex_RightLowerA (P, O) + FullOffset] = ReducedIndex_RightLowerA (P, O) + ReducedOffset; }); ReducedOffset += ReducedSize_A ; FullOffset += FullSize_A ;
	For_each_configuration_in_pattern_do_fkt(Pattern_RightLowerSq, [&](unsigned long long P, unsigned long long O){ tmpWeights[FullIndex_RightLowerSq(P, O) + FullOffset] = ReducedIndex_RightLowerSq(P, O) + ReducedOffset; }); ReducedOffset += ReducedSize_Sq; FullOffset += FullSize_Sq;
	For_each_configuration_in_pattern_do_fkt(Pattern_LowerRightB , [&](unsigned long long P, unsigned long long O){ tmpWeights[FullIndex_LowerRightB (P, O) + FullOffset] = ReducedIndex_LowerRightB (P, O) + ReducedOffset; }); ReducedOffset += ReducedSize_B ; FullOffset += FullSize_B ;
	For_each_configuration_in_pattern_do_fkt(Pattern_RightLowerD4, [&](unsigned long long P, unsigned long long O){ tmpWeights[FullIndex_RightLowerD4(P, O) + FullOffset] = ReducedIndex_RightLowerD4(P, O) + ReducedOffset; }); ReducedOffset += ReducedSize_D4; FullOffset += FullSize_D4;
	For_each_configuration_in_pattern_do_fkt(Pattern_RightLowerD5, [&](unsigned long long P, unsigned long long O){ tmpWeights[FullIndex_RightLowerD5(P, O) + FullOffset] = ReducedIndex_RightLowerD5(P, O) + ReducedOffset; }); ReducedOffset += ReducedSize_D5; FullOffset += FullSize_D5;
	For_each_configuration_in_pattern_do_fkt(Pattern_RightLowerD6, [&](unsigned long long P, unsigned long long O){ tmpWeights[FullIndex_RightLowerD6(P, O) + FullOffset] = ReducedIndex_RightLowerD6(P, O) + ReducedOffset; }); ReducedOffset += ReducedSize_D6; FullOffset += FullSize_D6;
	For_each_configuration_in_pattern_do_fkt(Pattern_RightLowerD7, [&](unsigned long long P, unsigned long long O){ tmpWeights[FullIndex_RightLowerD7(P, O) + FullOffset] = ReducedIndex_RightLowerD7(P, O) + ReducedOffset; }); ReducedOffset += ReducedSize_D7; FullOffset += FullSize_D7;
	
	std::vector<std::string> Penis;
	for (int i = 0; i < FullSize; i++)
		Penis.push_back(std::to_string(tmpWeights[i]).append("\n"));
	write_to_file<std::string>("Penis_vec.log", Penis);
	
	unsigned long long Pattern_A[] = {Pattern_RightLowerA, Pattern_LeftUpperA, Pattern_LeftLowerA, Pattern_RightUpperA};
	unsigned long long Pattern_B[] = {Pattern_LowerRightB, Pattern_LowerLeftB, Pattern_LeftLowerB, Pattern_LeftUpperB, Pattern_UpperLeftB, Pattern_UpperRightB, Pattern_RightUpperB, Pattern_RightLowerB};
	unsigned long long Pattern_C[] = {Pattern_LowerC, Pattern_UpperC, Pattern_LeftC, Pattern_RightC};
	unsigned long long Pattern_D3[] = {Pattern_RightLowerD3, Pattern_LeftUpperD3, Pattern_LeftLowerD3, Pattern_RightUpperD3};
	unsigned long long Pattern_D4[] = {Pattern_RightLowerD4, Pattern_LeftUpperD4, Pattern_LeftLowerD4, Pattern_RightUpperD4};
	unsigned long long Pattern_D5[] = {Pattern_RightLowerD5, Pattern_LeftUpperD5, Pattern_LeftLowerD5, Pattern_RightUpperD5};
	unsigned long long Pattern_D6[] = {Pattern_RightLowerD6, Pattern_LeftUpperD6, Pattern_LeftLowerD6, Pattern_RightUpperD6};
	unsigned long long Pattern_D7[] = {Pattern_RightLowerD7, Pattern_LeftUpperD7, Pattern_LeftLowerD7, Pattern_RightUpperD7};
	unsigned long long Pattern_L1[] = {Pattern_LowerL1, Pattern_UpperL1, Pattern_LeftL1, Pattern_RightL1};
	unsigned long long Pattern_L2[] = {Pattern_LowerL2, Pattern_UpperL2, Pattern_LeftL2, Pattern_RightL2};
	unsigned long long Pattern_L3[] = {Pattern_LowerL3, Pattern_UpperL3, Pattern_LeftL3, Pattern_RightL3};
	unsigned long long Pattern_Sq[] = {Pattern_RightLowerSq, Pattern_LeftUpperSq, Pattern_LeftLowerSq, Pattern_RightUpperSq};
	int(*ReducedIndex_A[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_RightLowerA, ReducedIndex_LeftUpperA, ReducedIndex_LeftLowerA, ReducedIndex_RightUpperA};
	int(*ReducedIndex_B[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_LowerRightB, ReducedIndex_LowerLeftB, ReducedIndex_LeftLowerB, ReducedIndex_LeftUpperB, ReducedIndex_UpperLeftB, ReducedIndex_UpperRightB, ReducedIndex_RightUpperB, ReducedIndex_RightLowerB};
	int(*ReducedIndex_C[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_LowerC, ReducedIndex_UpperC, ReducedIndex_LeftC, ReducedIndex_RightC};
	int(*ReducedIndex_D3[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_RightLowerD3, ReducedIndex_LeftUpperD3, ReducedIndex_LeftLowerD3, ReducedIndex_RightUpperD3};
	int(*ReducedIndex_D4[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_RightLowerD4, ReducedIndex_LeftUpperD4, ReducedIndex_LeftLowerD4, ReducedIndex_RightUpperD4};
	int(*ReducedIndex_D5[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_RightLowerD5, ReducedIndex_LeftUpperD5, ReducedIndex_LeftLowerD5, ReducedIndex_RightUpperD5};
	int(*ReducedIndex_D6[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_RightLowerD6, ReducedIndex_LeftUpperD6, ReducedIndex_LeftLowerD6, ReducedIndex_RightUpperD6};
	int(*ReducedIndex_D7[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_RightLowerD7, ReducedIndex_LeftUpperD7, ReducedIndex_LeftLowerD7, ReducedIndex_RightUpperD7};
	int(*ReducedIndex_L1[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_LowerL1, ReducedIndex_UpperL1, ReducedIndex_LeftL1, ReducedIndex_RightL1};
	int(*ReducedIndex_L2[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_LowerL2, ReducedIndex_UpperL2, ReducedIndex_LeftL2, ReducedIndex_RightL2};
	int(*ReducedIndex_L3[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_LowerL3, ReducedIndex_UpperL3, ReducedIndex_LeftL3, ReducedIndex_RightL3};
	int(*ReducedIndex_Sq[])(const unsigned long long P, const unsigned long long O) = {ReducedIndex_RightLowerSq, ReducedIndex_LeftUpperSq, ReducedIndex_LeftLowerSq, ReducedIndex_RightUpperSq};
	int(*FullIndex_A[])(const unsigned long long P, const unsigned long long O) = {FullIndex_RightLowerA, FullIndex_LeftUpperA, FullIndex_LeftLowerA, FullIndex_RightUpperA};
	int(*FullIndex_B[])(const unsigned long long P, const unsigned long long O) = {FullIndex_LowerRightB, FullIndex_LowerLeftB, FullIndex_LeftLowerB, FullIndex_LeftUpperB, FullIndex_UpperLeftB, FullIndex_UpperRightB, FullIndex_RightUpperB, FullIndex_RightLowerB};
	int(*FullIndex_C[])(const unsigned long long P, const unsigned long long O) = {FullIndex_LowerC, FullIndex_UpperC, FullIndex_LeftC, FullIndex_RightC};
	//int(*FullIndex_D3[])(const unsigned long long P, const unsigned long long O) = {FullIndex_RightLowerD3, FullIndex_LeftUpperD3, FullIndex_LeftLowerD3, FullIndex_RightUpperD3};
	int(*FullIndex_D4[])(const unsigned long long P, const unsigned long long O) = {FullIndex_RightLowerD4, FullIndex_LeftUpperD4, FullIndex_LeftLowerD4, FullIndex_RightUpperD4};
	int(*FullIndex_D5[])(const unsigned long long P, const unsigned long long O) = {FullIndex_RightLowerD5, FullIndex_LeftUpperD5, FullIndex_LeftLowerD5, FullIndex_RightUpperD5};
	int(*FullIndex_D6[])(const unsigned long long P, const unsigned long long O) = {FullIndex_RightLowerD6, FullIndex_LeftUpperD6, FullIndex_LeftLowerD6, FullIndex_RightUpperD6};
	int(*FullIndex_D7[])(const unsigned long long P, const unsigned long long O) = {FullIndex_RightLowerD7, FullIndex_LeftUpperD7, FullIndex_LeftLowerD7, FullIndex_RightUpperD7};
	int(*FullIndex_L1[])(const unsigned long long P, const unsigned long long O) = {FullIndex_LowerL1, FullIndex_UpperL1, FullIndex_LeftL1, FullIndex_RightL1};
	int(*FullIndex_L2[])(const unsigned long long P, const unsigned long long O) = {FullIndex_LowerL2, FullIndex_UpperL2, FullIndex_LeftL2, FullIndex_RightL2};
	int(*FullIndex_L3[])(const unsigned long long P, const unsigned long long O) = {FullIndex_LowerL3, FullIndex_UpperL3, FullIndex_LeftL3, FullIndex_RightL3};
	int(*FullIndex_Sq[])(const unsigned long long P, const unsigned long long O) = {FullIndex_RightLowerSq, FullIndex_LeftUpperSq, FullIndex_LeftLowerSq, FullIndex_RightUpperSq};

	// Reduced Index
	Success &= Test(Symmetry_A,  ReducedSize_A,  Pattern_A,  ReducedIndex_A, true);
	Success &= Test(Symmetry_B,  ReducedSize_B,  Pattern_B,  ReducedIndex_B, true);
	Success &= Test(Symmetry_C,  ReducedSize_C,  Pattern_C,  ReducedIndex_C, true);
	Success &= Test(Symmetry_D3, ReducedSize_D3, Pattern_D3, ReducedIndex_D3, true);
	Success &= Test(Symmetry_D4, ReducedSize_D4, Pattern_D4, ReducedIndex_D4, true);
	Success &= Test(Symmetry_D5, ReducedSize_D5, Pattern_D5, ReducedIndex_D5, true);
	Success &= Test(Symmetry_D6, ReducedSize_D6, Pattern_D6, ReducedIndex_D6, true);
	Success &= Test(Symmetry_D7, ReducedSize_D7, Pattern_D7, ReducedIndex_D7, false);
	Success &= Test(Symmetry_L1, ReducedSize_L1, Pattern_L1, ReducedIndex_L1, true);
	Success &= Test(Symmetry_L2, ReducedSize_L2, Pattern_L2, ReducedIndex_L2, true);
	Success &= Test(Symmetry_L3, ReducedSize_L3, Pattern_L3, ReducedIndex_L3, false);
	Success &= Test(Symmetry_Sq, ReducedSize_Sq, Pattern_Sq, ReducedIndex_Sq, true);

	//FullIndex
	FullOffset = 0;
	Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_C,  Pattern_C,  FullIndex_C , FullOffset); FullOffset += FullSize_C ;
	Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_L1, Pattern_L1, FullIndex_L1, FullOffset); FullOffset += FullSize_L1;
	Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_L2, Pattern_L2, FullIndex_L2, FullOffset); FullOffset += FullSize_L2;
	Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_L3, Pattern_L3, FullIndex_L3, FullOffset); FullOffset += FullSize_L3;
	Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_A,  Pattern_A,  FullIndex_A , FullOffset); FullOffset += FullSize_A ;
	Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_Sq, Pattern_Sq, FullIndex_Sq, FullOffset); FullOffset += FullSize_Sq;
	Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_B,  Pattern_B,  FullIndex_B , FullOffset); FullOffset += FullSize_B ;
	//Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_D3, Pattern_D3, FullIndex_D3, FullOffset); FullOffset += FullSize_D3;
	Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_D4, Pattern_D4, FullIndex_D4, FullOffset); FullOffset += FullSize_D4;
	Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_D5, Pattern_D5, FullIndex_D5, FullOffset); FullOffset += FullSize_D5;
	Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_D6, Pattern_D6, FullIndex_D6, FullOffset); FullOffset += FullSize_D6;
	Success &= Test_Feature_symmetrie_FullIndex(tmpWeights, Symmetry_D7, Pattern_D7, FullIndex_D7, FullOffset); FullOffset += FullSize_D7;
	
	delete[] tmpWeights;
	return Success;
}