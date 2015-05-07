#pragma once
#include <cstdint>

#if defined(_MSC_VER)
	#include <intrin.h>
#elif defined(__GNUC__)
	#include <x86intrin.h>
#else
	#error compiler not supported
#endif

// Predefined macros hell:
//
// __GNUC__           Compiler is gcc, Clang or Intel on Linux
// __INTEL_COMPILER   Compiler is Intel
// _MSC_VER           Compiler is MSVC or Intel on Windows
// _WIN32             Building on Windows (any)
// _WIN64             Building on Windows 64 bit

// #####################################
// ##    CPU specific optimizations   ##
// #####################################
//#define YORKFIELD
//#define HASWELL_ULT
//#define HASWELL_DT
//#define VISHERA

//#define HAS_MMX
//#define HAS_SSE
//#define HAS_SSE2
//#define HAS_SSE3
//#define HAS_SSSE3
//#define HAS_SSE4_1
//#define HAS_SSE4_2
//#define HAS_SSE4a
//#define HAS_AVX
//#define HAS_AVX2
//#define HAS_BMI1
//#define HAS_BMI2
//#define HAS_ABM
//#define HAS_TBM

#define CACHE_LINE_SIZE 64
// #####################################

// CPUs instruction sets
#ifdef YORKFIELD
	#define HAS_MMX
	#define HAS_SSE
	#define HAS_SSE2
	#define HAS_SSE3
	#define HAS_SSSE3
	#define HAS_SSE4_1
#endif
#ifdef HASWELL_ULT
	#define HAS_MMX
	#define HAS_SSE
	#define HAS_SSE2
	#define HAS_SSE3
	#define HAS_SSSE3
	#define HAS_SSE4_1
	#define HAS_SSE4_2
	#define HAS_AVX
	#define HAS_AVX2
	#define HAS_POPCNT
	#define HAS_LZCNT
#endif
#ifdef HASWELL_DT
	#define HAS_MMX
	#define HAS_SSE
	#define HAS_SSE2
	#define HAS_SSE3
	#define HAS_SSSE3
	#define HAS_SSE4_1
	#define HAS_SSE4_2
	#define HAS_AVX
	#define HAS_AVX2
	#define HAS_BMI1
	#define HAS_BMI2
	#define HAS_POPCNT
	#define HAS_LZCNT
#endif
#ifdef VISHERA
	#define HAS_MMX
	#define HAS_SSE
	#define HAS_SSE2
	#define HAS_SSE3
	#define HAS_SSSE3
	#define HAS_SSE4_1
	#define HAS_SSE4_2
	#define HAS_SSE4a
	#define HAS_AVX
	#define HAS_BMI1
	#define HAS_ABM
	#define HAS_TBM
#endif

// CPU instruction implications
#ifdef HAS_BMI1
	#define HAS_BEXTR	// Bit Field Extract
	#define HAS_BLSI	// Extract Lowest Set Isolated Bit	(x & -x)
	#define HAS_BLSMASK	// Get mask up to lowest set bit	(x ^ (x - 1))
	#define HAS_BLSR	// Reset lowest set bit			(x & (x - 1))
	#define HAS_LZCNT	// Leading Zero Count
	#define HAS_TZCNT	// Trailing Zero Count
#endif
#ifdef HAS_BMI2
	#define HAS_BZHI // Zero high bits starting with specified bit position
	#define HAS_PDEP // Parallel bits deposit
	#define HAS_PEXT // Parallel bits extract
#endif
#ifdef HAS_ABM
	#define HAS_POPCNT // Population count
	#define HAS_LZCNT // Leading Zero Count
#endif
#ifdef HAS_TBM
	#define HAS_BEXTR	// Bit Field Extract
	#define HAS_BLCFILL	// Fill from lowest clear bit			( x &  (x + 1))
	#define HAS_BLCI	// Isolate lowest clear bit			( x | ~(x + 1))
	#define HAS_BLCIC	// Isolate lowest clear bit and complement	(~x &  (x + 1))
	#define HAS_BLCMASK	// Mask from lowest clear bit			( x ^  (x + 1))
	#define HAS_BLCS	// Set lowest clear bit				( x |  (x + 1))
	#define HAS_BLSFILL	// Fill from lowest set bit			( x |  (x - 1))
	#define HAS_BLSIC	// Isolate lowest set bit and complement	(~x |  (x - 1))
	#define HAS_T1MSKC	// Inverse mask from trailing ones		(~x |  (x + 1))
	#define HAS_TZMSK	// Mask from trailing zeros			(~x &  (x - 1))
#endif


// alignas work-around
#if (__GNUC__ == 4) && (__GNUC_MINOR__ <= 7)
	#define alignas(A) __attribute__((aligned(A)))
#endif

// CACHE_LINE_ALIGNMENT
#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
	#define CACHE_LINE_ALIGNMENT __declspec(align(CACHE_LINE_SIZE))
#else
	#define CACHE_LINE_ALIGNMENT __attribute__((aligned(CACHE_LINE_SIZE)))
