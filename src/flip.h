#pragma once
#include <cstdio>
#include "utility.h"

// Edax          can do it in 40.6 clock cycles
// ProjectBrutus can do it in 44.9 clock cycles

namespace Flip
{
	extern unsigned long long (*flip_fp[])(const unsigned long long, const unsigned long long);

	template <unsigned char O> inline unsigned char OUTFLANK0()
	{
		if ((O & 0x3F) == 0x3F) return 0x20;
		if ((O & 0x1F) == 0x1F) return 0x10;
		if ((O & 0x0F) == 0x0F) return 0x08;
		if ((O & 0x07) == 0x07) return 0x04;
		if ((O & 0x03) == 0x03) return 0x02;
		if ((O & 0x01) == 0x01) return 0x01;
		return 0x00;
	}

	template <unsigned char O>
	inline unsigned char OUTFLANK1()
	{
		if ((O & 0x1F) == 0x1F) return 0x10;
		if ((O & 0x0F) == 0x0F) return 0x08;
		if ((O & 0x07) == 0x07) return 0x04;
		if ((O & 0x03) == 0x03) return 0x02;
		if ((O & 0x01) == 0x01) return 0x01;
		return 0x00;
	}

	template <unsigned char O>
	inline unsigned char OUTFLANK2()
	{
		unsigned char result = 0x00;

			 if ((O & 0x3C) == 0x3C) result = 0x80;
		else if ((O & 0x1C) == 0x1C) result = 0x40;
		else if ((O & 0x0C) == 0x0C) result = 0x20;
		else if ((O & 0x04) == 0x04) result = 0x10;
	
		if ((O & 0x01) == 0x01) return result | 0x01;
		return result;
	}

	template <unsigned char O>
	inline unsigned char OUTFLANK3()
	{
		unsigned char result = 0x00;

			 if ((O & 0x38) == 0x38) result = 0x80;
		else if ((O & 0x18) == 0x18) result = 0x40;
		else if ((O & 0x08) == 0x08) result = 0x20;
	
		if ((O & 0x03) == 0x03) return result | 0x01;
		if ((O & 0x02) == 0x02) return result | 0x02;
		return result;
	}

	template <unsigned char O>
	inline unsigned char OUTFLANK4()
	{
		unsigned char result = 0x00;

			 if ((O & 0x30) == 0x30) result = 0x80;
		else if ((O & 0x10) == 0x10) result = 0x40;
	
		if ((O & 0x07) == 0x07) return result | 0x01;
		if ((O & 0x06) == 0x06) return result | 0x02;
		if ((O & 0x04) == 0x04) return result | 0x04;
		return result;
	}

	template <unsigned char O>
	inline unsigned char OUTFLANK5()
	{
		unsigned char result = 0x00;

		if ((O & 0x20) == 0x20) result = 0x80;
	
		if ((O & 0x0F) == 0x0F) return result | 0x01;
		if ((O & 0x0E) == 0x0E) return result | 0x02;
		if ((O & 0x0C) == 0x0C) return result | 0x04;
		if ((O & 0x08) == 0x08) return result | 0x08;
		return result;
	}

	template <unsigned char O>
	inline unsigned char OUTFLANK6()
	{
		if ((O & 0x1F) == 0x1F) return 0x01;
		if ((O & 0x1E) == 0x1E) return 0x02;
		if ((O & 0x1C) == 0x1C) return 0x04;
		if ((O & 0x18) == 0x18) return 0x08;
		if ((O & 0x10) == 0x10) return 0x10;
		return 0x00;
	}

	template <unsigned char O>
	inline unsigned char OUTFLANK7()
	{
		if ((O & 0x3F) == 0x3F) return 0x01;
		if ((O & 0x3E) == 0x3E) return 0x02;
		if ((O & 0x3C) == 0x3C) return 0x04;
		if ((O & 0x38) == 0x38) return 0x08;
		if ((O & 0x30) == 0x30) return 0x10;
		if ((O & 0x20) == 0x20) return 0x20;
		return 0x00;
	}

