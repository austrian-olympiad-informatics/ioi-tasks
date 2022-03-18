#include "biscuits.h"
#include <vector>
using namespace std;

pair<int, long long> s[(int)1e6];
const int x_max = 5e5;
int dp[2][2*x_max];

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
long long count_tastiness2(long long x, std::vector<long long> &a) {
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
    int ans = 0;
    s[1] = make_pair(0,0);
    pair<int, long long> *p = s+1;
    while(p>s) {
        while(p->first < transition && p->second + a[p->first] < x) {
            p->second = (p->second + a[p->first])/2;
            p->first++;
        }
        if(p->first == transition) {
            ans += dp[transition%2][p->second];
            p--;
        } else {
            (p+1)->second = (p->second + a[p->first] - x)/2;
            p->second = (p->second + a[p->first])/2;
            p->first++;
            (p+1)->first = p->first;
            p++;
        }
    }
    return ans;
}

long long count_tastiness(long long x, std::vector<long long> a) {
    merge_biscuits(x, a);
    if(x<x_max) return count_tastiness2(x, a);
    const int sz = 59-__lg(x);

    //while(a.size()<=60) a.push_back(0);
    int ans = 0;
    s[1] = make_pair(0,0);
    pair<int, long long> *p = s+1;
    while(p>s) {
        while(p->first < sz && p->second + a[p->first] < x) {
            p->second = (p->second + a[p->first])/2;
            p->first++;
        }
        if(p->first == sz) {
            ans+=1+((p->second + a[p->first])>=x);
            p--;
        } else {
            (p+1)->second = (p->second + a[p->first] - x)/2;
            p->second = (p->second + a[p->first])/2;
            p->first++;
            (p+1)->first = p->first;
            p++;
        }
    }
    return ans;
}
