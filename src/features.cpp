#include "features.h"

/**
	DiagLookUp2 =     8 Byte
	BosIndex    =    52 Byte
	PowerOf3    =   512 Byte
	PowerOf3inv =   512 Byte
	Weights     = NumberOfFiles * Size * sizeof(float) Byte
	----------------------
	              1'084 Byte
**/

namespace Features
{
	int NumberOfFiles;

	//const unsigned short DiagLookUp[4][4] = {
	//	{0xFFFF, 0xFFFF, 0xFFFF,      0},
	//	{0xFFFF, 0xFFFF,      1, 0xFFFF},
	//	{0xFFFF,      2, 0xFFFF, 0xFFFF},
	//	{     3, 0xFFFF, 0xFFFF, 0xFFFF}
	//};

	const unsigned short DiagLookUp2[4] = {0, 729, 1458, 2187};

	const unsigned char BoxIndex[52] = {
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
	};

	const unsigned short PowerOf3[256] = {
		POWER_OF_3<0x00>(), POWER_OF_3<0x01>(), POWER_OF_3<0x02>(), POWER_OF_3<0x03>(), POWER_OF_3<0x04>(), POWER_OF_3<0x05>(), POWER_OF_3<0x06>(), POWER_OF_3<0x07>(), POWER_OF_3<0x08>(), POWER_OF_3<0x09>(), POWER_OF_3<0x0A>(), POWER_OF_3<0x0B>(), POWER_OF_3<0x0C>(), POWER_OF_3<0x0D>(), POWER_OF_3<0x0E>(), POWER_OF_3<0x0F>(),
		POWER_OF_3<0x10>(), POWER_OF_3<0x11>(), POWER_OF_3<0x12>(), POWER_OF_3<0x13>(), POWER_OF_3<0x14>(), POWER_OF_3<0x15>(), POWER_OF_3<0x16>(), POWER_OF_3<0x17>(), POWER_OF_3<0x18>(), POWER_OF_3<0x19>(), POWER_OF_3<0x1A>(), POWER_OF_3<0x1B>(), POWER_OF_3<0x1C>(), POWER_OF_3<0x1D>(), POWER_OF_3<0x1E>(), POWER_OF_3<0x1F>(),
		POWER_OF_3<0x20>(), POWER_OF_3<0x21>(), POWER_OF_3<0x22>(), POWER_OF_3<0x23>(), POWER_OF_3<0x24>(), POWER_OF_3<0x25>(), POWER_OF_3<0x26>(), POWER_OF_3<0x27>(), POWER_OF_3<0x28>(), POWER_OF_3<0x29>(), POWER_OF_3<0x2A>(), POWER_OF_3<0x2B>(), POWER_OF_3<0x2C>(), POWER_OF_3<0x2D>(), POWER_OF_3<0x2E>(), POWER_OF_3<0x2F>(),
		POWER_OF_3<0x30>(), POWER_OF_3<0x31>(), POWER_OF_3<0x32>(), POWER_OF_3<0x33>(), POWER_OF_3<0x34>(), POWER_OF_3<0x35>(), POWER_OF_3<0x36>(), POWER_OF_3<0x37>(), POWER_OF_3<0x38>(), POWER_OF_3<0x39>(), POWER_OF_3<0x3A>(), POWER_OF_3<0x3B>(), POWER_OF_3<0x3C>(), POWER_OF_3<0x3D>(), POWER_OF_3<0x3E>(), POWER_OF_3<0x3F>(),
		POWER_OF_3<0x40>(), POWER_OF_3<0x41>(), POWER_OF_3<0x42>(), POWER_OF_3<0x43>(), POWER_OF_3<0x44>(), POWER_OF_3<0x45>(), POWER_OF_3<0x46>(), POWER_OF_3<0x47>(), POWER_OF_3<0x48>(), POWER_OF_3<0x49>(), POWER_OF_3<0x4A>(), POWER_OF_3<0x4B>(), POWER_OF_3<0x4C>(), POWER_OF_3<0x4D>(), POWER_OF_3<0x4E>(), POWER_OF_3<0x4F>(),
		POWER_OF_3<0x50>(), POWER_OF_3<0x51>(), POWER_OF_3<0x52>(), POWER_OF_3<0x53>(), POWER_OF_3<0x54>(), POWER_OF_3<0x55>(), POWER_OF_3<0x56>(), POWER_OF_3<0x57>(), POWER_OF_3<0x58>(), POWER_OF_3<0x59>(), POWER_OF_3<0x5A>(), POWER_OF_3<0x5B>(), POWER_OF_3<0x5C>(), POWER_OF_3<0x5D>(), POWER_OF_3<0x5E>(), POWER_OF_3<0x5F>(),
		POWER_OF_3<0x60>(), POWER_OF_3<0x61>(), POWER_OF_3<0x62>(), POWER_OF_3<0x63>(), POWER_OF_3<0x64>(), POWER_OF_3<0x65>(), POWER_OF_3<0x66>(), POWER_OF_3<0x67>(), POWER_OF_3<0x68>(), POWER_OF_3<0x69>(), POWER_OF_3<0x6A>(), POWER_OF_3<0x6B>(), POWER_OF_3<0x6C>(), POWER_OF_3<0x6D>(), POWER_OF_3<0x6E>(), POWER_OF_3<0x6F>(),
		POWER_OF_3<0x70>(), POWER_OF_3<0x71>(), POWER_OF_3<0x72>(), POWER_OF_3<0x73>(), POWER_OF_3<0x74>(), POWER_OF_3<0x75>(), POWER_OF_3<0x76>(), POWER_OF_3<0x77>(), POWER_OF_3<0x78>(), POWER_OF_3<0x79>(), POWER_OF_3<0x7A>(), POWER_OF_3<0x7B>(), POWER_OF_3<0x7C>(), POWER_OF_3<0x7D>(), POWER_OF_3<0x7E>(), POWER_OF_3<0x7F>(),
		POWER_OF_3<0x80>(), POWER_OF_3<0x81>(), POWER_OF_3<0x82>(), POWER_OF_3<0x83>(), POWER_OF_3<0x84>(), POWER_OF_3<0x85>(), POWER_OF_3<0x86>(), POWER_OF_3<0x87>(), POWER_OF_3<0x88>(), POWER_OF_3<0x89>(), POWER_OF_3<0x8A>(), POWER_OF_3<0x8B>(), POWER_OF_3<0x8C>(), POWER_OF_3<0x8D>(), POWER_OF_3<0x8E>(), POWER_OF_3<0x8F>(),
		POWER_OF_3<0x90>(), POWER_OF_3<0x91>(), POWER_OF_3<0x92>(), POWER_OF_3<0x93>(), POWER_OF_3<0x94>(), POWER_OF_3<0x95>(), POWER_OF_3<0x96>(), POWER_OF_3<0x97>(), POWER_OF_3<0x98>(), POWER_OF_3<0x99>(), POWER_OF_3<0x9A>(), POWER_OF_3<0x9B>(), POWER_OF_3<0x9C>(), POWER_OF_3<0x9D>(), POWER_OF_3<0x9E>(), POWER_OF_3<0x9F>(),
		POWER_OF_3<0xA0>(), POWER_OF_3<0xA1>(), POWER_OF_3<0xA2>(), POWER_OF_3<0xA3>(), POWER_OF_3<0xA4>(), POWER_OF_3<0xA5>(), POWER_OF_3<0xA6>(), POWER_OF_3<0xA7>(), POWER_OF_3<0xA8>(), POWER_OF_3<0xA9>(), POWER_OF_3<0xAA>(), POWER_OF_3<0xAB>(), POWER_OF_3<0xAC>(), POWER_OF_3<0xAD>(), POWER_OF_3<0xAE>(), POWER_OF_3<0xAF>(),
		POWER_OF_3<0xB0>(), POWER_OF_3<0xB1>(), POWER_OF_3<0xB2>(), POWER_OF_3<0xB3>(), POWER_OF_3<0xB4>(), POWER_OF_3<0xB5>(), POWER_OF_3<0xB6>(), POWER_OF_3<0xB7>(), POWER_OF_3<0xB8>(), POWER_OF_3<0xB9>(), POWER_OF_3<0xBA>(), POWER_OF_3<0xBB>(), POWER_OF_3<0xBC>(), POWER_OF_3<0xBD>(), POWER_OF_3<0xBE>(), POWER_OF_3<0xBF>(),
		POWER_OF_3<0xC0>(), POWER_OF_3<0xC1>(), POWER_OF_3<0xC2>(), POWER_OF_3<0xC3>(), POWER_OF_3<0xC4>(), POWER_OF_3<0xC5>(), POWER_OF_3<0xC6>(), POWER_OF_3<0xC7>(), POWER_OF_3<0xC8>(), POWER_OF_3<0xC9>(), POWER_OF_3<0xCA>(), POWER_OF_3<0xCB>(), POWER_OF_3<0xCC>(), POWER_OF_3<0xCD>(), POWER_OF_3<0xCE>(), POWER_OF_3<0xCF>(),
		POWER_OF_3<0xD0>(), POWER_OF_3<0xD1>(), POWER_OF_3<0xD2>(), POWER_OF_3<0xD3>(), POWER_OF_3<0xD4>(), POWER_OF_3<0xD5>(), POWER_OF_3<0xD6>(), POWER_OF_3<0xD7>(), POWER_OF_3<0xD8>(), POWER_OF_3<0xD9>(), POWER_OF_3<0xDA>(), POWER_OF_3<0xDB>(), POWER_OF_3<0xDC>(), POWER_OF_3<0xDD>(), POWER_OF_3<0xDE>(), POWER_OF_3<0xDF>(),
		POWER_OF_3<0xE0>(), POWER_OF_3<0xE1>(), POWER_OF_3<0xE2>(), POWER_OF_3<0xE3>(), POWER_OF_3<0xE4>(), POWER_OF_3<0xE5>(), POWER_OF_3<0xE6>(), POWER_OF_3<0xE7>(), POWER_OF_3<0xE8>(), POWER_OF_3<0xE9>(), POWER_OF_3<0xEA>(), POWER_OF_3<0xEB>(), POWER_OF_3<0xEC>(), POWER_OF_3<0xED>(), POWER_OF_3<0xEE>(), POWER_OF_3<0xEF>(),
		POWER_OF_3<0xF0>(), POWER_OF_3<0xF1>(), POWER_OF_3<0xF2>(), POWER_OF_3<0xF3>(), POWER_OF_3<0xF4>(), POWER_OF_3<0xF5>(), POWER_OF_3<0xF6>(), POWER_OF_3<0xF7>(), POWER_OF_3<0xF8>(), POWER_OF_3<0xF9>(), POWER_OF_3<0xFA>(), POWER_OF_3<0xFB>(), POWER_OF_3<0xFC>(), POWER_OF_3<0xFD>(), POWER_OF_3<0xFE>(), POWER_OF_3<0xFF>()
	};

