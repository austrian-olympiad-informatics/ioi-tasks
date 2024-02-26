#include "soccer.h"

#include <cstring>
#include <iostream>

const int MAXN = 100;

class solver
{
    int N;
    std::vector<std::vector<int>> C;
    int dp[MAXN][MAXN][MAXN][MAXN];

public:
    solver(int N, std::vector<std::vector<int>> C) : N(N), C(std::move(C))
    {
        for (int i = 0; i < MAXN; ++i)
            for (int j = 0; j < MAXN; ++j)
                for (int k = 0; k < MAXN; ++k)
                    for (int l = 0; l < MAXN; ++l)
                        dp[i][j][k][l] = -1e9;
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
                    dp[i][i][j][k] = k - j + 1;
                    ans = std::max(ans, k - j + 1);
                }
            }
        }

        for (int len = 2; len <= N; ++len)
        {
            for (int i = 0; i + len - 1 < N; ++i)
            {
                for (int j = 0; j < N; ++j)
                {
                    for (int k = j; k < N; ++k)
                    {
                        if (1 == C[i][k] || 1 == C[i + len - 1][k])
                            break;
                        int &dp_val = dp[i][i + len - 1][j][k];

                        for (int l = j; l >= 0; l--)
                        {
                            if (1 == C[i + 1][l] || 1 == C[i + len - 1][l])
                                break;
                            for (int r = k; r < N; ++r)
                            {
                                if (1 == C[i + 1][r] || 1 == C[i + len - 1][r])
                                    break;
                                dp_val = std::max(dp_val, k - j + 1 + dp[i + 1][i + len - 1][l][r]);
                            }
                        }

                        for (int l = j; l >= 0; l--)
                        {
                            if (1 == C[i][l] || 1 == C[i + len - 2][l])
                                break;
                            for (int r = k; r < N; ++r)
                            {
                                if (1 == C[i][r] || 1 == C[i + len - 2][r])
                                    break;
                                dp_val = std::max(dp_val, k - j + 1 + dp[i][i + len - 2][l][r]);
                            }
                        }

                        ans = std::max(ans, dp_val);
                    }
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
