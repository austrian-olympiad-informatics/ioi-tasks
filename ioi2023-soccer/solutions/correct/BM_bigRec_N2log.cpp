#include "soccer.h"

#include <algorithm>
#include <set>
#include <vector>
#include <iostream>

using namespace std;

const int MAXN = 3202;

int N, maxi, where;
int F[MAXN][MAXN];
int bMost[MAXN];
int id[MAXN];
set<int> block;
pair<pair<int, int>, int> ans[MAXN * MAXN];
int up[MAXN][MAXN];
int upLeft[MAXN][MAXN];
int upRight[MAXN][MAXN];
int down[MAXN][MAXN];
int downLeft[MAXN][MAXN];
int downRight[MAXN][MAXN];
int sum[MAXN][MAXN];
int nxt[2][MAXN][MAXN];

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

    for (int i = 0; i < N; i++)
    {
        nxt[0][i][N - 1] = nxt[1][i][N - 1] = N;
        for (int j = N - 2; j >= 0; j--)
        {
            int nxt_elem = F[i][j + 1];

            nxt[nxt_elem][i][j] = j + 1;
            nxt[1 - nxt_elem][i][j] = nxt[1 - nxt_elem][i][j + 1];
        }
    }

    bMost[0] = -1;
    bMost[N - 1] = -1;
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
                    ans[where++] = {{h * N + (a + 1), v * N + (bMost[pos] + 1)}, v * h};
                }
            }
            block.insert(pos);
        }
    }

    sort(ans, ans + where);

    for (int mId = 0; mId < where; mId++)
    {
        auto i = ans[mId];
        maxi = i.second;

        int h1 = i.first.first % N, h2 = i.first.first % N + i.first.first / N - 1;
        int v1 = i.first.second % N, v2 = i.first.second % N + i.first.second / N - 1;

        int upMax = 0, downMin = 0;
        int x = h1 - 1;
        while (x < h2)
        {
            int xx = nxt[0][v1 - 1][x];
            int yy = (xx == N ? N : nxt[1][v1 - 1][xx] - 1);
            x = yy;
            int b = min(h2 + 1, yy + 1);
            int a = max(h1 - 1, xx - 1);
            // std::cerr<<v1<<" "<<v2<<" "<<h1<<" "<<h2<<" "<<a<<" "<<b<<"\n";
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
                int val1 = (b - a - 1) * N + (a + 1), val2 = (downMin - upMax - 1) * N + (upMax + 1);
                int l = 0, r = mId - 1;
                while (l != r)
                {
                    int half = (l + r) / 2;
                    if (ans[half].first.first < val1 || (ans[half].first.first == val1 && ans[half].first.second < val2))
                    {
                        l = half + 1;
                    }
                    else
                    {
                        r = half;
                    }
                }
                const auto &k = ans[l];
                if (k.first.first != val1 || k.first.second != val2)
                    continue;
                maxi = max(maxi, i.second + k.second - i.first.second / N * (b - a - 1));
            }
        }

        x = h1 - 1;
        while (x < h2)
        {
            int xx = nxt[0][v2 + 1][x];
            int yy = (xx == N ? N : nxt[1][v2 + 1][xx] - 1);
            x = yy;
            int b = min(h2 + 1, yy + 1);
            int a = max(h1 - 1, xx - 1);
            // std::cerr<<v1<<" "<<v2<<" "<<h1<<" "<<h2<<" "<<a<<" "<<b<<"\n";
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
                int val1 = (b - a - 1) * N + (a + 1), val2 = (downMin - upMax - 1) * N + (upMax + 1);
                int l = 0, r = mId - 1;
                while (l != r)
                {
                    int half = (l + r) / 2;
                    if (ans[half].first.first < val1 || (ans[half].first.first == val1 && ans[half].first.second < val2))
                    {
                        l = half + 1;
                    }
                    else
                    {
                        r = half;
                    }
                }
                const auto &k = ans[l];
                if (k.first.first != val1 || k.first.second != val2)
                    continue;
                maxi = max(maxi, i.second + k.second - i.first.second / N * (b - a - 1));
            }
        }
        // std::cerr<<v1<<" "<<v2<<" "<<h1<<" "<<h2<<" "<<maxi<<"!!\n";
        ans[mId].second = maxi;
    }

    maxi = 0;
    for (int i = 0; i < where; i++)
        maxi = max(maxi, ans[i].second);
    return maxi;
}
