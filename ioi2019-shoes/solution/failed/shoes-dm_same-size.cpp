#include "shoes.h"

using namespace std;

long long count_swaps(vector<int> S)
{
	int current = 0, opposite = 0;
	long long swaps = 0;
	while (current < (int)S.size()) {
		while (S[current] + S[opposite] != 0) {
			opposite++;
		}
		int newSwaps = opposite - current - (S[current] < 0 ? 1 : 0);
		swaps += newSwaps;
		if (newSwaps > 0) {
		    S[opposite] = S[opposite - 1];
		}
		current += 2;
		opposite++;
	}
	return swaps;
}
