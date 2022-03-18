#include "testlib.h"
#include <bits/stdc++.h>
#include <vector>
using namespace std;

static int MAXK = 60;
static long long MAXSUM = 1e18;
static long long MAXSOLS = 2e5;

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
 * Generates a uniformly random double between 0 and 1.
 */
double uniformRandom() {
	return rnd.next();
}

/**
 * Generates a geometrically random value with parameter param.
 */
int geomRandom(double param) {
	return log2((double)rnd.next()) / log2(1.0 - param);
}

//Returns number of solutions in range [0..2^P-1]
long long findAns(long long X, std::vector<long long> _A, int P) {
    int K = 63;
    if (P >= K) P = K - 1;
    
    vector<long long> A;
    for (int i = 0; i < (int)_A.size(); i++) A.push_back(_A[i]);
    
    while (A.size() < 63) A.push_back(0);
    
    vector<long long> MEM(K); //number of solutions in range [0..2^i-1]
    vector<long long> PTH(K); //previous thresholds
    
    long long CURSUM = 0;
    for (int i = 0; i < K; i++) {
		if (i) CURSUM += A[i - 1] * (1LL << (i - 1));
		
		MEM[i] = 0;
		long long CTH = CURSUM / X; //maximum possible solution
		PTH[i] = CTH;
		
		if (i == 0) {
			MEM[i] = 1;
			continue;
		}
		
		for (int j = i - 1; j >= 0; j--) {
			if (j == 0) { //special case: 0 or 1
				if (CTH >= 1) MEM[i] += 2;
				else MEM[i] += 1;
			}
			else if (CTH >= (1LL << (j + 1))) { //threshold is bigger than entire range
				MEM[i] += 2LL * MEM[j];
				break;
			}
			else if (CTH >= (1LL << j)) { //threshold is bigger than half the range
				MEM[i] += MEM[j];
				CTH = min(CTH - (1LL << j), PTH[j]);
			}
			else CTH = min(CTH, PTH[j]);
		}
	}
	
	return MEM[P];
}

/**
 * Generates a testcase with a threshold on the maximum number of solutions.
 * 
 * X will be generated as a number between 5*1e7 and 1e8.
 */
void genSmallSols(int maxQ, double taperRatio) {
	printf("%d\n", maxQ);

	for (int i = 0; i < maxQ; i++) {
		int K = MAXK;
		long long X = uniformRandom(5 * (long long)1e7, (long long)1e8);
		
		vector<long long> A(K);
		
		long long curSolutions = 1, curSum = 0; //trivial solution
		for (int i = 0; i < K; i++) {
			if (MAXSUM <= (1LL << i)) A[i] = 0;
			else {
				long long lo = 0, hi = min((long long)3 * X, MAXSUM / (1 << i));
				
				//attempt to find a meaningful threshold
				while (lo < hi) {
					A[i] = uniformRandom(lo, hi);
					if (findAns(X, A, 63) >= MAXSOLS) hi = A[i] - 1;
					else if (findAns(X, A, i + 1) >= taperRatio * curSolutions) { //controlling rate of growth of solutions
						hi = A[i] - 1;
					}
					else if (findAns(X, A, i + 1) == curSolutions) {
						lo = A[i] + 1;
					}
					else break; //Found a mid-solution
				}
				
				if (findAns(X, A, i + 1) >= taperRatio * curSolutions || findAns(X, A, i + 1) == curSolutions
					|| findAns(X, A, 63) >= MAXSOLS) { //failed
						
					//round 2, randomly choose high/low
					long long lo = 0, hi = min((long long)(1.1 * X), MAXSUM / (1LL << i));
					while (lo < hi) {
						A[i] = uniformRandom(lo, hi);
						if (findAns(X, A, 63) >= MAXSOLS) {
							hi = A[i] - 1;
							A[i] = 0;
						}
						else if (findAns(X, A, i + 1) >= taperRatio * curSolutions) { //doubling solutions
							if (uniformRandom() > 0.5) break;
							else hi = A[i];
						}
						else if (findAns(X, A, i + 1) == curSolutions) {
							if (uniformRandom() > 0.5) break;
							else lo = A[i];
						}
					}
				}
				
				curSolutions = findAns(X, A, i + 1);
			}
			
			if (curSum > MAXSUM - (1LL << i) * A[i]) break;
			curSum += (1LL << i) * A[i];
		}
		
		long long currentSum = 0;
		for (int i = 0; i < K; i++) { //strip off from back (probably need to change this)
			A[i] = min(A[i], (long long)((MAXSUM - currentSum) / (1LL << i)));
			currentSum += (1LL << i) * A[i];
		}
		
		printf("%d %lld\n", K, X);
		for (int i = 0; i < K; i++) {
			if (i > 0) printf(" %lld", A[i]);
			else printf("%lld", A[i]);
		}
		printf("\n");
	}
}

/**
 * taperConstant is a benchmark on the rate of growth of number of solutions.
 * It should be at least 1.7 for reasonable testcases (more than 1e5 solutions).
 */
int main(int argc, char* argv[]) {
	registerGen(argc, argv, 0);
	int q = atoi(argv[1]);
	double taperConstant = atof(argv[2]);
	genSmallSols(q, taperConstant);
}
