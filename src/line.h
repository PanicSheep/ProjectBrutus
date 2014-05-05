#pragma once
#include <algorithm>
#include <cassert>

class CLine
{
public:
	unsigned char size;
	unsigned char * line;

	CLine() : size(0), line(0) {}
	CLine(const unsigned char size) : size(size) {
		line = new unsigned char[size];
		std::fill_n(line, size, 64);
	}
	CLine & operator=(const CLine & other) {
		if (this != &other){
			size = other.size;
			delete[] line;
			line = new unsigned char[size];
			std::copy_n(other.line, size, line);
		}
		return *this;
	}
	~CLine() { delete[] line; }

	inline void NewPV(const unsigned char newMove, const CLine & newLine){
		assert(size == newLine.size+1);
		line[0] = newMove;
		std::copy_n(newLine.line, newLine.size, line+1);
	}
	inline void NoMoves() { std::fill_n(line, size, 64); }
	inline unsigned char BestMove() const { return line[0]; }
	
	inline const std::string GetPV(const int depth) const
	{
		if (depth >= size)
			return "--";
		else if ((line[depth] >= 0) && (line[depth] <= 64))
			return FIELD_NAME(line[depth]);
		else
			return "??";
	}
	inline const std::string GetPV(const int StartDepth, const int num) const
	{
		std::string s;
		for (int i = 0; i < num; i++)
		{
			s.append(GetPV(StartDepth + i));
			s.append(" ");
		}
		return s.substr(0, s.length() - 1);
	}
	inline const std::string GetPV() const { return GetPV(0, size); }
	
};