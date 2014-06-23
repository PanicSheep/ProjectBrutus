#include "utility.h"

void REVERSE_BITORDER(unsigned long long & v)
{
	v = ((v >>  1) & 0x5555555555555555ULL) | ((v & 0x5555555555555555ULL) <<  1);
	v = ((v >>  2) & 0x3333333333333333ULL) | ((v & 0x3333333333333333ULL) <<  2);
	v = ((v >>  4) & 0x0F0F0F0F0F0F0F0FULL) | ((v & 0x0F0F0F0F0F0F0F0FULL) <<  4);
	v = ((v >>  8) & 0x00FF00FF00FF00FFULL) | ((v & 0x00FF00FF00FF00FFULL) <<  8);
	v = ((v >> 16) & 0x0000FFFF0000FFFFULL) | ((v & 0x0000FFFF0000FFFFULL) << 16);
	v = ( v >> 32                         ) | ( v                          << 32);
}

unsigned char FIELD_INDEX(const std::string & s)
{
	for (int i = 0; i < 64; i++)
		if (field_name[i] == s)
			return i;
	return 64;
}
unsigned long long NEIGHBOUR(unsigned long long BitBoard)
{
    unsigned long long BitBoardNeighbour = 0;
    while (BitBoard)
    {
        BitBoardNeighbour |= neighbour[BitScanLSB(BitBoard)];
        RemoveLSB(BitBoard);
    }
    return BitBoardNeighbour;
}

unsigned long long AFFECTABLE(unsigned long long BitBoard)
{
    unsigned long long BitBoardAffectable = 0;
    while (BitBoard)
    {
        BitBoardAffectable |= affectable[BitScanLSB(BitBoard)];
        RemoveLSB(BitBoard);
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
		
		 if (millis >= 100) sprintf_s(buff, "%4ums", millis);
	else if (millis >=  10) sprintf_s(buff, "%2.1fms", static_cast<double>(micros) / 1000.0);
	else if (millis >=   1) sprintf_s(buff, "%1.2fms", static_cast<double>(micros) / 1000.0);
	else if (micros >= 100) sprintf_s(buff, "%4uus", micros);
	else if (micros >=  10) sprintf_s(buff, "%2.1fus", static_cast<double>(nanoss) / 1000.0);
	else if (micros >=   1) sprintf_s(buff, "%1.2fus", static_cast<double>(nanoss) / 1000.0);
	else if (nanoss >= 100) sprintf_s(buff, "%4uns", nanoss);
	else if (nanoss >=  10) sprintf_s(buff, "%2.1fns", static_cast<double>(picos) / 1000.0);
	else if (nanoss >=   1) sprintf_s(buff, "%1.2fns", static_cast<double>(picos) / 1000.0);
	else if (picos        ) sprintf_s(buff, "%4ups", picos);
	else
		sprintf_s(buff, "Error!");

	return std::string(buff);
}

std::string board(const unsigned long long P, const unsigned long long O)
{
	std::string s;
	for (unsigned int i = 0; i < 8; i++)
	{
		for (unsigned int j = 0; j < 8; j++)
		{
			if (P & (0x8000000000000000ULL >> (i * 8 + j)))
				s.append("X");
			else if (O & (0x8000000000000000ULL >> (i * 8 + j)))
				s.append("O");
			else
				s.append("-");
		}
	}
	return s;
}

std::string board2D(const unsigned long long P, const unsigned long long O)
{
	std::string s = "  H G F E D C B A  \n";
	for (unsigned int i = 0; i < 8; i++)
	{
		s.append(std::to_string(9-i));
		for (unsigned int j = 0; j < 8; j++)
		{
			if (P & (0x8000000000000000ULL >> (i * 8 + j)))
				s.append(" X");
			else if (O & (0x8000000000000000ULL >> (i * 8 + j)))
				s.append(" O");
			else
				s.append(" -");
		}
		s.append(" " + std::to_string(i) + "\n");
	}
	return s.append("  H G F E D C B A  ");
}

void print_board(const unsigned long long P, const unsigned long long O)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (P & (0x8000000000000000ULL >> (i*8+j)))
				std::cout << "X";
			else if (O & (0x8000000000000000ULL >> (i*8+j)))
				std::cout << "O";
			else
				std::cout << "-";
		}
	}
}

void print_board(const unsigned long long P, const unsigned long long O, const unsigned long long PossibleMoves)
{
	std::cout << "  H G F E D C B A  \n";
	for (int i = 0; i < 8; i++)
	{
		std::cout << 8-i << " ";
		for (int j = 0; j < 8; j++)
		{
			if (P & (0x8000000000000000ULL >> (i * 8 + j)))
				std::cout << "X ";
			else if (O & (0x8000000000000000ULL >> (i * 8 + j)))
				std::cout << "O ";
			else if (PossibleMoves & (0x8000000000000000ULL >> (i * 8 + j)))
				std::cout << ". ";
			else
				std::cout << "- ";
		}
		std::cout << 8-i << std::endl;
	}
	std::cout << "  H G F E D C B A  \n";
}

void print_progressbar(const int width, const float fraction)
{
	const unsigned char done = 219;
	const unsigned char togo = 177;
	const int d = static_cast<const int>(fraction * width);
	printf("%s%s", std::string(d, done).c_str(), std::string(width - d, togo).c_str());
	//std::cout << std::string(d, done) << std::string(width - d, togo);
}
std::string progressbar_percentage(const int width, const float fraction)
{
	const unsigned char done = 219;
	const unsigned char togo = 177;
	const int d = static_cast<const int>(fraction * width);
	std::string s = std::string(d, done);
	s.append(std::string(width - d, togo));
	s.append(" ");
	s.append(std::to_string(static_cast<int>(fraction * 1000) / 10.0f));
	s.append("%");
	return s;
}

std::string ThousandsSeparator(unsigned long long n)
{
	if (n < 1000)
		return std::to_string(n);
	else
		return ThousandsSeparator(n / 1000ULL).append("'").append(std::string(3 - std::to_string(n % 1000ULL).length(), '0')).append(std::to_string(n % 1000ULL));
}

std::string DateTimeNow()
{
	std::chrono::system_clock::time_point p = std::chrono::system_clock::now();

	std::time_t t = std::chrono::system_clock::to_time_t(p);
	return std::string(std::ctime(&t));
}