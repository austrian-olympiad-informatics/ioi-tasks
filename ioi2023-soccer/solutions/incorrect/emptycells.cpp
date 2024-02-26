#include "soccer.h"
#include <iostream>
int biggest_stadium(int N, std::vector<std::vector<int>> C)
{
    int ans = 0;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (0 == C[i][j])
            {
                ans += 1;
            }
        }
    }

    return ans;
}
