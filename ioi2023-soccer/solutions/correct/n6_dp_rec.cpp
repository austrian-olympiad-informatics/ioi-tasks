#include "soccer.h"

#include <cstring>

class solver
{
    int N;
    std::vector<std::vector<int>> C;
    int dp[100][100][100][100];

    int solve(int d, int u, int l, int r)
    {
        if (dp[d][u][l][r] != -1)
            return dp[d][u][l][r];
        int curr = r - l + 1;

        int ans = curr;
        if (d > 0)
        {
            for (int i = l; i <= r; ++i)
            {
                for (int j = i; j <= r; ++j)
                {
                    if (1 == C[d - 1][j])
                        break;
                    ans = std::max(ans, curr + solve(d - 1, u, i, j));
                }
            }
        }
        if (u + 1 < N)
        {
            for (int i = l; i <= r; ++i)
            {
                for (int j = i; j <= r; ++j)
                {
                    if (1 == C[u + 1][j])
                        break;
                    ans = std::max(ans, curr + solve(d, u + 1, i, j));
                }
            }
        }

        return dp[d][u][l][r] = ans;
    }

public:
    solver(int N, std::vector<std::vector<int>> C) : N(N), C(std::move(C))
    {
        memset(dp, -1, sizeof dp);
    }

    int solve()
    {
        int ans = 0;
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                for (int k = j; k < N; ++k)
                {
                    if (1 == C[i][k])
                        break;
                    ans = std::max(ans, solve(i, i, j, k));
                }
            }
        }

        return ans;
    }
};

int biggest_stadium(int N, std::vector<std::vector<int>> C)
{
    solver s(N, C);
    return s.solve();
}
