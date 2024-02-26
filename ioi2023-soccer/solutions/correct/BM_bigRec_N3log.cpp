#include "soccer.h"

#include <algorithm>
#include <set>
#include <vector>

using namespace std;

const int MAXN = 3202;

int N, maxi, where;
int F[MAXN][MAXN];
int bMost[MAXN];
int id[MAXN];
set<int> block;
pair<pair<int, int>, int> ans[MAXN * MAXN];
int up[MAXN][MAXN];
int down[MAXN][MAXN];
int sum[MAXN][MAXN];

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
            F[i][j] = 1;
        id[i] = i;
    }

    for (int i = 1; i < N - 1; i++)
    {
        for (int j = 1; j < N - 1; j++)
            F[i][j] = FF[i - 1][j - 1];
    }

    bMost[0] = -1;
    bMost[N - 1] = -1;

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

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            sum[i][j] = F[i][j];
            if (j != 0)
                sum[i][j] += sum[i][j - 1];
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

        int last = h1 - 1, upMax = 0, downMin = N - 1;
        for (int j = h1; j <= h2 + 1; j++)
        {
            if (j == h2 + 1 || F[v1 - 1][j] == 1)
            {
                if (last + 1 < j)
                {
                    int val1 = (j - last - 1) * N + (last + 1), val2 = (downMin - upMax - 1) * N + (upMax + 1);
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
                    maxi = max(maxi, i.second + k.second - i.first.second / N * (j - last - 1));
                }
                last = j;
                upMax = 0, downMin = N - 1;
            }
            else
            {
                upMax = max(upMax, up[v1][j]);
                downMin = min(downMin, down[v1][j]);
            }
        }
        last = h1 - 1, upMax = 0, downMin = N - 1;
        for (int j = h1; j <= h2 + 1; j++)
        {
            if (j == h2 + 1 || F[v2 + 1][j] == 1)
            {
                if (last + 1 < j)
                {
                    int val1 = (j - last - 1) * N + (last + 1), val2 = (downMin - upMax - 1) * N + (upMax + 1);
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
                    maxi = max(maxi, i.second + k.second - i.first.second / N * (j - last - 1));
                }
                last = j;
                upMax = 0, downMin = N - 1;
            }
            else
            {
                upMax = max(upMax, up[v1][j]);
                downMin = min(downMin, down[v1][j]);
            }
        }

        ans[mId].second = maxi;
    }

    maxi = 0;
    for (int i = 0; i < where; i++)
        maxi = max(maxi, ans[i].second);
    return maxi;
}
