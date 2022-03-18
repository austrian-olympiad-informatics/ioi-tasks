#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

/**
 * Generates a uniformly random value between low and high inclusive.
 */
long long uniformRandom(long long low, long long high) {
	return rnd.next(low, high);
}

/**
 * Generates a uniformly random value between low and high inclusive.
 */
int uniformRandom(int low, int high) {
	return rnd.next(low, high);
}

/**
 * Generates a geometrically random value with parameter param.
 */
int geomRandom(double param) {
	return log2((double)rnd.next()) / log2(1.0 - param);
}

/**
 * Generates a testcase with a threshold on the maximum sum of all values.
 * 
 * KMode parameters:
 *   0: k = maxK
 *   1: k <= 3
 *   2: uniformly random from maxK/2 to maxK
 * 
 * XMode parameters:
 *   0: x = 1
 *   1: x <= 5
 *   2: uniformly random from totalSum/5 to totalSum
 */
void gen(int maxK, int maxQ, long long maxX, long long maxSum, 
		int KMode, int XMode) {
	
	printf("%d\n", maxQ);
	for (int i = 0; i < maxQ; i++) {
		int K;
		switch (KMode) {
			case 0:
				K = maxK;
				break;
			case 1:
				K = min(maxK, uniformRandom(1, 3));
				break;
			default:
				K = uniformRandom(maxK/2, maxK);
		}
		
		vector<long long> A(K);
		
		for (int i = 0; i < K; i++) {
			if (maxSum <= (1LL << i)) A[i] = 0;
			else A[i] = uniformRandom(0LL, (long long)(pow(maxSum / (1LL << i), 0.8))); //must not exceed threshold
		}
		
		long long currentSum = 0;
		for (int i = 0; i < K; i++) { //strip off from back (probably need to change this)
			A[i] = min(A[i], (long long)((maxSum - currentSum) / (1LL << i)));
			currentSum += (1LL << i) * A[i];
		}
		
		long long X;
		switch (XMode) {
			case 0:
				X = 1;
				break;
			case 1:
				X = uniformRandom(1, 5);
				break;
			case 2:
				X = min(maxX, uniformRandom((long long)(currentSum / 5), (long long)(currentSum)));
				break;
		}
		
		assert(K <= 60);
		assert(currentSum <= maxSum);
		
		printf("%d %lld\n", K, X);
		for (int i = 0; i < K; i++) {
			if (i > 0) printf(" %lld", A[i]);
			else printf("%lld", A[i]);
		}
		printf("\n");
	}
}


int main(int argc, char* argv[]) {
	registerGen(argc, argv, 0);
	int maxK = atoi(argv[1]);
	int maxQ = atoi(argv[2]);
	long long maxX = atoll(argv[3]);
	if (maxX == -1) maxX = 1e18;
	long long maxSum = atoll(argv[4]);
	if (maxSum == -1) maxSum = 1e18;
	int KMode = atoi(argv[5]);
	int XMode = atoi(argv[6]);
	gen(maxK, maxQ, maxX, maxSum, KMode, XMode);
}
