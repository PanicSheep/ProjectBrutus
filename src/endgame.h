#pragma once
#include "count_last_flip.h"
#include "features.h"
#include "flip.h"
#include "move.h"
#include "position.h"
#include "search.h"
#include "utility.h"
#include <iostream>

class CSearch;

// For benchmarking purposes
int EvaluateExact(const unsigned long long P, const unsigned long long O, unsigned long long & NodeCounter, const unsigned long long NumberOfEmptyStones);
void EvaluateExact(CSearch & search);
void EvaluateEndCut(CSearch & search);
void EvaluateLimitedDepth(CSearch & search);
void EvaluateMPC(CSearch & search);