#include "biscuits.h"
#include <vector>
#include <cstdio>
#include <assert.h>
using namespace std;

long long arr[(int) 4e7];
long long count_tastiness(long long x, std::vector<long long> a) {
    while(a.size()<=60) {
        a.push_back(0);
    }
    vector<long long> s;
    s.push_back(a[0]);
    for(int i=1; i<(int) a.size(); i++) {
        s.push_back(s.back() + (a[i]<<i));
    }
    arr[0] = 0;
    int sz = 1;
    for(int i=0; i<(int) s.size(); i++) {
        long long threshold = min(1LL<<i, s[i]/x-(1LL<<i)+1);
        for(int j=0; arr[j]<threshold; j++) {
            arr[sz++] = arr[j]+(1LL<<i);
        }
    }
    /*vector<long long> ans;
    ans.reserve((int) 4e7);
    ans.push_back(0);
    for(int i=0; i<(int) s.size(); i++) {
        if((x<<i)>(1LL<<60)) break;
        long long threshold = min(1LL<<i, s[i]/x-(1LL<<i)+1);
        for(int j=0; ans[j]<threshold; j++) {
            ans.push_back(ans[j]+(1LL<<i));
        }
    }*/

    //return ans.size();
    return sz;
}