	const unsigned short PowerOf3inv[256] = {
		POWER_OF_3_INV<0x00>(), POWER_OF_3_INV<0x01>(), POWER_OF_3_INV<0x02>(), POWER_OF_3_INV<0x03>(), POWER_OF_3_INV<0x04>(), POWER_OF_3_INV<0x05>(), POWER_OF_3_INV<0x06>(), POWER_OF_3_INV<0x07>(), POWER_OF_3_INV<0x08>(), POWER_OF_3_INV<0x09>(), POWER_OF_3_INV<0x0A>(), POWER_OF_3_INV<0x0B>(), POWER_OF_3_INV<0x0C>(), POWER_OF_3_INV<0x0D>(), POWER_OF_3_INV<0x0E>(), POWER_OF_3_INV<0x0F>(),
		POWER_OF_3_INV<0x10>(), POWER_OF_3_INV<0x11>(), POWER_OF_3_INV<0x12>(), POWER_OF_3_INV<0x13>(), POWER_OF_3_INV<0x14>(), POWER_OF_3_INV<0x15>(), POWER_OF_3_INV<0x16>(), POWER_OF_3_INV<0x17>(), POWER_OF_3_INV<0x18>(), POWER_OF_3_INV<0x19>(), POWER_OF_3_INV<0x1A>(), POWER_OF_3_INV<0x1B>(), POWER_OF_3_INV<0x1C>(), POWER_OF_3_INV<0x1D>(), POWER_OF_3_INV<0x1E>(), POWER_OF_3_INV<0x1F>(),
		POWER_OF_3_INV<0x20>(), POWER_OF_3_INV<0x21>(), POWER_OF_3_INV<0x22>(), POWER_OF_3_INV<0x23>(), POWER_OF_3_INV<0x24>(), POWER_OF_3_INV<0x25>(), POWER_OF_3_INV<0x26>(), POWER_OF_3_INV<0x27>(), POWER_OF_3_INV<0x28>(), POWER_OF_3_INV<0x29>(), POWER_OF_3_INV<0x2A>(), POWER_OF_3_INV<0x2B>(), POWER_OF_3_INV<0x2C>(), POWER_OF_3_INV<0x2D>(), POWER_OF_3_INV<0x2E>(), POWER_OF_3_INV<0x2F>(),
		POWER_OF_3_INV<0x30>(), POWER_OF_3_INV<0x31>(), POWER_OF_3_INV<0x32>(), POWER_OF_3_INV<0x33>(), POWER_OF_3_INV<0x34>(), POWER_OF_3_INV<0x35>(), POWER_OF_3_INV<0x36>(), POWER_OF_3_INV<0x37>(), POWER_OF_3_INV<0x38>(), POWER_OF_3_INV<0x39>(), POWER_OF_3_INV<0x3A>(), POWER_OF_3_INV<0x3B>(), POWER_OF_3_INV<0x3C>(), POWER_OF_3_INV<0x3D>(), POWER_OF_3_INV<0x3E>(), POWER_OF_3_INV<0x3F>(),
		POWER_OF_3_INV<0x40>(), POWER_OF_3_INV<0x41>(), POWER_OF_3_INV<0x42>(), POWER_OF_3_INV<0x43>(), POWER_OF_3_INV<0x44>(), POWER_OF_3_INV<0x45>(), POWER_OF_3_INV<0x46>(), POWER_OF_3_INV<0x47>(), POWER_OF_3_INV<0x48>(), POWER_OF_3_INV<0x49>(), POWER_OF_3_INV<0x4A>(), POWER_OF_3_INV<0x4B>(), POWER_OF_3_INV<0x4C>(), POWER_OF_3_INV<0x4D>(), POWER_OF_3_INV<0x4E>(), POWER_OF_3_INV<0x4F>(),
		POWER_OF_3_INV<0x50>(), POWER_OF_3_INV<0x51>(), POWER_OF_3_INV<0x52>(), POWER_OF_3_INV<0x53>(), POWER_OF_3_INV<0x54>(), POWER_OF_3_INV<0x55>(), POWER_OF_3_INV<0x56>(), POWER_OF_3_INV<0x57>(), POWER_OF_3_INV<0x58>(), POWER_OF_3_INV<0x59>(), POWER_OF_3_INV<0x5A>(), POWER_OF_3_INV<0x5B>(), POWER_OF_3_INV<0x5C>(), POWER_OF_3_INV<0x5D>(), POWER_OF_3_INV<0x5E>(), POWER_OF_3_INV<0x5F>(),
		POWER_OF_3_INV<0x60>(), POWER_OF_3_INV<0x61>(), POWER_OF_3_INV<0x62>(), POWER_OF_3_INV<0x63>(), POWER_OF_3_INV<0x64>(), POWER_OF_3_INV<0x65>(), POWER_OF_3_INV<0x66>(), POWER_OF_3_INV<0x67>(), POWER_OF_3_INV<0x68>(), POWER_OF_3_INV<0x69>(), POWER_OF_3_INV<0x6A>(), POWER_OF_3_INV<0x6B>(), POWER_OF_3_INV<0x6C>(), POWER_OF_3_INV<0x6D>(), POWER_OF_3_INV<0x6E>(), POWER_OF_3_INV<0x6F>(),
		POWER_OF_3_INV<0x70>(), POWER_OF_3_INV<0x71>(), POWER_OF_3_INV<0x72>(), POWER_OF_3_INV<0x73>(), POWER_OF_3_INV<0x74>(), POWER_OF_3_INV<0x75>(), POWER_OF_3_INV<0x76>(), POWER_OF_3_INV<0x77>(), POWER_OF_3_INV<0x78>(), POWER_OF_3_INV<0x79>(), POWER_OF_3_INV<0x7A>(), POWER_OF_3_INV<0x7B>(), POWER_OF_3_INV<0x7C>(), POWER_OF_3_INV<0x7D>(), POWER_OF_3_INV<0x7E>(), POWER_OF_3_INV<0x7F>(),
		POWER_OF_3_INV<0x80>(), POWER_OF_3_INV<0x81>(), POWER_OF_3_INV<0x82>(), POWER_OF_3_INV<0x83>(), POWER_OF_3_INV<0x84>(), POWER_OF_3_INV<0x85>(), POWER_OF_3_INV<0x86>(), POWER_OF_3_INV<0x87>(), POWER_OF_3_INV<0x88>(), POWER_OF_3_INV<0x89>(), POWER_OF_3_INV<0x8A>(), POWER_OF_3_INV<0x8B>(), POWER_OF_3_INV<0x8C>(), POWER_OF_3_INV<0x8D>(), POWER_OF_3_INV<0x8E>(), POWER_OF_3_INV<0x8F>(),
		POWER_OF_3_INV<0x90>(), POWER_OF_3_INV<0x91>(), POWER_OF_3_INV<0x92>(), POWER_OF_3_INV<0x93>(), POWER_OF_3_INV<0x94>(), POWER_OF_3_INV<0x95>(), POWER_OF_3_INV<0x96>(), POWER_OF_3_INV<0x97>(), POWER_OF_3_INV<0x98>(), POWER_OF_3_INV<0x99>(), POWER_OF_3_INV<0x9A>(), POWER_OF_3_INV<0x9B>(), POWER_OF_3_INV<0x9C>(), POWER_OF_3_INV<0x9D>(), POWER_OF_3_INV<0x9E>(), POWER_OF_3_INV<0x9F>(),
		POWER_OF_3_INV<0xA0>(), POWER_OF_3_INV<0xA1>(), POWER_OF_3_INV<0xA2>(), POWER_OF_3_INV<0xA3>(), POWER_OF_3_INV<0xA4>(), POWER_OF_3_INV<0xA5>(), POWER_OF_3_INV<0xA6>(), POWER_OF_3_INV<0xA7>(), POWER_OF_3_INV<0xA8>(), POWER_OF_3_INV<0xA9>(), POWER_OF_3_INV<0xAA>(), POWER_OF_3_INV<0xAB>(), POWER_OF_3_INV<0xAC>(), POWER_OF_3_INV<0xAD>(), POWER_OF_3_INV<0xAE>(), POWER_OF_3_INV<0xAF>(),
		POWER_OF_3_INV<0xB0>(), POWER_OF_3_INV<0xB1>(), POWER_OF_3_INV<0xB2>(), POWER_OF_3_INV<0xB3>(), POWER_OF_3_INV<0xB4>(), POWER_OF_3_INV<0xB5>(), POWER_OF_3_INV<0xB6>(), POWER_OF_3_INV<0xB7>(), POWER_OF_3_INV<0xB8>(), POWER_OF_3_INV<0xB9>(), POWER_OF_3_INV<0xBA>(), POWER_OF_3_INV<0xBB>(), POWER_OF_3_INV<0xBC>(), POWER_OF_3_INV<0xBD>(), POWER_OF_3_INV<0xBE>(), POWER_OF_3_INV<0xBF>(),
		POWER_OF_3_INV<0xC0>(), POWER_OF_3_INV<0xC1>(), POWER_OF_3_INV<0xC2>(), POWER_OF_3_INV<0xC3>(), POWER_OF_3_INV<0xC4>(), POWER_OF_3_INV<0xC5>(), POWER_OF_3_INV<0xC6>(), POWER_OF_3_INV<0xC7>(), POWER_OF_3_INV<0xC8>(), POWER_OF_3_INV<0xC9>(), POWER_OF_3_INV<0xCA>(), POWER_OF_3_INV<0xCB>(), POWER_OF_3_INV<0xCC>(), POWER_OF_3_INV<0xCD>(), POWER_OF_3_INV<0xCE>(), POWER_OF_3_INV<0xCF>(),
		POWER_OF_3_INV<0xD0>(), POWER_OF_3_INV<0xD1>(), POWER_OF_3_INV<0xD2>(), POWER_OF_3_INV<0xD3>(), POWER_OF_3_INV<0xD4>(), POWER_OF_3_INV<0xD5>(), POWER_OF_3_INV<0xD6>(), POWER_OF_3_INV<0xD7>(), POWER_OF_3_INV<0xD8>(), POWER_OF_3_INV<0xD9>(), POWER_OF_3_INV<0xDA>(), POWER_OF_3_INV<0xDB>(), POWER_OF_3_INV<0xDC>(), POWER_OF_3_INV<0xDD>(), POWER_OF_3_INV<0xDE>(), POWER_OF_3_INV<0xDF>(),
		POWER_OF_3_INV<0xE0>(), POWER_OF_3_INV<0xE1>(), POWER_OF_3_INV<0xE2>(), POWER_OF_3_INV<0xE3>(), POWER_OF_3_INV<0xE4>(), POWER_OF_3_INV<0xE5>(), POWER_OF_3_INV<0xE6>(), POWER_OF_3_INV<0xE7>(), POWER_OF_3_INV<0xE8>(), POWER_OF_3_INV<0xE9>(), POWER_OF_3_INV<0xEA>(), POWER_OF_3_INV<0xEB>(), POWER_OF_3_INV<0xEC>(), POWER_OF_3_INV<0xED>(), POWER_OF_3_INV<0xEE>(), POWER_OF_3_INV<0xEF>(),
		POWER_OF_3_INV<0xF0>(), POWER_OF_3_INV<0xF1>(), POWER_OF_3_INV<0xF2>(), POWER_OF_3_INV<0xF3>(), POWER_OF_3_INV<0xF4>(), POWER_OF_3_INV<0xF5>(), POWER_OF_3_INV<0xF6>(), POWER_OF_3_INV<0xF7>(), POWER_OF_3_INV<0xF8>(), POWER_OF_3_INV<0xF9>(), POWER_OF_3_INV<0xFA>(), POWER_OF_3_INV<0xFB>(), POWER_OF_3_INV<0xFC>(), POWER_OF_3_INV<0xFD>(), POWER_OF_3_INV<0xFE>(), POWER_OF_3_INV<0xFF>()
	};

