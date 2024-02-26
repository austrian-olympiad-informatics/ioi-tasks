#include "soccer.h"
#include <iostream>
int biggest_stadium(int N, std::vector<std::vector<int>> C)
{
    int X = -1, Y = -1;
    bool run = true;
    for (int i = 0; i < N && run; ++i)
    {
        for (int j = 0; j < N && run; ++j)
        {
            if (1 == C[i][j])
            {
                X = i;
                Y = j;
                run = false;
            }
        }
    }

    if (X == -1)
    {
        return N * N;
    }

    int ans = 0;
    auto check = [&](int x, int y)
    {
        ans = std::max(ans, (x + y) * N - x * y);
    };

    check(X, Y);
    check(N - 1 - X, Y);
    check(N - 1 - X, N - 1 - Y);
    check(X, N - 1 - Y);

    return ans;
}
