#include "testlib.h"
#include <vector>
#include <assert.h>
using namespace std;

int nontrivial;
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
	nontrivial = 0;
    for(int i=0; i<58; i++) {
        if(MEM[i]<MEM[i+1] && MEM[i+1]<2*MEM[i]) {
            nontrivial++;
        }
    }
	return MEM[K - 1];
}

// calculates the total tastiness of all biscuits
long long sum(vector<long long> a) {
    long long ans = 0;
    for(int i=0; i<60; i++) {
        ans += (a[i]<<i);
    }
    return ans;
}
const long long tastiness_max = 1000000000000000000LL;

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 0);
    long long x = atoll(argv[1]);
    int q = atoi(argv[2]);
    int bits = 58-__lg(x);
    int k = 60;
    vector<long long> a(60);
    //assert(bits>40);
    int nontrivial_threshold = 60 - bits;
    printf("%d\n", q);
    while(q--) {
        printf("%d %lld\n", k, x);
        do {
            for(int i=0; i<bits; i++) {
                a[i] = x - rnd.next(20) + 10;
            }
            a[0] = x + rnd.next(20);
            a[bits] = 0;
            while(sum(a)<1e18) {
                a[bits] *= rnd.next(1.05, 1.1);
                a[bits]++;
            }
            a[bits] *= 0.9;
            assert(sum(a)<1e18);
            do {
                int idx = rnd.next(1, bits-1);
                a[idx] *= rnd.next(0.95, 0.98);
                count_tastiness(x, a);
                if(sum(a)<9e17) {
                    break;
                }
            } while(nontrivial<nontrivial_threshold);
            assert(sum(a)>=8e17);
        } while(nontrivial<nontrivial_threshold);

        for(int i=0; i<k; i++) {
            if(i) printf(" ");
            printf("%lld", a[i]);
        }
        printf("\n");
    }
}
