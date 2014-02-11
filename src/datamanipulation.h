#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <cassert>

#pragma pack(1)
// 144 Bit = 18 Byte
struct DATASET_POSITON_SCORE
{
	unsigned long long P;	// 64 bit
	unsigned long long O;	// 64 bit
	signed char depth;		//  8 bit
	signed char score;		//  8 bit
	void Reset(){
		depth = -1;
		score = SCHAR_MIN;
	}
};
#pragma pack()

#pragma pack(1)
// 192 Bit = 24 Byte
struct DATASET_POSITON_SCORE_PV
{
	unsigned long long P;	// 64 bit
	unsigned long long O;	// 64 bit
	signed char depth;		//  8 bit
	signed char score;		//  8 bit
	unsigned char PV[6];	// 48 bit
	void Reset(){
		depth = -1;
		score = SCHAR_MIN;
		for (int i = 0; i < 6; ++i)
			PV[i] = 64;
	}
};
#pragma pack()

#pragma pack(1)
// 648 Bit = 81 Byte
struct DATASET_POSITON_FULL_SCORE
{
	unsigned long long P;	//  64 bit
	unsigned long long O;	//  64 bit
	signed char depth;		//   8 bit
	signed char score[64];	// 512 bit
	void Reset(){
		depth = -1;
		for (int i = 0; i < 64; ++i)
			score[i] = SCHAR_MIN;
	}
};
#pragma pack()


template<class T>
std::vector<T> read_vector(const char * const filename)
{
	std::vector<T> vector;
	T data;
	FILE* file;
	fopen_s(&file, filename, "rb");
	if(!file){
		std::cerr << "ERROR: File '" << filename << "' could not be opened!" << std::endl;
		return std::vector<T>();
	}

	while (fread(&data, sizeof(T), 1, file))
		vector.push_back(std::move(data));

	fclose(file);
	return vector;
}

template<class T>
void read_vector(const char * const filename, std::vector<T>& vector)
{
	assert(vector.size() == 0);
	FILE* file;
	fopen_s(&file, filename, "rb");
	if(!file){
		std::cerr << "ERROR: File '" << filename << "' could not be opened!" << std::endl;
		return;
	}

	const int N = 10000;
	std::size_t ValidData;
	T DataArray[N];
	while (ValidData = fread(&DataArray, sizeof(T), N, file))
		for (std::size_t i = 0; i < ValidData; ++i)
			vector.push_back(DataArray[i]);

	fclose(file);
}

template<class T>
void read_vector(const char * const filename, std::vector<T>& vector, std::size_t size)
{
	assert(vector.size() == size);
	FILE* file;
	fopen_s(&file, filename, "rb");
	if(!file){
		std::cerr << "ERROR: File '" << filename << "' could not be opened!" << std::endl;
		return;
	}

	const std::size_t N = 10000;
	int ValidData;
	T DataArray[N];
	std::vector<T>::iterator it = vector.begin();
	while (ValidData = fread(&DataArray, sizeof(T), std::min(N, size), file)){
		for (int i = 0; i < ValidData; ++i)
			*(it++) = DataArray[i];
		size -= ValidData;
	}

	fclose(file);
}

template<class T>
void write_to_file(const char * const filename, const std::vector<T>& vector)
{
	FILE* file;
	fopen_s(&file, filename, "wb");
	
	const std::vector<T>::const_iterator end = vector.cend();
	for (std::vector<T>::const_iterator it = vector.cbegin(); it != end; ++it)
		fwrite(&(*it), sizeof(T), 1, file);

	fclose(file);
}

template<class T>
bool Test_datamanipulation()
{
	const char * const filename = std::tmpnam(NULL); // create temporary file name
	
	// create temporary vector
	const int N = 10000;
	std::vector<T> data(N);
	data[0] = 1;
	data[1] = 1;
	for (int i = 2; i < N; ++i)
		data[i] = data[i-1] + data[i-2];

	write_to_file(filename, data);
	data.clear();
	data = read_vector<double>(filename);

	// check data
	if (data[0] != 1) return false;
	if (data[1] != 1) return false;
	for (int i = 2; i < N; ++i)
		if (data[i] != data[i-1] + data[i-2]) return false;

	data.clear();
	read_vector(filename, data);

	// check data
	if (data[0] != 1) return false;
	if (data[1] != 1) return false;
	for (int i = 2; i < N; ++i)
		if (data[i] != data[i-1] + data[i-2]) return false;

	data.clear();
	data = std::vector<T>(N);
	read_vector(filename, data, N);

	// check data
	if (data[0] != 1) return false;
	if (data[1] != 1) return false;
	for (int i = 2; i < N; ++i)
		if (data[i] != data[i-1] + data[i-2]) return false;

	if (std::remove(filename)) // delete temporary file
		std::cerr << "ERROR: could not remove temporary file: '" << filename << "'." << std::endl;

	return true;
}