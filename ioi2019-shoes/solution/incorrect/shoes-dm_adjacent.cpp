#include "shoes.h"

using namespace std;

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


long long count_swaps(vector<int> S)
{
	S = change(S);
    long long ans = 0;
    for (int i = 0; i < (int)S.size(); i += 2)
    {
        ans += S[i] > 0 ? 1 : 0;
    }
    return ans;
}
