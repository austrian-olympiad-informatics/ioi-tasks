// Generator for task mushrooms
// It takes n & k from argv and
//   generates a random binary sequence with k 1-bits and n-k 0-bits.
//   The sequence is selected uniformly from all such sequences.
// Author: Kian Mirjalali
#include "common.h"

/// Generates a random bit with Bernoulli distribution with Prob[1] = a/b
inline int bernoulli(int a, int b) {
	return int(rnd.next(b) < a);
}

int main(int argc, char *argv[]) {
	registerGen(argc, argv, 1);

	int n = atoi(argv[1]);
	ensuref(2 <= n, "n must be at least 2.");
	int k = atoi(argv[2]);
	ensuref(0 <= k && k <= n-1, "k must be in range [0, n-1]");

	VI v(n);
	v[0] = 0;
	for (int i = n-1; i > 0; i--)
		k -= (v[i] = bernoulli(k, i));

	write_plain(v);
}
