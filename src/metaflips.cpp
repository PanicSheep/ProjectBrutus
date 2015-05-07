#include "metaflips.h"

static const char field_name[64][3] = {
	"A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1",
	"A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2",
	"A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3",
	"A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4",
	"A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5",
	"A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6",
	"A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7",
	"A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8"
};

static const char letter[8][2] = {"A", "B", "C", "D", "E", "F", "G", "H"};

unsigned long long horizontal(int C)
{
	const int L = C % 8;
	const int N = C / 8;
	return 0xFFULL << (N * 8);
}
unsigned long long vertical(int C)
{
	const int L = C % 8;
	const int N = C / 8;
	return 0x0101010101010101ULL << L;
}
unsigned long long diagonal(int C)
{
	const int L = C % 8;
	const int N = C / 8;
	if (L > N) return 0x8040201008040201ULL >> ((L-N)*8);
	else return 0x8040201008040201ULL << ((N-L)*8);
}
unsigned long long codiagonal(int C)
{
	const int L = C % 8;
	const int N = C / 8;
	if (N + L - 7 > 0) return 0x0102040810204080ULL << ((N+L-7)*8);
	else return 0x0102040810204080ULL >> (-(N+L-7)*8);
}
unsigned long long v_line(int C)
{
	const int L = C % 8;
	const int N = C / 8;
	switch (N)
	{
	case 0: return diagonal(C) | codiagonal(C);
	case 1: return (diagonal(C-8) | codiagonal(C-8)) << 8;
	case 6: return (diagonal(C+8) | codiagonal(C+8)) >> 8;
	case 7: return diagonal(C) | codiagonal(C);
	}
}

void print_board1D(const unsigned long long P, const unsigned long long O)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (P & (0x8000000000000000ULL >> i*8+j))
				std::cout << "X";
			else if (O & (0x8000000000000000ULL >> i*8+j))
				std::cout << "O";
			else
				std::cout << ".";
		}
		std::cout << std::endl;
	}
}

void print_horizontal(const int C)
{
	const int L = C % 8;
	const int N = C / 8;
	if (L == 1)
		printf("\tconst unsigned long long O_h = (O >> %d) & 0x1FULL;\n", N*8+2);
	else
		printf("\tconst unsigned long long O_h = (O >> %d) & 0x3FULL;\n", N*8+1);
	if (L == 0)
		printf("\tconst unsigned long long P_h = P >> %d;\n", N*8+2);
	else if (L == 1)
		printf("\tconst unsigned long long P_h = P >> %d;\n", N*8+3);
	else
		printf("\tconst unsigned long long P_h = P >> %d;\n", N*8);
	printf("\tconst unsigned char outflank_h = OUTFLANK_%d[O_h] & P_h;\n", L);
	if (C < 32)
		printf("\tconst unsigned long long flips_h = FLIPS_%d[outflank_h] << %d;\n", L, N*8+1);
	else
		printf("\tconst unsigned long long flips_h = static_cast<unsigned long long>(FLIPS_%d[outflank_h]) << %d;\n", L, N*8+1);
	printf("\n");
}

void print_vertical(const int C)
{
	const int L = C % 8;
	const int N = C / 8;
	printf("\tconst unsigned long long O_v = ((O & 0x%016llXULL) * 0x%016llXULL) >> %d;\n", vertical(C) & ~TB_BOARDER, 0x0102040810204080ULL >> L, (N == 1) ? 58 : 57);
	if (N == 0)
		printf("\tconst unsigned long long P_v = ((P & 0x%016llXULL) * 0x%016llXULL) >> %d;\n", vertical(C), 0x0102040810204080ULL >> L, 58);
	else if (N == 1)
		printf("\tconst unsigned long long P_v = ((P & 0x%016llXULL) * 0x%016llXULL) >> %d;\n", vertical(C), 0x0102040810204080ULL >> L, 59);
	else
		printf("\tconst unsigned long long P_v = ((P & 0x%016llXULL) * 0x%016llXULL) >> %d;\n", vertical(C), 0x0102040810204080ULL >> L, 56);
	printf("\tconst unsigned char outflank_v = OUTFLANK_%d[O_v] & P_v;\n", N);
	printf("\tconst unsigned long long flips_v = A1A8[FLIPS_%d[outflank_v]] << %d;\n", N, L);
	printf("\n");
}