	template <unsigned char P>
	inline unsigned char FLIP0()
	{
		if (P == 0x20) return 0x7E >> 1;
		if (P == 0x10) return 0x3E >> 1;
		if (P == 0x08) return 0x1E >> 1;
		if (P == 0x04) return 0x0E >> 1;
		if (P == 0x02) return 0x06 >> 1;
		if (P == 0x01) return 0x02 >> 1;
		return 0x00; // Don't care
	}

	template <unsigned char P>
	inline unsigned char FLIP1()
	{
		if (P == 0x10) return 0x7C >> 1;
		if (P == 0x08) return 0x3C >> 1;
		if (P == 0x04) return 0x1C >> 1;
		if (P == 0x02) return 0x0C >> 1;
		if (P == 0x01) return 0x04 >> 1;
		if (P == 0x00) return 0x00 >> 1;
		return 0x00; // Don't care
	}

	template <unsigned char P>
	inline unsigned char FLIP2()
	{
		if ((P & 0x04) == 0x04) return 0x00;

		if (P == 0x00) return 0x00 >> 1;
		if (P == 0x01) return 0x02 >> 1;
		if (P == 0x10) return 0x08 >> 1;
		if (P == 0x11) return 0x0A >> 1;
		if (P == 0x20) return 0x18 >> 1;
		if (P == 0x21) return 0x1A >> 1;
		if (P == 0x40) return 0x38 >> 1;
		if (P == 0x41) return 0x3A >> 1;
		if (P == 0x80) return 0x78 >> 1;
		if (P == 0x81) return 0x7A >> 1;
		return 0x00; // Don't care
	}

	template <unsigned char P>
	inline unsigned char FLIP3()
	{
		if ((P & 0x08) == 0x08) return 0x00;

		if (P == 0x00) return 0x00 >> 1;
		if (P == 0x01) return 0x06 >> 1;
		if (P == 0x02) return 0x04 >> 1;
		if (P == 0x20) return 0x10 >> 1;
		if (P == 0x21) return 0x16 >> 1;
		if (P == 0x22) return 0x14 >> 1;
		if (P == 0x40) return 0x30 >> 1;
		if (P == 0x41) return 0x36 >> 1;
		if (P == 0x42) return 0x34 >> 1;
		if (P == 0x80) return 0x70 >> 1;
		if (P == 0x81) return 0x76 >> 1;
		if (P == 0x82) return 0x74 >> 1;
		return 0x00; // Don't care
	}

	template <unsigned char P>
	inline unsigned char FLIP4()
	{
		if ((P & 0x10) == 0x10) return 0x00;

		if (P == 0x00) return 0x00 >> 1;
		if (P == 0x01) return 0x0E >> 1;
		if (P == 0x02) return 0x0C >> 1;
		if (P == 0x04) return 0x08 >> 1;
		if (P == 0x40) return 0x20 >> 1;
		if (P == 0x41) return 0x2E >> 1;
		if (P == 0x42) return 0x2C >> 1;
		if (P == 0x44) return 0x28 >> 1;
		if (P == 0x80) return 0x60 >> 1;
		if (P == 0x81) return 0x6E >> 1;
		if (P == 0x82) return 0x6C >> 1;
		if (P == 0x84) return 0x68 >> 1;
		return 0x00; // Don't care
	}

	template <unsigned char P>
	inline unsigned char FLIP5()
	{
		if ((P & 0x20) == 0x20) return 0x00;

		if (P == 0x00) return 0x00 >> 1;
		if (P == 0x01) return 0x1E >> 1;
		if (P == 0x02) return 0x1C >> 1;
		if (P == 0x04) return 0x18 >> 1;
		if (P == 0x08) return 0x10 >> 1;
		if (P == 0x80) return 0x40 >> 1;
		if (P == 0x81) return 0x5E >> 1;
		if (P == 0x82) return 0x5C >> 1;
		if (P == 0x84) return 0x58 >> 1;
		if (P == 0x88) return 0x50 >> 1;
		return 0x00; // Don't care
	}

