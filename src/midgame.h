#include "endgame.h"

static const unsigned char selectivity[5][4] = { // D, d, sigma, percentile
	{ 0,  0, 64, 100}, // no selectivity
	{11,  8, 64,    }, // end cut
	{11,  8, 64,  99.9}, // 3.3
	{ 8,  4, 11,  99}, // 2.6
	{  ,   , 11,  95}, // 2.0 sigma
	{  ,   , 11,  87}, // 1.5 sigma
	{  ,   , 11,  73}, // 1.1 sigma
	{  ,   , 11,  68}, // 1.0 sigma
	
};