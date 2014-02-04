#include "count_last_flip.h"

const unsigned char CLF_0[256] = {
	 0,  0,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
	 8,  8,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
	10, 10,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
	 8,  8,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
	12, 12,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
	 8,  8,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
	10, 10,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
	 8,  8,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,  6,  6,  0,  0,  2,  2,  0,  0,  4,  4,  0,  0,  2,  2,  0,  0,
};
const unsigned char CLF_1[256] = {
	 0,  0,  0,  0,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
	 6,  6,  6,  6,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
	 8,  8,  8,  8,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
	 6,  6,  6,  6,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
	10, 10, 10, 10,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
	 6,  6,  6,  6,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
	 8,  8,  8,  8,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
	 6,  6,  6,  6,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,  4,  4,  4,  4,  0,  0,  0,  0,  2,  2,  2,  2,  0,  0,  0,  0,
};
const unsigned char CLF_2[256] = {
	 0,  2,  0,  0,  0,  2,  0,  0,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
	 4,  6,  4,  4,  4,  6,  4,  4,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
	 6,  8,  6,  6,  6,  8,  6,  6,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
	 4,  6,  4,  4,  4,  6,  4,  4,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
	 8, 10,  8,  8,  8, 10,  8,  8,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
	 4,  6,  4,  4,  4,  6,  4,  4,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
	 6,  8,  6,  6,  6,  8,  6,  6,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
	 4,  6,  4,  4,  4,  6,  4,  4,  0,  2,  0,  0,  0,  2,  0,  0,  2,  4,  2,  2,  2,  4,  2,  2,  0,  2,  0,  0,  0,  2,  0,  0,
};
const unsigned char CLF_3[256] = {
	 0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
	 2,  6,  4,  4,  2,  2,  2,  2,  2,  6,  4,  4,  2,  2,  2,  2,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
	 4,  8,  6,  6,  4,  4,  4,  4,  4,  8,  6,  6,  4,  4,  4,  4,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
	 2,  6,  4,  4,  2,  2,  2,  2,  2,  6,  4,  4,  2,  2,  2,  2,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
	 6, 10,  8,  8,  6,  6,  6,  6,  6, 10,  8,  8,  6,  6,  6,  6,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
	 2,  6,  4,  4,  2,  2,  2,  2,  2,  6,  4,  4,  2,  2,  2,  2,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
	 4,  8,  6,  6,  4,  4,  4,  4,  4,  8,  6,  6,  4,  4,  4,  4,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
	 2,  6,  4,  4,  2,  2,  2,  2,  2,  6,  4,  4,  2,  2,  2,  2,  0,  4,  2,  2,  0,  0,  0,  0,  0,  4,  2,  2,  0,  0,  0,  0,
};
const unsigned char CLF_4[256] = {
	 0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,
	 2,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,
	 0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,
	 4, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  4, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,
	 0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,
	 2,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,
	 0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  4,  4,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,
};
const unsigned char CLF_5[256] = {
	 0,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 2, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
	 2, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
	 0,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  8,  6,  6,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};
const unsigned char CLF_6[256] = {
	 0, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0, 10,  8,  8,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};