	template <unsigned char P>
	inline unsigned char FLIP6()
	{
		if (P == 0x00) return 0x00 >> 1;
		if (P == 0x01) return 0x3E >> 1;
		if (P == 0x02) return 0x3C >> 1;
		if (P == 0x04) return 0x38 >> 1;
		if (P == 0x08) return 0x30 >> 1;
		if (P == 0x10) return 0x20 >> 1;
		return 0x00; // Don't care
	}

	template <unsigned char P>
	inline unsigned char FLIP7()
	{
		if (P == 0x00) return 0x00 >> 1;
		if (P == 0x01) return 0x7E >> 1;
		if (P == 0x02) return 0x7C >> 1;
		if (P == 0x04) return 0x78 >> 1;
		if (P == 0x08) return 0x70 >> 1;
		if (P == 0x10) return 0x60 >> 1;
		if (P == 0x20) return 0x40 >> 1;
		return 0x00; // Don't care
	}

	extern const unsigned char OUTFLANK_0[];
	extern const unsigned char OUTFLANK_1[];
	extern const unsigned char OUTFLANK_2[];
	extern const unsigned char OUTFLANK_3[];
	extern const unsigned char OUTFLANK_4[];
	extern const unsigned char OUTFLANK_5[];
	extern const unsigned char OUTFLANK_6[];
	extern const unsigned char OUTFLANK_7[];
	extern const unsigned char FLIPS_0[];
	extern const unsigned char FLIPS_1[];
	extern const unsigned char FLIPS_2[];
	extern const unsigned char FLIPS_3[];
	extern const unsigned char FLIPS_4[];
	extern const unsigned char FLIPS_5[];
	extern const unsigned char FLIPS_6[];
	extern const unsigned char FLIPS_7[];
	
	template <unsigned char I> inline unsigned long long _STRETCH_A1A8() { return ((I * 0x0204081020408100ULL) & 0x0101010101010101ULL) * 0xFFULL; }
	template <unsigned char I> inline unsigned long long _STRETCH_A8A1() { return (((I * 0x0080402010080402ULL) & 0x8080808080808080ULL) >> 7) * 0xFFULL; }
	template <unsigned char I> inline unsigned long long _STRETCH_V   () { return I * 0x0202020202020202ULL; }

	const unsigned long long T_BOARDER = 0xFF00000000000000ULL;
	const unsigned long long B_BOARDER = 0x00000000000000FFULL;
	const unsigned long long L_BOARDER = 0x8080808080808080ULL;
	const unsigned long long R_BOARDER = 0x0101010101010101ULL;
	const unsigned long long TB_BOARDER = T_BOARDER | B_BOARDER;
	const unsigned long long LR_BOARDER = L_BOARDER | R_BOARDER;
	const unsigned long long BOARDER = TB_BOARDER | LR_BOARDER;

	template <int N> inline unsigned long long horizontal() { return 0xFFULL << (N * 8); }
	template <int L> inline unsigned long long vertical() { return 0x0101010101010101ULL << L; }
	template <int L, int N> inline unsigned long long down() { return vertical<L>() >> ((7-N)*8); }
	template <int L, int N> inline unsigned long long up() { return vertical<L>() << (N*8); }

	template <bool B> inline unsigned long long diagonal_2       (const int I);
	template <      > inline unsigned long long diagonal_2<true >(const int I) { return 0x8040201008040201ULL >>  I; }
	template <      > inline unsigned long long diagonal_2<false>(const int I) { return 0x8040201008040201ULL << -I; }
	template <int L, int N> inline unsigned long long diagonal() { return diagonal_2<(L > N)>((L-N)*8); }

	template <bool B> inline unsigned long long codiagonal_2       (const int I);
	template <      > inline unsigned long long codiagonal_2<true >(const int I) { return 0x0102040810204080ULL >>  I; }
	template <      > inline unsigned long long codiagonal_2<false>(const int I) { return 0x0102040810204080ULL << -I; }
	template <int L, int N> inline unsigned long long codiagonal() { return codiagonal_2<(7-N-L > 0)>((7-N-L)*8); }

