#include "biscuits.h"
#include <vector>
#include <unordered_map>
using namespace std;

const int x_max = 3e5+1;
long long dp[61][2*x_max];

long long f(int idx, long long w, long long x, vector<long long> &a) {
    if(idx==60) return 1;
    if(dp[idx][w]!=-1) return dp[idx][w];
    return dp[idx][w] = f(idx+1, (w+a[idx])/2, x, a) + (w+a[idx]>=x ? f(idx+1, (w+a[idx]-x)/2, x, a) : 0);
}

void merge_biscuits(long long x, vector<long long> &a) {
    while(a.size()<61) a.push_back(0);
    for(int i=0; i<(int)a.size(); i++) {
        if(a[i]>x+1) {
            a[i+1] += (a[i]-x)/2;
            a[i] -= ((a[i]-x)/2)*2;
        }
    }
}

long long count_tastiness(long long x, std::vector<long long> a) {
    for(int i=0; i<=60; i++) {
        for(int j=0; j<=2*x; j++) {
            dp[i][j] = -1;
        }
    }
    merge_biscuits(x, a);
    return f(0, 0, x, a);
}
