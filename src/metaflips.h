#include <iostream>

const unsigned long long T_BOARDER = 0xFF00000000000000ULL;
const unsigned long long B_BOARDER = 0x00000000000000FFULL;
const unsigned long long L_BOARDER = 0x8080808080808080ULL;
const unsigned long long R_BOARDER = 0x0101010101010101ULL;
const unsigned long long TB_BOARDER = T_BOARDER | B_BOARDER;
const unsigned long long LR_BOARDER = L_BOARDER | R_BOARDER;
const unsigned long long BOARDER = TB_BOARDER | LR_BOARDER;