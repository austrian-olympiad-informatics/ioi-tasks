#include "soccer.h"

int biggest_stadium(int N, std::vector<std::vector<int>> F)
{
    int cnt = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            cnt += F[i][j];
    }
    return cnt;
}
