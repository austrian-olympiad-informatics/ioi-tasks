#include "shoes.h"
#include <map>
#include <queue>

using namespace std;

map<vector<int>, int> m;
queue<vector<int>> q;

inline bool isAcceptable(vector<int> &S)
{
    for (int i = 0; i < S.size(); i += 2)
    {
        if (S[i] > 0 || S[i] + S[i + 1] != 0)
        {
            return false;
        }
    }
    return true;
}

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
    if (isAcceptable(S))
    {
        return 0;
    }
    m[S] = 0;
    q.push(S);
    while (true)
    {
        vector<int> cur = q.front();
        q.pop();
        int curSwaps = m[cur];
        for (int i = 1; i < S.size(); i++)
        {
            swap(cur[i - 1], cur[i]);
            if (m.count(cur) == 0)
            {
                if (isAcceptable(cur))
                {
                    return curSwaps + 1;
                }
                q.push(cur);
                m[cur] = curSwaps + 1;
            }
            swap(cur[i - 1], cur[i]);
        }
    }
}
