#include "benchmark.h"

void FForum_Benchmark(int begin, int end, bool verbose, CHashTable* hashTable)
{
	struct CFForum
	{
		unsigned long long P, O;
		int Score;
		CFForum(const unsigned long long P, const unsigned long long O, const int Score) : P(P), O(O), Score(Score) {}
	};
	static const CFForum FForum[] = {
		CFForum(0x3E0C0D2B772B7000ULL, 0x0071725488140E7CULL,  18),
		CFForum(0x7E2059000C19303EULL, 0x001E267FF3E60C00ULL,  10),
		CFForum(0x040C0D306C707A01ULL, 0x083070CF938F853EULL,   2),
		CFForum(0x7EB8B9D127070C2CULL, 0x0006462E58383012ULL,   0),
		CFForum(0x0019D6D6C8F0A07CULL, 0x7C242829370D1C00ULL,  32),
		CFForum(0x1C1C14223F9A047EULL, 0x20E0EADCC0647800ULL,  14),
		CFForum(0x18BC8F9F877F3018ULL, 0x2440706078000E06ULL,   8),
		CFForum(0x0000081F22426064ULL, 0x14BCF7E0DCBC9C98ULL,   8),
		CFForum(0x10B070222E942C3CULL, 0x2C0F0F5DD16B1000ULL,  -8),
		CFForum(0x002054A49716247CULL, 0x781CAB5B68E91800ULL,  10),
		CFForum(0x1202068A1A3070FEULL, 0x050D1935E54F8D00ULL,  30),
		CFForum(0x243C3B2B270F2000ULL, 0x0082C4D4D8F01C7CULL,  -8),
		CFForum(0x3E0C0F2B772B1000ULL, 0x0070705488142E7CULL,  14),
		CFForum(0x3E044F001C19303EULL, 0x0038307FE3E60C00ULL,  18),
		CFForum(0x00040D106C707201ULL, 0x081870EF938F8D3EULL,   4),
		CFForum(0x7EB8B99D0F0F0028ULL, 0x0006466270301C12ULL,  24),
		CFForum(0x0019C6D6C8F0A078ULL, 0x7C243829370D1800ULL,   8),
		CFForum(0x7004835360E91800ULL, 0x00387CAC9F16247CULL,  -2),
		CFForum(0x18BC878F87033018ULL, 0x24407870383C0E06ULL,   8),
		CFForum(0xEF7F3F1F18000000ULL, 0x1080C0E0E6F8FEFEULL,   6),
		CFForum(0xFF60365C786C7C00ULL, 0x009CC8A0849080F0ULL,   0),
		CFForum(0x3C3E345010142000ULL, 0x0080CBAFEF6B1D08ULL,   2),
		CFForum(0x00081C0BE5F9BD3CULL, 0x2030E2F41A060200ULL,   4),
		CFForum(0x241C0B070F130044ULL, 0x00025438F0ECFCB0ULL,   0),
		CFForum(0x0002707919D0E000ULL, 0x081C0F86662F18BCULL,   0),
		CFForum(0x0018060B135B0F00ULL, 0x7C2479746C242021ULL,   0),
		CFForum(0x20002C0C1A5F3C22ULL, 0x143CD2F3E4A00008ULL,  -2),
		CFForum(0x000143F569D30100ULL, 0x20383C0A162C7C1CULL,   0),
		CFForum(0x3C18C8E4C8F1B000ULL, 0x4024371B370C0800ULL,  10),
		CFForum(0x70A0D0A8979A007CULL, 0x00182C5468643E00ULL,   0),
		CFForum(0x00006070FCE08000ULL, 0x7C3C9C0C021D3C7CULL,  -2),
		CFForum(0x3034205031393D25ULL, 0x0088D8AECEC60200ULL,  -4),
		CFForum(0x7F20131B03530500ULL, 0x001C2C647C0C103CULL,  -8),
		CFForum(0x0000000462743A1EULL, 0x00057FFB9D0B0521ULL,  -2),
		CFForum(0x0001CFFD61710800ULL, 0x383C30021E0E1620ULL,   0),
		CFForum(0x040400665C3C1E00ULL, 0x1039FF99A3C18000ULL,   0),
		CFForum(0x00007068363C3830ULL, 0x3CBC8E94C8C08004ULL, -20),
		CFForum(0x00005921039F2800ULL, 0x3C3C26DE7C600000ULL,   4),
		CFForum(0xBC48707068400000ULL, 0x00B48E8C94B8B000ULL,  64),
		CFForum(0x0101312303010100ULL, 0x9E7ECEDCFC1E0800ULL,  38),
		CFForum(0x000200F8642C1800ULL, 0x7C3C7E0618D02472ULL,   0),
		CFForum(0x000C040486040200ULL, 0x3801FB7B391B1D3CULL,   6),
		CFForum(0x0000706062F60800ULL, 0x3E3C0C1E1C08143EULL, -12),
		CFForum(0x222563F1F0340000ULL, 0x08081C0E0CC83C1CULL, -14),
		CFForum(0x1EB8D0E8D8380000ULL, 0x00042C142446BC0CULL,   6),
		CFForum(0x1C04060703173078ULL, 0x003838783C280C02ULL,  -8),
		CFForum(0x7C3C780058E83000ULL, 0x000004FC24140C3CULL,   4),
		CFForum(0x00000C5860603800ULL, 0x04B8F0A69C98047EULL,  28),
		CFForum(0x10300604160C0420ULL, 0x240CF8F8E8F01808ULL,  16),
		CFForum(0x083808082A180800ULL, 0x0000777755673424ULL,  10),
		CFForum(0x080002831B233D0CULL, 0x02041C7C64DC0000ULL,   6),
		CFForum(0x00101810181C1000ULL, 0x100927EFE7E32100ULL,   0),
		CFForum(0x000078327C042404ULL, 0x0C1C070D033B0900ULL,  -2),
		CFForum(0x00C0F0F4ECC00000ULL, 0x38300F0810381C10ULL,  -2),
		CFForum(0x000007D0E0C0B818ULL, 0x00A0782C1E3C0400ULL,   0),
		CFForum(0x0000706840FA1000ULL, 0x3E3C0C143E040C00ULL,   2),
		CFForum(0x00001838340E1000ULL, 0x000007070B712D7CULL, -10),
		CFForum(0x200008042E3C2C00ULL, 0x1C38767A50C00000ULL,   4),
		CFForum(0x00000000010F3331ULL, 0x0000013E3EF00C04ULL,  64),
	};

	unsigned long long OverallNodeCounter = 0;
	std::vector<CSearch> SearchVector = std::vector<CSearch>();

	std::chrono::high_resolution_clock::time_point startTime, endTime, OverallStartTime, OverallEndTime;

	for (int i = begin; i <= end; i++)
		SearchVector.push_back(CSearch(FForum[i-1].P, FForum[i-1].O, -64, 64, hashTable));

	int i = begin;
	if (verbose)
	{
		printf(" # |depth|score|       time (s) |      nodes (N) |    N/s    |       PV\n");
		printf("---+-----+-----+----------------+----------------+-----------+----------------\n");
	}
	OverallStartTime = std::chrono::high_resolution_clock::now(); //Start Time
	for (auto it = SearchVector.begin(); it != SearchVector.end(); ++it){
		startTime = std::chrono::high_resolution_clock::now(); //Start Time
		it->Evaluate();
		endTime = std::chrono::high_resolution_clock::now(); //Stop Time
		hashTable->AdvanceDate();
		if (it->score != FForum[i-1].Score)
			printf("%3u was miscalculated. It's score should be %+2.2d.\n", i, FForum[i-1].Score);
		if (verbose)
		{
			std::chrono::milliseconds time_span = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
			if (time_span == std::chrono::milliseconds(0))
				printf("%3u|  %2u | %+2.2d | %14s | %14llu |           | %2s %2s %2s %2s %2s\n", i, NumberOfEmptyStones(it->P, it->O), it->score, time_format(time_span).c_str(), it->NodeCounter, it->GetPV(0), it->GetPV(1), it->GetPV(2), it->GetPV(3), it->GetPV(4));
			else
				printf("%3u|  %2u | %+2.2d | %14s | %14llu | %9d | %2s %2s %2s %2s %2s\n", i, NumberOfEmptyStones(it->P, it->O), it->score, time_format(time_span).c_str(), it->NodeCounter, it->NodeCounter*1000/time_span.count(), it->GetPV(0), it->GetPV(1), it->GetPV(2), it->GetPV(3), it->GetPV(4));
		}
		++i;
	}
	OverallEndTime = std::chrono::high_resolution_clock::now(); //Stop Time
	for (auto it = SearchVector.begin() ; it != SearchVector.end(); ++it)
		OverallNodeCounter += it->NodeCounter;
	std::chrono::milliseconds time_span = std::chrono::duration_cast<std::chrono::milliseconds>(OverallEndTime - OverallStartTime);
	if (time_span == std::chrono::milliseconds(0))
		printf("%16llu nodes in %14ss.\n", OverallNodeCounter, time_format(time_span).c_str());
	else
		printf("%16llu nodes in %14ss (%9d N/s).\n", OverallNodeCounter, time_format(time_span).c_str(), OverallNodeCounter*1000/time_span.count());
	if (verbose)
		printf("\n");
}

