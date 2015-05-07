#pragma once
#include "BitManipulations.h"

namespace Flip_Fast
{
	extern uint64_t (*flip_fp[])(const uint64_t, const uint64_t);
}

FORCE_INLINE uint64_t flip(const uint64_t P, const uint64_t O, const uint8_t move) { return Flip_Fast::flip_fp[move](P, O); }