const unsigned char CLF_7[256] = {
	 0, 12, 10, 10,  8,  8,  8,  8,  6,  6,  6,  6,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
	 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0, 12, 10, 10,  8,  8,  8,  8,  6,  6,  6,  6,  6,  6,  6,  6,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
	 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

inline unsigned char A1(const unsigned long long P)
{
	unsigned char n_flipped;
	
	n_flipped  = CLF_0[P & 0xFFULL];
	n_flipped += CLF_0[((P & 0x0101010101010101ULL) * 0x0102040810204080ULL) >> 56];
	n_flipped += CLF_0[((P & 0x8040201008040201ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char B1(const unsigned long long P)
{
	unsigned char n_flipped;
	
	n_flipped  = CLF_1[P & 0xFFULL];
	n_flipped += CLF_0[((P & 0x0202020202020202ULL) * 0x0081020408102040ULL) >> 56];
	n_flipped += CLF_1[((P & 0x0080402010080402ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char C1(const unsigned long long P)
{
	unsigned char n_flipped;
	
	n_flipped  = CLF_2[P & 0xFFULL];
	n_flipped += CLF_0[((P & 0x0404040404040404ULL) * 0x0040810204081020ULL) >> 56];
	n_flipped += CLF_2[((P & 0x0000804020110A04ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char D1(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_3[P & 0xFFULL];
	n_flipped += CLF_0[((P & 0x0808080808080808ULL) * 0x0020408102040810ULL) >> 56];
	n_flipped += CLF_3[((P & 0x0000008041221408ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char E1(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_4[P & 0xFFULL];
	n_flipped += CLF_0[((P & 0x1010101010101010ULL) * 0x0010204081020408ULL) >> 56];
	n_flipped += CLF_4[((P & 0x0000000182442810ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char F1(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_5[P & 0xFFULL];
	n_flipped += CLF_0[((P & 0x2020202020202020ULL) * 0x0008102040810204ULL) >> 56];
	n_flipped += CLF_5[((P & 0x0000010204885020ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char G1(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_6[P & 0xFFULL];
	n_flipped += CLF_0[((P & 0x4040404040404040ULL) * 0x0004081020408102ULL) >> 56];
	n_flipped += CLF_6[((P & 0x0001020408102040ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char H1(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_7[P & 0xFFULL];
	n_flipped += CLF_0[((P & 0x8080808080808080ULL) * 0x0002040810204081ULL) >> 56];
	n_flipped += CLF_7[((P & 0x0102040810204080ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char A2(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_0[(P >> 8) & 0xFFULL];
	n_flipped += CLF_1[((P & 0x0101010101010101ULL) * 0x0102040810204080ULL) >> 56];
	n_flipped += CLF_0[((P & 0x4020100804020100ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char B2(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_1[(P >> 8) & 0xFFULL];
	n_flipped += CLF_1[((P & 0x0202020202020202ULL) * 0x0081020408102040ULL) >> 56];
	n_flipped += CLF_1[((P & 0x8040201008040201ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char C2(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_1[((P & 0x0404040404040404ULL) * 0x0040810204081020ULL) >> 56];
	n_flipped += CLF_2[(P >> 8) & 0xFFULL];
	n_flipped += CLF_2[((P & 0x00804020110A0400ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char D2(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_1[((P & 0x0808080808080808ULL) * 0x0020408102040810ULL) >> 56];
	n_flipped += CLF_3[(P >> 8) & 0xFFULL];
	n_flipped += CLF_3[((P & 0x0000804122140800ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char E2(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_4[(P >> 8) & 0xFFULL];
	n_flipped += CLF_1[((P & 0x1010101010101010ULL) * 0x0010204081020408ULL) >> 56];
	n_flipped += CLF_4[((P & 0x0000018244281000ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char F2(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_5[(P >> 8) & 0xFFULL];
	n_flipped += CLF_1[((P & 0x2020202020202020ULL) * 0x0008102040810204ULL) >> 56];
	n_flipped += CLF_5[((P & 0x0001020488502000ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char G2(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_6[(P >> 8) & 0xFFULL];
	n_flipped += CLF_1[((P & 0x4040404040404040ULL) * 0x0004081020408102ULL) >> 56];
	n_flipped += CLF_6[((P & 0x0102040810204080ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char H2(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_7[(P >> 8) & 0xFFULL];
	n_flipped += CLF_1[((P & 0x8080808080808080ULL) * 0x0002040810204081ULL) >> 56];
	n_flipped += CLF_7[((P & 0x0204081020408000ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char A3(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_0[(P >> 16) & 0xFFULL];
	n_flipped += CLF_2[((P & 0x0101010101010101ULL) * 0x0102040810204080ULL) >> 56];
	n_flipped += CLF_2[((((P & 0x2010080402010204ULL) + 0x6070787C7E7F7E7C) & 0x8080808080808080) * 0x0002040810204081) >> 56];

	return n_flipped;
}

inline unsigned char B3(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_1[(P >> 16) & 0xFFULL];
	n_flipped += CLF_2[((P & 0x0202020202020202ULL) * 0x0081020408102040ULL) >> 56];
	n_flipped += CLF_2[((((P & 0x4020100804020408ULL) + 0x406070787C7E7C78) & 0x8080808080808080) * 0x0002040810204081) >> 56];

	return n_flipped;
}

inline unsigned char C3(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_2[(P >> 16) & 0xFFULL];
	n_flipped += CLF_2[((P & 0x0404040404040404ULL) * 0x0040810204081020ULL) >> 56];
	n_flipped += CLF_2[((P & 0x0000000102040810ULL) * 0x0101010101010101ULL) >> 56];
	n_flipped += CLF_2[((P & 0x8040201008040201ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char D3(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_3[(P >> 16) & 0xFFULL];
	n_flipped += CLF_2[((P & 0x0808080808080808ULL) * 0x0020408102040810ULL) >> 56];
	n_flipped += CLF_3[((P & 0x0000010204081020ULL) * 0x0101010101010101ULL) >> 56];
	n_flipped += CLF_3[((P & 0x0080402010080402ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char E3(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_4[(P >> 16) & 0xFFULL];
	n_flipped += CLF_2[((P & 0x1010101010101010ULL) * 0x0010204081020408ULL) >> 56];
	n_flipped += CLF_4[((P & 0x0001020408102040ULL) * 0x0101010101010101ULL) >> 56];
	n_flipped += CLF_4[((P & 0x0000804020100804ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char F3(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_5[(P >> 16) & 0xFFULL];
	n_flipped += CLF_2[((P & 0x2020202020202020ULL) * 0x0008102040810204ULL) >> 56];
	n_flipped += CLF_5[((P & 0x0102040810204080ULL) * 0x0101010101010101ULL) >> 56];
	n_flipped += CLF_5[((P & 0x0000008040201008ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char G3(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_6[(P >> 16) & 0xFFULL];
	n_flipped += CLF_2[((P & 0x4040404040404040ULL) * 0x0004081020408102ULL) >> 56];
	n_flipped += CLF_2[((((P & 0x0204081020402010ULL) + 0x7E7C787060406070) & 0x8080808080808080) * 0x0002040810204081) >> 56];

	return n_flipped;
}

inline unsigned char H3(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_7[(P >> 16) & 0xFFULL];
	n_flipped += CLF_2[((P & 0x8080808080808080ULL) * 0x0002040810204081ULL) >> 56];
	n_flipped += CLF_2[((((P & 0x0408102040804020ULL) + 0x7C78706040004060) & 0x8080808080808080) * 0x0002040810204081) >> 56];

	return n_flipped;
}

inline unsigned char A4(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_0[(P >> 24) & 0xFFULL];
	n_flipped += CLF_3[((P & 0x0101010101010101ULL) * 0x0102040810204080ULL) >> 56];
	n_flipped += CLF_3[((((P & 0x1008040201020408ULL) + 0x70787C7E7F7E7C78) & 0x8080808080808080) * 0x0002040810204081) >> 56];

	return n_flipped;
}

inline unsigned char B4(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_1[(P >> 24) & 0xFFULL];
	n_flipped += CLF_3[((P & 0x0202020202020202ULL) * 0x0081020408102040ULL) >> 56];
	n_flipped += CLF_3[((((P & 0x2010080402040810ULL) + 0x6070787C7E7C7870) & 0x8080808080808080) * 0x0002040810204081) >> 56];

	return n_flipped;
}

inline unsigned char C4(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_2[(P >> 24) & 0xFFULL];
	n_flipped += CLF_3[((P & 0x0404040404040404ULL) * 0x0040810204081020ULL) >> 56];
	n_flipped += CLF_2[((P & 0x0000010204081020ULL) * 0x0101010101010101ULL) >> 56];
	n_flipped += CLF_2[((P & 0x4020100804020100ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char D4(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_3[(P >> 24) & 0xFFULL];
	n_flipped += CLF_3[((P & 0x0808080808080808ULL) * 0x0020408102040810ULL) >> 56];
	n_flipped += CLF_3[((P & 0x0001020408102040ULL) * 0x0101010101010101ULL) >> 56];
	n_flipped += CLF_3[((P & 0x8040201008040201ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char E4(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_4[(P >> 24) & 0xFFULL];
	n_flipped += CLF_3[((P & 0x1010101010101010ULL) * 0x0010204081020408ULL) >> 56];
	n_flipped += CLF_4[((P & 0x0102040810204080ULL) * 0x0101010101010101ULL) >> 56];
	n_flipped += CLF_4[((P & 0x0080402010080402ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char F4(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_5[(P >> 24) & 0xFFULL];
	n_flipped += CLF_3[((P & 0x2020202020202020ULL) * 0x0008102040810204ULL) >> 56];
	n_flipped += CLF_5[((P & 0x0204081020408000ULL) * 0x0101010101010101ULL) >> 56];
	n_flipped += CLF_5[((P & 0x0000804020100804ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char G4(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_6[(P >> 24) & 0xFFULL];
	n_flipped += CLF_3[((P & 0x4040404040404040ULL) * 0x0004081020408102ULL) >> 56];
	n_flipped += CLF_3[((((P & 0x0408102040201008ULL) + 0x7C78706040607078) & 0x8080808080808080) * 0x0002040810204081) >> 56];

	return n_flipped;
}

inline unsigned char H4(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_7[(P >> 24) & 0xFFULL];
	n_flipped += CLF_3[((P & 0x8080808080808080ULL) * 0x0002040810204081ULL) >> 56];
	n_flipped += CLF_3[((((P & 0x0810204080402010ULL) + 0x7870604000406070) & 0x8080808080808080) * 0x0002040810204081) >> 56];

	return n_flipped;
}

inline unsigned char A5(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_0[(P >> 32) & 0xFFULL];
	n_flipped += CLF_4[((P & 0x0101010101010101ULL) * 0x0102040810204080ULL) >> 56];
	n_flipped += CLF_4[((((P & 0x0804020102040810ULL) + 0x787C7E7F7E7C7870) & 0x8080808080808080) * 0x0002040810204081) >> 56];

	return n_flipped;
}

inline unsigned char B5(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_1[(P >> 32) & 0xFFULL];
	n_flipped += CLF_4[((P & 0x0202020202020202ULL) * 0x0081020408102040ULL) >> 56];
	n_flipped += CLF_4[((((P & 0x1008040204081020ULL) + 0x70787C7E7C787060) & 0x8080808080808080) * 0x0002040810204081) >> 56];

	return n_flipped;
}

inline unsigned char C5(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_2[(P >> 32) & 0xFFULL];
	n_flipped += CLF_4[((P & 0x0404040404040404ULL) * 0x0040810204081020ULL) >> 56];
	n_flipped += CLF_2[((P & 0x0001020408102040ULL) * 0x0101010101010101ULL) >> 56];
	n_flipped += CLF_2[((P & 0x2010080402010000ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char D5(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_3[(P >> 32) & 0xFFULL];
	n_flipped += CLF_4[((P & 0x0808080808080808ULL) * 0x0020408102040810ULL) >> 56];
	n_flipped += CLF_3[((P & 0x0102040810204080ULL) * 0x0101010101010101ULL) >> 56];
	n_flipped += CLF_3[((P & 0x4020100804020100ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char E5(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_4[(P >> 32) & 0xFFULL];
	n_flipped += CLF_4[((P & 0x1010101010101010ULL) * 0x0010204081020408ULL) >> 56];
	n_flipped += CLF_4[((P & 0x0204081020408000ULL) * 0x0101010101010101ULL) >> 56];
	n_flipped += CLF_4[((P & 0x8040201008040201ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char F5(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_5[(P >> 32) & 0xFFULL];
	n_flipped += CLF_4[((P & 0x2020202020202020ULL) * 0x0008102040810204ULL) >> 56];
	n_flipped += CLF_5[((P & 0x0408102040800000ULL) * 0x0101010101010101ULL) >> 56];
	n_flipped += CLF_5[((P & 0x0080402010080402ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char G5(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_6[(P >> 32) & 0xFFULL];
	n_flipped += CLF_4[((P & 0x4040404040404040ULL) * 0x0004081020408102ULL) >> 56];
	n_flipped += CLF_4[((((P & 0x0810204020100804ULL) + 0x787060406070787C) & 0x8080808080808080) * 0x0002040810204081) >> 56];

	return n_flipped;
}

inline unsigned char H5(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_7[(P >> 32) & 0xFFULL];
	n_flipped += CLF_4[((P & 0x8080808080808080ULL) * 0x0002040810204081ULL) >> 56];
	n_flipped += CLF_4[((((P & 0x1020408040201008ULL) + 0x7060400040607078) & 0x8080808080808080) * 0x0002040810204081) >> 56];

	return n_flipped;
}

inline unsigned char A6(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_0[(P >> 40) & 0xFFULL];
	n_flipped += CLF_5[((P & 0x0101010101010101ULL) * 0x0102040810204080ULL) >> 56];
	n_flipped += CLF_5[((((P & 0x0402010204081020ULL) + 0x7C7E7F7E7C787060) & 0x8080808080808080) * 0x0002040810204081) >> 56];

	return n_flipped;
}

inline unsigned char B6(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_1[(P >> 40) & 0xFFULL];
	n_flipped += CLF_5[((P & 0x0202020202020202ULL) * 0x0081020408102040ULL) >> 56];
	n_flipped += CLF_5[((((P & 0x0804020408102040ULL) + 0x787C7E7C78706040) & 0x8080808080808080) * 0x0002040810204081) >> 56];

	return n_flipped;
}

inline unsigned char C6(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_2[(P >> 40) & 0xFFULL];
	n_flipped += CLF_5[((P & 0x0404040404040404ULL) * 0x0040810204081020ULL) >> 56];
	n_flipped += CLF_2[((P & 0x0102040810204080ULL) * 0x0101010101010101ULL) >> 56];
	n_flipped += CLF_2[((P & 0x1008040201000000ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char D6(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_3[(P >> 40) & 0xFFULL];
	n_flipped += CLF_5[((P & 0x0808080808080808ULL) * 0x0020408102040810ULL) >> 56];
	n_flipped += CLF_3[((P & 0x0204081020408000ULL) * 0x0101010101010101ULL) >> 56];
	n_flipped += CLF_3[((P & 0x2010080402010000ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char E6(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_4[(P >> 40) & 0xFFULL];
	n_flipped += CLF_5[((P & 0x1010101010101010ULL) * 0x0010204081020408ULL) >> 56];
	n_flipped += CLF_4[((P & 0x0408102040800000ULL) * 0x0101010101010101ULL) >> 56];
	n_flipped += CLF_4[((P & 0x4020100804020100ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char F6(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_5[(P >> 40) & 0xFFULL];
	n_flipped += CLF_5[((P & 0x2020202020202020ULL) * 0x0008102040810204ULL) >> 56];
	n_flipped += CLF_5[((P & 0x0810204080000000ULL) * 0x0101010101010101ULL) >> 56];
	n_flipped += CLF_5[((P & 0x8040201008040201ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char G6(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_6[(P >> 40) & 0xFFULL];
	n_flipped += CLF_5[((P & 0x4040404040404040ULL) * 0x0004081020408102ULL) >> 56];
	n_flipped += CLF_5[((((P & 0x1020402010080402ULL) + 0x7060406070787C7E) & 0x8080808080808080) * 0x0002040810204081) >> 56];

	return n_flipped;
}

inline unsigned char H6(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_7[(P >> 40) & 0xFFULL];
	n_flipped += CLF_5[((P & 0x8080808080808080ULL) * 0x0002040810204081ULL) >> 56];
	n_flipped += CLF_5[((((P & 0x2040804020100804ULL) + 0x604000406070787C) & 0x8080808080808080) * 0x0002040810204081) >> 56];

	return n_flipped;
}

inline unsigned char A7(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_0[(P >> 48) & 0xFFULL];
	n_flipped += CLF_6[((P & 0x0101010101010101ULL) * 0x0102040810204080ULL) >> 56];
	n_flipped += CLF_0[((P & 0x0001020408102040ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char B7(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_1[(P >> 48) & 0xFFULL];
	n_flipped += CLF_6[((P & 0x0202020202020202ULL) * 0x0081020408102040ULL) >> 56];
	n_flipped += CLF_1[((P & 0x0102040810204080ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char C7(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_2[(P >> 48) & 0xFFULL];
	n_flipped += CLF_6[((P & 0x0404040404040404ULL) * 0x0040810204081020ULL) >> 56];
	n_flipped += CLF_2[((P & 0x00040A1120408000ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char D7(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_3[(P >> 48) & 0xFFULL];
	n_flipped += CLF_6[((P & 0x0808080808080808ULL) * 0x0020408102040810ULL) >> 56];
	n_flipped += CLF_3[((P & 0x0008142241800000ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char E7(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_4[(P >> 48) & 0xFFULL];
	n_flipped += CLF_6[((P & 0x1010101010101010ULL) * 0x0010204081020408ULL) >> 56];
	n_flipped += CLF_4[((P & 0x0010284482010000ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char F7(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_5[(P >> 48) & 0xFFULL];
	n_flipped += CLF_6[((P & 0x2020202020202020ULL) * 0x0008102040810204ULL) >> 56];
	n_flipped += CLF_5[((P & 0x0020508804020100ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char G7(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_6[(P >> 48) & 0xFFULL];
	n_flipped += CLF_6[((P & 0x4040404040404040ULL) * 0x0004081020408102ULL) >> 56];
	n_flipped += CLF_6[((P & 0x8040201008040201ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char H7(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_7[(P >> 48) & 0xFFULL];
	n_flipped += CLF_6[((P & 0x8080808080808080ULL) * 0x0002040810204081ULL) >> 56];
	n_flipped += CLF_7[((P & 0x0080402010080402ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char A8(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_0[P >> 56];
	n_flipped += CLF_7[((P & 0x0101010101010101ULL) * 0x0102040810204080ULL) >> 56];
	n_flipped += CLF_0[((P & 0x0102040810204080ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char B8(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_1[P >> 56];
	n_flipped += CLF_7[((P & 0x0202020202020202ULL) * 0x0081020408102040ULL) >> 56];
	n_flipped += CLF_1[((P & 0x0204081020408000ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char C8(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_2[P >> 56];
	n_flipped += CLF_7[((P & 0x0404040404040404ULL) * 0x0040810204081020ULL) >> 56];
	n_flipped += CLF_2[((P & 0x040A112040800000ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char D8(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_3[P >> 56];
	n_flipped += CLF_7[((P & 0x0808080808080808ULL) * 0x0020408102040810ULL) >> 56];
	n_flipped += CLF_3[((P & 0x0814224180000000ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char E8(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_4[P >> 56];
	n_flipped += CLF_7[((P & 0x1010101010101010ULL) * 0x0010204081020408ULL) >> 56];
	n_flipped += CLF_4[((P & 0x1028448201000000ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char F8(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_5[P >> 56];
	n_flipped += CLF_7[((P & 0x2020202020202020ULL) * 0x0008102040810204ULL) >> 56];
	n_flipped += CLF_5[((P & 0x2050880402010000ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char G8(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_6[P >> 56];
	n_flipped += CLF_7[((P & 0x4040404040404040ULL) * 0x0004081020408102ULL) >> 56];
	n_flipped += CLF_6[((P & 0x4020100804020100ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

inline unsigned char H8(const unsigned long long P)
{
	unsigned char n_flipped;

	n_flipped  = CLF_7[P >> 56];
	n_flipped += CLF_7[((P & 0x8080808080808080ULL) * 0x0002040810204081ULL) >> 56];
	n_flipped += CLF_7[((P & 0x8040201008040201ULL) * 0x0101010101010101ULL) >> 56];

	return n_flipped;
}

namespace CountLastFlip
{
	unsigned char (*CLF_fp[])(const unsigned long long) = {
		A1, B1, C1, D1,	E1, F1, G1, H1,
		A2, B2, C2, D2,	E2, F2, G2, H2,
		A3, B3, C3, D3,	E3, F3, G3, H3,
		A4, B4, C4, D4,	E4, F4, G4, H4,
		A5, B5, C5, D5,	E5, F5, G5, H5,
		A6, B6, C6, D6,	E6, F6, G6, H6,
		A7, B7, C7, D7,	E7, F7, G7, H7,
		A8, B8, C8, D8,	E8, F8, G8, H8
	};
}