#include "testlib.h"
#include <bits/stdc++.h>

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 0);
	int q = atoi(argv[1]);
	long long x = atoll(argv[2]);
	long long ans = atoll(argv[3]);
	printf("%d\n", q);
	while(q--) {
        long long x_tmp = x==1 ? 1 : 1+rnd.next(x-1);
        printf("1 %lld\n", x_tmp);
        printf("%lld\n", rnd.next(ans*x_tmp));
	}

}
