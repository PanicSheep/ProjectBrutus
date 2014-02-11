#pragma once
#include <cmath>
#include <string>
#include <sstream>
#include <cassert>
#include <chrono>
#include <emmintrin.h>
#include <iostream>

#define POP_COUNT_use_BITBOARDTRICKERY
//#define POP_COUNT_use__popcnt
//#define POP_COUNT_use__popcnt64
//#define POP_COUNT_use_mm_popcnt_u32
//#define POP_COUNT_use_mm_popcnt_u64

//#define BIT_SCAN_LS1B_use_LOOKUP
//#define BIT_SCAN_LS1B_use_POP_COUNT
#ifdef __GNUC__ //GNU compiler
	#define BIT_SCAN_LS1B_use_INTRINSIC_BitScanForward64
#elif _WIN32 //Visual Studio
	#if defined _M_IX86 //Visual Studio x86
		#define BIT_SCAN_LS1B_use_INTRINSIC_BitScanForward32
		#pragma intrinsic(_BitScanForward)
		#pragma intrinsic(_BitScanReverse)
	#elif defined _M_X64 //Visual Studio x64
		#define BIT_SCAN_LS1B_use_INTRINSIC_BitScanForward64
		#pragma intrinsic(_BitScanForward64)
		#pragma intrinsic(_BitScanReverse64)
	#endif
#else
	#error Unknown compiler.
#endif

#define SIGN_OR_ZERO_BRANCHING
#define ABS_BRANCHING
#define MAX_BRANCHING
#define MIN_BRANCHING
//#define SIGN_OR_ZERO_NO_BRANCHING
//#define ABS_NO_BRANCHING
//#define MAX_NO_BRANCHING
//#define MIN_NO_BRANCHING

#define CACHE_LINE_SIZE 64
#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
	#define CACHE_LINE_ALIGNMENT __declspec(align(CACHE_LINE_SIZE))
#else
	#define CACHE_LINE_ALIGNMENT __attribute__((aligned(CACHE_LINE_SIZE)))
#endif

#ifdef _MSC_VER
	#define FORCE_INLINE __forceinline
#elif defined(__GNUC__)
	#define FORCE_INLINE inline __attribute__((always_inline))
#else
	#define FORCE_INLINE inline
#endif


#if defined(POP_COUNT_use_BITBOARDTRICKERY)
	FORCE_INLINE unsigned long long POP_COUNT(unsigned long long Input){
		Input -=  (Input >> 1) & 0x5555555555555555ULL;
		Input  = ((Input >> 2) & 0x3333333333333333ULL) + (Input & 0x3333333333333333ULL);
		Input  = ((Input >> 4) + Input) & 0x0F0F0F0F0F0F0F0FULL;
		return (Input * 0x0101010101010101ULL) >> 56;
	}
	FORCE_INLINE unsigned long long POP_COUNT_MAX15(unsigned long long Input){
		Input -=  (Input >> 1) & 0x5555555555555555ULL;
		Input  = ((Input >> 2) & 0x3333333333333333ULL) + (Input & 0x3333333333333333ULL);
		return (Input * 0x1111111111111111ULL) >> 60;
	}
	FORCE_INLINE unsigned long long POP_COUNT_PLUS(__m128i Input)
	{
		Input = _mm_sub_epi64(Input, _mm_and_si128((_mm_srli_epi64(Input, 1)), _mm_set1_epi64x(0x5555555555555555ULL)));
		Input = _mm_add_epi64(_mm_and_si128(Input, _mm_set1_epi64x(0x3333333333333333ULL)), _mm_and_si128(_mm_srli_epi64(Input, 2), _mm_set1_epi64x(0x3333333333333333ULL)));
		Input = _mm_and_si128(_mm_add_epi64(Input, _mm_srli_epi64(Input, 4)), _mm_set1_epi64x(0x0F0F0F0F0F0F0F0FULL));
		const unsigned long long A = (((unsigned long long *)&Input)[1] * 0x0101010101010101ULL) >> 56;
		const unsigned long long B = (((unsigned long long *)&Input)[0] * 0x0101010101010101ULL) >> 56;
		return A + B;
	}
	FORCE_INLINE unsigned long long POP_COUNT_MINUS(__m128i Input)
	{
		Input = _mm_sub_epi64(Input, _mm_and_si128((_mm_srli_epi64(Input, 1)), _mm_set1_epi64x(0x5555555555555555ULL)));
		Input = _mm_add_epi64(_mm_and_si128(Input, _mm_set1_epi64x(0x3333333333333333ULL)), _mm_and_si128(_mm_srli_epi64(Input, 2), _mm_set1_epi64x(0x3333333333333333ULL)));
		Input = _mm_and_si128(_mm_add_epi64(Input, _mm_srli_epi64(Input, 4)), _mm_set1_epi64x(0x0F0F0F0F0F0F0F0FULL));
		const unsigned long long A = (((unsigned long long *)&Input)[1] * 0x0101010101010101ULL) >> 56;
		const unsigned long long B = (((unsigned long long *)&Input)[0] * 0x0101010101010101ULL) >> 56;
		return A - B;
	}
