#pragma once
#include <cmath>
#include <string>
#include <sstream>
#include <cassert>
#include <chrono>
#include <ctime>
#include <emmintrin.h>
#include <iostream>
#include "bitmanipulations.h"

template <typename T> inline T ABS(const T & a) { return a > 0 ? a : -a; }
template <typename T> inline T MIN(const T & a, const T & b) { return a < b ? a : b; }
template <typename T> inline T MAX(const T & a, const T & b) { return a > b ? a : b; }
template <typename T> inline T BIND(const T & a, const T & min, const T & max) { return a < min ? min : (a > max ? max : a); }

static const std::string field_name[65] = {
	"A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1",
	"A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2",
	"A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3",
	"A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4",
	"A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5",
	"A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6",
	"A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7",
	"A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8", "--"
};
static const unsigned char field_type[64] = {   
	0, 1, 2, 3, 3, 2, 1, 0,
	1, 4, 5, 6, 6, 5, 4, 1,
	2, 5, 7, 8, 8, 7, 5, 2,
	3, 6, 8, 9, 9, 8, 6, 3,
	3, 6, 8, 9, 9, 8, 6, 3,
	2, 5, 7, 8, 8, 7, 5, 2,
	1, 4, 5, 6, 6, 5, 4, 1,
	0, 1, 2, 3, 3, 2, 1, 0
};
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

void REVERSE_BITORDER(unsigned long long & v);
inline const std::string FIELD_NAME(const char Index) { return field_name[Index]; }
unsigned char FIELD_INDEX(const std::string & s);
inline unsigned char QUADRANT_ID(const char Index) { return quadrant_id_4_bit[Index]; }
inline unsigned char QUADRANT_ID_2_BIT(const char Index) { return quadrant_id_2_bit[Index]; }
inline unsigned long long QUADRANT(const char Index) { return quadrant[Index]; }
inline int MAX_DIFFERENCE_GAIN(const int Index) { return max_difference_1[Index]; }
inline unsigned long long NEIGHBOUR(const int Index) { return neighbour[Index]; }
unsigned long long NEIGHBOUR(const unsigned long long BitBoard);
inline unsigned long long AFFECTABLE(const int Index){ return affectable[Index]; }
unsigned long long AFFECTABLE(unsigned long long BitBoard);
unsigned long long diagonal_flip(unsigned long long b);
unsigned long long codiagonal_flip(unsigned long long b);
unsigned long long vertical_flip(unsigned long long b);
unsigned long long horizontal_flip(unsigned long long b);
std::string SCIENTIFIC_NOTATION(double Number);
std::string time_format(const std::chrono::milliseconds duration);
std::string short_time_format(std::chrono::duration<long long, std::pico> duration);
std::string board(const unsigned long long P, const unsigned long long O);
std::string board2D(const unsigned long long P, const unsigned long long O);
void print_board(const unsigned long long, const unsigned long long);
void print_board(const unsigned long long P, const unsigned long long O, const unsigned long long PossibleMoves);
void print_progressbar(const int width, const float fraction);
std::string progressbar_percentage(const int width, const float fraction);
std::string ThousandsSeparator(unsigned long long n);
std::string DateTimeNow();