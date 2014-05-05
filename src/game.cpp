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