#elif defined(POP_COUNT_use__popcnt)
	FORCE_INLINE unsigned int POP_COUNT(unsigned long long Input) { return __popcnt(static_cast<unsigned int>(Input)) + __popcnt(static_cast<unsigned int>(Input >> 32)); }
#elif defined(POP_COUNT_use__popcnt64)
	FORCE_INLINE unsigned __int64 POP_COUNT(unsigned long long Input) { return __popcnt64(Input); }
#elif defined(POP_COUNT_use_mm_popcnt_u32)
	FORCE_INLINE unsigned __int64 POP_COUNT(unsigned long long Input) { return _mm_popcnt_u32(static_cast<unsigned int>(Input)) + _mm_popcnt_u32(static_cast<unsigned int>(Input >> 32)); }
#elif defined(POP_COUNT_use_mm_popcnt_u64)
	FORCE_INLINE unsigned __int64 POP_COUNT(unsigned long long Input) { return _mm_popcnt_u64(Input); }
#endif

static const char field_name[65][3] = {
	"A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1",
	"A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2",
	"A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3",
	"A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4",
	"A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5",
	"A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6",
	"A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7",
	"A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8", "--"
};
#if defined(BIT_SCAN_LS1B_use_LOOKUP)
	static const unsigned char index64[64] = {   
		63,  0, 58,  1, 59, 47, 53,  2,
		60, 39, 48, 27, 54, 33, 42,  3,
		61, 51, 37, 40, 49, 18, 28, 20,
		55, 30, 34, 11, 43, 14, 22,  4,
		62, 57, 46, 52, 38, 26, 32, 41,
		50, 36, 17, 19, 29, 10, 13, 21,
		56, 45, 25, 31, 35, 16,  9, 12,
		44, 24, 15,  8, 23,  7,  6,  5
	};
