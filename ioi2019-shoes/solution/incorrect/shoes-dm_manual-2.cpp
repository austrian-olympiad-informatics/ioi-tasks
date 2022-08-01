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
    if (S.size() != 4)
    {
        return S[0] < 0 ? 0 : 1;
    }

    if (S[0] != 1 && S[0] != -1)
    {
        for (int i = 0; i < 4; i++)
        {
            S[i] = S[i] < 0 ? -S[i] - 3 : 3 - S[i];
        }
    }

    if (S[0] == -1 && S[1] == 1 && S[2] == -2 && S[3] == 2)
    {
        return 0;
    }
    if (S[0] == -1 && S[1] == 1 && S[2] == 2 && S[3] == -2)
    {
        return 1;
    }
    if (S[0] == -1 && S[1] == -2 && S[2] == 1 && S[3] == 2)
    {
        return 1;
    }
    if (S[0] == -1 && S[1] == -2 && S[2] == 2 && S[3] == 1)
    {
        return 2;
    }
    if (S[0] == -1 && S[1] == 2 && S[2] == -2 && S[3] == 1)
    {
        return 3;
    }
    if (S[0] == -1 && S[1] == 2 && S[2] == 1 && S[3] == -2)
    {
        return 2;
    }

    if (S[0] == 1 && S[1] == -1 && S[2] == -2 && S[3] == 2)
    {
        return 1;
    }
    if (S[0] == 1 && S[1] == -1 && S[2] == 2 && S[3] == -2)
    {
        return 2;
    }
    if (S[0] == 1 && S[1] == -2 && S[2] == -1 && S[3] == 2)
    {
        return 2;
    }
    if (S[0] == 1 && S[1] == -2 && S[2] == 2 && S[3] == -1)
    {
        return 3;
    }
    if (S[0] == 1 && S[1] == 2 && S[2] == -2 && S[3] == -1)
    {
        return 4;
    }
    if (S[0] == 1 && S[1] == 2 && S[2] == -1 && S[3] == -2)
    {
        return 3;
    }

    return 0;
}
