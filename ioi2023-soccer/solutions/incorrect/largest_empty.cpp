#include "soccer.h"
#include <iostream>
#include <vector>
#include <set>
struct dsu
{
    std::vector<int> par;
    std::vector<int> sz;
    dsu(int n) : par(n, -1) { sz.assign(n, 1); }

    int get(int x)
    {
        if (par[x] == -1)
            return x;
        return par[x] = get(par[x]);
    }

    void merge(int x, int y)
    {
        int px = get(x), py = get(y);
        if (px == py)
            return;

        if (sz[px] > sz[py])
        {
            std::swap(px, py);
            std::swap(x, y); //:) lyft
        }

        sz[py] += sz[px];
        par[px] = py;
    }
};
const int MAXN = 3202;
int d0[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
dsu d(MAXN *MAXN);
int biggest_stadium(int N, std::vector<std::vector<int>> C)
{
    int ans = 0;

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (C[i][j] == 0)
            {
                for (int k = 0; k < 4; ++k)
                {
                    int ni = i + d0[k][0], nj = j + d0[k][1];
                    if (ni >= 0 && nj >= 0 && ni < N && nj < N)
                    {
                        if (0 == C[ni][nj])
                        {
                            d.merge(i * N + j, ni * N + nj);
                            ans = std::max(ans, d.sz[d.get(i * N + j)]);
                        }
                    }
                }
            }
        }
    }

    std::set<int> st;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            int dd = d.get(i * N + j);
            if (d.sz[dd] == ans)
                st.insert(dd);
        }
    }

    std::cerr << st.size() << "\n";

    return ans;
}