#endif
static const unsigned char quadrant_id_2_bit[64] = {   
	0, 0, 0, 0, 1, 1, 1, 1,
	0, 0, 0, 0, 1, 1, 1, 1,
	0, 0, 0, 0, 1, 1, 1, 1,
	0, 0, 0, 0, 1, 1, 1, 1,
	2, 2, 2, 2, 3, 3, 3, 3,
	2, 2, 2, 2, 3, 3, 3, 3,
	2, 2, 2, 2, 3, 3, 3, 3,
	2, 2, 2, 2, 3, 3, 3, 3
};
static const unsigned long long quadrant[16] = {   
	0x0000000000000000ULL, //0000
	0x000000000F0F0F0FULL, //0001
	0x00000000F0F0F0F0ULL, //0010
	0x00000000FFFFFFFFULL, //0011
	0x0F0F0F0F00000000ULL, //0100
	0x0F0F0F0F0F0F0F0FULL, //0101
	0x0F0F0F0FF0F0F0F0ULL, //0110
	0x0F0F0F0FFFFFFFFFULL, //0111
	0xF0F0F0F000000000ULL, //1000
	0xF0F0F0F00F0F0F0FULL, //1001
	0xF0F0F0F0F0F0F0F0ULL, //1010
	0xF0F0F0F0FFFFFFFFULL, //1011
	0xFFFFFFFF00000000ULL, //1100
	0xFFFFFFFF0F0F0F0FULL, //1101
	0xFFFFFFFFF0F0F0F0ULL, //1110
	0xFFFFFFFFFFFFFFFFULL  //1111
};
static const unsigned char quadrant_id_4_bit[64] = {   
	1, 1, 1, 1, 2, 2, 2, 2,
	1, 1, 1, 1, 2, 2, 2, 2,
	1, 1, 1, 1, 2, 2, 2, 2,
	1, 1, 1, 1, 2, 2, 2, 2,
	4, 4, 4, 4, 8, 8, 8, 8,
	4, 4, 4, 4, 8, 8, 8, 8,
	4, 4, 4, 4, 8, 8, 8, 8,
	4, 4, 4, 4, 8, 8, 8, 8
};
static const char trivial_evaluation[64] = {
	 18, -10,  16, 12, 12, 16, -10,  18,
	-10, -10,   6,  8,  8,  6, -10, -10,
	 16,   6,  14, 10, 10, 14,   6,  16,
	 12,   8,  10,  0,  0, 10,   8,  12,
	 12,   8,  10,  0,  0, 10,   8,  12,
	 16,   6,  14, 10, 10, 14,   6,  16,
	-10, -10,   6,  8,  8,  6, -10, -10,
	 18, -10,  16, 12, 12, 16, -10,  18
};
static const char evaluation_order[64] = {
	10,  3,  7,  6,  6,  7,  3, 10,
	 3,  2,  4,  5,  5,  4,  2,  3,
	 7,  4,  8,  9,  9,  8,  4,  7,
	 6,  5,  9,  1,  1,  9,  5,  6,
	 6,  5,  9,  1,  1,  9,  5,  6,
	 7,  4,  8,  9,  9,  8,  4,  7,
	 3,  2,  4,  5,  5,  4,  2,  3,
	10,  3,  7,  6,  6,  7,  3, 10
};
static const unsigned char max_difference_1[64] = {   
	37, 33, 31, 29, 29, 31, 33, 37, 
	33, 31, 29, 27, 27, 29, 31, 33, 
	31, 29, 31, 29, 29, 31, 29, 31, 
	29, 27, 29, 31, 31, 29, 27, 29, 
	29, 27, 29, 31, 31, 29, 27, 29, 
	31, 29, 31, 29, 29, 31, 29, 31, 
	33, 31, 29, 27, 27, 29, 31, 33, 
	37, 33, 31, 29, 29, 31, 33, 37
};

static const char stability_cutoff_limits[64] = {   
	//-99, -99, -99, -99, -99,  -2,  22,   0,
	// 26,   3,  31,  11,  31,  14,  28,  -3,
	// 25, -99, -99, -99, -99, -99, -99, -99,
	//-99, -99, -99, -99, -99, -99, -99, -99,
	//-99, -99, -99, -99, -99, -99, -99, -99,
	//-99, -99, -99, -99, -99, -99, -99, -99,
	//-99, -99, -99, -99, -99, -99, -99, -99,
	//-99, -99, -99, -99, -99, -99, -99, -99
	-99, -99, -99, -99, -99, -15, -16, -15,
	-16, -17, -18, -17, -18, -19, -20, -19,
	-20, -99, -99, -99, -99, -99, -99, -99,
	-99, -99, -99, -99, -99, -99, -99, -99,
	-99, -99, -99, -99, -99, -99, -99, -99,
	-99, -99, -99, -99, -99, -99, -99, -99,
	-99, -99, -99, -99, -99, -99, -99, -99,
	-99, -99, -99, -99, -99, -99, -99, -99
};

CACHE_LINE_ALIGNMENT

