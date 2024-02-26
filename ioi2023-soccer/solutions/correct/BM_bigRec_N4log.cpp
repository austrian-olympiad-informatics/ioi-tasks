#include "soccer.h"

#include <algorithm>
#include <set>
#include <vector>

using namespace std;

int N, maxi, where;
int F[2502][2502];
int bMost[2502];
int id[2502];
set<int> block;
set<pair<pair<pair<int, int>, pair<int, int>>, int>> ans;
int val[2502 * 2502];

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
                if (bMost[a] != bMost[pos] && bMost[b] != bMost[pos])
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
        for (auto j : ans)
        {
            if (j.first.first.first == i.first.first.first)
                break;

            if (i.first.second.first <= j.first.second.first && j.first.second.first + j.first.first.first - 1 <= i.first.second.first + i.first.first.first - 1)
            {
                if (i.first.second.second >= j.first.second.second && j.first.second.second + j.first.first.second - 1 >= i.first.second.second + i.first.first.second - 1)
                {
                    maxi = max(maxi, val[i.second] + val[j.second] - i.first.first.second * j.first.first.first);
                }
            }
        }
        val[i.second] = maxi;
    }

    maxi = 0;
    for (int i = 0; i < where; i++)
        maxi = max(maxi, val[i]);
    return maxi;
}
