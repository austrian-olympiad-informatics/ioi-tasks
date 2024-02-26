#include "soccer.h"

#include <algorithm>
#include <set>
#include <map>
#include <vector>

using namespace std;

const int MAXN = 3202;

int N, maxi, where;
int F[MAXN][MAXN];
int bMost[MAXN];
int id[MAXN];
set<int> block;
set<pair<pair<pair<int, int>, pair<int, int>>, int>> ans;
int val[MAXN * MAXN];
int up[MAXN][MAXN];
int upLeft[MAXN][MAXN];
int upRight[MAXN][MAXN];
int down[MAXN][MAXN];
int downLeft[MAXN][MAXN];
int downRight[MAXN][MAXN];
int sum[MAXN][MAXN];
set<pair<int, int>> trees[MAXN];

bool small(int x, int y)
{
    return bMost[x] > bMost[y];
}

int biggest_stadium(int NN, std::vector<std::vector<int>> FF)
{
    N = NN;
    N += 2;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            F[i][j] = 1;
            down[i][j] = N - 1;
            downLeft[i][j] = N - 1;
            downRight[i][j] = N - 1;
        }
        id[i] = i;
    }

    for (int i = 1; i < N - 1; i++)
    {
        for (int j = 1; j < N - 1; j++)
            F[i][j] = FF[i - 1][j - 1];
    }

    bMost[0] = -1;
    bMost[N - 1] = -1;

    for (int i = 1; i < N - 1; i++)
    {
        int last = 0;
        for (int j = 1; j < N; j++)
        {
            if (F[i][j] == 1)
            {
                if (last + 1 < j)
                    trees[i].insert({last, j});
                last = j;
            }
        }
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            sum[i][j] = F[i][j];
            if (j != 0)
                sum[i][j] += sum[i][j - 1];
        }
    }

    for (int j = 0; j < N; j++)
        down[N - 1][j] = N - 1;
    for (int i = 1; i < N - 1; i++)
    {
        for (int j = 0; j < N; j++)
        {
            up[i][j] = up[i - 1][j];
            down[N - 1 - i][j] = down[N - i][j];
            if (F[i][j] == 1)
                up[i][j] = i;
            if (F[N - 1 - i][j] == 1)
                down[N - 1 - i][j] = N - 1 - i;
        }
    }

    for (int i = 1; i < N - 1; i++)
    {
        for (int j = 1; j < N - 1; j++)
        {
            up[i][j] = up[i - 1][j];
            if (F[i][j] == 1)
            {
                up[i][j] = i;
            }
            else
            {
                upLeft[i][j] = max(upLeft[i][j - 1], up[i][j]);
            }
        }
        for (int j = N - 2; j > 0; j--)
        {
            if (F[i][j] == 0)
            {
                upRight[i][j] = max(upRight[i][j + 1], up[i][j]);
            }
        }
    }
    for (int i = N - 2; i > 0; i--)
    {
        for (int j = 1; j < N - 1; j++)
        {
            down[i][j] = down[i + 1][j];
            if (F[i][j] == 1)
            {
                down[i][j] = i;
            }
            else
            {
                downLeft[i][j] = min(downLeft[i][j - 1], down[i][j]);
            }
        }
        for (int j = N - 2; j > 0; j--)
        {
            if (F[i][j] == 0)
            {
                downRight[i][j] = min(downRight[i][j + 1], down[i][j]);
            }
        }
    }

    for (int i = 1; i < N - 1; i++)
    {
        for (int j = 1; j < N - 1; j++)
        {
            if (F[i][j] == 1)
                bMost[j] = i;
        }
        std::sort(id + 1, id + N - 1, small);
        block.clear();
        block.insert(0);
        block.insert(N - 1);

        for (int j = 1; j < N - 1; j++)
        {
            int pos = id[j];
            if (bMost[pos] != i)
            {
                auto it = block.lower_bound(pos);
                int b = (*it);
                it--;
                int a = (*it);
                int v = i - bMost[pos], h = b - a - 1;
                if (bMost[a] != bMost[pos] && bMost[b] != bMost[pos] && sum[i + 1][b - 1] - sum[i + 1][a] >= 1)
                {
                    ans.insert({{{h, v}, {a + 1, bMost[pos] + 1}}, where});
                    val[where++] = v * h;
                }
            }
            block.insert(pos);
        }
    }

    for (auto i : ans)
    {
        maxi = val[i.second];

        int h1 = i.first.second.first, h2 = i.first.second.first + i.first.first.first - 1;
        int v1 = i.first.second.second, v2 = i.first.second.second + i.first.first.second - 1;

        int upMax = 0, downMin = 0;
        auto it = trees[v1 - 1].lower_bound({h1, -1});
        if (it != trees[v1 - 1].begin())
            it--;
        if (it != trees[v1 - 1].end() && (*it).second <= h1)
            it++;
        while (it != trees[v1 - 1].end() && (*it).first < h2)
        {
            int b = min(h2 + 1, (*it).second);
            int a = max(h1 - 1, (*it).first);
            if (a + 1 < b)
            {
                if (a == h1 - 1)
                {
                    upMax = upRight[v1 - 1][a + 1], downMin = downRight[v1 - 1][a + 1];
                }
                else
                {
                    upMax = upLeft[v1 - 1][b - 1], downMin = downLeft[v1 - 1][b - 1];
                }
                auto k = (*ans.lower_bound({{{b - a - 1, downMin - upMax - 1}, {a + 1, upMax + 1}}, -1}));
                maxi = max(maxi, val[i.second] + val[k.second] - i.first.first.second * k.first.first.first);
            }
            it++;
        }

        it = trees[v2 + 1].lower_bound({h1, -1});
        if (it != trees[v2 + 1].begin())
            it--;
        if (it != trees[v2 + 1].end() && (*it).second <= h1)
            it++;
        while (it != trees[v2 + 1].end() && (*it).first < h2)
        {
            int b = min(h2 + 1, (*it).second);
            int a = max(h1 - 1, (*it).first);
            if (a + 1 < b)
            {
                if (a == h1 - 1)
                {
                    upMax = upRight[v2 + 1][a + 1], downMin = downRight[v2 + 1][a + 1];
                }
                else
                {
                    upMax = upLeft[v2 + 1][b - 1], downMin = downLeft[v2 + 1][b - 1];
                }
                auto k = (*ans.lower_bound({{{b - a - 1, downMin - upMax - 1}, {a + 1, upMax + 1}}, -1}));
                maxi = max(maxi, val[i.second] + val[k.second] - i.first.first.second * k.first.first.first);
            }
            it++;
        }

        val[i.second] = maxi;
    }

    maxi = 0;
    for (int i = 0; i < where; i++)
        maxi = max(maxi, val[i]);
    return maxi;
}
