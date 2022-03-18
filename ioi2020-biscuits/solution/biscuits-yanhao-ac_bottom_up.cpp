#include "biscuits.h"
#include <vector>

long long dp[61];
long long b[61];
long long count_tastiness(long long x, std::vector<long long> a) {
    long long sum = 0;
    for(int i=0; i<=60-std::__lg(x); i++) {
        dp[i] = 0;
        long long j = (1LL<<i);
        for(int h=i-1; h>=0; h--) {
            if((j>>h)>0) {
                dp[i] += dp[h];
                j = std::min(j, b[h])-(1LL<<h);
            }
        }
        dp[i] += (j==1);
        sum += (i<(int)a.size() ? (a[i]<<i) : 0);
        b[i] = sum/x+1;
    }
    return dp[60-std::__lg(x)];
}
