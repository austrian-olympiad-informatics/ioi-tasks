#include "shoes.h"

using namespace std;

class Fenwick
{
    vector<int> a;

    int count(int right)
    {
        int cnt = 0;
        for (; right >= 0; right = (right & (right + 1)) - 1)
        {
            cnt += a[right];
        }
        return cnt;
    }

public:
    explicit Fenwick(int n)
    {
        a.assign(n, 0);
    }

    int count(int left, int right)
    {
        return count(right) - count(left - 1);
    }

    void put(int index)
    {
        for (; index < int(a.size()); index = index | (index + 1))
        {
            a[index]++;
        }
    }
};

vector<int> create_index(int n, vector<int> &S)
{
    vector<int> index(2 * n + 1);
    for (int i = 0; i < 2 * n; i++)
    {
        index[S[i] + n] = i;
    }
    return index;
}

long long count_adjacent(int n, vector<int> &S)
{
    vector<int> index = create_index(n, S);
    Fenwick f = Fenwick(2 * n);
    long long ans = 0;
    for (int i = 0; i < 2 * n; i++)
    {
        if (S[i] != 0)
        {
            int pos = index[-S[i] + n];
            ans += pos - i - f.count(i, pos) - (S[i] < 0 ? 1 : 0);
            S[pos] = 0;
            f.put(pos);
        }
    }
    return ans;
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
    int n = S.size() / 2;
    return count_adjacent(n, S);
}
