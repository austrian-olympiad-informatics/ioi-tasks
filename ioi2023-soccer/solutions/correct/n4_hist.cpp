#include "soccer.h"
#include <iostream>
#include <array>
#include <map>
const int MAXN = 3202;

class solver
{
    int N;
    std::vector<std::vector<int>> C;
    int nxt[2][MAXN][MAXN], prv[2][MAXN][MAXN];
    int up[MAXN][MAXN], down[MAXN][MAXN];

    std::vector<std::array<int, 4>> rects;
    std::map<std::array<int, 4>, int> rect_to_ind;
    std::vector<int> dp;
    void gen_max_rectangles()
    {
        for (int i = 0; i < N; i++)
        {
            std::vector<std::array<int, 3>> stack; // (start, w, h)
            for (int j = 0; j <= N; j++)
            {
                int curr_h = j < N ? up[i][j] : 0;
                int curr_start = j, curr_w = 1;
                while (!stack.empty() && stack.back()[2] >= curr_h)
                {
                    auto [start, w, h] = stack.back();
                    if (h > 0)
                    {
                        rects.push_back({i - h + 1, start, i, start + w - 1 + curr_w - 1});
                        rect_to_ind[rects.back()] = (int)rects.size() - 1;
                    }
                    curr_start = start;
                    curr_w += w;

                    stack.pop_back();
                }

                stack.push_back({curr_start, curr_w, curr_h});
            }
        }

        dp.assign(rects.size(), -1);
    }

    void init()
    {
        for (int i = 0; i < N; i++)
        {
            nxt[0][i][N - 1] = nxt[1][i][N - 1] = N;
            for (int j = N - 2; j >= 0; j--)
            {
                int nxt_elem = C[i][j + 1];

                nxt[nxt_elem][i][j] = j + 1;
                nxt[1 - nxt_elem][i][j] = nxt[1 - nxt_elem][i][j + 1];
            }

            prv[0][i][0] = prv[1][i][0] = -1;
            for (int j = 1; j < N; j++)
            {
                int prv_elem = C[i][j - 1];

                prv[prv_elem][i][j] = j - 1;
                prv[1 - prv_elem][i][j] = prv[1 - prv_elem][i][j - 1];
            }

            for (int j = 0; j < N; j++)
            {
                up[i][j] = (C[i][j] == 1 || i == 0) ? 1 - C[i][j] : 1 + up[i - 1][j];
                down[N - i - 1][j] = (C[N - i - 1][j] == 1 || i == 0) ? 1 - C[N - i - 1][j] : 1 + down[N - i][j];
            }
        }
    }

    int calc(int ind)
    {
        if (dp[ind] != -1)
            return dp[ind];
        int area = (rects[ind][2] - rects[ind][0] + 1) * (rects[ind][3] - rects[ind][1] + 1);
        int ans = area;
        for (int i = 0; i < (int)rects.size(); i++)
        {
            if (ind == i)
                continue;
            if (rects[i][0] <= rects[ind][0] && rects[ind][2] <= rects[i][2] &&
                rects[ind][1] <= rects[i][1] && rects[i][3] <= rects[ind][3])
            {
                ans = std::max(ans, area + calc(i) - (rects[ind][2] - rects[ind][0] + 1) * (rects[i][3] - rects[i][1] + 1));
            }
        }

        return dp[ind] = ans;
    }

public:
    solver(int N, std::vector<std::vector<int>> C) : N(N), C(std::move(C))
    {
        init();
        gen_max_rectangles();
    }

    int solve()
    {
        int ans = 0;
        for (int i = 0; i < (int)rects.size(); i++)
        {
            ans = std::max(ans, calc(i));
        }
        return ans;
    }
};

int biggest_stadium(int N, std::vector<std::vector<int>> C)
{
    solver s(N, C);
    return s.solve();
}
