#include "soccer.h"
#include <iostream>
#include <array>
#include <map>
#include <algorithm>
#include <cassert>
#include <string.h>
#define xx first
#define yy second

using namespace std;
typedef pair<int, int> pii;
const int maxN = 501;
int N, dp[maxN][maxN][maxN];

vector<vector<int>> pre;

int query(int li, int lj, int ri, int rj)
{
    return pre[ri][rj] - (li ? pre[li - 1][rj] : 0) - (lj ? pre[ri][lj - 1] : 0) + (li && lj ? pre[li - 1][lj - 1] : 0);
}

pii calc_range(int i, int l, int r)
{
    int li = i - 1, ri = i;
    while (li >= 0 && query(li, l, i - 1, r) == 0)
        li--;
    while (ri < N && query(i, l, ri, r) == 0)
        ri++;
    return pii{li, ri};
}

int calc(int i, int l, int r)
{
    if (dp[i][l][r] != -1)
    {
        return dp[i][l][r];
    }
    pii range = calc_range(i, l, r);
    if (i != range.xx + 1)
    {
        return calc(range.xx + 1, l, r);
    }
    int li = range.xx;
    int ri = range.yy;
    int res = 0;

    for (int L = l, R = l; R <= r; ++L, R = max(R, L))
    {
        auto extendable = [&](int R)
        {
            return (li >= 0 && query(li, L, i - 1, R) == 0) || (ri < N && query(i, L, ri, R) == 0);
        };
        if (!extendable(R))
        {
            continue;
        }
        while (R + 1 <= r && extendable(R + 1))
            R++;

        int Li = li;
        int Ri = ri;
        while (Li >= 0 && query(Li, L, i - 1, R) == 0)
            Li--;
        while (Ri < N && query(i, L, Ri, R) == 0)
            Ri++;

        res = max(res, calc(i, L, R) + (R - L + 1) * (li - Li + Ri - ri));
        R++;
    }
    return dp[i][l][r] = res;
}

int biggest_stadium(int n, vector<vector<int>> C)
{
    N = n, pre = C;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            pre[i][j] += (j ? pre[i][j - 1] : 0);
        }
    }
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            pre[i][j] += (i ? pre[i - 1][j] : 0);
        }
    }
    memset(dp, -1, sizeof dp);
    int res = 0;
    for (int i = 0; i < N; ++i)
    {
        pii range = calc_range(i, 0, N - 1);
        res = max(res, calc(i, 0, N - 1) + N * (range.yy - range.xx - 1));
    }
    return res;
}
