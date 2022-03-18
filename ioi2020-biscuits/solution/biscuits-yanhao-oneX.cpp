#include "biscuits.h"
#include <vector>
using namespace std;

void merge_biscuits(vector<long long> &a) {
    while(a.size()<61) a.push_back(0);
    for(int i=0; i<(int)a.size(); i++) {
        if(a[i]>2) {
            a[i+1] += (a[i]-1)/2;
            a[i] = (a[i]%2 ? 1 : 2);
        }
    }
}

long long count_tastiness(long long x, std::vector<long long> a) {
    merge_biscuits(a);
    long long ans = 1;
    long long current = 0;
    for(int i=60; i>=0; i--) {
        if(a[i]==0) {
            ans *= (current+1);
            current = 0;
        } else {
            current = current*2 + a[i];
        }
    }
    ans *= (current+1);
    return ans;
}
