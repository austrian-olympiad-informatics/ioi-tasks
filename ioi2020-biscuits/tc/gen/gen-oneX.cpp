#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 0);
	int q = atoi(argv[1]);
	int k = atoi(argv[2]);
	printf("%d\n", q);
	while(q--) {
        printf("%d 1\n", k);
        for(int i=0; i<k; i++) {
            if(i) printf(" ");
            printf("%d", rnd.next(3));
        }
        printf("\n");
	}
}