static const unsigned long long neighbour[64] = { // Neighbours to the input-field
	0x0000000000000302ULL, 0x0000000000000705ULL, 0x0000000000000e0aULL, 0x0000000000001c14ULL,	0x0000000000003828ULL, 0x0000000000007050ULL, 0x000000000000e0a0ULL, 0x000000000000c040ULL,
	0x0000000000030203ULL, 0x0000000000070507ULL, 0x00000000000e0a0eULL, 0x00000000001c141cULL,	0x0000000000382838ULL, 0x0000000000705070ULL, 0x0000000000e0a0e0ULL, 0x0000000000c040c0ULL,
	0x0000000003020300ULL, 0x0000000007050700ULL, 0x000000000e0a0e00ULL, 0x000000001c141c00ULL,	0x0000000038283800ULL, 0x0000000070507000ULL, 0x00000000e0a0e000ULL, 0x00000000c040c000ULL,
	0x0000000302030000ULL, 0x0000000705070000ULL, 0x0000000e0a0e0000ULL, 0x0000001c141c0000ULL,	0x0000003828380000ULL, 0x0000007050700000ULL, 0x000000e0a0e00000ULL, 0x000000c040c00000ULL,
	0x0000030203000000ULL, 0x0000070507000000ULL, 0x00000e0a0e000000ULL, 0x00001c141c000000ULL,	0x0000382838000000ULL, 0x0000705070000000ULL, 0x0000e0a0e0000000ULL, 0x0000c040c0000000ULL,
	0x0003020300000000ULL, 0x0007050700000000ULL, 0x000e0a0e00000000ULL, 0x001c141c00000000ULL,	0x0038283800000000ULL, 0x0070507000000000ULL, 0x00e0a0e000000000ULL, 0x00c040c000000000ULL,
	0x0302030000000000ULL, 0x0705070000000000ULL, 0x0e0a0e0000000000ULL, 0x1c141c0000000000ULL,	0x3828380000000000ULL, 0x7050700000000000ULL, 0xe0a0e00000000000ULL, 0xc040c00000000000ULL,
	0x0203000000000000ULL, 0x0507000000000000ULL, 0x0a0e000000000000ULL, 0x141c000000000000ULL,	0x2838000000000000ULL, 0x5070000000000000ULL, 0xa0e0000000000000ULL, 0x40c0000000000000ULL
};
static const unsigned long long lines[64] = { // Lines into all directions origin the input-field
	0x81412111090503FEULL, 0x02824222120A07FDULL, 0x0404844424150EFBULL, 0x08080888492A1CF7ULL,	0x10101011925438EFULL, 0x2020212224A870DFULL, 0x404142444850E0BFULL, 0x8182848890A0C07FULL,
	0x412111090503FE03ULL, 0x824222120A07FD07ULL, 0x04844424150EFB0EULL, 0x080888492A1CF71CULL,	0x101011925438EF38ULL, 0x20212224A870DF70ULL, 0x4142444850E0BFE0ULL, 0x82848890A0C07FC0ULL,
	0x2111090503FE0305ULL, 0x4222120A07FD070AULL, 0x844424150EFB0E15ULL, 0x0888492A1CF71C2AULL,	0x1011925438EF3854ULL, 0x212224A870DF70A8ULL, 0x42444850E0BFE050ULL, 0x848890A0C07FC0A0ULL,
	0x11090503FE030509ULL, 0x22120A07FD070A12ULL, 0x4424150EFB0E1524ULL, 0x88492A1CF71C2A49ULL,	0x11925438EF385492ULL, 0x2224A870DF70A824ULL, 0x444850E0BFE05048ULL, 0x8890A0C07FC0A090ULL,
	0x090503FE03050911ULL, 0x120A07FD070A1222ULL, 0x24150EFB0E152444ULL, 0x492A1CF71C2A4988ULL,	0x925438EF38549211ULL, 0x24A870DF70A82422ULL, 0x4850E0BFE0504844ULL, 0x90A0C07FC0A09088ULL,
	0x0503FE0305091121ULL, 0x0A07FD070A122242ULL, 0x150EFB0E15244484ULL, 0x2A1CF71C2A498808ULL,	0x5438EF3854921110ULL, 0xA870DF70A8242221ULL, 0x50E0BFE050484442ULL, 0xA0C07FC0A0908884ULL,
	0x03FE030509112141ULL, 0x07FD070A12224282ULL, 0x0EFB0E1524448404ULL, 0x1CF71C2A49880808ULL,	0x38EF385492111010ULL, 0x70DF70A824222120ULL, 0xE0BFE05048444241ULL, 0xC07FC0A090888482ULL,
	0xFE03050911214181ULL, 0xFD070A1222428202ULL, 0xFB0E152444840404ULL, 0xF71C2A4988080808ULL,	0xEF38549211101010ULL, 0xDF70A82422212020ULL, 0xBFE0504844424140ULL, 0x7FC0A09088848281ULL
};
static const unsigned long long to_check[64] = { // Fields that have to be checked to find out if the input-field could be played
	0x81412111090500FCULL, 0x02824222120A00F8ULL, 0x04048444241500F1ULL, 0x08080888492A00E3ULL,	0x10101011925400C7ULL, 0x2020212224A8008FULL, 0x404142444850001FULL, 0x8182848890A0003FULL,
	0x412111090500FC00ULL, 0x824222120A00F800ULL, 0x048444241500F100ULL, 0x080888492A00E300ULL,	0x101011925400C700ULL, 0x20212224A8008F00ULL, 0x4142444850001F00ULL, 0x82848890A0003F00ULL,
	0x2111090500FC0005ULL, 0x4222120A00F8000AULL, 0x8444241500F00011ULL, 0x0888492A00E30022ULL,	0x1011925400C70044ULL, 0x212224A8000F0088ULL, 0x42444850001F0050ULL, 0x848890A0003F00A0ULL,
	0x11090500FC000509ULL, 0x22120A00F8000A12ULL, 0x44241500F0001524ULL, 0x88492A00E3002A49ULL,	0x11925400C7005492ULL, 0x2224A8000F00A824ULL, 0x444850001F005048ULL, 0x8890A0003F00A090ULL,
	0x090500FC00050911ULL, 0x120A00F8000A1222ULL, 0x241500F000152444ULL, 0x492A00E3002A4988ULL,	0x925400C700549211ULL, 0x24A8000F00A82422ULL, 0x4850001F00504844ULL, 0x90A0003F00A09088ULL,
	0x0500FC0005091121ULL, 0x0A00F8000A122242ULL, 0x1100F00015244484ULL, 0x2200E3002A498808ULL,	0x4400C70054921110ULL, 0x88000F00A8242221ULL, 0x50001F0050484442ULL, 0xA0003F00A0908884ULL,
	0x00FC000509112141ULL, 0x00F8000A12224282ULL, 0x00F1001524448404ULL, 0x00E3002A49880808ULL,	0x00C7005492111010ULL, 0x008F00A824222120ULL, 0x001F005048444241ULL, 0x003F00A090888482ULL,
	0xFC00050911214181ULL, 0xF8000A1222428202ULL, 0xF100152444840404ULL, 0xE3002A4988080808ULL,	0xC700549211101010ULL, 0x8F00A82422212020ULL, 0x1F00504844424140ULL, 0x3F00A09088848281ULL
};
static const unsigned long long affectable[64] = { // Fields that could be affected by playing the input-field
	0x004121110905037EULL, 0x00024222120A067CULL, 0x0004044424140E7AULL, 0x00080808482A1C76ULL,	0x001010101254386EULL, 0x002020222428705EULL, 0x004042444850603EULL, 0x0082848890A0C07EULL,
	0x0021110905037E00ULL, 0x004222120A067C00ULL, 0x00044424140E7A00ULL, 0x000808482A1C7600ULL,	0x0010101254386E00ULL, 0x0020222428705E00ULL, 0x0042444850603E00ULL, 0x00848890A0C07E00ULL,
	0x00110905037E0300ULL, 0x0022120A067C0600ULL, 0x004424140E7A0E00ULL, 0x0008482A1C761C00ULL,	0x00101254386E3800ULL, 0x00222428705E7000ULL, 0x00444850603E6000ULL, 0x008890A0C07EC000ULL,
	0x000905037E030500ULL, 0x00120A067C060A00ULL, 0x0024140E7A0E1400ULL, 0x00482A1C761C2A00ULL,	0x001254386E385400ULL, 0x002428705E702800ULL, 0x004850603E605000ULL, 0x0090A0C07EC0A000ULL,
	0x0005037E03050900ULL, 0x000A067C060A1200ULL, 0x00140E7A0E142400ULL, 0x002A1C761C2A4800ULL,	0x0054386E38541200ULL, 0x0028705E70282400ULL, 0x0050603E60504800ULL, 0x00A0C07EC0A09000ULL,
	0x00037E0305091100ULL, 0x00067C060A122200ULL, 0x000E7A0E14244400ULL, 0x001C761C2A480800ULL,	0x00386E3854121000ULL, 0x00705E7028242200ULL, 0x00603E6050484400ULL, 0x00C07EC0A0908800ULL,
	0x007E030509112100ULL, 0x007C060A12224200ULL, 0x007A0E1424440400ULL, 0x00761C2A48080800ULL,	0x006E385412101000ULL, 0x005E702824222000ULL, 0x003E605048444200ULL, 0x007EC0A090888400ULL,
	0x7E03050911214100ULL, 0x7C060A1222420200ULL, 0x7A0E142444040400ULL, 0x761C2A4808080800ULL,	0x6E38541210101000ULL, 0x5E70282422202000ULL, 0x3E60504844424000ULL, 0x7EC0A09088848200ULL
};