	template <int L> struct v_line_2
	{
		template <int N> static inline unsigned long long call   () { return 0; }
		template <     > static inline unsigned long long call<0>() { return  diagonal<L,0>() | codiagonal<L,0>(); }
		template <     > static inline unsigned long long call<1>() { return (diagonal<L,0>() | codiagonal<L,0>()) << 8; }
		template <     > static inline unsigned long long call<6>() { return (diagonal<L,7>() | codiagonal<L,7>()) >> 8; }
		template <     > static inline unsigned long long call<7>() { return  diagonal<L,7>() | codiagonal<L,7>(); }
	};
	template <int L, int N> inline unsigned long long v_line() { return v_line_2<L>::call<N>(); }

	template <int N> struct Y_line_2
	{
		template <int L> static inline unsigned long long call   () { return 0; }
		template <     > static inline unsigned long long call<0>() { return    diagonal<0,N>() | down<0,N>(); }
		template <     > static inline unsigned long long call<1>() { return (  diagonal<0,N>() | down<0,N>()) << 1; }
		template <     > static inline unsigned long long call<6>() { return (codiagonal<7,N>() | down<7,N>()) >> 1; }
		template <     > static inline unsigned long long call<7>() { return  codiagonal<7,N>() | down<7,N>(); }
	};
	template <int L, int N> inline unsigned long long Y_line() { return Y_line_2<N>::call<L>(); }

	template <int N> struct lambda_line_2
	{
		template <int L> static inline unsigned long long call   () { return 0; }
		template <     > static inline unsigned long long call<0>() { return  codiagonal<0,N>() | up<0,N>(); }
		template <     > static inline unsigned long long call<1>() { return (codiagonal<0,N>() | up<0,N>()) << 1; }
		template <     > static inline unsigned long long call<6>() { return (  diagonal<7,N>() | up<7,N>()) >> 1; }
		template <     > static inline unsigned long long call<7>() { return    diagonal<7,N>() | up<7,N>(); }
	};
	template <int L, int N> inline unsigned long long lambda_line() { return lambda_line_2<N>::call<L>(); }


	template <int I> inline const unsigned char * OUTFLANK_   ();
	template <     > inline const unsigned char * OUTFLANK_<0>() { return OUTFLANK_0; }
	template <     > inline const unsigned char * OUTFLANK_<1>() { return OUTFLANK_1; }
	template <     > inline const unsigned char * OUTFLANK_<2>() { return OUTFLANK_2; }
	template <     > inline const unsigned char * OUTFLANK_<3>() { return OUTFLANK_3; }
	template <     > inline const unsigned char * OUTFLANK_<4>() { return OUTFLANK_4; }
	template <     > inline const unsigned char * OUTFLANK_<5>() { return OUTFLANK_5; }
	template <     > inline const unsigned char * OUTFLANK_<6>() { return OUTFLANK_6; }
	template <     > inline const unsigned char * OUTFLANK_<7>() { return OUTFLANK_7; }

	template <int I> inline const unsigned char * FLIPS_   ();
	template <     > inline const unsigned char * FLIPS_<0>() { return FLIPS_0; }
	template <     > inline const unsigned char * FLIPS_<1>() { return FLIPS_1; }
	template <     > inline const unsigned char * FLIPS_<2>() { return FLIPS_2; }
	template <     > inline const unsigned char * FLIPS_<3>() { return FLIPS_3; }
	template <     > inline const unsigned char * FLIPS_<4>() { return FLIPS_4; }
	template <     > inline const unsigned char * FLIPS_<5>() { return FLIPS_5; }
	template <     > inline const unsigned char * FLIPS_<6>() { return FLIPS_6; }
	template <     > inline const unsigned char * FLIPS_<7>() { return FLIPS_7; }

	template <int I> inline unsigned char PLUS_1   () { return 1; }
	template <     > inline unsigned char PLUS_1<1>() { return 2; }
	
