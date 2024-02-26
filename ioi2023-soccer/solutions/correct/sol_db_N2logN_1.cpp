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
const int maxL = 10;
int N;

map<pair<int, pii>, int> dp;
vector<vector<int>> pre;

int query(int li, int lj, int ri, int rj)
{
    return pre[ri][rj] - (li ? pre[li - 1][rj] : 0) - (lj ? pre[ri][lj - 1] : 0) + (li && lj ? pre[li - 1][lj - 1] : 0);
}

pii calc_range(int i, int l, int r)
{
    int li = i - 1, ri = i;
    for (int j = maxL; j >= 0; --j)
    {
        while (li - (1 << j) >= 0 && query(li - (1 << j), l, i - 1, r) == 0)
        {
            li -= 1 << j;
        }
        while (ri + (1 << j) < N && query(i, l, ri + (1 << j), r) == 0)
        {
            ri += 1 << j;
        }
    }
    if (li >= 0 && query(li, l, i - 1, r) == 0)
        li--;
    if (ri < N && query(i, l, ri, r) == 0)
        ri++;
    return pii{li, ri};
}

int calc(int i, int l, int r)
{
    auto it = dp.find({i, {l, r}});
    if (it != dp.end())
    {
        return it->yy;
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
        for (int i = maxL; i >= 0; --i)
        {
            if (L + (1 << i) > R)
            {
                continue;
            }
            L += 1 << i;
            if (extendable(R))
                L -= 1 << i;
        }
        if (!extendable(R))
        {
            continue;
        }
        for (int i = maxL; i >= 0; --i)
        {
            if (R + (1 << i) <= r && extendable(R + (1 << i)))
            {
                R += 1 << i;
            }
        }
        pii range = calc_range(i, L, R);
        res = max(res, calc(i, L, R) + (R - L + 1) * (li - range.xx + range.yy - ri));
        R++;
    }
    return dp[{i, {l, r}}] = res;
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
    int res = 0;
    for (int i = 0; i < N; ++i)
    {
        pii range = calc_range(i, 0, N - 1);
        res = max(res, calc(i, 0, N - 1) + N * (range.yy - range.xx - 1));
    }
    return res;
}