#ifdef SIGN_OR_ZERO_BRANCHING
	template <class T> T SIGN_OR_ZERO(const T & Value)
	{
		if (Value > 0)
			return 1;
		else if (0 > Value)
			return -1;
		else
			return 0;
	}
#elif defined(SIGN_OR_ZERO_NO_BRANCHING)
	template <class T> inline T SIGN_OR_ZERO(const T Value) { return (Value != 0) | (Value >> sizeof(T) * CHAR_BIT - 1); }
	template <> inline int SIGN_OR_ZERO(const int Value) { return (Value != 0) | (Value >> 31); }
	template <> inline char SIGN_OR_ZERO(const char Value) { return (Value != 0) | (Value >> 7); }
	template <> inline long long SIGN_OR_ZERO(const long long Value) { return (Value != 0) | (Value >> 63); }
#endif

#if defined(ABS_BRANCHING)
	template<class T> inline T ABS(T value) { return value > 0 ? value : -value; }
#elif defined(ABS_NO_BRANCHING)
	template<class T> inline T ABS(const T & Value)
	{
		const T mask = Value >> sizeof(T) * CHAR_BIT - 1;
		return (Value + mask) ^ mask;
	}
	template<> inline int ABS(const int & Value)
	{
		const int mask = Value >> 31;
		return (Value + mask) ^ mask;
	}
	template<> inline char ABS(const char & Value)
	{
		const char mask = Value >> 7;
		return (Value + mask) ^ mask;
	}
