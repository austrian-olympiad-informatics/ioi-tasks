#include "soccer.h"
#include <iostream>
#include <array>
#include <map>
#include <algorithm>
#include <cassert>
#define xx first
#define yy second

using namespace std;
typedef pair<int, int> pii;

bool is_convenient(int N, vector<vector<int>> C)
{
    auto prow = C, pcol = C;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            prow[i][j] += (j ? prow[i][j - 1] : 0);
            pcol[i][j] += (i ? pcol[i - 1][j] : 0);
        }
    }
    vector<pii> pos;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (!C[i][j])
                pos.push_back({i, j});
        }
    }
    for (int i = 0; i < pos.size(); ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            auto p = pos[i], q = pos[j];
            if (p.xx != q.xx && p.yy != q.yy)
            {
                if (C[p.xx][q.yy] && C[q.xx][p.yy])
                {
                    return false;
                }
                continue;
            }
            if (p > q)
                swap(p, q);
            if (p.xx == q.xx && prow[p.xx][q.yy] - (p.yy ? prow[p.xx][p.yy - 1] : 0) > 0)
            {
                return false;
            }
            if (p.yy == q.yy && pcol[q.xx][p.yy] - (p.xx ? pcol[p.xx - 1][p.yy] : 0) > 0)
            {
                return false;
            }
        }
    }
    return true;
}

int biggest_stadium(int N, vector<vector<int>> C)
{
    int ans = 0;
    for (int mask = 0; mask < (1 << N * N); ++mask)
    {
        auto subset = C;
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                int ind = N * i + j, active = mask >> ind & 1;
                if (!active && C[i][j])
                {
                    goto skip;
                }
                subset[i][j] = active;
            }
        }
        if (is_convenient(N, subset))
        {
            ans = max(ans, N * N - __builtin_popcount(mask));
        }
    skip:;
    }
    return ans;
}
