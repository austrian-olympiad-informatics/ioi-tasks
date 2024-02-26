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

const int maxN = 2003;
int minr[maxN], maxl[maxN];

vector<pii> ivals;

int biggest_stadium(int N, vector<vector<int>> C)
{
    fill(minr, minr + maxN, maxN);
    for (int i = 0; i < N; ++i)
    {
        int l = N, r = -1;
        for (int j = 0; j < N; ++j)
        {
            if (C[i][j] == 0)
            {
                l = min(l, j);
                r = max(r, j);
            }
        }
        l++, r++;
        minr[l] = min(minr[l], r);
        maxl[r] = max(maxl[r], l);
        ivals.push_back({l, r});
    }
    for (int i = N; i >= 1; --i)
    {
        maxl[i] = max(maxl[i + 1], maxl[i]);
    }
    for (int i = 1; i <= N; ++i)
    {
        minr[i] = min(minr[i - 1], minr[i]);
    }
    int sum = 0;
    for (int i = 1; i < N - 1; ++i)
    {
        pii p = ivals[i - 1];
        pii q = ivals[i];
        pii r = ivals[i + 1];
        if (q.yy - q.xx < p.yy - p.xx && q.yy - q.xx < r.yy - r.xx)
        {
            return 0;
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
            return 0;
        }
        sum += R - L + 1;
    }
    return sum;
}
