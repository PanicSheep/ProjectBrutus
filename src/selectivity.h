#pragma once
#include <string>

struct CSelectivity
{
	float T;
	std::string percentile;
	CSelectivity(const float T, const std::string percentile) : T(T), percentile(percentile) {}
};

static const CSelectivity SelectivityTable[10] = {
	CSelectivity(99.f, ""),    // 0
	CSelectivity(3.3f, "99%"), // 1
	CSelectivity(2.6f, "98%"), // 2
	CSelectivity(2.0f, "95%"), // 3
	CSelectivity(1.5f, "87%"), // 4
	CSelectivity(1.2f, "77%"), // 5
	CSelectivity(1.1f, "73%"), // 6
	CSelectivity(0.7f, "52%"), // 7
	CSelectivity(0.5f, "38%"), // 8
	CSelectivity(0.2f, "16%"), // 9
};