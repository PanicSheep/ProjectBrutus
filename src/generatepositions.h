#pragma once
#include "datamanipulation.h"
#include "position.h"
#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <omp.h>
#include <random>
#include <set>
#include <string.h>
#include <thread>
#include <vector>

typedef std::pair<unsigned long long, unsigned long long> ULLULL;

void Work(std::set<ULLULL>& PositionSet, std::atomic_flag& spinlock, const unsigned long long N, const int Empties, const bool ETH);

template <class T>
void GeneratePositions(const unsigned long long N, const int Empties, const bool ETH, const std::string Filename)
{
	std::size_t nthreads = std::thread::hardware_concurrency();
	std::atomic_flag spinlock;
	std::set<ULLULL> PositionSet;
	std::vector<std::thread> threads(nthreads-1);
	std::chrono::high_resolution_clock::time_point startTime, endTime;
	spinlock.clear(std::memory_order_release);

	startTime = std::chrono::high_resolution_clock::now(); //Start Time

	// Start workers
	for (int i = 0; i < nthreads-1; ++i)
		threads[i] = std::thread(Work, std::ref(PositionSet), std::ref(spinlock), N, Empties, ETH);
	Work(PositionSet, spinlock, N, Empties, ETH);
  
	// Join workers
	for (auto& t : threads)
		t.join();

	endTime = std::chrono::high_resolution_clock::now(); //Stop Time

	std::vector<T> DataVector;
	T data;
	for (auto it = PositionSet.begin(); it != PositionSet.end(); ++it){
		data.Reset();
		data.P = it->first;
		data.O = it->second;
		DataVector.push_back(data);
	}

	write_to_file(Filename, DataVector);
}