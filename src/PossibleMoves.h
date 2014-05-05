#pragma once
#include <string>

inline std::string PossibleMoves_version() { return std::string("Version 3.0 ").append(__DATE__).append(" Advanced Kogge-Stone parallel prefix"); }
bool HasMoves(const unsigned long long P, const unsigned long long O);
unsigned long long PossibleMoves(const unsigned long long P, const unsigned long long O);