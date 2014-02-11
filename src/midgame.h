#pragma once
#include "endgame.h"
#include "utility.h"

class CSearch;

//static const unsigned char selectivity[5][4] = { // D, d, sigma, percentile
//	{ 0,  0, 64, 100}, // no selectivity
//	{11,  8, 64,    }, // end cut
//	{11,  8, 64,  99.9}, // 3.3
//	{ 8,  4, 11,  99}, // 2.6
//	{  ,   , 11,  95}, // 2.0 sigma
//	{  ,   , 11,  87}, // 1.5 sigma
//	{  ,   , 11,  73}, // 1.1 sigma
//	{  ,   , 11,  68}, // 1.0 sigma
//	
//};

inline int LimitedDepth_0(const CActiveConfigurations & actives, const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter)
{
	++NodeCounter;
	return actives.EvaluateFeatures(P, O);
}
inline int LimitedDepth_0(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter)
{
	++NodeCounter;
	return EvaluateFeatures(P, O);
}
int LimitedDepth_1(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, int alpha, int beta);
int LimitedDepth_2(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, int beta);

int PVS_LimitedDepth(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, int beta, int depth, unsigned char * pline);

//int D1, d1, D2, d2, DE, dE;
int MPC(CSearch & search, const unsigned long long P, const unsigned long long O, int alpha, int beta, int depth, unsigned char * pline);