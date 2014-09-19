#pragma once
#include <cassert>
#include <intrin.h>

// #####################################
// ##    CPU specific optimizations   ##
// #####################################
//#define YORKFIELD
#define HASWELL

//#define HAS_MMX
//#define HAS_SSE
//#define HAS_SSE2
//#define HAS_SSE3
//#define HAS_SSSE3
//#define HAS_SSE4_1
//#define HAS_SSE4_2
//#define HAS_AVX
//#define HAS_AVX2
//#define HAS_BMI1
//#define HAS_BMI2

#define CACHE_LINE_SIZE 64
// #####################################

// CPU instruction set implications
#ifdef YORKFIELD
	#define HAS_MMX
	#define HAS_SSE
	#define HAS_SSE2
	#define HAS_SSE3
	#define HAS_SSSE3
	#define HAS_SSE4_1
#endif
#ifdef HASWELL
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
#endif

// CPU instruction implications
#if defined(HAS_SSE4_2)
	#define HAS_POPCNT // Population count
#endif
#if defined(HAS_BMI1)
	#define HAS_BEXTR // Bit Field Extract
	#define HAS_BLSI // Extract Lowest Set Isolated Bit (x & -x)
	#define HAS_BLSMASK // Get mask up to lowest set bit (x ^ (x - 1))
	#define HAS_BLSR // Reset lowest set bit (x & (x - 1))
#endif
#if defined(HAS_BMI2)
	#define HAS_PDEP // Parallel bits deposit
	#define HAS_PEXT // Parallel bits extract
#endif

// alignas work-around
#if defined(_MSC_VER)
#define alignas(A) __declspec(align(A))
#elif (__GNUC__==4) && (__GNUC_MINOR__ <= 7) 
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
#if defined(_MSC_VER)
	#pragma intrinsic(_BitScanForward64)
	#pragma intrinsic(_BitScanReverse64)

	FORCE_INLINE unsigned char BitScanLSB(unsigned long * index, const unsigned long long mask) { return _BitScanForward64(index, mask); }
	FORCE_INLINE unsigned long BitScanLSB(const unsigned long long mask){
		unsigned long index;
		assert(mask != 0);
		_BitScanForward64(&index, mask);
		return index;
	}
	FORCE_INLINE unsigned char BitScanMSB(unsigned long * index, const unsigned long long mask) { return _BitScanReverse64(index, mask); }
	FORCE_INLINE unsigned long BitScanMSB(const unsigned long long mask){
		unsigned long index;
		assert(mask != 0);
		_BitScanReverse64(&index, mask);
		return index;
	}
#elif defined(__GNUC__)
	inline unsigned long BitScanLSB(unsigned long long & b){
		unsigned long long Ret;
		assert(b != 0);
		__asm__
		(
			"bsfq %[Input], %[Ret]"
			:[Ret] "=r" (Ret)
			:[b] "mr" (b)
		);
		return static_cast<unsigned long>(Ret);
	}
	inline unsigned long BitScanMSB(unsigned long long & b){
		unsigned long long Ret;
		assert(b != 0);
		__asm__
		(
			"bsrq %[Input], %[Ret]"
			:[Ret] "=r" (Ret)
			:[b] "mr" (b)
		);
		return static_cast<unsigned long>(Ret);
	}
#endif

#ifdef HAS_BLSI
	FORCE_INLINE unsigned long long GetLSB(const unsigned long long b){ return _blsi_u64(b); }
#else
	FORCE_INLINE unsigned long long GetLSB(const unsigned long long b){ return b & -b; }
#endif

FORCE_INLINE unsigned long long GetMSB(const unsigned long long b){ return 1ULL << BitScanMSB(b); }

#ifdef HAS_BLSR
	FORCE_INLINE void RemoveLSB(unsigned long long & b){ b = _blsr_u64(b); }
#else
	FORCE_INLINE void RemoveLSB(unsigned long long & b){ b &= (b - 1); }
#endif

FORCE_INLINE void RemoveMSB(unsigned long long & b){ b ^= GetMSB(b); }


// PopCount
#ifdef HAS_POPCNT
	#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
		FORCE_INLINE unsigned long long PopCount(const unsigned long long b) { return _mm_popcnt_u64(b); }
		FORCE_INLINE unsigned long long PopCount_max15(const unsigned long long b) { return PopCount(b); }
	#else
		FORCE_INLINE unsigned long long PopCount(unsigned long long b){
			__asm__("popcnt %1, %0" : "=r" (b) : "r" (b));
			return b;
		}
		FORCE_INLINE unsigned long long PopCount_max15(unsigned long long b) { PopCount(b); }
	#endif
#else
	inline unsigned long long PopCount(unsigned long long b){
		b -=  (b >> 1) & 0x5555555555555555ULL;
		b  = ((b >> 2) & 0x3333333333333333ULL) + (b & 0x3333333333333333ULL);
		b  = ((b >> 4) + b) & 0x0F0F0F0F0F0F0F0FULL;
		return (b * 0x0101010101010101ULL) >> 56;
	}
	inline unsigned long long PopCount_max15(unsigned long long b){
		b -=  (b >> 1) & 0x5555555555555555ULL;
		b  = ((b >> 2) & 0x3333333333333333ULL) + (b & 0x3333333333333333ULL);
		return (b * 0x1111111111111111ULL) >> 60;
	}
#endif


// PDep
#ifdef HAS_PDEP
	FORCE_INLINE unsigned long long PDep(const unsigned long long src, const unsigned long long mask) { return _pdep_u64(src, mask); }
#else
	inline unsigned long long PDep(unsigned long long src, unsigned long long mask){
		unsigned long long res = 0;
		for (unsigned long long bb = 1; mask; bb += bb)
		{
			if (src & bb)
				res |= mask & -mask;
			mask &= mask - 1;
		}
		return res;
	}
#endif


// PExt
#ifdef HAS_PEXT
	FORCE_INLINE unsigned long long PExt(const unsigned long long src, const unsigned long long mask) { return _pext_u64(src, mask); }
#else
	inline unsigned long long PExt(unsigned long long src, unsigned long long mask){
		unsigned long long res = 0;
		for (unsigned long long bb = 1; mask; bb += bb)
		{
			if (src & mask & -mask)
				res |= bb;
			mask &= mask - 1;
		}
		return res;
	}
#endif


// BExtr
#ifdef HAS_BEXTR
	FORCE_INLINE unsigned long long BExtr(const unsigned long long src, const unsigned int start, unsigned int len) { return _bextr_u64(src, start, len); }
#else
	FORCE_INLINE unsigned long long BExtr(const unsigned long long src, const unsigned int start, unsigned int len) { return (src >> start) & ((1ULL << len)-1); }
#endif