#include "soccer.h"
#include <iostream>
#include <vector>

int biggest_stadium(int N, std::vector<std::vector<int>> C)
{
    std::vector<std::vector<int>> up(N);
    int ans = 0;
    for (int i = 0; i < N; ++i)
    {
        up[i].resize(N + 1);
        for (int j = 0; j < N; ++j)
        {
            up[i][j] = (1 - C[i][j]) + (i ? up[i - 1][j] : 0);
        }

        std::vector<std::pair<int, int>> s;
        for (int j = 0; j <= N; ++j)
        {
            int h = up[i][j], w = 1;
            while (!s.empty() && s.back().first >= h)
            {
                auto [x, y] = s.back();
                ans = std::max(x * y, ans);
                s.pop_back();
                w += y;
            }
            s.push_back({h, w});
        }
    }
    return ans;
}