	float (*Weights)[Size];

	const unsigned long long Pattern_LowerC = 0x00000000000042FFULL;
	const unsigned long long Pattern_UpperC = 0xFF42000000000000ULL;
	const unsigned long long Pattern_LeftC = 0x80C080808080C080ULL;
	const unsigned long long Pattern_RightC = 0x0103010101010301ULL;
	const unsigned long long Pattern_LowerL1 = 0x000000000000FF00ULL;
	const unsigned long long Pattern_UpperL1 = 0x00FF000000000000ULL;
	const unsigned long long Pattern_LeftL1 = 0x4040404040404040ULL;
	const unsigned long long Pattern_RightL1 = 0x0202020202020202ULL;
	const unsigned long long Pattern_LowerL2 = 0x0000000000FF0000ULL;
	const unsigned long long Pattern_UpperL2 = 0x0000FF0000000000ULL;
	const unsigned long long Pattern_LeftL2 = 0x2020202020202020ULL;
	const unsigned long long Pattern_RightL2 = 0x0404040404040404ULL;
	const unsigned long long Pattern_LowerL3 = 0x00000000FF000000ULL;
	const unsigned long long Pattern_UpperL3 = 0x000000FF00000000ULL;
	const unsigned long long Pattern_LeftL3 = 0x1010101010101010ULL;
	const unsigned long long Pattern_RightL3 = 0x0808080808080808ULL;
	const unsigned long long Pattern_RightLowerA = 0x8040201008050307ULL;
	const unsigned long long Pattern_LeftUpperA = 0xE0C0A01008040201ULL;
	const unsigned long long Pattern_LeftLowerA = 0x0102040810A0C0E0ULL;
	const unsigned long long Pattern_RightUpperA = 0x0703050810204080ULL;
	const unsigned long long Pattern_RightLowerSq = 0x0000000000070707ULL;
	const unsigned long long Pattern_LeftUpperSq = 0xE0E0E00000000000ULL;
	const unsigned long long Pattern_LeftLowerSq = 0x0000000000E0E0E0ULL;
	const unsigned long long Pattern_RightUpperSq = 0x0707070000000000ULL;
	const unsigned long long Pattern_LowerRightB = 0x0000000000001F1FULL;
	const unsigned long long Pattern_LowerLeftB = 0x000000000000F8F8ULL;
	const unsigned long long Pattern_LeftLowerB = 0x000000C0C0C0C0C0ULL;
	const unsigned long long Pattern_LeftUpperB = 0xC0C0C0C0C0000000ULL;
	const unsigned long long Pattern_UpperLeftB = 0xF8F8000000000000ULL;
	const unsigned long long Pattern_UpperRightB = 0x1F1F000000000000ULL;
	const unsigned long long Pattern_RightUpperB = 0x0303030303000000ULL;
	const unsigned long long Pattern_RightLowerB = 0x0000000303030303ULL;
	const unsigned long long Pattern_RightLowerD3 = 0x0000000000010204ULL;
	const unsigned long long Pattern_LeftUpperD3 = 0x2040800000000000ULL;
	const unsigned long long Pattern_LeftLowerD3 = 0x0000000000804020ULL;
	const unsigned long long Pattern_RightUpperD3 = 0x0402010000000000ULL;
	const unsigned long long Pattern_RightLowerD4 = 0x0000000001020408ULL;
	const unsigned long long Pattern_LeftUpperD4 = 0x1020408000000000ULL;
	const unsigned long long Pattern_LeftLowerD4 = 0x0000000080402010ULL;
	const unsigned long long Pattern_RightUpperD4 = 0x0804020100000000ULL;
	const unsigned long long Pattern_RightLowerD5 = 0x0000000102040810ULL;
	const unsigned long long Pattern_LeftUpperD5 = 0x0810204080000000ULL;
	const unsigned long long Pattern_LeftLowerD5 = 0x0000008040201008ULL;
	const unsigned long long Pattern_RightUpperD5 = 0x1008040201000000ULL;
	const unsigned long long Pattern_RightLowerD6 = 0x0000010204081020ULL;
	const unsigned long long Pattern_LeftUpperD6 = 0x0408102040800000ULL;
	const unsigned long long Pattern_LeftLowerD6 = 0x0000804020100804ULL;
	const unsigned long long Pattern_RightUpperD6 = 0x2010080402010000ULL;
	const unsigned long long Pattern_RightLowerD7 = 0x0001020408102040ULL;
	const unsigned long long Pattern_LeftUpperD7 = 0x0204081020408000ULL;
	const unsigned long long Pattern_LeftLowerD7 = 0x0080402010080402ULL;
	const unsigned long long Pattern_RightUpperD7 = 0x4020100804020100ULL;
	const unsigned long long Pattern_CenterX = 0x0000241818240000ULL;

