#include "biscuits.h"
#include <vector>
#include <assert.h>
#include <bitset>
#include <string.h>
using namespace std;

const int x_max = (1<<24)+1;
long long dp[2][2*x_max];
short ans[1<<29];

void merge_biscuits(long long x, vector<long long> &a) {
    while(a.size()<61) a.push_back(0);
    for(int i=0; i<(int)a.size(); i++) {
        if(a[i]>x+1) {
            a[i+1] += (a[i]-x)/2;
            a[i] -= ((a[i]-x)/2)*2;
        }
    }
}

const int transition = 22;

pair<int, long long> s[(int)8e6];
long long count_tastiness(long long x, std::vector<long long> a) {
    if(x<x_max) {
        for(int j=0; j<=2*x; j++) {
            dp[0][j] = 1;
            dp[1][j] = 1;
        }
        if(a[0]<x) {
            a[1] += a[0]/2;
            a[0] = 0;
        }
        merge_biscuits(x, a);
        // use bottom up- bottom up is faster!
        long long threshold[60];
        threshold[0] = a[0]/2;
        for(int i=1; i<60; i++) {
            threshold[i] = (threshold[i-1]+a[i-1])/2;
        }

        for(int i=59-__lg(x); i>=transition; i--) {
            bool b = i%2;
            for(int j=0; j<x-a[i]; j++) {
                dp[b][j] = dp[!b][(j+a[i])/2];
            }
            for(int j=x-a[i]; j<=threshold[i]; j++) {
                dp[b][j] = dp[!b][(j+a[i])/2] + dp[!b][(j+a[i]-x)/2];
            }
        }
        int offset = 1;
        s[0] = make_pair(0,0);
        long long ret = 0;
        while(offset) {
            while(s[offset-1].first < transition && s[offset-1].second + a[s[offset-1].first] < x) {
                s[offset-1].second = (s[offset-1].second + a[s[offset-1].first])/2;
                s[offset-1].first++;
            }
            if(s[offset-1].first == transition) {
                ret += dp[transition%2][s[offset-1].second];
                offset--;
            } else {
                s[offset].second = (s[offset-1].second + a[s[offset-1].first] - x)/2;
                s[offset-1].second = (s[offset-1].second + a[s[offset-1].first])/2;
                s[offset-1].first++;
                s[offset].first = s[offset-1].first;
                offset++;
            }
        }
        return ret;
    } else {
        while(a.size()<=60) {
            a.push_back(0);
        }
        merge_biscuits(x, a);
        vector<long long> sum;
        sum.push_back(a[0]);
        for(int i=1; i<(int) a.size(); i++) {
            sum.push_back(sum.back() + (a[i]<<i));
        }
        unsigned long long tmp = 1;
        for(int i=0; i<6; i++) {
            int threshold = min(2LL<<i, sum[i]/x+1);
            tmp |= (tmp<<(1LL<<i));
            if(threshold>=64) continue;
            tmp &= ((1LL<<threshold)-1);
            assert(tmp);
        }
        ans[0] = 64;
        for(int i=6; i<35; i++) {
            long long threshold = min(1LL<<i, sum[i]/x-(1LL<<i)+1);
            if(threshold<0) continue;
            int k = 1<<(i-6);
            memcpy(ans+k, ans, threshold/64*sizeof(short));
            ans[threshold/64+k] = min((int)ans[threshold/64], (int)(threshold&63));
        }
        long long ret = 0;
        short pop[65];
        pop[0] = 0;
        for(int i=0; i<64; i++) {
            if((1LL<<i)&tmp) {
                pop[i+1] = pop[i]+1;
            } else {
                pop[i+1] = pop[i];
            }
        }
        for(int i=0; i<(1<<29); i++) {
            ret += pop[ans[i]];
        }
        return ret;
    }
}