#endif


// FORCE_INLINE
#if defined(_MSC_VER)
	#define FORCE_INLINE __forceinline
#elif defined(__GNUC__)
	#define FORCE_INLINE inline __attribute__((always_inline))
#else
	#define FORCE_INLINE inline
#endif


// BitScan
// OUT: mask == 0 results in undefined index
#if defined(_MSC_VER)
	#pragma intrinsic(_BitScanForward64)
	#pragma intrinsic(_BitScanForward)
	#pragma intrinsic(_BitScanReverse64)
	#pragma intrinsic(_BitScanReverse)

	FORCE_INLINE unsigned char BitScanLSB(unsigned long * index, const uint64_t mask) { return _BitScanForward64(index, mask); }
	FORCE_INLINE unsigned char BitScanLSB(unsigned long * index, const uint32_t mask) { return _BitScanForward  (index, mask); }
	FORCE_INLINE unsigned long BitScanLSB(const uint64_t mask){	unsigned long index; _BitScanForward64(&index, mask); return index; }
	FORCE_INLINE unsigned long BitScanLSB(const uint32_t mask){	unsigned long index; _BitScanForward  (&index, mask); return index;	}
	template <typename T> FORCE_INLINE unsigned char bsf(unsigned long * index, const T mask) { return BitScanLSB(index, mask); }

	FORCE_INLINE unsigned char BitScanMSB(unsigned long * index, const uint64_t mask) { return _BitScanReverse64(index, mask); }
	FORCE_INLINE unsigned char BitScanMSB(unsigned long * index, const uint32_t mask) { return _BitScanReverse  (index, mask); }
	FORCE_INLINE unsigned long BitScanMSB(const uint64_t mask){ unsigned long index; _BitScanReverse64(&index, mask); return index; }
	FORCE_INLINE unsigned long BitScanMSB(const uint32_t mask){	unsigned long index; _BitScanReverse  (&index, mask); return index; }
	template <typename T> FORCE_INLINE unsigned char bsr(unsigned long * index, const T mask) { return BitScanMSB(index, mask); }
#elif defined(__GNUC__)
	FORCE_INLINE unsigned char BitScanLSB(unsigned long * index, const uint64_t mask) { *index = __builtin_ctzll(mask); return mask; }
	FORCE_INLINE unsigned char BitScanLSB(unsigned long * index, const uint32_t mask) { *index = __builtin_ctz  (mask); return mask; }
	FORCE_INLINE unsigned long BitScanLSB(const uint64_t mask){ return __builtin_ctzll(mask); }
	FORCE_INLINE unsigned long BitScanLSB(const uint32_t mask){ return __builtin_ctz  (mask); }
	template <typename T> FORCE_INLINE unsigned char bsf(unsigned long * index, const T mask) { return BitScanLSB(index, mask); }

	FORCE_INLINE unsigned char BitScanMSB(unsigned long * index, const uint64_t mask) { *index = __builtin_clzll(mask) ^ 63; return mask; }
	FORCE_INLINE unsigned char BitScanMSB(unsigned long * index, const uint32_t mask) { *index = __builtin_clz  (mask) ^ 31; return mask; }
	FORCE_INLINE unsigned long BitScanMSB(const uint64_t mask){ return __builtin_clzll(mask) ^ 63; }
	FORCE_INLINE unsigned long BitScanMSB(const uint32_t mask){ return __builtin_clz  (mask) ^ 31; }
	template <typename T> FORCE_INLINE unsigned char bsr(unsigned long * index, const T mask) { return BitScanMSB(index, mask); }
#endif


// Count Leading Zeros
#if defined(_MSC_VER)
	FORCE_INLINE uint64_t CountLeadingZeros(const uint64_t mask) { return _lzcnt_u64(mask); }
	FORCE_INLINE uint32_t CountLeadingZeros(const uint32_t mask) { return _lzcnt_u32(mask); }
#elif defined(__GNUC__)
	FORCE_INLINE uint64_t CountLeadingZeros(const uint64_t mask) { return __builtin_clzll(mask); }
	FORCE_INLINE uint32_t CountLeadingZeros(const uint32_t mask) { return __builtin_clz  (mask); }
#endif
	template <typename T> FORCE_INLINE T clz(const T mask) { return CountLeadingZeros(mask); }

// Count Trailing Zeros
#if defined(_MSC_VER)
	FORCE_INLINE uint64_t CountTrailingZeros(const uint64_t mask) { return _tzcnt_u64(mask); }
	FORCE_INLINE uint32_t CountTrailingZeros(const uint32_t mask) { return _tzcnt_u32(mask); }
#elif defined(__GNUC__)
	FORCE_INLINE uint64_t CountTrailingZeros(const uint64_t mask) { return __builtin_ctzll(mask); }
	FORCE_INLINE uint32_t CountTrailingZeros(const uint32_t mask) { return __builtin_ctz  (mask); }
#endif
	template <typename T> FORCE_INLINE T ctz(const T mask) { return CountTrailingZeros(mask); }