	int Feature_LowerC(const unsigned long long P, const unsigned long long O)
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
	int Feature_UpperC(const unsigned long long P, const unsigned long long O)
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
	int Feature_LeftC(const unsigned long long P, const unsigned long long O)
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
	int Feature_RightC(const unsigned long long P, const unsigned long long O)
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
	int Feature_LowerL1(const unsigned long long P, const unsigned long long O)
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
	int Feature_LowerL2(const unsigned long long P, const unsigned long long O)
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
	int Feature_LowerL3(const unsigned long long P, const unsigned long long O)
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
	int Feature_UpperL3(const unsigned long long P, const unsigned long long O)
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
	int Feature_UpperL2(const unsigned long long P, const unsigned long long O)
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
	int Feature_UpperL1(const unsigned long long P, const unsigned long long O)
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
	int Feature_LeftL1(const unsigned long long P, const unsigned long long O)
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
	int Feature_LeftL2(const unsigned long long P, const unsigned long long O)
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
	int Feature_LeftL3(const unsigned long long P, const unsigned long long O)
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
	int Feature_RightL3(const unsigned long long P, const unsigned long long O)
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
	int Feature_RightL2(const unsigned long long P, const unsigned long long O)
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
	int Feature_RightL1(const unsigned long long P, const unsigned long long O)
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
	int Feature_RightLowerA(const unsigned long long P, const unsigned long long O)
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
	int Feature_LeftUpperA(const unsigned long long P, const unsigned long long O)
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
	int Feature_LeftLowerA(const unsigned long long P, const unsigned long long O)
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
	int Feature_RightUpperA(const unsigned long long P, const unsigned long long O)
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
	int Feature_RightLowerSq(const unsigned long long P, const unsigned long long O)
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
	int Feature_LeftUpperSq(const unsigned long long P, const unsigned long long O)
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
	int Feature_LeftLowerSq(const unsigned long long P, const unsigned long long O)
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
	int Feature_RightUpperSq(const unsigned long long P, const unsigned long long O)
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
	int Feature_LowerRightB(const unsigned long long P, const unsigned long long O)
	{
		return PowerOf3[P & 0x000000000000001FULL] * 486
			 + PowerOf3[O & 0x000000000000001FULL] * 243
			 + PowerOf3[(P & 0x0000000000001F00ULL) >> 8] * 2
			 + PowerOf3[(O & 0x0000000000001F00ULL) >> 8];
	}
	int Feature_LowerLeftB(const unsigned long long P, const unsigned long long O)
	{
		return PowerOf3inv[P & 0x00000000000000F8ULL] * 486
			 + PowerOf3inv[O & 0x00000000000000F8ULL] * 243
			 + PowerOf3inv[(P & 0x000000000000F800ULL) >> 8] * 2
			 + PowerOf3inv[(O & 0x000000000000F800ULL) >> 8];
	}
	int Feature_LeftLowerB(const unsigned long long P, const unsigned long long O)
	{
		return PowerOf3[((P & 0x0000008080808080ULL) * 0x0010204081000000ULL) >> 59] * 486
			 + PowerOf3[((O & 0x0000008080808080ULL) * 0x0010204081000000ULL) >> 59] * 243
			 + PowerOf3[((P & 0x0000004040404040ULL) * 0x0020408102000000ULL) >> 59] * 2
			 + PowerOf3[((O & 0x0000004040404040ULL) * 0x0020408102000000ULL) >> 59];
	}
	int Feature_LeftUpperB(const unsigned long long P, const unsigned long long O)
	{
		return PowerOf3inv[((P & 0x8080808080000000ULL) * 0x0000000010204081ULL) >> 56] * 486
			 + PowerOf3inv[((O & 0x8080808080000000ULL) * 0x0000000010204081ULL) >> 56] * 243
			 + PowerOf3inv[((P & 0x4040404040000000ULL) * 0x0000000010204081ULL) >> 55] * 2
			 + PowerOf3inv[((O & 0x4040404040000000ULL) * 0x0000000010204081ULL) >> 55];
	}
	int Feature_UpperLeftB(const unsigned long long P, const unsigned long long O)
	{
		return PowerOf3inv[(P & 0xF800000000000000ULL) >> 56] * 486
			 + PowerOf3inv[(O & 0xF800000000000000ULL) >> 56] * 243
			 + PowerOf3inv[(P & 0x00F8000000000000ULL) >> 48] * 2
			 + PowerOf3inv[(O & 0x00F8000000000000ULL) >> 48];
	}
	int Feature_UpperRightB(const unsigned long long P, const unsigned long long O)
	{
		return PowerOf3[(P & 0x1F00000000000000ULL) >> 56] * 486
			 + PowerOf3[(O & 0x1F00000000000000ULL) >> 56] * 243
			 + PowerOf3[(P & 0x001F000000000000ULL) >> 48] * 2
			 + PowerOf3[(O & 0x001F000000000000ULL) >> 48];
	}
	int Feature_RightUpperB(const unsigned long long P, const unsigned long long O)
	{
		return PowerOf3[((P & 0x0101010101000000ULL) * 0x0000001008040201ULL) >> 56] * 486
			 + PowerOf3[((O & 0x0101010101000000ULL) * 0x0000001008040201ULL) >> 56] * 243
			 + PowerOf3[((P & 0x0202020202000000ULL) * 0x0000001008040201ULL) >> 57] * 2
			 + PowerOf3[((O & 0x0202020202000000ULL) * 0x0000001008040201ULL) >> 57];
	}
	int Feature_RightLowerB(const unsigned long long P, const unsigned long long O)
	{
		return PowerOf3inv[((P & 0x0000000101010101ULL) * 0x8040201008000000ULL) >> 56] * 486
			 + PowerOf3inv[((O & 0x0000000101010101ULL) * 0x8040201008000000ULL) >> 56] * 243
			 + PowerOf3inv[((P & 0x0000000202020202ULL) * 0x4020100804000000ULL) >> 56] * 2
			 + PowerOf3inv[((O & 0x0000000202020202ULL) * 0x4020100804000000ULL) >> 56];
	}
	int Feature_RightLowerD3(const unsigned long long P, const unsigned long long O)
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
	int Feature_LeftUpperD3(const unsigned long long P, const unsigned long long O)
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
	int Feature_LeftLowerD3(const unsigned long long P, const unsigned long long O)
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
	int Feature_RightUpperD3(const unsigned long long P, const unsigned long long O)
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
	int Feature_RightLowerD4(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[((P & 0x0000000000000408ULL) * 0x0101010101010101ULL) >> 58] * 2
			 + PowerOf3[((O & 0x0000000000000408ULL) * 0x0101010101010101ULL) >> 58];
		sym2 = PowerOf3inv[((P & 0x0000000001020000ULL) * 0x0101010101010101ULL) >> 50] * 2
			 + PowerOf3inv[((O & 0x0000000001020000ULL) * 0x0101010101010101ULL) >> 50];
		return sym1 > sym2 ? 9*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 9*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int Feature_LeftUpperD4(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[((P & 0x0000408000000000ULL) * 0x0101010101010101ULL) >> 62] * 2
			 + PowerOf3[((O & 0x0000408000000000ULL) * 0x0101010101010101ULL) >> 62];
		sym2 = PowerOf3inv[((P & 0x1020000000000000ULL) * 0x0101010101010101ULL) >> 54] * 2
			 + PowerOf3inv[((O & 0x1020000000000000ULL) * 0x0101010101010101ULL) >> 54];
		return sym1 > sym2 ? 9*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 9*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int Feature_LeftLowerD4(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[((P & 0x0000000080400000ULL) * 0x0101010101010101ULL) >> 62] * 2
			 + PowerOf3[((O & 0x0000000080400000ULL) * 0x0101010101010101ULL) >> 62];
		sym2 = PowerOf3inv[((P & 0x0000000000002010ULL) * 0x0101010101010101ULL) >> 54] * 2
			 + PowerOf3inv[((O & 0x0000000000002010ULL) * 0x0101010101010101ULL) >> 54];
		return sym1 > sym2 ? 9*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 9*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int Feature_RightUpperD4(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[((P & 0x0804000000000000ULL) * 0x0101010101010101ULL) >> 58] * 2
			 + PowerOf3[((O & 0x0804000000000000ULL) * 0x0101010101010101ULL) >> 58];
		sym2 = PowerOf3inv[((P & 0x0000020100000000ULL) * 0x0101010101010101ULL) >> 50] * 2
			 + PowerOf3inv[((O & 0x0000020100000000ULL) * 0x0101010101010101ULL) >> 50];
		return sym1 > sym2 ? 9*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 9*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int Feature_RightLowerD5(const unsigned long long P, const unsigned long long O)
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
	int Feature_LeftUpperD5(const unsigned long long P, const unsigned long long O)
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
	int Feature_LeftLowerD5(const unsigned long long P, const unsigned long long O)
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
	int Feature_RightUpperD5(const unsigned long long P, const unsigned long long O)
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
	int Feature_RightLowerD6(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[((P & 0x0000000000081020ULL) * 0x0101010101010101ULL) >> 59] * 2
			 + PowerOf3[((O & 0x0000000000081020ULL) * 0x0101010101010101ULL) >> 59];
		sym2 = PowerOf3inv[((P & 0x0000010204000000ULL) * 0x0101010101010101ULL) >> 51] * 2
			 + PowerOf3inv[((O & 0x0000010204000000ULL) * 0x0101010101010101ULL) >> 51];
		return sym1 > sym2 ? 27*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 27*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int Feature_LeftUpperD6(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[((P & 0x0000002040800000ULL) * 0x0101010101010101ULL) >> 61] * 2
			 + PowerOf3[((O & 0x0000002040800000ULL) * 0x0101010101010101ULL) >> 61];
		sym2 = PowerOf3inv[((P & 0x0408100000000000ULL) * 0x0101010101010101ULL) >> 53] * 2
			 + PowerOf3inv[((O & 0x0408100000000000ULL) * 0x0101010101010101ULL) >> 53];
		return sym1 > sym2 ? 27*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 27*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int Feature_LeftLowerD6(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[((P & 0x0000804020000000ULL) * 0x0101010101010101ULL) >> 61] * 2
			 + PowerOf3[((O & 0x0000804020000000ULL) * 0x0101010101010101ULL) >> 61];
		sym2 = PowerOf3inv[((P & 0x0000000000100804ULL) * 0x0101010101010101ULL) >> 53] * 2
			 + PowerOf3inv[((O & 0x0000000000100804ULL) * 0x0101010101010101ULL) >> 53];
		return sym1 > sym2 ? 27*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 27*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int Feature_RightUpperD6(const unsigned long long P, const unsigned long long O)
	{
		int sym1, sym2;
		sym1 = PowerOf3[((P & 0x2010080000000000ULL) * 0x0101010101010101ULL) >> 59] * 2
			 + PowerOf3[((O & 0x2010080000000000ULL) * 0x0101010101010101ULL) >> 59];
		sym2 = PowerOf3inv[((P & 0x0000000402010000ULL) * 0x0101010101010101ULL) >> 51] * 2
			 + PowerOf3inv[((O & 0x0000000402010000ULL) * 0x0101010101010101ULL) >> 51];
		return sym1 > sym2 ? 27*sym2+sym1 - (((sym2+1)*sym2) >> 1) : 27*sym1+sym2 - (((sym1+1)*sym1) >> 1);
	}
	int Feature_RightLowerD7(const unsigned long long P, const unsigned long long O)
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
	int Feature_LeftUpperD7(const unsigned long long P, const unsigned long long O)
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
	int Feature_LeftLowerD7(const unsigned long long P, const unsigned long long O)
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
	int Feature_RightUpperD7(const unsigned long long P, const unsigned long long O)
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
	int Feature_CenterX(const unsigned long long P, const unsigned long long O)
	{
		return PowerOf3[((P & 0x0000240000240000ULL) * 0x00000A0000040000ULL) >> 60] * 32
			 + PowerOf3[((O & 0x0000240000240000ULL) * 0x00000A0000040000ULL) >> 60] * 16
			 + (((P & 0x0000001818000000ULL) * 0x0000000802000000ULL) >> 60);
	}

	void Initialize()
	{
		std::vector<std::string> Filenames;
		std::string s;
		for (int i = 0; i < 15; ++i)
		{
			s = "weights range " + std::to_string(i);
			if (ConfigFile::Configurations.count(s))
				Filenames.push_back(ConfigFile::Configurations[s]);
		}

		NumberOfFiles = Filenames.size();
		Weights = new float[NumberOfFiles][Size]();
		std::vector<double> weights;
		for (int j = 0; j < NumberOfFiles; ++j)
		{
			read_vector(Filenames[j], weights);
			for (int i = 0; i < Size; ++i)
			{
				//if (std::abs(weights[i]) >= 0.01f)
					Weights[j][i] = static_cast<float>(weights[i]);
				//else if (j-1 >= 0)
				//	Weights[j][i] = Weights[j-1][i];
			}
			weights.clear();
		}

	}

	void Finalize()
	{
		delete[] Weights;
	}
}

void FillConfigurationArray(const unsigned long long P, const unsigned long long O, int* const Array)
{
	using namespace Features;
	unsigned long long BitBoardPossible;
	int Index = 0;
	int Offset = 0;

	if (Feature_C){
		Array[Index++] = Feature_LowerC(P, O);
		Array[Index++] = Feature_UpperC(P, O);
		Array[Index++] = Feature_LeftC(P, O);
		Array[Index++] = Feature_RightC(P, O);
		Offset += Size_C;
	}
	if (Feature_L1){
		Array[Index++] = Offset + Feature_LowerL1(P, O);
		Array[Index++] = Offset + Feature_UpperL1(P, O);
		Array[Index++] = Offset + Feature_LeftL1(P, O);
		Array[Index++] = Offset + Feature_RightL1(P, O);
		Offset += Size_L1;
	}
	if (Feature_L2){
		Array[Index++] = Offset + Feature_LowerL2(P, O);
		Array[Index++] = Offset + Feature_UpperL2(P, O);
		Array[Index++] = Offset + Feature_LeftL2(P, O);
		Array[Index++] = Offset + Feature_RightL2(P, O);
		Offset += Size_L2;
	}
	if (Feature_L3){
		Array[Index++] = Offset + Feature_LowerL3(P, O);
		Array[Index++] = Offset + Feature_UpperL3(P, O);
		Array[Index++] = Offset + Feature_LeftL3(P, O);
		Array[Index++] = Offset + Feature_RightL3(P, O);
		Offset += Size_L3;
	}
	if (Feature_A){
		Array[Index++] = Offset + Feature_RightLowerA(P, O);
		Array[Index++] = Offset + Feature_LeftUpperA(P, O);
		Array[Index++] = Offset + Feature_LeftLowerA(P, O);
		Array[Index++] = Offset + Feature_RightUpperA(P, O);
		Offset += Size_A;
	}
	if (Feature_Sq){
		Array[Index++] = Offset + Feature_RightLowerSq(P, O);
		Array[Index++] = Offset + Feature_LeftUpperSq(P, O);
		Array[Index++] = Offset + Feature_LeftLowerSq(P, O);
		Array[Index++] = Offset + Feature_RightUpperSq(P, O);
		Offset += Size_Sq;
	}
	if (Feature_B){
		Array[Index++] = Offset + Feature_LowerRightB(P, O);
		Array[Index++] = Offset + Feature_LowerLeftB(P, O);
		Array[Index++] = Offset + Feature_LeftLowerB(P, O);
		Array[Index++] = Offset + Feature_LeftUpperB(P, O);
		Array[Index++] = Offset + Feature_UpperLeftB(P, O);
		Array[Index++] = Offset + Feature_UpperRightB(P, O);
		Array[Index++] = Offset + Feature_RightUpperB(P, O);
		Array[Index++] = Offset + Feature_RightLowerB(P, O);
		Offset += Size_B;
	}
	if (Feature_D3){
		Array[Index++] = Offset + Feature_RightLowerD3(P, O);
		Array[Index++] = Offset + Feature_LeftUpperD3(P, O);
		Array[Index++] = Offset + Feature_LeftLowerD3(P, O);
		Array[Index++] = Offset + Feature_RightUpperD3(P, O);
		Offset += Size_D3;
	}
	if (Feature_D4){
		Array[Index++] = Offset + Feature_RightLowerD4(P, O);
		Array[Index++] = Offset + Feature_LeftUpperD4(P, O);
		Array[Index++] = Offset + Feature_LeftLowerD4(P, O);
		Array[Index++] = Offset + Feature_RightUpperD4(P, O);
		Offset += Size_D4;
	}
	if (Feature_D5){
		Array[Index++] = Offset + Feature_RightLowerD5(P, O);
		Array[Index++] = Offset + Feature_LeftUpperD5(P, O);
		Array[Index++] = Offset + Feature_LeftLowerD5(P, O);
		Array[Index++] = Offset + Feature_RightUpperD5(P, O);
		Offset += Size_D5;
	}
	if (Feature_D6){
		Array[Index++] = Offset + Feature_RightLowerD6(P, O);
		Array[Index++] = Offset + Feature_LeftUpperD6(P, O);
		Array[Index++] = Offset + Feature_LeftLowerD6(P, O);
		Array[Index++] = Offset + Feature_RightUpperD6(P, O);
		Offset += Size_D6;
	}
	if (Feature_D7){
		Array[Index++] = Offset + Feature_RightLowerD7(P, O);
		Array[Index++] = Offset + Feature_LeftUpperD7(P, O);
		Array[Index++] = Offset + Feature_LeftLowerD7(P, O);
		Array[Index++] = Offset + Feature_RightUpperD7(P, O);
		Offset += Size_D7;
	}
	if (Feature_X){
		Array[Index++] = Offset + Feature_CenterX(P, O);
		Offset += Size_X;
	}
	if (Feature_PlayersBoarder){
		Array[Index++] = Offset + (int)POP_COUNT(PlayersBoarder(P, O));
		Offset += Size_PlayersBoarder;
	}
	if (Feature_OpponentsBoarder){
		Array[Index++] = Offset + (int)POP_COUNT(OpponentsBoarder(P, O));
		Offset += Size_OpponentsBoarder;
	}
	if (Feature_PlayersExposeds){
		Array[Index++] = Offset + (int)POP_COUNT(PlayersExposed(P, O));
		Offset += Size_PlayersExposeds;
	}
	if (Feature_OpponentsExposeds){
		Array[Index++] = Offset + (int)POP_COUNT(OpponentsExposed(P, O));
		Offset += Size_OpponentsExposeds;
	}

	if (Feature_Possible || Feature_Possible_E || Feature_Possible_XC)
		BitBoardPossible = PossibleMoves(P, O);

	if (Feature_Possible){
		Array[Index++] = Offset + (int)POP_COUNT(BitBoardPossible);
		Offset += Size_Possible;
	}
	if (Feature_Possible_E){
		Array[Index++] = Offset + (int)POP_COUNT(BitBoardPossible & 0x8100000000000081UL);
		Offset += Size_Possible_E;
	}
	if (Feature_Possible_XC){
		Array[Index++] = Offset + (int)POP_COUNT(BitBoardPossible & 0x42C300000000C342UL);
		Offset += Size_Possible_XC;
	}
	if (Feature_Parity){
		const unsigned long long E = ~(P | O);
		const unsigned long long parity = (POP_COUNT(E & 0xF0F0F0F000000000ULL) & 1) << 3 
										| (POP_COUNT(E & 0x0F0F0F0F00000000ULL) & 1) << 2 
										| (POP_COUNT(E & 0x00000000F0F0F0F0ULL) & 1) << 1 
										|  POP_COUNT(E & 0x000000000F0F0F0FULL) & 1;
		Array[Index++] = Offset + (int)POP_COUNT(parity);
		Offset += Size_Parity;
	}
	if (Feature_Quadrants){
		const unsigned long long E = ~(P | O);
		Array[Index++] = Offset + (int)POP_COUNT(E & 0xF0F0F0F000000000ULL);
		Array[Index++] = Offset + (int)POP_COUNT(E & 0x0F0F0F0F00000000ULL);
		Array[Index++] = Offset + (int)POP_COUNT(E & 0xF0F0F0F000000000ULL);
		Array[Index++] = Offset + (int)POP_COUNT(E & 0x000000000F0F0F0FULL);
		Offset += Size_Quadrants;
	}
	if (Feature_NumberOfOwnStones){
		Array[Index++] = Offset + (int)POP_COUNT(P);
		Offset += Size_NumberOfOwnStones;
	}
}

void FillConfigurationArraySorted(const unsigned long long P, const unsigned long long O, int* const Array)
{
	using namespace Features;
	unsigned long long BitBoardPossible;
	int Index = 0;
	int Offset = 0;

	if (Feature_C){
		Array[Index+0] = Feature_LowerC(P, O);
		Array[Index+1] = Feature_UpperC(P, O);
		Array[Index+2] = Feature_LeftC(P, O);
		Array[Index+3] = Feature_RightC(P, O);
		Sort4(Array, Index);
		Index += Symmetry_C;
		Offset += Size_C;
	}
	if (Feature_L1){
		Array[Index+0] = Offset + Feature_LowerL1(P, O);
		Array[Index+1] = Offset + Feature_UpperL1(P, O);
		Array[Index+2] = Offset + Feature_LeftL1(P, O);
		Array[Index+3] = Offset + Feature_RightL1(P, O);
		Sort4(Array, Index);
		Index += Symmetry_L1;
		Offset += Size_L1;
	}
	if (Feature_L2){
		Array[Index+0] = Offset + Feature_LowerL2(P, O);
		Array[Index+1] = Offset + Feature_UpperL2(P, O);
		Array[Index+2] = Offset + Feature_LeftL2(P, O);
		Array[Index+3] = Offset + Feature_RightL2(P, O);
		Sort4(Array, Index);
		Index += Symmetry_L2;
		Offset += Size_L2;
	}
	if (Feature_L3){
		Array[Index+0] = Offset + Feature_LowerL3(P, O);
		Array[Index+1] = Offset + Feature_UpperL3(P, O);
		Array[Index+2] = Offset + Feature_LeftL3(P, O);
		Array[Index+3] = Offset + Feature_RightL3(P, O);
		Sort4(Array, Index);
		Index += Symmetry_L3;
		Offset += Size_L3;
	}
	if (Feature_A){
		Array[Index+0] = Offset + Feature_RightLowerA(P, O);
		Array[Index+1] = Offset + Feature_LeftUpperA(P, O);
		Array[Index+2] = Offset + Feature_LeftLowerA(P, O);
		Array[Index+3] = Offset + Feature_RightUpperA(P, O);
		Sort4(Array, Index);
		Index += Symmetry_A;
		Offset += Size_A;
	}
	if (Feature_Sq){
		Array[Index+0] = Offset + Feature_RightLowerSq(P, O);
		Array[Index+1] = Offset + Feature_LeftUpperSq(P, O);
		Array[Index+2] = Offset + Feature_LeftLowerSq(P, O);
		Array[Index+3] = Offset + Feature_RightUpperSq(P, O);
		Sort4(Array, Index);
		Index += Symmetry_Sq;
		Offset += Size_Sq;
	}
	if (Feature_B){
		Array[Index+0] = Offset + Feature_LowerRightB(P, O);
		Array[Index+1] = Offset + Feature_LowerLeftB(P, O);
		Array[Index+2] = Offset + Feature_LeftLowerB(P, O);
		Array[Index+3] = Offset + Feature_LeftUpperB(P, O);
		Array[Index+4] = Offset + Feature_UpperLeftB(P, O);
		Array[Index+5] = Offset + Feature_UpperRightB(P, O);
		Array[Index+6] = Offset + Feature_RightUpperB(P, O);
		Array[Index+7] = Offset + Feature_RightLowerB(P, O);
		Sort8(Array, Index);
		Index += Symmetry_B;
		Offset += Size_B;
	}
	if (Feature_D3){
		Array[Index+0] = Offset + Feature_RightLowerD3(P, O);
		Array[Index+1] = Offset + Feature_LeftUpperD3(P, O);
		Array[Index+2] = Offset + Feature_LeftLowerD3(P, O);
		Array[Index+3] = Offset + Feature_RightUpperD3(P, O);
		Sort4(Array, Index);
		Index += Symmetry_D3;
		Offset += Size_D3;
	}
	if (Feature_D4){
		Array[Index+0] = Offset + Feature_RightLowerD4(P, O);
		Array[Index+1] = Offset + Feature_LeftUpperD4(P, O);
		Array[Index+2] = Offset + Feature_LeftLowerD4(P, O);
		Array[Index+3] = Offset + Feature_RightUpperD4(P, O);
		Sort4(Array, Index);
		Index += Symmetry_D4;
		Offset += Size_D4;
	}
	if (Feature_D5){
		Array[Index+0] = Offset + Feature_RightLowerD5(P, O);
		Array[Index+1] = Offset + Feature_LeftUpperD5(P, O);
		Array[Index+2] = Offset + Feature_LeftLowerD5(P, O);
		Array[Index+3] = Offset + Feature_RightUpperD5(P, O);
		Sort4(Array, Index);
		Index += Symmetry_D5;
		Offset += Size_D5;
	}
	if (Feature_D6){
		Array[Index+0] = Offset + Feature_RightLowerD6(P, O);
		Array[Index+1] = Offset + Feature_LeftUpperD6(P, O);
		Array[Index+2] = Offset + Feature_LeftLowerD6(P, O);
		Array[Index+3] = Offset + Feature_RightUpperD6(P, O);
		Sort4(Array, Index);
		Index += Symmetry_D6;
		Offset += Size_D6;
	}
	if (Feature_D7){
		Array[Index+0] = Offset + Feature_RightLowerD7(P, O);
		Array[Index+1] = Offset + Feature_LeftUpperD7(P, O);
		Array[Index+2] = Offset + Feature_LeftLowerD7(P, O);
		Array[Index+3] = Offset + Feature_RightUpperD7(P, O);
		Sort4(Array, Index);
		Index += Symmetry_D7;
		Offset += Size_D7;
	}
	if (Feature_X){
		Array[Index++] = Offset + Feature_CenterX(P, O);
		Offset += Size_X;
	}
	if (Feature_PlayersBoarder){
		Array[Index++] = Offset + (int)POP_COUNT(PlayersBoarder(P, O));
		Offset += Size_PlayersBoarder;
	}
	if (Feature_OpponentsBoarder){
		Array[Index++] = Offset + (int)POP_COUNT(OpponentsBoarder(P, O));
		Offset += Size_OpponentsBoarder;
	}
	if (Feature_PlayersExposeds){
		Array[Index++] = Offset + (int)POP_COUNT(PlayersExposed(P, O));
		Offset += Size_PlayersExposeds;
	}
	if (Feature_OpponentsExposeds){
		Array[Index++] = Offset + (int)POP_COUNT(OpponentsExposed(P, O));
		Offset += Size_OpponentsExposeds;
	}

	if (Feature_Possible || Feature_Possible_E || Feature_Possible_XC)
		BitBoardPossible = PossibleMoves(P, O);

	if (Feature_Possible){
		Array[Index++] = Offset + (int)POP_COUNT(BitBoardPossible);
		Offset += Size_Possible;
	}
	if (Feature_Possible_E){
		Array[Index++] = Offset + (int)POP_COUNT(BitBoardPossible & 0x8100000000000081UL);
		Offset += Size_Possible_E;
	}
	if (Feature_Possible_XC){
		Array[Index++] = Offset + (int)POP_COUNT(BitBoardPossible & 0x42C300000000C342UL);
		Offset += Size_Possible_XC;
	}
	if (Feature_Parity){
		unsigned long long Parity = (POP_COUNT(~(P | O) & 0xF0F0F0F000000000ULL) & 1) << 3 
								  | (POP_COUNT(~(P | O) & 0x0F0F0F0F00000000ULL) & 1) << 2 
								  | (POP_COUNT(~(P | O) & 0x00000000F0F0F0F0ULL) & 1) << 1 
								  |  POP_COUNT(~(P | O) & 0x000000000F0F0F0FULL) & 1;
		Array[Index++] = Offset + (int)POP_COUNT(Parity);
		Offset += Size_Parity;
	}
	if (Feature_Quadrants){
		Array[Index+0] = Offset + (int)POP_COUNT(~(P | O) & 0xF0F0F0F000000000ULL);
		Array[Index+1] = Offset + (int)POP_COUNT(~(P | O) & 0x0F0F0F0F00000000ULL);
		Array[Index+2] = Offset + (int)POP_COUNT(~(P | O) & 0xF0F0F0F000000000ULL);
		Array[Index+3] = Offset + (int)POP_COUNT(~(P | O) & 0x000000000F0F0F0FULL);
		Sort4(Array, Index);
		Index += Symmetry_Quadrants;
		Offset += Size_Quadrants;
	}
	if (Feature_NumberOfOwnStones){
		Array[Index++] = Offset + (int)POP_COUNT(P);
		Offset += Size_NumberOfOwnStones;
	}
}

int EvaluateFeatures(const unsigned long long P, const unsigned long long O)
{
	float sum = 0.0f;
	unsigned char BoxIndex = Features::BoxIndex[NumberOfEmptyStones(P, O)];
	// ############ Remove safety
	if (BoxIndex >= Features::NumberOfFiles)
		BoxIndex = Features::NumberOfFiles - 1;
	// ############ Remove safety

	int Array[Features::Symmetries];
	FillConfigurationArray(P, O, Array);

	for (int i = 0; i < Features::Symmetries; ++i)
		sum += Features::Weights[BoxIndex][Array[i]];
	
	return static_cast<int>(std::floorf(sum + 0.5f));
}

CActiveConfigurations::CActiveConfigurations(const unsigned long long P, const unsigned long long O) : m_P(O), m_O(P)
{
	unsigned char index = Features::BoxIndex[NumberOfEmptyStones(m_P, m_O)];
	// ############ Remove safety
	if (index >= Features::NumberOfFiles)
		index = Features::NumberOfFiles - 1;
	// ############ Remove safety

	FillConfigurationArray(m_P, m_O, m_Array);
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
	// ############ Remove safety
	if (BoxIndex >= Features::NumberOfFiles)
		BoxIndex = Features::NumberOfFiles - 1;
	// ############ Remove safety

	float * weights = Features::Weights[BoxIndex];

	float sum = 0.0f;

	if (Feature_C){
		sum += weights[(diff & Pattern_LowerC) ? Feature_LowerC(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_UpperC) ? Feature_UpperC(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftC ) ? Feature_LeftC (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightC) ? Feature_RightC(P, O) : m_Array[Index]]; Index++;
		Offset += Size_C;
	}
	if (Feature_L1){
		sum += weights[(diff & Pattern_LowerL1) ? Offset + Feature_LowerL1(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_UpperL1) ? Offset + Feature_UpperL1(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftL1 ) ? Offset + Feature_LeftL1 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightL1) ? Offset + Feature_RightL1(P, O) : m_Array[Index]]; Index++;
		Offset += Size_L1;
	}
	if (Feature_L2){
		sum += weights[(diff & Pattern_LowerL2) ? Offset + Feature_LowerL2(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_UpperL2) ? Offset + Feature_UpperL2(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftL2 ) ? Offset + Feature_LeftL2 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightL2) ? Offset + Feature_RightL2(P, O) : m_Array[Index]]; Index++;
		Offset += Size_L2;
	}
	if (Feature_L3){
		sum += weights[(diff & Pattern_LowerL3) ? Offset + Feature_LowerL3(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_UpperL3) ? Offset + Feature_UpperL3(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftL3 ) ? Offset + Feature_LeftL3 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightL3) ? Offset + Feature_RightL3(P, O) : m_Array[Index]]; Index++;
		Offset += Size_L3;
	}
	if (Feature_A){
		sum += weights[(diff & Pattern_RightLowerA) ? Offset + Feature_RightLowerA(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftUpperA ) ? Offset + Feature_LeftUpperA (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftLowerA ) ? Offset + Feature_LeftLowerA (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightUpperA) ? Offset + Feature_RightUpperA(P, O) : m_Array[Index]]; Index++;
		Offset += Size_A;
	}
	if (Feature_Sq){
		sum += weights[(diff & Pattern_RightLowerSq) ? Offset + Feature_RightLowerSq(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftUpperSq ) ? Offset + Feature_LeftUpperSq (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftLowerSq ) ? Offset + Feature_LeftLowerSq (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightUpperSq) ? Offset + Feature_RightUpperSq(P, O) : m_Array[Index]]; Index++;
		Offset += Size_Sq;
	}
	if (Feature_B){
		sum += weights[(diff & Pattern_LowerRightB) ? Offset + Feature_LowerRightB(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LowerLeftB ) ? Offset + Feature_LowerLeftB (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftLowerB ) ? Offset + Feature_LeftLowerB (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftUpperB ) ? Offset + Feature_LeftUpperB (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_UpperLeftB ) ? Offset + Feature_UpperLeftB (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_UpperRightB) ? Offset + Feature_UpperRightB(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightUpperB) ? Offset + Feature_RightUpperB(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightLowerB) ? Offset + Feature_RightLowerB(P, O) : m_Array[Index]]; Index++;
		Offset += Size_B;
	}
	if (Feature_D3){
		sum += weights[(diff & Pattern_RightLowerD3) ? Offset + Feature_RightLowerD3(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftUpperD3 ) ? Offset + Feature_LeftUpperD3 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftLowerD3 ) ? Offset + Feature_LeftLowerD3 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightUpperD3) ? Offset + Feature_RightUpperD3(P, O) : m_Array[Index]]; Index++;
		Offset += Size_D3;
	}
	if (Feature_D4){
		sum += weights[(diff & Pattern_RightLowerD4) ? Offset + Feature_RightLowerD4(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftUpperD4 ) ? Offset + Feature_LeftUpperD4 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftLowerD4 ) ? Offset + Feature_LeftLowerD4 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightUpperD4) ? Offset + Feature_RightUpperD4(P, O) : m_Array[Index]]; Index++;
		Offset += Size_D4;
	}
	if (Feature_D5){
		sum += weights[(diff & Pattern_RightLowerD5) ? Offset + Feature_RightLowerD5(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftUpperD5 ) ? Offset + Feature_LeftUpperD5 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftLowerD5 ) ? Offset + Feature_LeftLowerD5 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightUpperD5) ? Offset + Feature_RightUpperD5(P, O) : m_Array[Index]]; Index++;
		Offset += Size_D5;
	}
	if (Feature_D6){
		sum += weights[(diff & Pattern_RightLowerD6) ? Offset + Feature_RightLowerD6(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftUpperD6 ) ? Offset + Feature_LeftUpperD6 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftLowerD6 ) ? Offset + Feature_LeftLowerD6 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightUpperD6) ? Offset + Feature_RightUpperD6(P, O) : m_Array[Index]]; Index++;
		Offset += Size_D6;
	}
	if (Feature_D7){
		sum += weights[(diff & Pattern_RightLowerD7) ? Offset + Feature_RightLowerD7(P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftUpperD7 ) ? Offset + Feature_LeftUpperD7 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_LeftLowerD7 ) ? Offset + Feature_LeftLowerD7 (P, O) : m_Array[Index]]; Index++;
		sum += weights[(diff & Pattern_RightUpperD7) ? Offset + Feature_RightUpperD7(P, O) : m_Array[Index]]; Index++;
		Offset += Size_D7;
	}
	if (Feature_X){
		sum += weights[(diff & Pattern_CenterX) ? Offset + Feature_CenterX(P, O) : m_Array[Index]]; Index++;
		Offset += Size_X;
	}
	if (Feature_PlayersBoarder){
		sum += weights[Offset + (int)POP_COUNT(PlayersBoarder(P, O))];
		Offset += Size_PlayersBoarder;
	}
	if (Feature_OpponentsBoarder){
		sum += weights[Offset + (int)POP_COUNT(OpponentsBoarder(P, O))];
		Offset += Size_OpponentsBoarder;
	}
	if (Feature_PlayersExposeds){
		sum += weights[Offset + (int)POP_COUNT(PlayersExposed(P, O))];
		Offset += Size_PlayersExposeds;
	}
	if (Feature_OpponentsExposeds){
		sum += weights[Offset + (int)POP_COUNT(OpponentsExposed(P, O))];
		Offset += Size_OpponentsExposeds;
	}

	if (Feature_Possible || Feature_Possible_E || Feature_Possible_XC)
		BitBoardPossible = PossibleMoves(P, O);

	if (Feature_Possible){
		sum += weights[Offset + (int)POP_COUNT(BitBoardPossible)];
		Offset += Size_Possible;
	}
	if (Feature_Possible_E){
		sum += weights[Offset + (int)POP_COUNT(BitBoardPossible & 0x8100000000000081UL)];
		Offset += Size_Possible_E;
	}
	if (Feature_Possible_XC){
		sum += weights[Offset + (int)POP_COUNT(BitBoardPossible & 0x42C300000000C342UL)];
		Offset += Size_Possible_XC;
	}
	if (Feature_Parity){
		const unsigned long long E = ~(P | O);
		const unsigned long long parity = (POP_COUNT(E & 0xF0F0F0F000000000ULL) & 1) << 3 
										| (POP_COUNT(E & 0x0F0F0F0F00000000ULL) & 1) << 2 
										| (POP_COUNT(E & 0x00000000F0F0F0F0ULL) & 1) << 1 
										|  POP_COUNT(E & 0x000000000F0F0F0FULL) & 1;
		sum += weights[Offset + (int)POP_COUNT(parity)];
		Offset += Size_Parity;
	}
	if (Feature_Quadrants){
		const unsigned long long E = ~(P | O);
		sum += weights[Offset + (int)POP_COUNT(E & 0xF0F0F0F000000000ULL)];
		sum += weights[Offset + (int)POP_COUNT(E & 0x0F0F0F0F00000000ULL)];
		sum += weights[Offset + (int)POP_COUNT(E & 0xF0F0F0F000000000ULL)];
		sum += weights[Offset + (int)POP_COUNT(E & 0x000000000F0F0F0FULL)];
		Offset += Size_Quadrants;
	}
	if (Feature_NumberOfOwnStones){
		sum += weights[Offset + (int)POP_COUNT(P)];
		Offset += Size_NumberOfOwnStones;
	}

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