	template <int I> inline unsigned char PLUS_2   () { return 0; }
	template <     > inline unsigned char PLUS_2<0>() { return 2; }
	template <     > inline unsigned char PLUS_2<1>() { return 3; }

	template <int I> inline unsigned char MASK_1   () { return 0x3FULL; }
	template <     > inline unsigned char MASK_1<1>() { return 0x1FULL; }

	template <int I> inline unsigned char SHIFT_1() { return 56+PLUS_1<I>(); }
	template <int I> inline unsigned char SHIFT_2() { return 56+PLUS_2<I>(); }

	template <int L> inline unsigned long long MULT_V() { return 0x0102040810204080ULL >> L; }
	
	template <int L, int N> inline unsigned long long MULT_752     ();
	template <            > inline unsigned long long MULT_752<0,2>() { return 0x0102040404040404ULL; } // A3
	template <            > inline unsigned long long MULT_752<0,3>() { return 0x0102040808080808ULL; } // A4
	template <            > inline unsigned long long MULT_752<0,4>() { return 0x0102040810101010ULL; } // A5
	template <            > inline unsigned long long MULT_752<0,5>() { return 0x0102040810202020ULL; } // A6
	template <            > inline unsigned long long MULT_752<1,2>() { return MULT_752<0,2>() >> 1; }  // B3
	template <            > inline unsigned long long MULT_752<1,3>() { return MULT_752<0,3>() >> 1; }  // B4
	template <            > inline unsigned long long MULT_752<1,4>() { return MULT_752<0,4>() >> 1; }  // B5
	template <            > inline unsigned long long MULT_752<1,5>() { return MULT_752<0,5>() >> 1; }  // B6
	
	template <int L, int N> inline unsigned long long MULT_853     ();
	template <            > inline unsigned long long MULT_853<7,2>() { return 0x0008080810204081ULL; } // H3
	template <            > inline unsigned long long MULT_853<7,3>() { return 0x0010101010204081ULL; } // H4
	template <            > inline unsigned long long MULT_853<7,4>() { return 0x0020202020204081ULL; } // H5
	template <            > inline unsigned long long MULT_853<7,5>() { return 0x0040404040404081ULL; } // H6
	template <            > inline unsigned long long MULT_853<6,2>() { return MULT_853<7,2>() << 1; }  // G3
	template <            > inline unsigned long long MULT_853<6,3>() { return MULT_853<7,3>() << 1; }  // G4
	template <            > inline unsigned long long MULT_853<6,4>() { return MULT_853<7,4>() << 1; }  // G5
	template <            > inline unsigned long long MULT_853<6,5>() { return MULT_853<7,5>() << 1; }  // G6
	
	template <int N> inline unsigned long long MULT_851   ();
	template <     > inline unsigned long long MULT_851<2>() { return 0x2020201008040201ULL; }
	template <     > inline unsigned long long MULT_851<3>() { return 0x1010101008040201ULL; }
	template <     > inline unsigned long long MULT_851<4>() { return 0x0808080808040201ULL; }
	template <     > inline unsigned long long MULT_851<5>() { return 0x0404040404040201ULL; }
	
	template <int N> inline unsigned long long MULT_952   ();
	template <     > inline unsigned long long MULT_952<2>() { return 0x0402010101010101ULL; }
	template <     > inline unsigned long long MULT_952<3>() { return 0x0804020101010101ULL; }
	template <     > inline unsigned long long MULT_952<4>() { return 0x1008040201010101ULL; }
	template <     > inline unsigned long long MULT_952<5>() { return 0x2010080402010101ULL; }

	template <int L, int N> inline unsigned long long MASK_H() { return horizontal<N>(); }
	template <int L, int N> inline unsigned long long BB_H() { return (1ULL << (N*8+L)) + 1; }