#ifdef HAS_BLSI
	FORCE_INLINE uint64_t GetLSB(const uint64_t b) { return _blsi_u64(b); }
#else
	FORCE_INLINE uint64_t GetLSB(const uint64_t b) { return b & -b; }
#endif

FORCE_INLINE uint64_t GetMSB(const uint64_t b) { return 1ULL << BitScanMSB(b); }

#ifdef HAS_BLSR
	FORCE_INLINE void RemoveLSB(uint64_t & b) { b = _blsr_u64(b); }
#else
	FORCE_INLINE void RemoveLSB(uint64_t & b) { b &= b - 1; }
#endif

FORCE_INLINE void RemoveMSB(uint64_t & b) { b ^= GetMSB(b); }


// PopCount
#ifdef HAS_POPCNT
	#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
		FORCE_INLINE uint64_t PopCount(const uint64_t b) { return _mm_popcnt_u64(b); }
		FORCE_INLINE uint32_t PopCount(const uint32_t b) { return _mm_popcnt_u32(b); }
		FORCE_INLINE uint64_t PopCount_max15(const uint64_t b) { return PopCount(b); }
		FORCE_INLINE uint32_t PopCount_max15(const uint32_t b) { return PopCount(b); }
	#else
		FORCE_INLINE uint64_t PopCount(uint64_t b){ return __builtin_popcountll(b); }
		FORCE_INLINE uint32_t PopCount(uint32_t b){ return __builtin_popcount  (b); }
		FORCE_INLINE uint64_t PopCount_max15(uint64_t b) { PopCount(b); }
		FORCE_INLINE uint32_t PopCount_max15(uint32_t b) { PopCount(b); }
	#endif
#else
	inline uint64_t PopCount(uint64_t b){
		b -=  (b >> 1) & 0x5555555555555555ULL;
		b  = ((b >> 2) & 0x3333333333333333ULL) + (b & 0x3333333333333333ULL);
		b  = ((b >> 4) + b) & 0x0F0F0F0F0F0F0F0FULL;
		return (b * 0x0101010101010101ULL) >> 56;
	}
	inline uint64_t PopCount_max15(uint64_t b){
		b -=  (b >> 1) & 0x5555555555555555ULL;
		b  = ((b >> 2) & 0x3333333333333333ULL) + (b & 0x3333333333333333ULL);
		return (b * 0x1111111111111111ULL) >> 60;
	}
#endif


// BExtr
#if defined(HAS_BEXTR) || defined(HAS_TBM)
	FORCE_INLINE uint64_t BExtr(const uint64_t src, const unsigned int start, unsigned int len) { return _bextr_u64(src, start, len); }
#else
	FORCE_INLINE uint64_t BExtr(const uint64_t src, const unsigned int start, unsigned int len) { return (src >> start) & ((1ULL << len)-1); }
#endif


// BZHI
#ifdef HAS_BZHI
	FORCE_INLINE uint64_t BZHI(const uint64_t src, const uint64_t index) { return _bzhi_u64(src, index); }
#else
	FORCE_INLINE uint64_t BZHI(const uint64_t src, const uint64_t index) { return src & ((1ULL << index) - 1); }
#endif

#ifdef HAS_BLCFILL
#else
#endif

#ifdef HAS_BLCI
#else
#endif

#ifdef HAS_BLCIC
#else
#endif

#ifdef HAS_BLCMASK
#else
#endif

#ifdef HAS_BLCS
#else
#endif

#ifdef HAS_BLSFILL
#else
#endif

#ifdef HAS_BLSIC
#else
#endif

#ifdef HAS_T1MSKC
#else
#endif

#ifdef HAS_TZMSK
#else
#endif


// PDep
#ifdef HAS_PDEP
	FORCE_INLINE uint64_t PDep(const uint64_t src, const uint64_t mask) { return _pdep_u64(src, mask); }
#else
	inline uint64_t PDep(uint64_t src, uint64_t mask){
		uint64_t res = 0;
		for (uint64_t bb = 1; mask; bb += bb)
		{
			if (src & bb)
				res |= GetLSB(mask);
			RemoveLSB(mask);
		}
		return res;
	}
#endif


// PExt
#ifdef HAS_PEXT
	FORCE_INLINE uint64_t PExt(const uint64_t src, const uint64_t mask) { return _pext_u64(src, mask); }
#else
	inline uint64_t PExt(uint64_t src, uint64_t mask){
		uint64_t res = 0;
		for (uint64_t bb = 1; mask; bb += bb)
		{
			if (src & GetLSB(mask))
				res |= bb;
			RemoveLSB(mask);
		}
		return res;
	}
#endif

// BSWAP
#if defined(_MSC_VER)
	FORCE_INLINE uint64_t BSwap(const uint64_t b) { return _byteswap_uint64(b); }
#elif defined(__GNUC__)
	FORCE_INLINE uint64_t BSwap(const uint64_t b) { return __builtin_bswap64(b); }
#endif