bool Test_Patterns_POP_COUNT(const int symmeties, const unsigned long long * const Pattern)
{
	bool Success = true;
	unsigned long long * POPCOUNT = new unsigned long long[symmeties];

	for (int i = 0; i < symmeties; i++)
		POPCOUNT[i] = POP_COUNT(Pattern[i]);
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

void For_all_configurations_in_pattern_do_fkt(const unsigned long long Pattern, std::function<void(const unsigned long long, const unsigned long long)> fkt)
{
	unsigned long long P, O;
	const unsigned long long POPCOUNT = POP_COUNT(Pattern);
	unsigned long long * BitConfig = new unsigned long long[POPCOUNT];
	unsigned long long tmp;

	// Get Pattern's bit configuration
	tmp = Pattern;
	for (int i = 0; i < POPCOUNT; i++)
	{
		BitConfig[i] = 1ULL << BIT_SCAN_LS1B(tmp);
		REMOVE_LS1B(tmp);
	}

	for (unsigned long long i = 0; i < (1ULL << POPCOUNT); i++)
	{
		P = 0;
		tmp = i;
		while (tmp)
		{
			P |= BitConfig[BIT_SCAN_LS1B(tmp)];
			REMOVE_LS1B(tmp);
		}

		// For each opponents configuration
		for (unsigned long long j = 0; j < (1ULL << POPCOUNT); j++)
		{
			// A field can only be occupied by one player
			if ((i & j) != 0)
				continue;

			O = 0;
			tmp = j;
			while (tmp)
			{
				O |= BitConfig[BIT_SCAN_LS1B(tmp)];
				REMOVE_LS1B(tmp);
			}

			// If some of the 4 center fields are in the pattern, they have to be occupied
			if (((P | O) & (0x0000001818000000ULL & Pattern)) != (0x0000001818000000ULL & Pattern))
				continue;

			fkt(P, O);
		}
	}

	delete[] BitConfig;
}

bool Test_Index_Range(const int symmeties, const int size, unsigned long long * Pattern, int(*Feature[])(const unsigned long long P, const unsigned long long O))
{
	bool Success = true;

	for (int k = 0; k < symmeties; k++)
	{
		For_all_configurations_in_pattern_do_fkt(Pattern[k],
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

	For_all_configurations_in_pattern_do_fkt(Pattern[0],
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

bool Test_Index_Covering(const int symmeties, const int size, unsigned long long * Pattern, int(*Feature[])(const unsigned long long P, const unsigned long long O))
{
	bool Success = true;
	int * taken = new int[size];

	for (int k = 0; k < symmeties; k++)
	{
		for (int i = 0; i < size; i++)
			taken[i] = 0;

		For_all_configurations_in_pattern_do_fkt(Pattern[k],
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
	const unsigned long long POPCOUNT = POP_COUNT(Pattern[0]);
	unsigned long long * BitConfig = new unsigned long long[POPCOUNT];

	// Test patern's population count to be equal
	Success &= Test_Patterns_POP_COUNT(symmeties, Pattern);

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

		For_all_configurations_in_pattern_do_fkt(Pattern[k],
			[Feature, k, &taken](const unsigned long long P, const unsigned long long O){ taken[Feature[k](P, O)]++; }
		);

		UniqueCount = 0;
		for (unsigned long long i = 0; i < (1ULL << POPCOUNT); i++)
		{
			P = 0;
			tmp = i;
			while (tmp)
			{
				P |= BitConfig[BIT_SCAN_LS1B(tmp)];
				REMOVE_LS1B(tmp);
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
					O |= BitConfig[BIT_SCAN_LS1B(tmp)];
					REMOVE_LS1B(tmp);
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

	unsigned long long Pattern_C[] = {Pattern_LowerC, Pattern_UpperC, Pattern_LeftC, Pattern_RightC};
	int(*Feature_C[])(const unsigned long long P, const unsigned long long O) = {Feature_LowerC, Feature_UpperC, Feature_LeftC, Feature_RightC};
	Success &= Test(Symmetry_C, Size_C, Pattern_C, Feature_C, true);

	unsigned long long Pattern_L1[] = {Pattern_LowerL1, Pattern_UpperL1, Pattern_LeftL1, Pattern_RightL1};
	int(*Feature_L1[])(const unsigned long long P, const unsigned long long O) = {Feature_LowerL1, Feature_UpperL1, Feature_LeftL1, Feature_RightL1};
	Success &= Test(Symmetry_L1, Size_L1, Pattern_L1, Feature_L1, true);

	unsigned long long Pattern_L2[] = {Pattern_LowerL2, Pattern_UpperL2, Pattern_LeftL2, Pattern_RightL2};
	int(*Feature_L2[])(const unsigned long long P, const unsigned long long O) = {Feature_LowerL2, Feature_UpperL2, Feature_LeftL2, Feature_RightL2};
	Success &= Test(Symmetry_L2, Size_L2, Pattern_L2, Feature_L2, true);

	unsigned long long Pattern_L3[] = {Pattern_LowerL3, Pattern_UpperL3, Pattern_LeftL3, Pattern_RightL3};
	int(*Feature_L3[])(const unsigned long long P, const unsigned long long O) = {Feature_LowerL3, Feature_UpperL3, Feature_LeftL3, Feature_RightL3};
	Success &= Test(Symmetry_L3, Size_L3, Pattern_L3, Feature_L3, false);

	unsigned long long Pattern_A[] = {Pattern_RightLowerA, Pattern_LeftUpperA, Pattern_LeftLowerA, Pattern_RightUpperA};
	int(*Feature_A[])(const unsigned long long P, const unsigned long long O) = {Feature_RightLowerA, Feature_LeftUpperA, Feature_LeftLowerA, Feature_RightUpperA};
	Success &= Test(Symmetry_A, Size_A, Pattern_A, Feature_A, true);

	unsigned long long Pattern_Sq[] = {Pattern_RightLowerSq, Pattern_LeftUpperSq, Pattern_LeftLowerSq, Pattern_RightUpperSq};
	int(*Feature_Sq[])(const unsigned long long P, const unsigned long long O) = {Feature_RightLowerSq, Feature_LeftUpperSq, Feature_LeftLowerSq, Feature_RightUpperSq};
	Success &= Test(Symmetry_Sq, Size_Sq, Pattern_Sq, Feature_Sq, true);

	unsigned long long Pattern_B[] = {Pattern_LowerRightB, Pattern_LowerLeftB, Pattern_LeftLowerB, Pattern_LeftUpperB, Pattern_UpperLeftB, Pattern_UpperRightB, Pattern_RightUpperB, Pattern_RightLowerB};
	int(*Feature_B[])(const unsigned long long P, const unsigned long long O) = {Feature_LowerRightB, Feature_LowerLeftB, Feature_LeftLowerB, Feature_LeftUpperB, Feature_UpperLeftB, Feature_UpperRightB, Feature_RightUpperB, Feature_RightLowerB};
	Success &= Test(Symmetry_B, Size_B, Pattern_B, Feature_B, true);

	unsigned long long Pattern_D3[] = {Pattern_RightLowerD3, Pattern_LeftUpperD3, Pattern_LeftLowerD3, Pattern_RightUpperD3};
	int(*Feature_D3[])(const unsigned long long P, const unsigned long long O) = {Feature_RightLowerD3, Feature_LeftUpperD3, Feature_LeftLowerD3, Feature_RightUpperD3};
	Success &= Test(Symmetry_D3, Size_D3, Pattern_D3, Feature_D3, true);

	unsigned long long Pattern_D4[] = {Pattern_RightLowerD4, Pattern_LeftUpperD4, Pattern_LeftLowerD4, Pattern_RightUpperD4};
	int(*Feature_D4[])(const unsigned long long P, const unsigned long long O) = {Feature_RightLowerD4, Feature_LeftUpperD4, Feature_LeftLowerD4, Feature_RightUpperD4};
	Success &= Test(Symmetry_D4, Size_D4, Pattern_D4, Feature_D4, true);

	unsigned long long Pattern_D5[] = {Pattern_RightLowerD5, Pattern_LeftUpperD5, Pattern_LeftLowerD5, Pattern_RightUpperD5};
	int(*Feature_D5[])(const unsigned long long P, const unsigned long long O) = {Feature_RightLowerD5, Feature_LeftUpperD5, Feature_LeftLowerD5, Feature_RightUpperD5};
	Success &= Test(Symmetry_D5, Size_D5, Pattern_D5, Feature_D5, true);

	unsigned long long Pattern_D6[] = {Pattern_RightLowerD6, Pattern_LeftUpperD6, Pattern_LeftLowerD6, Pattern_RightUpperD6};
	int(*Feature_D6[])(const unsigned long long P, const unsigned long long O) = {Feature_RightLowerD6, Feature_LeftUpperD6, Feature_LeftLowerD6, Feature_RightUpperD6};
	Success &= Test(Symmetry_D6, Size_D6, Pattern_D6, Feature_D6, true);

	unsigned long long Pattern_D7[] = {Pattern_RightLowerD7, Pattern_LeftUpperD7, Pattern_LeftLowerD7, Pattern_RightUpperD7};
	int(*Feature_D7[])(const unsigned long long P, const unsigned long long O) = {Feature_RightLowerD7, Feature_LeftUpperD7, Feature_LeftLowerD7, Feature_RightUpperD7};
	Success &= Test(Symmetry_D7, Size_D7, Pattern_D7, Feature_D7, false);

	return Success;
}