	template <int L, int N> inline unsigned long long O_H(const unsigned long long O) { return (O >> (N*8+PLUS_1<L>())) & MASK_1<L>(); }
	template <int L, int N> inline unsigned long long P_H(const unsigned long long P) { return  P >> (N*8+PLUS_2<L>()); }
	template <int L, int N> inline unsigned long long FLIPS_H(const unsigned long long P, const unsigned long long O) { return static_cast<unsigned long long>(FLIPS_<L>()[OUTFLANK_<L>()[O_H<L,N>(O)] & P_H<L,N>(P)]) << (N*8+1); }

	template <int L, int N> inline unsigned long long O_V(const unsigned long long O) { return ((O & vertical<L>() & ~TB_BOARDER) * MULT_V<L>()) >> SHIFT_1<N>(); }
	template <int L, int N> inline unsigned long long P_V(const unsigned long long P) { return ((P & vertical<L>())               * MULT_V<L>()) >> SHIFT_2<N>(); }
	template <int L, int N> inline unsigned long long FLIPS_V(const unsigned long long P, const unsigned long long O) { return STRETCH_A1A8[FLIPS_<N>()[OUTFLANK_<N>()[O_V<L,N>(O)] & P_V<L,N>(P)]] & vertical<L>(); }

	template <int L, int N> inline unsigned long long O_D(const unsigned long long O) { return ((O & diagonal<L,N>() & ~BOARDER) * 0x0101010101010101ULL) >> SHIFT_1<L>(); }
	template <int L, int N> inline unsigned long long P_D(const unsigned long long P) { return ((P & diagonal<L,N>())            * 0x0101010101010101ULL) >> SHIFT_2<L>(); }
	template <int L, int N> inline unsigned long long FLIPS_D(const unsigned long long P, const unsigned long long O) { return STRETCH_V[FLIPS_<L>()[OUTFLANK_<L>()[O_D<L,N>(O)] & P_D<L,N>(P)]] & diagonal<L,N>(); }

	template <int L, int N> inline unsigned long long O_C(const unsigned long long O) { return ((O & codiagonal<L,N>() & ~BOARDER) * 0x0101010101010101ULL) >> SHIFT_1<L>(); }
	template <int L, int N> inline unsigned long long P_C(const unsigned long long P) { return ((P & codiagonal<L,N>())            * 0x0101010101010101ULL) >> SHIFT_2<L>(); }
	template <int L, int N> inline unsigned long long FLIPS_C(const unsigned long long P, const unsigned long long O) { return STRETCH_V[FLIPS_<L>()[OUTFLANK_<L>()[O_C<L,N>(O)] & P_C<L,N>(P)]] & codiagonal<L,N>(); }

	template <int L, int N> inline unsigned long long O_DC(const unsigned long long O) { return ((O & v_line<L,N>() & ~BOARDER) * 0x0101010101010101ULL) >> SHIFT_1<L>(); }
	template <int L, int N> inline unsigned long long P_DC(const unsigned long long P) { return ((P & v_line<L,N>())            * 0x0101010101010101ULL) >> SHIFT_2<L>(); }
	template <int L, int N> inline unsigned long long FLIPS_DC(const unsigned long long P, const unsigned long long O) { return STRETCH_V[FLIPS_<L>()[OUTFLANK_<L>()[O_DC<L,N>(O)] & P_DC<L,N>(P)]] & v_line<L,N>(); }

	template <int L, int N> inline unsigned long long O_752(const unsigned long long O) { return ((O & Y_line<L,N>() & ~TB_BOARDER) * MULT_752<L,N>()) >> SHIFT_1<N>(); }
	template <int L, int N> inline unsigned long long P_752(const unsigned long long P) { return ((P & Y_line<L,N>())               * MULT_752<L,N>()) >> SHIFT_2<N>(); }
	template <int L, int N> inline unsigned long long FLIPS_752(const unsigned long long P, const unsigned long long O) { return STRETCH_A1A8[FLIPS_<N>()[OUTFLANK_<N>()[O_752<L,N>(O)] & P_752<L,N>(P)]] & Y_line<L,N>(); }