#endif

#if defined(MAX_BRANCHING)
	template<class T> inline T MAX(T a, T b) { return a < b ? b : a; }
#elif defined(MAX_NO_BRANCHING)
	template<class T> inline T MAX(T a, T b) { return a - ((a - b) & ((a - b) >> (sizeof(T) * CHAR_BIT - 1))); }
	template<> inline int MAX(int a, int b) { return a - ((a - b) & ((a - b) >> 31)); }
	template<> inline char MAX(char a, char b) { return a - ((a - b) & ((a - b) >> 7)); }
#endif

#if defined(MIN_BRANCHING)
	template<class T> inline T MIN(T a, T b) { return b < a ? b : a; }
#elif defined(MIN_NO_BRANCHING)
	template<class T> inline T MIN(T a, T b) { return b + ((a - b) & ((a - b) >> (sizeof(T) * CHAR_BIT - 1))); }
	template<> inline int MIN(int a, int b) { return b + ((a - b) & ((a - b) >> 31)); }
	template<> inline char MIN(char a, char b) { return b + ((a - b) & ((a - b) >> 7)); }
#endif

	template<class T> inline T BIND(T value, T alpha, T beta)
	{
		if (value > beta) return beta;
		if (value < alpha) return alpha;
		return value;
	}
	
#if defined(BIT_SCAN_LS1B_use_LOOKUP)
	inline unsigned char BIT_SCAN_LS1B(unsigned long long const &Input){ return index64[((Input & -Input) * 0x07EDD5E59A4E28C2UL) >> 58]; }
#elif defined(BIT_SCAN_LS1B_use_POP_COUNT)
	inline unsigned char BIT_SCAN_LS1B(unsigned long long const &Input){ return POP_COUNT((Input & -Input) - 1); }
