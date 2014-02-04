#pragma once

// Edax          can do it in 31.0 clock cycles.
// ProjectBrutus can do it in 32.8 clock cycles

namespace CountLastFlip
{
	extern unsigned char (*CLF_fp[64])(const unsigned long long);
}

inline unsigned char count_last_flip(const unsigned long long P, const unsigned char c) { return CountLastFlip::CLF_fp[c](P); }