	template <int L, int N> inline unsigned long long O_851(const unsigned long long O) { return ((O & lambda_line<L,N>() & ~TB_BOARDER) * MULT_851<N>()) >> SHIFT_1<7-N>(); }
	template <int L, int N> inline unsigned long long P_851(const unsigned long long P) { return ((P & lambda_line<L,N>())               * MULT_851<N>()) >> SHIFT_2<7-N>(); }
	template <int L, int N> inline unsigned long long FLIPS_851_A(const unsigned long long P, const unsigned long long O) { return STRETCH_A8A1[FLIPS_<7-N>()[OUTFLANK_<7-N>()[O_851<L,N>(O)] & P_851<L,N>(P)]] & lambda_line<L,N>(); }
	template <int L, int N> inline unsigned long long FLIPS_851_B(const unsigned long long P, const unsigned long long O) { return STRETCH_A8A1[FLIPS_<7-N>()[OUTFLANK_<7-N>()[O_851<L-1,N>(O >> 1)] & P_851<L-1,N>(P >> 1)]] & lambda_line<L,N>(); }
	
	template <int L, int N> inline unsigned long long O_853(const unsigned long long O) { return ((O & lambda_line<L,N>() & ~TB_BOARDER) * MULT_853<L,N>()) >> SHIFT_1<N>(); }
	template <int L, int N> inline unsigned long long P_853(const unsigned long long P) { return ((P & lambda_line<L,N>())               * MULT_853<L,N>()) >> SHIFT_2<N>(); }
	template <int L, int N> inline unsigned long long FLIPS_853(const unsigned long long P, const unsigned long long O) { return STRETCH_A1A8[FLIPS_<N>()[OUTFLANK_<N>()[O_853<L,N>(O)] & P_853<L,N>(P)]] & lambda_line<L,N>(); }
	
	template <int L, int N> inline unsigned long long O_952(const unsigned long long O) { return (((O & Y_line<L,N>() & ~TB_BOARDER) >> (N+L-7)) * MULT_952<N>()) >> SHIFT_1<7-N>(); }
	template <int L, int N> inline unsigned long long P_952(const unsigned long long P) { return (((P & Y_line<L,N>()              ) >> (N+L-7)) * MULT_952<N>()) >> SHIFT_2<7-N>(); }
	template <int L, int N> inline unsigned long long FLIPS_952(const unsigned long long P, const unsigned long long O) { return STRETCH_A8A1[FLIPS_<7-N>()[OUTFLANK_<7-N>()[O_952<L,N>(O)] & P_952<L,N>(P)]] & Y_line<L,N>(); }
	
	template <int L, int N> unsigned long long ZZ(const unsigned long long P, const unsigned long long O) { return FLIPS_H<L,N>(P, O) | FLIPS_V<L,N>(P, O) | FLIPS_DC<L,N>(P, O); }
	template <int L, int N> unsigned long long AA(const unsigned long long P, const unsigned long long O) { return FLIPS_H<L,N>(P, O) | FLIPS_752<L,N>(P, O) | FLIPS_851_A<L,N>(P, O); }
	template <int L, int N> unsigned long long BB(const unsigned long long P, const unsigned long long O) { return FLIPS_H<L,N>(P, O) | FLIPS_752<L,N>(P, O) | FLIPS_851_B<L,N>(P, O); }
	template <int L, int N> unsigned long long GH(const unsigned long long P, const unsigned long long O) { return FLIPS_H<L,N>(P, O) | FLIPS_853<L,N>(P, O) | FLIPS_952<L,N>(P, O); }
	template <int L, int N> unsigned long long XX(const unsigned long long P, const unsigned long long O) { return FLIPS_H<L,N>(P, O) | FLIPS_V<L,N>(P, O) | FLIPS_D<L,N>(P, O) | FLIPS_C<L,N>(P, O); }

	void PrintArrays();
}

FORCE_INLINE unsigned long long flip(const unsigned long long P, const unsigned long long O, const unsigned char c) { return Flip::flip_fp[c](P, O); }