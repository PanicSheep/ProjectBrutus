#include "game.h"

bool UseHashTableValue(HashTableValueType htValue, int alpha, int beta, signed char depth, unsigned char selectivity, int & value)
{
	if ((htValue.depth >= depth) && (htValue.selectivity <= selectivity))
	{
		if (htValue.alpha >= beta){
			value = beta;
			return true;
		}
		if (htValue.beta <= alpha){
			value = alpha;
			return true;
		}
		if (htValue.alpha == htValue.beta){
			value = htValue.alpha;
			return true;
		}
		//if (htValue.alpha > alpha)
		//	alpha = htValue.alpha;
		//if (htValue.beta < beta)
		//	beta = htValue.beta;
	}
	return false;
}

bool UseHashTableValue(HashTableValueType htValue, int alpha, int beta, signed char depth, unsigned char selectivity, int & value, bool & GotProbCut)
{
	if ((htValue.depth >= depth) && (htValue.selectivity <= selectivity))
	{
		if (htValue.alpha >= beta){
			value = beta;
			if (htValue.selectivity != 0) GotProbCut = true;
			return true;
		}
		if (htValue.beta <= alpha){
			value = alpha;
			if (htValue.selectivity != 0) GotProbCut = true;
			return true;
		}
		if (htValue.alpha == htValue.beta){
			value = htValue.alpha;
			if (htValue.selectivity != 0) GotProbCut = true;
			return true;
		}
		//if (htValue.alpha > alpha)
		//	alpha = htValue.alpha;
		//if (htValue.beta < beta)
		//	beta = htValue.beta;
	}
	return false;
}