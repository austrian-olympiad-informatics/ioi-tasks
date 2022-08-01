// Dmitry _kun_ Sayutin (2019)

#include <bits/stdc++.h>
#include "shoes.h"

using std::cin;
using std::cout;
using std::cerr;

using std::vector;
using std::map;
using std::array;
using std::set;
using std::string;

using std::pair;
using std::make_pair;

using std::tuple;
using std::make_tuple;
using std::get;

using std::min;
using std::abs;
using std::max;
using std::swap;

using std::unique;
using std::sort;
using std::generate;
using std::reverse;
using std::min_element;
using std::max_element;

#ifdef LOCAL
#define LASSERT(X) assert(X)
#else
#define LASSERT(X) {}
#endif

#define SZ(vec)         int((vec).size())
#define ALL(data)       data.begin(),data.end()
#define RALL(data)      data.rbegin(),data.rend()
#define TYPEMAX(type)   std::numeric_limits<type>::max()
#define TYPEMIN(type)   std::numeric_limits<type>::min()

vector<int> change(vector<int> v) {
	int n = (int)v.size() / 2;
	vector<int> cnt(n, 0);
	for(int i = 0; i < 2*n; i++)
		if(v[i] > 0)
			cnt[v[i] - 1]++;

	for(int i = 1; i < n; i++)
		cnt[i] += cnt[i-1];

	vector<int> cntl = cnt, cntr = cnt;

	for(int i = 0; i < 2*n; i++)
		if(v[i] > 0)
			v[i] = (--cntr[v[i] - 1]) + 1;
		else
			v[i] = -((--cntl[-v[i] - 1]) + 1);
	return v;
}


long long count_swaps(vector<int> s) {
	s = change(s);
    vector<pair<int, bool>> in(SZ(s));

    vector<int> where(SZ(s) / 2, -1);
    
    for (int i = 0; i != SZ(s); ++i) {
        in[i].first = abs(s[i]) - 1;
        in[i].second = (s[i] < 0 ? 0 : 1);

        if (where[in[i].first] == -1)
            where[in[i].first] = i;
    }

    vector<int> fenw(SZ(in));
    for (int i = 0; i != SZ(in); ++i)
        fenw[i] = 1 + i - (i & (i+1));

    long long ans = 0;
    vector<char> dead(SZ(in), false);

    for (int pos = SZ(in) - 1; pos >= 0; --pos)
        if (not dead[pos]) {
            dead[pos] = 1;

            int i = where[in[pos].first];
            dead[i] = 1;

            int dist = 0;
            for (int p = pos; p >= 0; p = (p & (p + 1)) - 1)
                dist += fenw[p];
            
            for (int p = i; p >= 0; p = (p & (p + 1)) - 1)
                dist -= fenw[p];

            if (in[pos].second == 1)
                --dist;

            ans += dist;
            for (int p = i; p < SZ(fenw); p = p | (p + 1))
                fenw[p] -= 1;
        }
    
    
    return ans;
}
