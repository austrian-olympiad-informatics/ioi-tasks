#include "biscuits.h"
#include <vector>
#include <assert.h>
using namespace std;

bool is_valid(long long y, long long x, vector<long long> &a) {
    long long carry = 0;
    for(int i=0; y>0; i++) {
        assert(i<a.size());
        if(y & 1) {
            if(carry + a[i] < x) return false;
            carry = (carry + a[i] - x)/2;
        } else {
            carry = (carry + a[i])/2;
        }
        y = y/2;
    }
    return true;
}

long long count_tastiness(long long x, std::vector<long long> a) {
    long long s = 0;
    for(int i=0; i<a.size(); i++) {
        s += (a[i]<<i);
    }
    while(a.size()<60) a.push_back(0);
    long long ans = 0;
    long long i=0;
    while(i<=s/x) {
        if(is_valid(i, x, a)) {
            i++;
            ans++;
        } else {
            i += i & (-i);
        }
    }
    return ans;
}