void print_diagonal(const int C)
{
	const int L = C % 8;
	const int N = C / 8;
	printf("\tconst unsigned long long O_d = ((O & 0x%016llXULL) * 0x0101010101010101ULL) >> %d;\n", diagonal(C) & ~BOARDER, (L == 1) ? 58 : 57);
	if (L == 0)
		printf("\tconst unsigned long long P_d = ((P & 0x%016llXULL) * 0x0101010101010101ULL) >> %d;\n", diagonal(C), 58);
	else if (L == 1)
		printf("\tconst unsigned long long P_d = ((P & 0x%016llXULL) * 0x0101010101010101ULL) >> %d;\n", diagonal(C), 59);
	else
		printf("\tconst unsigned long long P_d = ((P & 0x%016llXULL) * 0x0101010101010101ULL) >> %d;\n", diagonal(C), 56);
	printf("\tconst unsigned char outflank_d = OUTFLANK_%d[O_d] & P_d;\n", L);
	printf("\tconst unsigned long long flips_d = STRETCH[FLIPS_%d[outflank_d]] & 0x%016llXULL;\n", L, diagonal(C));
	printf("\n");
}

void print_codiagonal(const int C)
{
	const int L = C % 8;
	const int N = C / 8;
	printf("\tconst unsigned long long O_c = ((O & 0x%016llXULL) * 0x0101010101010101ULL) >> %d;\n", codiagonal(C) & ~BOARDER, (L == 1) ? 58 : 57);
	if (L == 0)
		printf("\tconst unsigned long long P_c = ((P & 0x%016llXULL) * 0x0101010101010101ULL) >> %d;\n", codiagonal(C), 58);
	else if (L == 1)
		printf("\tconst unsigned long long P_c = ((P & 0x%016llXULL) * 0x0101010101010101ULL) >> %d;\n", codiagonal(C), 59);
	else
		printf("\tconst unsigned long long P_c = ((P & 0x%016llXULL) * 0x0101010101010101ULL) >> %d;\n", codiagonal(C), 56);
	printf("\tconst unsigned char outflank_c = OUTFLANK_%d[O_c] & P_c;\n", L);
	printf("\tconst unsigned long long flips_c = STRETCH[FLIPS_%d[outflank_c]] & 0x%016llXULL;\n", L, codiagonal(C));
	printf("\n");
}

void print_dc(const int C)
{
	const int L = C % 8;
	const int N = C / 8;
	printf("\tconst unsigned long long O_dc = ((O & 0x%016llXULL) * 0x0101010101010101ULL) >> %d;\n", v_line(C) & ~BOARDER, (L == 1) ? 58 : 57);
	if (L == 0)
		printf("\tconst unsigned long long P_dc = ((P & 0x%016llXULL) * 0x0101010101010101ULL) >> %d;\n", v_line(C), 58);
	else if (L == 1)
		printf("\tconst unsigned long long P_dc = ((P & 0x%016llXULL) * 0x0101010101010101ULL) >> %d;\n", v_line(C), 59);
	else
		printf("\tconst unsigned long long P_dc = ((P & 0x%016llXULL) * 0x0101010101010101ULL) >> %d;\n", v_line(C), 56);
	printf("\tconst unsigned char outflank_dc = OUTFLANK_%d[O_dc] & P_dc;\n", L);
	printf("\tconst unsigned long long flips_dc = STRETCH[FLIPS_%d[outflank_dc]] & 0x%016llXULL;\n", L, v_line(C));
	printf("\n");
}

int main()
{
	int L, N;

	for (int C = 0; C < 64; C++)
	{
		L = C % 8;
		N = C / 8;
		printf("inline unsigned long long %s(const unsigned long long P, const unsigned long long O)\n", field_name[C]);
		printf("{\n");

		switch (N)
		{
		case 0:
		case 1:
		case 6:
		case 7:
			print_horizontal(C);
			print_vertical(C);
			print_dc(C);
			printf("\treturn flips_h | flips_v | flips_dc;\n");
			printf("}\n");
			printf("\n");
			continue;
		}

		//switch (L)
		//{
		//case 0:
		//case 1:
		//case 6:
		//case 7:
		//	print_horizontal(L, N);
		//	print_vertical(L, N);

		//	continue;
		//}

		print_horizontal(C);
		print_vertical(C);
		print_diagonal(C);
		print_codiagonal(C);

		printf("\treturn flips_h | flips_v | flips_d | flips_c;\n");
		printf("}\n");
		printf("\n");
	}
	return 0;
}