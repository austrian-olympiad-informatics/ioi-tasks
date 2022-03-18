#include "testlib.h"
#include <cassert>
#define MAXN 1000
using namespace std;

int main(int argc, char **argv) {
  // read in test cases
	registerGen(argc, argv, 1);
	int n = atoi(argv[1]);
	int k = atoi(argv[2]);
	int is_partial = atoi(argv[3]);
	int perm[n];
	for(int i=0; i<n; i++) perm[i]=i;
	int ctr = 1;
	for(int i=3; i<=n; i++) {
        ctr *= i;
	}
	ctr = min(ctr, 10);
	printf("%d\n", ctr);

	do {
        if(perm[0] < perm[n-1]) {
            printf("%d %d\n", n, k);
            for(int i=1; i<n; i++) {
                printf("%d %d\n", perm[i-1], perm[i]);
            }
            printf("%d\n", n*(n-1));
            for(int i=0; i<n; i++) {
                for(int j=0; j<n; j++) {
                    if(i==j) continue;
                    printf("%d %d %d\n", perm[i], perm[j], i<j ? perm[i+1] : perm[i-1]);
                }
            }
            ctr--;
            if(ctr==0) break;
        }
	} while(next_permutation(perm, perm+n));
	printf("%d %d\n", is_partial, rnd.next(235375));

	return 0;
}
