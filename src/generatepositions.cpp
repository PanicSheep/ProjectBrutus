#include "generatepositions.h"

void Work(std::set<ULLULL>& PositionSet, std::atomic_flag& spinlock, const unsigned long long N, const int Empties, const bool ETH)
{
	auto local_rnd = std::bind(std::uniform_int_distribution<int>(0,64), std::mt19937_64(std::chrono::system_clock::now().time_since_epoch().count() + std::this_thread::get_id().hash()));
	unsigned long long P, O, BitBoardPossible;
	std::size_t size;
	int moves;
	
	do
	{
		ResetPosition(P, O, ETH);
		moves = 60 - Empties;
		while (moves > 0)
		{
			BitBoardPossible = PossibleMoves(P, O);
			if (!BitBoardPossible){
				std::swap(P, O);
				BitBoardPossible = PossibleMoves(P, O);
				if (!BitBoardPossible){
					ResetPosition(P, O, ETH);
					moves = 60 - Empties;
					continue;
				}
			}
			for (int k = local_rnd() % POP_COUNT(BitBoardPossible); k > 0; --k)
				REMOVE_LS1B(BitBoardPossible);
			PlayStone(P, O, BIT_SCAN_LS1B(BitBoardPossible));
			--moves;
		}

		while (spinlock.test_and_set(std::memory_order_acquire)) ;
			size = PositionSet.size();
			if (size < N){
				if (PositionSet.count(ULLULL(P, O)) == 0){
					PositionSet.insert(ULLULL(P, O));
					size++;
				}
			}
		spinlock.clear(std::memory_order_release);

	} while (size < N);
}