#include "soccer.h"
#include <iostream>
int n;
std::vector<std::vector<int>> c;
std::vector<std::vector<int>> pre;
std::vector<std::vector<int>> st;

int sum(int a, int b, int x, int y)
{
    if (a > x)
        std::swap(a, x);
    if (b > y)
        std::swap(b, y);
    return pre[x][y] - (b ? pre[x][b - 1] : 0) - (a ? pre[a - 1][y] : 0) + (a && b ? pre[a - 1][b - 1] : 0);
}

bool connected(int a, int b, int x, int y)
{
    return sum(a, b, x, b) + sum(x, b, x, y) == 0 || sum(a, b, a, y) + sum(a, y, x, y) == 0;
}

std::vector<std::pair<int, int>> curr;
int mx = 0;
void dfs(int x, int y)
{
    if (c[x][y] || st[x][y])
        return;

    st[x][y] = 1;
    bool ok = true;
    for (auto i : curr)
    {
        ok &= connected(i.first, i.second, x, y);
    }

    if (ok)
        curr.push_back({x, y});
    else
    {
        curr = {{x, y}};
    }

    mx = std::max(mx, (int)curr.size());

    if (x > 0)
        dfs(x - 1, y);
    if (y > 0)
        dfs(x, y - 1);
    if (y + 1 < n)
        dfs(x, y + 1);
    if (x + 1 < n)
        dfs(x + 1, y);
}

int biggest_stadium(int N, std::vector<std::vector<int>> C)
{
    n = N;
    c = C;
    pre.assign(n, std::vector<int>(n, 0));
    st.assign(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            pre[i][j] = c[i][j];
            if (i)
                pre[i][j] += pre[i - 1][j];
            if (j)
                pre[i][j] += pre[i][j - 1];
            if (i && j)
                pre[i][j] -= pre[i - 1][j - 1];
        }
    }

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            dfs(i, j);
        }
    }

    return mx;
}
