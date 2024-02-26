#include "soccer.h"

int biggest_stadium(int N, std::vector<std::vector<int>> F)
{
    int lastRow = 0;
    bool inc = true;
    int last1 = -1, last2 = -1, cnt = 0;
    for (int i = 0; i < N; i++)
    {
        lastRow = i;
        bool finish = false;
        int n1 = -1, n2 = -1;
        for (int j = 0; j < N; j++)
        {
            if (F[i][j] == 0)
            {
                if (finish)
                    return -1;
                if (n1 == -1)
                    n1 = j;
                n2 = j;
                cnt++;
            }
            else if (j > 0 && F[i][j - 1] == 0)
            {
                finish = true;
            }
        }
        if (n1 == -1)
        {
            if (last1 != -1)
                break;
        }
        else
        {
            if (last1 != -1)
            {
                if ((last1 < n1 && last2 < n2) || (last1 > n1 && last2 > n2))
                    return -1;
                if (!inc && (last1 > n1 || last2 < n2))
                    return -1;
                if (last1 < n1 || n2 < last2)
                    inc = false;
            }
            last1 = n1;
            last2 = n2;
        }
    }

    for (int i = lastRow + 1; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (F[i][j] == 0)
                return -1;
        }
    }

    return cnt;
}
