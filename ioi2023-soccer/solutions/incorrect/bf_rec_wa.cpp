#include "soccer.h"
#include <iostream>

class solver
{
    int N;
    std::vector<std::vector<int>> C;
    int ans;

    void explore_stadium(int row, int globalX, int globalY, int currX, int currY, int state, int cells)
    {
        ans = std::max(ans, cells);
        if (row < N)
        {
            for (int j = 0; j < N; j++)
            {
                for (int k = j; k < N; k++)
                {
                    if (0 == C[row][k])
                    {
                        if ((j <= globalX && globalY <= k) ||
                            (globalX <= j && k <= globalY))
                        {
                            if (0 == state)
                            {
                                if (j <= currX && currY <= k)
                                {
                                    explore_stadium(row + 1, globalX, globalY, j, k, 0, cells + (k - j + 1));
                                }
                                else if (currX <= j && k <= currY)
                                {
                                    explore_stadium(row + 1, globalX, globalY, j, k, 1, cells + (k - j + 1));
                                }
                            }
                            else
                            {
                                if (currX <= j && k <= currY)
                                {
                                    explore_stadium(row + 1, globalX, globalY, j, k, 1, cells + (k - j + 1));
                                }
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }

public:
    solver(int N, std::vector<std::vector<int>> C) : N(N), C(std::move(C)), ans(0) {}

    int solve()
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                for (int k = j; k < N; k++)
                {
                    if (0 == C[i][k])
                    {
                        explore_stadium(i + 1, j, k, j, k, 0, k - j + 1);
                    }
                    else
                    {
                        break;
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