void PossibleMoves_Benchmark(const int N)
{
	unsigned long long a = 0x0569871035164909ULL &  0x9601656461938970ULL;
	unsigned long long b = 0x0569871035164909ULL & ~0x9601656461938970ULL;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = PossibleMoves(a, b);
		b = PossibleMoves(a, b);
		a = PossibleMoves(a, b);
		b = PossibleMoves(a, b);
		a = PossibleMoves(a, b);
		b = PossibleMoves(a, b);
		a = PossibleMoves(a, b);
		b = PossibleMoves(a, b);
		a = PossibleMoves(a, b);
		b = PossibleMoves(a, b);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("PossibleMoves  | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("PossibleMoves  | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void PossibleMoves2_Benchmark(const int N)
{
	unsigned long long a = 0x0569871035164909ULL &  0x9601656461938970ULL;
	unsigned long long b = 0x0569871035164909ULL & ~0x9601656461938970ULL;
	unsigned long long c, d;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		PossibleMoves(a, b, c, d);
		PossibleMoves(c, d, a, b);
		PossibleMoves(a, b, c, d);
		PossibleMoves(c, d, a, b);
		PossibleMoves(a, b, c, d);
		PossibleMoves(c, d, a, b);
		PossibleMoves(a, b, c, d);
		PossibleMoves(c, d, a, b);
		PossibleMoves(a, b, c, d);
		PossibleMoves(c, d, a, b);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("PossibleMoves2 | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("PossibleMoves2 | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void PlayStone_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<unsigned char>(0, 63), std::mt19937_64(N));
	std::vector<unsigned char> c(N+9);
	unsigned long long a = 0x0569871035164909ULL &  0x9601656461938970ULL;
	unsigned long long b = 0x0569871035164909ULL & ~0x9601656461938970ULL;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < N+9; i++) c[i] = rnd();

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		PlayStone(a, b, c[i  ]);
		PlayStone(a, b, c[i+1]);
		PlayStone(a, b, c[i+2]);
		PlayStone(a, b, c[i+3]);
		PlayStone(a, b, c[i+4]);
		PlayStone(a, b, c[i+5]);
		PlayStone(a, b, c[i+6]);
		PlayStone(a, b, c[i+7]);
		PlayStone(a, b, c[i+8]);
		PlayStone(a, b, c[i+9]);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("PlayStone      | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("PlayStone      | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

//void TryPlayStone_Benchmark(const int N)
//{
//	auto rnd = std::bind(std::uniform_int_distribution<unsigned char>(0, 63), std::mt19937_64(N));
//	std::vector<unsigned char> c(N+9);
//	unsigned long long a = 0x0569871035164909ULL &  0x9601656461938970ULL;
//	unsigned long long b = 0x0569871035164909ULL & ~0x9601656461938970ULL;
//	std::chrono::high_resolution_clock::time_point startTime, endTime;
//
//	for (int i = 0; i < N+9; i++) c[i] = rnd();
//
//	startTime = std::chrono::high_resolution_clock::now();
//	for (int i = 0; i < N; i++)
//	{
//		TryPlayStone(a, b, c[i  ]);
//		TryPlayStone(a, b, c[i+1]);
//		TryPlayStone(a, b, c[i+2]);
//		TryPlayStone(a, b, c[i+3]);
//		TryPlayStone(a, b, c[i+4]);
//		TryPlayStone(a, b, c[i+5]);
//		TryPlayStone(a, b, c[i+6]);
//		TryPlayStone(a, b, c[i+7]);
//		TryPlayStone(a, b, c[i+8]);
//		TryPlayStone(a, b, c[i+9]);
//	}
//	endTime = std::chrono::high_resolution_clock::now();
//	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
//	if (a) //Prevent dead code elimination.
//		printf("TryPlayStone   | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
//	else
//		printf("TryPlayStone   | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
//}

void Flip_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<unsigned char>(0, 63), std::mt19937_64(N));
	std::vector<unsigned char> c(N+9);
	unsigned long long a = 0x0569871035164909ULL &  0x9601656461938970ULL;
	unsigned long long b = 0x0569871035164909ULL & ~0x9601656461938970ULL;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < N+9; i++) c[i] = rnd();

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = flip(a, b, c[i  ]);
		b = flip(a, b, c[i+1]);
		a = flip(a, b, c[i+2]);
		b = flip(a, b, c[i+3]);
		a = flip(a, b, c[i+4]);
		b = flip(a, b, c[i+5]);
		a = flip(a, b, c[i+6]);
		b = flip(a, b, c[i+7]);
		a = flip(a, b, c[i+8]);
		b = flip(a, b, c[i+9]);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("Flip           | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("Flip           | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void Count_last_flip_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<unsigned char>(0, 63), std::mt19937_64(N));
	std::vector<unsigned char> c(N+9);
	unsigned long long a = 0x0569871035164909ULL &  0x9601656461938970ULL;
	unsigned long long b = 0x0569871035164909ULL & ~0x9601656461938970ULL;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < N+9; i++) c[i] = rnd();

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		b = count_last_flip(a, c[i  ]);
		a = count_last_flip(b, c[i+1]);
		b = count_last_flip(a, c[i+2]);
		a = count_last_flip(b, c[i+3]);
		b = count_last_flip(a, c[i+4]);
		a = count_last_flip(b, c[i+5]);
		b = count_last_flip(a, c[i+6]);
		a = count_last_flip(b, c[i+7]);
		b = count_last_flip(a, c[i+8]);
		a = count_last_flip(b, c[i+9]);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("Count last flip| %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("Count last flip| %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void POP_COUNT_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<unsigned long long>(0ULL, 0xFFFFFFFFFFFFFFFFULL), std::mt19937_64(N));
	unsigned long long b[64];
	unsigned long long a = 0;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < 64; i++) b[i] = rnd();

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = POP_COUNT(b[a]);
		a = POP_COUNT(a);
		a = POP_COUNT(a);
		a = POP_COUNT(a);
		a = POP_COUNT(a);
		a = POP_COUNT(a);
		a = POP_COUNT(a);
		a = POP_COUNT(a);
		a = POP_COUNT(a);
		a = POP_COUNT(a);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("POP_COUNT      | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("POP_COUNT      | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void POP_COUNT_MAX15_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<unsigned long long>(0ULL, 0xFFFFFFFFFFFFFFFFULL), std::mt19937_64(N));
	unsigned long long b[64];
	unsigned long long a = 0;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < 64; i++) b[i] = rnd();

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = POP_COUNT_MAX15(b[a]);
		a = POP_COUNT_MAX15(a);
		a = POP_COUNT_MAX15(a);
		a = POP_COUNT_MAX15(a);
		a = POP_COUNT_MAX15(a);
		a = POP_COUNT_MAX15(a);
		a = POP_COUNT_MAX15(a);
		a = POP_COUNT_MAX15(a);
		a = POP_COUNT_MAX15(a);
		a = POP_COUNT_MAX15(a);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("POP_COUNT_MAX15| %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("POP_COUNT_MAX15| %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void POP_COUNT_PLUS_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<unsigned long long>(0ULL, 0xFFFFFFFFFFFFFFFFULL), std::mt19937_64(N));
	__m128i b[128];
	unsigned long long a = 0;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < 128; i++) b[i] = _mm_set_epi64x(rnd(), rnd());

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = POP_COUNT_PLUS(b[a]);
		a = POP_COUNT_PLUS(b[a]);
		a = POP_COUNT_PLUS(b[a]);
		a = POP_COUNT_PLUS(b[a]);
		a = POP_COUNT_PLUS(b[a]);
		a = POP_COUNT_PLUS(b[a]);
		a = POP_COUNT_PLUS(b[a]);
		a = POP_COUNT_PLUS(b[a]);
		a = POP_COUNT_PLUS(b[a]);
		a = POP_COUNT_PLUS(b[a]);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("POP_COUNT_PLUS | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("POP_COUNT_PLUS | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void Parity_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<unsigned long long>(0ULL, 0xFFFFFFFFFFFFFFFFULL), std::mt19937_64(N));
	unsigned long long b[64];
	unsigned long long a = 0;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < 64; i++) b[i] = rnd();

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = parity(b[a]);
		a = parity(a);
		a = parity(a);
		a = parity(a);
		a = parity(a);
		a = parity(a);
		a = parity(a);
		a = parity(a);
		a = parity(a);
		a = parity(a);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("Parity         | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("Parity         | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void BIT_SCAN_LS1B_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<unsigned long long>(1ULL, 0xFFFFFFFFFFFFFFFFULL), std::mt19937_64(N));
	unsigned long long b[64];
	unsigned long long a = 0;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < 64; i++) b[i] = rnd();

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = BIT_SCAN_LS1B(b[a]);
		a = BIT_SCAN_LS1B(a);
		a = BIT_SCAN_LS1B(a);
		a = BIT_SCAN_LS1B(a);
		a = BIT_SCAN_LS1B(a);
		a = BIT_SCAN_LS1B(a);
		a = BIT_SCAN_LS1B(a);
		a = BIT_SCAN_LS1B(a);
		a = BIT_SCAN_LS1B(a);
		a = BIT_SCAN_LS1B(a);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("BIT_SCAN_LS1B  | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("BIT_SCAN_LS1B  | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void REMOVE_LS1B_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<unsigned long long>(1ULL, 0xFFFFFFFFFFFFFFFFULL), std::mt19937_64(N));
	std::vector<unsigned long long> a(N);
	unsigned long long b = 0ULL;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < N; i++) a[i] = rnd();

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		b |= a[i];
		REMOVE_LS1B(b);
		REMOVE_LS1B(b);
		REMOVE_LS1B(b);
		REMOVE_LS1B(b);
		REMOVE_LS1B(b);
		REMOVE_LS1B(b);
		REMOVE_LS1B(b);
		REMOVE_LS1B(b);
		REMOVE_LS1B(b);
		REMOVE_LS1B(b);
	}
	endTime = std::chrono::high_resolution_clock::now();

	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (b) //Prevent dead code elimination.
		printf("REMOVE_LS1B    | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("REMOVE_LS1B    | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void StableStones_corner_and_co_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<unsigned long long>(0ULL, 0xFFFFFFFFFFFFFFFFULL), std::mt19937_64(N));
	unsigned long long b[64];
	unsigned long long a = 0;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < 64; i++) b[i] = rnd();

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = StableStones_corner_and_co(b[a % 64]);
		a = StableStones_corner_and_co(a);
		a = StableStones_corner_and_co(a);
		a = StableStones_corner_and_co(a);
		a = StableStones_corner_and_co(a);
		a = StableStones_corner_and_co(a);
		a = StableStones_corner_and_co(a);
		a = StableStones_corner_and_co(a);
		a = StableStones_corner_and_co(a);
		a = StableStones_corner_and_co(a);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("corner and co  | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("corner and co  | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void StableStones_affectables_Benchmark(const int N)
{
	unsigned long long a = 0x0569871035164909ULL &  0x9601656461938970ULL;
	unsigned long long b = 0x0569871035164909ULL & ~0x9601656461938970ULL;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = StableStones_affectables(a, b);
		b = StableStones_affectables(a, b);
		a = StableStones_affectables(a, b);
		b = StableStones_affectables(a, b);
		a = StableStones_affectables(a, b);
		b = StableStones_affectables(a, b);
		a = StableStones_affectables(a, b);
		b = StableStones_affectables(a, b);
		a = StableStones_affectables(a, b);
		b = StableStones_affectables(a, b);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("affectables    | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("affectables    | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void StableStones_full_edges_second_order_Benchmark(const int N)
{
	unsigned long long a = 0x0569871035164909ULL &  0x9601656461938970ULL;
	unsigned long long b = 0x0569871035164909ULL & ~0x9601656461938970ULL;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = StableStones_full_edges_second_order(a, b);
		b = StableStones_full_edges_second_order(a, b);
		a = StableStones_full_edges_second_order(a, b);
		b = StableStones_full_edges_second_order(a, b);
		a = StableStones_full_edges_second_order(a, b);
		b = StableStones_full_edges_second_order(a, b);
		a = StableStones_full_edges_second_order(a, b);
		b = StableStones_full_edges_second_order(a, b);
		a = StableStones_full_edges_second_order(a, b);
		b = StableStones_full_edges_second_order(a, b);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("full_edges     | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("full_edges     | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void StableStones_triangles_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<unsigned long long>(0ULL, 0xFFFFFFFFFFFFFFFFULL), std::mt19937_64(N));
	unsigned long long b[64];
	unsigned long long a = 0;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < 64; i++) b[i] = rnd();

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = StableStones_triangles(b[a % 64]);
		a = StableStones_triangles(a);
		a = StableStones_triangles(a);
		a = StableStones_triangles(a);
		a = StableStones_triangles(a);
		a = StableStones_triangles(a);
		a = StableStones_triangles(a);
		a = StableStones_triangles(a);
		a = StableStones_triangles(a);
		a = StableStones_triangles(a);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("triangles      | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("triangles      | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void StableStones_skyline_Benchmark(const int N)
{
	auto rnd = std::bind(std::uniform_int_distribution<unsigned long long>(0ULL, 0xFFFFFFFFFFFFFFFFULL), std::mt19937_64(N));
	unsigned long long b[64];
	unsigned long long a = 0;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	for (int i = 0; i < 64; i++) b[i] = rnd();

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = StableStones_skyline(b[a % 64]);
		a = StableStones_skyline(a);
		a = StableStones_skyline(a);
		a = StableStones_skyline(a);
		a = StableStones_skyline(a);
		a = StableStones_skyline(a);
		a = StableStones_skyline(a);
		a = StableStones_skyline(a);
		a = StableStones_skyline(a);
		a = StableStones_skyline(a);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("skyline        | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("skyline        | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void StableStones_Benchmark(const int N)
{
	unsigned long long a = 0x0569871035164909ULL &  0x9601656461938970ULL;
	unsigned long long b = 0x0569871035164909ULL & ~0x9601656461938970ULL;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a = StableStones(a, b);
		b = StableStones(a, b);
		a = StableStones(a, b);
		b = StableStones(a, b);
		a = StableStones(a, b);
		b = StableStones(a, b);
		a = StableStones(a, b);
		b = StableStones(a, b);
		a = StableStones(a, b);
		b = StableStones(a, b);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("StableStones   | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("StableStones   | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}

void EvaluateFeatures_Benchmark(const int N)
{
	unsigned long long a = 0x0569871035164909ULL &  0x9601656461938970ULL;
	unsigned long long b = 0x0569871035164909ULL & ~0x9601656461938970ULL;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		a ^= EvaluateFeatures(a & b & 0xFFFFFFE7E7FFFFFFULL, a & ~b | 0x0000001818000000ULL);
		b ^= EvaluateFeatures(a & b & 0xFFFFFFE7E7FFFFFFULL, a & ~b | 0x0000001818000000ULL);
		a ^= EvaluateFeatures(a & b & 0xFFFFFFE7E7FFFFFFULL, a & ~b | 0x0000001818000000ULL);
		b ^= EvaluateFeatures(a & b & 0xFFFFFFE7E7FFFFFFULL, a & ~b | 0x0000001818000000ULL);
		a ^= EvaluateFeatures(a & b & 0xFFFFFFE7E7FFFFFFULL, a & ~b | 0x0000001818000000ULL);
		b ^= EvaluateFeatures(a & b & 0xFFFFFFE7E7FFFFFFULL, a & ~b | 0x0000001818000000ULL);
		a ^= EvaluateFeatures(a & b & 0xFFFFFFE7E7FFFFFFULL, a & ~b | 0x0000001818000000ULL);
		b ^= EvaluateFeatures(a & b & 0xFFFFFFE7E7FFFFFFULL, a & ~b | 0x0000001818000000ULL);
		a ^= EvaluateFeatures(a & b & 0xFFFFFFE7E7FFFFFFULL, a & ~b | 0x0000001818000000ULL);
		b ^= EvaluateFeatures(a & b & 0xFFFFFFE7E7FFFFFFULL, a & ~b | 0x0000001818000000ULL);
	}
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	if (a) //Prevent dead code elimination.
		printf("EvalFeatures   | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
	else
		printf("EvalFeatures   | %6.1f | %14s\n", static_cast<double>(duration.count()) / N / 10 * 1000000, time_format(duration));
}


unsigned long long Solve(const int d, DATASET_POSITON_SCORE* DataArray, const unsigned long long size)
{
	CHashTable * HashTable = new CHashTable(24);
	unsigned long long NodeCounter = 0;
	std::chrono::high_resolution_clock::time_point startTime, endTime;

	if (d < 5)
	{
		startTime = std::chrono::high_resolution_clock::now();
		for (unsigned long long i = 0; i < size; i++)
			EvaluateExact(DataArray[i].P, DataArray[i].O, NodeCounter, d);
		endTime = std::chrono::high_resolution_clock::now();
	}
	else
	{
		startTime = std::chrono::high_resolution_clock::now();
		for (unsigned long long i = 0; i < size; i++)
		{
			CSearch search(DataArray[i].P, DataArray[i].O, -64, 64, HashTable);
			search.Evaluate();
			NodeCounter += search.NodeCounter;
		}
		endTime = std::chrono::high_resolution_clock::now();
	}

	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	printf("%3u| %14s | %14llu | %9d | %9d | %s\n", d, time_format(std::chrono::duration_cast<std::chrono::milliseconds>(duration)), 
		NodeCounter, NodeCounter*1000/duration.count(), size*1000/duration.count(), short_time_format(std::chrono::duration_cast<std::chrono::duration<long long, std::pico>>(duration)/size));

	delete HashTable;
	return NodeCounter;
}

void ReadInFile(const char * const filename, DATASET_POSITON_SCORE * DataArray, const unsigned long long size)
{
	FILE* file;
	fopen_s(&file, filename, "rb");

	std::size_t ValidData = fread(DataArray, sizeof(DATASET_POSITON_SCORE), size, file);
	if (ValidData != size)
		std::cerr << "ERROR: Length of file is wrong!" << std::endl;

	fclose(file);
}

unsigned long long HugeBenchmak(const int LowerLimit, const int UpperLimit, const int seconds)
{
	unsigned long long NodeCounter = 0;
	DATASET_POSITON_SCORE* DataArray;
	char buffer[1000];
	unsigned long long size[21] = {
		 5000000, //d0
		 2000000, //d1
		 1000000, //d2
		  800000, //d3
		  300000, //d4
		  100000, //d5
		   50000, //d6
		   20000, //d7
		    6000, //d8
		    2000, //d9
		    1000, //d10
		     400, //d11
		     150, //d12
		      60, //d13
		      20, //d14
		      10, //d15
		       5, //d16
			   2, //d17
			   1, //d18
			   1, //d19
			   1  //d20
	};


	printf(" d |       time (s) |      nodes (N) |    N/s    |    P/s    | time/P \n");
	printf("---+----------------+----------------+-----------+-----------+--------\n");

	for (int d = LowerLimit; d <= UpperLimit; ++d)
	{
		DataArray = new DATASET_POSITON_SCORE[size[d] * seconds];
		sprintf_s(buffer, "G:\\Reversi\\pos\\rnd_d%d_100M.b", d);
		ReadInFile(buffer, DataArray, size[d] * seconds);
		NodeCounter += Solve(d, DataArray, size[d] * seconds);
		delete [] DataArray;
	}
	printf("%16llu nodes.\n", NodeCounter);

	return NodeCounter;
}

int main(int argc, char* argv[])
{
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

	//CHashTable* hashTable = new CHashTable(24);
	ConfigFile::Initialize(std::string("F:\\Reversi\\ProjectBrutus.ini"));
	Features::Initialize();

	SolverBenchmarkEmpties(10, 20, 10, CSearch::END, 0, 1, 24);
	//SolverBenchmarkDepth(24/*Empties*/, 5/*Secounds*/, 0/*Lowerdepth*/, 14/*Upperdepth*/, 0/*selectivity*/, 1/*nthreads*/, 24/*HashTableBits*/);
	//HugeBenchmak(0, 20, 20);
	//FForum_Benchmark( 1, 19, true, hashTable);
	//FForum_Benchmark(20, 39, true, hashTable);
	//FForum_Benchmark(40, 59, true, hashTable);

	//HugeBenchmak(20, 20);

	//printf(" d |   Runtime [s] \n");
	//printf("---+----------------\n");
	//std::chrono::high_resolution_clock::time_point startTime, endTime;
	//for (int d = 0; d < 20; ++d){
	//	CSearch search(START_POSITION_ETH_P, START_POSITION_ETH_O, -64, 64, d, 0, hashTablePV, hashTableL1, hashTableL2, CSearch::NodeType::PV_Node);
	//	startTime = std::chrono::high_resolution_clock::now();
	//	search.Evaluate();
	//	endTime = std::chrono::high_resolution_clock::now();
	//	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	//	printf("%3u| %14s\n", d, time_format(duration).c_str());
	//}

	//std::vector<DATASET_POSITON_SCORE> Data;
	//read_vector("G:\\Reversi\\pos\\rnd_d5_10M.b", Data);
	//const std::vector<DATASET_POSITON_SCORE>::const_iterator end = Data.cend();
	//for (std::vector<DATASET_POSITON_SCORE>::const_iterator it = Data.cbegin(); it != end; ++it)
	//	std::cout << EvaluateFeatures(it->P, it->O) << "\t" << (int)it->score << std::endl;
	//return 0;
	//HugeBenchmak(20, 20);

	//if (Test_All())
	//	std::cout << "Test successful!" << std::endl;
	//else
	//	std::cout << "Test failed!" << std::endl;
	//return 0;

	//HugeBenchmak(20, 20);
	//HugeBenchmak(10, 20);
	//HugeBenchmak(10, 20);
	//HugeBenchmak(10, 20);
	//return 0;

	//printf("   Routine     |  [ns]  |   Runtime [s] \n");
	//printf("---------------+--------+---------------\n");
	//PossibleMoves_Benchmark(8000000);
	//PossibleMoves_Benchmark(8000000);
	//PossibleMoves_Benchmark(8000000);
	//PossibleMoves_Benchmark(8000000);
	//PossibleMoves_Benchmark(8000000);
	//PossibleMoves2_Benchmark(8000000);
	//PossibleMoves2_Benchmark(8000000);
	//PossibleMoves2_Benchmark(8000000);
	//PossibleMoves2_Benchmark(8000000);
	//PossibleMoves2_Benchmark(8000000);
	//PlayStone_Benchmark(10000000);
	//Flip_Benchmark(10000000);
	//Flip_Benchmark(10000000);
	//Flip_Benchmark(10000000);
	//Flip_Benchmark(10000000);
	//Flip_Benchmark(10000000);
	//Parity_Benchmark(10000000);
	//Parity_Benchmark(10000000);
	//Parity_Benchmark(10000000);
	//Parity_Benchmark(10000000);
	//Parity_Benchmark(10000000);
	//Count_last_flip_Benchmark(20000000);
	//POP_COUNT_Benchmark(30000000);
	//POP_COUNT_MAX15_Benchmark(30000000);
	//POP_COUNT_PLUS_Benchmark(30000000);
	//BIT_SCAN_LS1B_Benchmark(200000000);
	//REMOVE_LS1B_Benchmark(200000000);
	//StableStones_corner_and_co_Benchmark(50000000);
	//StableStones_affectables_Benchmark(10000000);
	//StableStones_full_edges_second_order_Benchmark(50000000);
	//StableStones_triangles_Benchmark(50000000);
	//StableStones_skyline_Benchmark(10000000);
	//StableStones_Benchmark(3000000);
	//EvaluateFeatures_Benchmark(800000);
	//EvaluateFeatures_Benchmark(800000);
	//EvaluateFeatures_Benchmark(800000);
	
	//FForum_Benchmark( 1, 19, true, hashTable);
	//FForum_Benchmark(20, 39, true, hashTable);
	//FForum_Benchmark(40, 59, true, hashTable);

	//delete hashTable;
	Features::Finalize();
	return 0;
}