#include "biscuits.h"
#include <vector>
#include <map>

using namespace std;
map<long long, long long> m;
long long f(vector<long long> &s, long long x, long long n) {
    if(n<=0) return 0;
    if(n==1) return 1;
    if(m.find(n)!=m.end()) {
        return m[n];
    }
    long long a = __lg(n-1);
    return m[n] = f(s,x,1LL<<a) + f(s,x,min(n,1+s[a]/x)-(1LL<<a));
}
long long count_tastiness(long long x, std::vector<long long> a) {
    m.clear();
    for(int i=1; i<(int)a.size(); i++) {
        a[i] = a[i-1] + (a[i]<<i);
    }
    while(a.size()<=60) a.push_back(a.back());
	return f(a, x, 1+a.back());
}
