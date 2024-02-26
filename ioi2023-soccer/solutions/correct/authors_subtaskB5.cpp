// ##################################
// ##                              ##
// ##   Solution for Subtask B5    ##
// ##         (78.25 pts)          ##
// ##                              ##
// ##################################

#include <iostream>
#include <vector>
#include <algorithm>

#include "soccer.h"

using namespace std;

int solve(int N, vector<int> cl, vector<int> cr)
{
    vector<vector<int>> max_cl(N, vector<int>(N, (1 << 30)));
    vector<vector<int>> min_cr(N, vector<int>(N, (1 << 30)));

    // Calculate Min/Max
    for (int i = 0; i < N; i++)
    {
        int cur_cl = -(1 << 30), cur_cr = (1 << 30);
        for (int j = i; j < N; j++)
        {
            cur_cl = max(cur_cl, cl[j]);
            cur_cr = min(cur_cr, cr[j]);
            max_cl[i][j] = cur_cl;
            min_cr[i][j] = cur_cr;
        }
    }

    // Dynamic Programming
    vector<vector<int>> dp(N, vector<int>(N, 0));
    for (int i = 0; i < N; i++)
    {
        dp[i][i] = (min_cr[i][i] - max_cl[i][i] + 1);
    }
    for (int len = 0; len < N - 1; len++)
    {
        for (int i = 0; i < N - len; i++)
        {
            int j = i + len;
            if (i >= 1)
                dp[i - 1][j] = max(dp[i - 1][j], dp[i][j] + (min_cr[i - 1][j] - max_cl[i - 1][j] + 1));
            if (j < N - 1)
                dp[i][j + 1] = max(dp[i][j + 1], dp[i][j] + (min_cr[i][j + 1] - max_cl[i][j + 1] + 1));
        }
    }

    // Return
    return dp[0][N - 1];
}

int biggest_stadium(int N, vector<vector<int>> C)
{
    int Answer = 0;

    // Search "the red line"
    for (int i = 0; i <= N; i++)
    {
        vector<int> cl(N, i);
        vector<int> cr(N, i - 1);
        for (int j = 0; j < N; j++)
        {
            for (int k = i - 1; k >= 0; k--)
            {
                if (C[j][k] == 1)
                    break;
                cl[j] = k;
            }
            for (int k = i; k < N; k++)
            {
                if (C[j][k] == 1)
                    break;
                cr[j] = k;
            }
        }

        // Calculate DP
        int ret = solve(N, cl, cr);
        Answer = max(Answer, ret);
    }

    // Return
    return Answer;
}