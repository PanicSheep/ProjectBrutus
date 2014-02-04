#include "utility.h"

unsigned long long NEIGHBOUR(unsigned long long BitBoard)
{
    unsigned long long BitBoardNeighbour = 0;
    while (BitBoard)
    {
        BitBoardNeighbour |= neighbour[BIT_SCAN_LS1B(BitBoard)];
        REMOVE_LS1B(BitBoard);
    }
    return BitBoardNeighbour;
}

unsigned long long LINES(unsigned long long BitBoard)
{
    unsigned long long BitBoardLines = 0;
    while (BitBoard)
    {
        BitBoardLines |= lines[BIT_SCAN_LS1B(BitBoard)];
        REMOVE_LS1B(BitBoard);
    }
    return BitBoardLines;
}

unsigned long long TO_CHECK(unsigned long long BitBoard)
{
    unsigned long long BitBoardToCheck = 0;
    while (BitBoard)
    {
        BitBoardToCheck |= to_check[BIT_SCAN_LS1B(BitBoard)];
        REMOVE_LS1B(BitBoard);
    }
    return BitBoardToCheck;
}

unsigned long long AFFECTABLE(unsigned long long BitBoard)
{
    unsigned long long BitBoardAffectable = 0;
    while (BitBoard)
    {
        BitBoardAffectable |= affectable[BIT_SCAN_LS1B(BitBoard)];
        REMOVE_LS1B(BitBoard);
    }
    return BitBoardAffectable;
}

unsigned long long diagonal_flip(unsigned long long b)
{
	unsigned long long t;
	t  = (b ^ (b >> 7)) & 0x00aa00aa00aa00aaUL;
	b ^=  t ^ (t << 7);
	t  = (b ^ (b >> 14)) & 0x0000cccc0000ccccUL;
	b ^=  t ^ (t << 14);
	t  = (b ^ (b >> 28)) & 0x00000000f0f0f0f0UL;
	b ^=  t ^ (t << 28);
	return b;
}
unsigned long long codiagonal_flip(unsigned long long b)
{
	unsigned long long t;
	t  =  b ^ (b << 36);
	b ^= (t ^ (b >> 36)) & 0xf0f0f0f00f0f0f0fULL;
	t  = (b ^ (b << 18)) & 0xcccc0000cccc0000ULL;
	b ^=  t ^ (t >> 18);
	t  = (b ^ (b <<  9)) & 0xaa00aa00aa00aa00ULL;
	b ^=  t ^ (t >>  9);
	return b;
}
unsigned long long vertical_flip(unsigned long long b)
{
	b = ((b >> 8) & 0x00FF00FF00FF00FFUL) | ((b << 8) & 0xFF00FF00FF00FF00UL);
	b = ((b >> 16) & 0x0000FFFF0000FFFFUL) | ((b << 16) & 0xFFFF0000FFFF0000UL);
	b = ((b >> 32) & 0x00000000FFFFFFFFUL) | ((b << 32) & 0xFFFFFFFF00000000UL);
	return b;
}
unsigned long long horizontal_flip(unsigned long long b)
{
	b = ((b >> 1) & 0x5555555555555555UL) | ((b << 1) & 0xAAAAAAAAAAAAAAAAUL);
	b = ((b >> 2) & 0x3333333333333333UL) | ((b << 2) & 0xCCCCCCCCCCCCCCCCUL);
	b = ((b >> 4) & 0x0F0F0F0F0F0F0F0FUL) | ((b << 4) & 0xF0F0F0F0F0F0F0F0UL);
	return b;
}

std::string SCIENTIFIC_NOTATION(double Number)
{
    char Notation[19] = "YZEPTGMk munpfazy";
	std::stringstream sstm;
    int exp = static_cast<int>(floor(log10(std::abs(Number)) / 3.0));
		
    Number /= std::pow(10., 3 * exp);
		
	sstm << Notation[8 - exp] << Number;
	return sstm.str();
}

std::string time_format(const std::chrono::milliseconds duration)
{
	char time[15] = "          .   "; //d:hh:mm:ss.ccc
	int char_0 = static_cast<int>('0');

	typedef std::chrono::duration<int, std::ratio<24*3600> > days;
	long long c = duration.count() % 1000;
	long long s = std::chrono::duration_cast<std::chrono::seconds>(duration).count() % 60;
	int m = std::chrono::duration_cast<std::chrono::minutes>(duration).count() % 60;
	int h = std::chrono::duration_cast<std::chrono::hours>(duration).count() % 24;
	int d = std::chrono::duration_cast<days>(duration).count();

	if (d)
	{
		time[0] = char_0 + d;
		time[1] = ':';
	}
	if (d || h/10)
		time[2] = char_0 + h/10;
	if (d || h/10 || h)
	{
		time[3] = char_0 + h%10;
		time[4] = ':';
	}
	if (d || h || m/10)
		time[5] = char_0 + m/10;
	if (d || h || m/10 || m)
	{
		time[6] = char_0 + m%10;
		time[7] = ':';
	}
	if (d || h || m || s/10)
		time[8] = char_0 + s/10;

	time[9] = char_0 + s%10;
	time[11] = char_0 + c/100;
	time[12] = char_0 + (c/10)%10;
	time[13] = char_0 + c%10;

	return std::string(time);
}

std::string short_time_format(std::chrono::duration<long long, std::pico> duration)
{
	char buff[100];

	long long millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	long long micros = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
	long long nanoss = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
	long long picos = duration.count();
		
		 if (millis >= 100) sprintf(buff, "%4ums", millis);
	else if (millis >=  10) sprintf(buff, "%2.1fms", static_cast<double>(micros) / 1000.0);
	else if (millis >=   1) sprintf(buff, "%1.2fms", static_cast<double>(micros) / 1000.0);
	else if (micros >= 100) sprintf(buff, "%4uus", micros);
	else if (micros >=  10) sprintf(buff, "%2.1fus", static_cast<double>(nanoss) / 1000.0);
	else if (micros >=   1) sprintf(buff, "%1.2fus", static_cast<double>(nanoss) / 1000.0);
	else if (nanoss >= 100) sprintf(buff, "%4uns", nanoss);
	else if (nanoss >=  10) sprintf(buff, "%2.1fns", static_cast<double>(picos) / 1000.0);
	else if (nanoss >=   1) sprintf(buff, "%1.2fns", static_cast<double>(picos) / 1000.0);
	else if (picos        ) sprintf(buff, "%4ups", picos);
	else
		sprintf(buff, "Error!");

	return std::string(buff);
}

void print_board(const unsigned long long P, const unsigned long long O)
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