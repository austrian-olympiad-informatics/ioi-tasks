#include "biscuits.h"
#include <vector>

using namespace std;

long long count_tastiness(long long X, std::vector<long long> A) {
    int K = 63;
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
	
	return MEM[K - 1];
}
