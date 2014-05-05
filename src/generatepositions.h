#pragma once
#include "datamanipulation.h"
#include "position.h"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <omp.h>
#include <random>
#include <set>
#include <string.h>
#include <thread>
#include <vector>

typedef std::pair<unsigned long long, unsigned long long> ULLULL;

void GenerateRandomPositions(const std::string & filename, const unsigned long long N, const int Empties, const bool ETH);

void GeneratePerftPositions(const std::string & filename, const char depth, const bool ETH);