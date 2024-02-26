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

const int NUM_TRIES = 50;
const int SAMPLE_SIZE = 10;
const int maxN = 2003;
int minr[maxN], maxl[maxN];

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
        for (int j = 0; j < SAMPLE_SIZE; ++j)
        {
            auto p = pos[i], q = pos[rand() % pos.size()];
            if (p.xx != q.xx && p.yy != q.yy)
            {
                if (C[p.xx][q.yy] && C[q.xx][p.yy])
                {
                    cnt[i]++;
                }
                continue;
            }
            if (p > q)
                swap(p, q);
            if (p.xx == q.xx && prow[p.xx][q.yy] - (p.yy ? prow[p.xx][p.yy - 1] : 0) > 0)
            {
                cnt[i]++;
            }
            if (p.yy == q.yy && pcol[q.xx][p.yy] - (p.xx ? pcol[p.xx - 1][p.yy] : 0) > 0)
            {
                cnt[i]++;
            }
        }
    }
    return cnt;
}

bool is_regular(int N, vector<vector<int>> &C)
{
    vector<pii> ivals;
    fill(minr, minr + N + 3, maxN);
    fill(maxl, maxl + N + 3, 0);
    int cnt = 0, sum = 0;
    for (int i = 0; i < N; ++i)
    {
        int l = N, r = -1;
        for (int j = 0; j < N; ++j)
        {
            if (C[i][j] == 0)
            {
                l = min(l, j);
                r = max(r, j);
                cnt++;
            }
        }
        l++, r++;
        if (r)
            sum += r - l + 1;
        minr[l] = min(minr[l], r);
        maxl[r] = max(maxl[r], l);
        ivals.push_back({l, r});
    }
    if (sum != cnt)
    {
        return false;
    }
    for (int i = N; i >= 1; --i)
    {
        maxl[i] = max(maxl[i + 1], maxl[i]);
    }
    for (int i = 1; i <= N; ++i)
    {
        minr[i] = min(minr[i - 1], minr[i]);
    }
    for (int i = 1; i < N - 1; ++i)
    {
        pii p = ivals[i - 1];
        pii q = ivals[i];
        pii r = ivals[i + 1];
        if (q.yy - q.xx < p.yy - p.xx && q.yy - q.xx < r.yy - r.xx)
        {
            return false;
        }
    }
    for (auto q : ivals)
    {
        if (q.yy == 0)
        {
            continue;
        }
        int L = q.xx, R = q.yy;
        if (maxl[R + 1] > L || minr[L - 1] < R)
        {
            return false;
        }
    }
    return true;
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
        if (cnt[ind[0]] == 0 && is_regular(N, D))
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
        if (n == 0)
        {
            continue;
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