#elif defined(BIT_SCAN_LS1B_use_INTRINSIC_BitScanForward32)
	inline unsigned char BIT_SCAN_LS1B(unsigned long long const &Input){
		unsigned long index;
		assert(Input != 0);
		if (_BitScanForward(&index, static_cast<unsigned long>(Input)))
			return index;
		else
		{
			_BitScanForward(&index, static_cast<unsigned long>(Input >> 32));
			return index + 32;
		}
	}
#elif defined(BIT_SCAN_LS1B_use_INTRINSIC_BitScanForward64)
	#if defined(__GNUC__) && defined(__LP64__)
		inline unsigned char BIT_SCAN_LS1B(unsigned long long & Input)
		{
			unsigned long long Ret;
			assert(Input != 0);
			__asm__
			(
				"bsfq %[Input], %[Ret]"
				:[Ret] "=r" (Ret)
				:[Input] "mr" (Input)
			);
			return static_cast<unsigned char>(Ret);
		}
	#else
		inline unsigned long BIT_SCAN_LS1B(const unsigned long long Input){
			unsigned long index;
			assert(Input != 0);
			_BitScanForward64(&index, Input);
			return index;
		}
		inline unsigned long BIT_SCAN_MS1B(const unsigned long long Input){
			unsigned long index;
			assert(Input != 0);
			_BitScanReverse64(&index, Input);
			return index;
		}
	#endif
#endif

inline void REMOVE_LS1B(unsigned long long & Input){ Input &= (Input - 1); }
inline unsigned long long GET_LS1B(const unsigned long long Input){ return Input & -Input; }

inline void REVERSE_BITORDER(unsigned long long & v)
{
	v = ((v >>  1) & 0x5555555555555555ULL) | ((v & 0x5555555555555555ULL) <<  1);
	v = ((v >>  2) & 0x3333333333333333ULL) | ((v & 0x3333333333333333ULL) <<  2);
	v = ((v >>  4) & 0x0F0F0F0F0F0F0F0FULL) | ((v & 0x0F0F0F0F0F0F0F0FULL) <<  4);
	v = ((v >>  8) & 0x00FF00FF00FF00FFULL) | ((v & 0x00FF00FF00FF00FFULL) <<  8);
	v = ((v >> 16) & 0x0000FFFF0000FFFFULL) | ((v & 0x0000FFFF0000FFFFULL) << 16);
	v = ( v >> 32                         ) | ( v                          << 32);
}

inline const char * const FIELD_NAME(const char Index) { return field_name[Index]; }
inline unsigned char QUADRANT_ID(const char Index) { return quadrant_id_4_bit[Index]; }
inline unsigned char QUADRANT_ID_2_BIT(const char Index) { return quadrant_id_2_bit[Index]; }
inline unsigned long long QUADRANT(const char Index) { return quadrant[Index]; }
inline int TRIVIAL_EVALUATION(const char Index) { return trivial_evaluation[Index]; }
inline char EVALUATION_ORDER(const char Index) { return evaluation_order[Index]; }
inline char STABILITY_CUTOFF_LIMIT(const char Index) { return stability_cutoff_limits[Index]; }
inline int MAX_DIFFERENCE_GAIN(const int Index) { return max_difference_1[Index]; }
inline unsigned long long NEIGHBOUR(const int Index) { return neighbour[Index]; }
unsigned long long NEIGHBOUR(const unsigned long long BitBoard);
inline unsigned long long LINES(const int Index){ return lines[Index]; }
unsigned long long LINES(unsigned long long BitBoard);
inline unsigned long long TO_CHECK(const int Index){ return to_check[Index]; }
unsigned long long TO_CHECK(unsigned long long BitBoard);
inline unsigned long long AFFECTABLE(const int Index){ return affectable[Index]; }
unsigned long long AFFECTABLE(unsigned long long BitBoard);
unsigned long long diagonal_flip(unsigned long long b);
unsigned long long codiagonal_flip(unsigned long long b);
unsigned long long vertical_flip(unsigned long long b);
unsigned long long horizontal_flip(unsigned long long b);
std::string SCIENTIFIC_NOTATION(double Number);
std::string time_format(const std::chrono::milliseconds duration);
std::string short_time_format(std::chrono::duration<long long, std::pico> duration);
void print_board(const unsigned long long, const unsigned long long);
void print_progressbar(const int width, const float fraction);
void print_progressbar_percentage(const int width, const float fraction);