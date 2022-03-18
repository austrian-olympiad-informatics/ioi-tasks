#include "biscuits.h"
#include <vector>
using namespace std;

long long dp[61];

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
    merge_biscuits(x, a);
    dp[0] = a[0]<x ? 1 : 2;
    for(int i=1; i<=60; i++) {
        if(a[i] >= x) {
            dp[i] = 2*dp[i-1];
        } else {
            dp[i] = dp[i-1];
            long long target = 2*(x-a[i]); // we are short of (target) coins of type idx-1
            for(int idx=i-1; idx>=0 && target<=2*x && target>0; idx--) {
                if(a[idx] < target) { // not enough to meet target, subtract from target
                    target -= a[idx];
                    target *= 2; // double because we now use lower value coins
                } else if(idx==0) {
                    dp[i] += (a[idx]-target<x ? 1: 2);
                } else {
                    target = 2*(x-a[idx]+target);
                    dp[i] += dp[idx-1];
                }
            }
        }
    }
    return dp[60];
}
