#pragma once
#include <atomic>
#include <algorithm>
#include <array>
#include <iostream>
#include <memory.h>
#include <numeric>
#include <omp.h>
#include <stdio.h>
#include <vector>
#include "datamanipulation.h"
#include "features.h"
#include "position.h"
#include "statistics.h"

inline double dot(const std::vector<double>& a, const std::vector<double>& b)
{
	assert(a.size() == b.size());
	double sum = 0.0;
	const std::size_t size = a.size();
	for (std::size_t i = 0; i < size; ++i)
		sum += a[i] * b[i];
	return sum;
}

inline double dot(const std::vector<double>& a)
{ 
	double sum = 0.0;
	const std::size_t size = a.size();
	for (std::size_t i = 0; i < size; ++i)
		sum += a[i] * a[i];
	return sum;
}