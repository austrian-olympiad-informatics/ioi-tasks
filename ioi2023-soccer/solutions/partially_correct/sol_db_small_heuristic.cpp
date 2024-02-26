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

const int NUM_TRIES = 10000;

vector<int> calc_error(int N, const vector<vector<int>> &C)
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
    vector<int> cnt(pos.size(), 0);
    for (int i = 0; i < pos.size(); ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            auto p = pos[i], q = pos[j];
            if (p.xx != q.xx && p.yy != q.yy)
            {
                if (C[p.xx][q.yy] && C[q.xx][p.yy])
                {
                    cnt[i]++, cnt[j]++;
                }
                continue;
            }
            if (p > q)
                swap(p, q);
            if (p.xx == q.xx && prow[p.xx][q.yy] - (p.yy ? prow[p.xx][p.yy - 1] : 0) > 0)
            {
                cnt[i]++, cnt[j]++;
            }
            if (p.yy == q.yy && pcol[q.xx][p.yy] - (p.xx ? pcol[p.xx - 1][p.yy] : 0) > 0)
            {
                cnt[i]++, cnt[j]++;
            }
        }
    }
    return cnt;
}

vector<pii> make_regular(int N, const vector<vector<int>> &C, vector<pii> pos)
{
    auto wnext = [](int n, int w)
    {
        int ans = n;
        for (int i = 0; i < w; ++i)
        {
            ans = min(ans, rand() % n);
        }
        return ans;
    };
    while (true)
    {
        auto D = C;
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                D[i][j] = 1;
            }
        }
        for (pii p : pos)
        {
            D[p.xx][p.yy] = 0;
        }
        auto cnt = calc_error(N, D);
        vector<int> ind;
        for (int i = 0; i < cnt.size(); ++i)
            ind.push_back(i);
        sort(ind.begin(), ind.end(), [&](int i, int j)
             { return cnt[i] > cnt[j]; });
        if (cnt[ind[0]] == 0)
        {
            return pos;
        }
        int n = 0;
        for (; n < pos.size(); ++n)
        {
            if (cnt[ind[n]] == 0)
            {
                break;
            }
        }
        pos.erase(pos.begin() + ind[wnext(n, 5)]);
    }
}

int biggest_stadium(int N, vector<vector<int>> C)
{
    vector<pii> pos;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (!C[i][j])
                pos.push_back({i, j});
        }
    }
    int ans = 0;
    for (int i = 0; i < NUM_TRIES; ++i)
    {
        auto res = make_regular(N, C, pos);
        ans = max(ans, (int)res.size());
    }
    return ans;
}
