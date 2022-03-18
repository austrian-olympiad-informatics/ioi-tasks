#include "biscuits.h"
#include <vector>
using namespace std;

const int x_max = 1e7+1;
long long dp[2][2*x_max];



void merge_biscuits(long long x, vector<long long> &a) {
    while(a.size()<61) a.push_back(0);
    for(int i=0; i<(int)a.size(); i++) {
        if(a[i]>x+1) {
            a[i+1] += (a[i]-x)/2;
            a[i] -= (a[i]-x)&(-2);
        }
    }
}

const int transition = 16;
pair<int, long long> s[2<<transition];

long long count_tastiness(long long x, std::vector<long long> a) {
    merge_biscuits(x, a);
    for(int j=0; j<=2*x; j++) {
        dp[0][j] = 1;
        dp[1][j] = 1;
    }
    if(a[0]<x) {
        a[1] += a[0]/2;
        a[0] = 0;
    }
    merge_biscuits(x, a);
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
        for(int j=max(0,(int)x-(int)a[i]); j<=threshold[i]; j++) {
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
}
