#include "soccer.h"
#include <iostream>

const int MAXN = 3202;
int biggest_stadium_wd(int N, const std::vector<std::vector<int>> &FF)
{
    int F[MAXN][MAXN];
    int left[MAXN][MAXN];
    int right[MAXN][MAXN];
    int mid[MAXN][MAXN];
    int last[MAXN];
    int left2[MAXN][MAXN];
    int right2[MAXN][MAXN];
    int mid2[MAXN][MAXN];
    int last2[MAXN];
    int ans = 0;
    for (int i = 0; i <= N + 1; i++)
    {
        last[i] = 0;
        last2[i] = 0;
        for (int j = 0; j <= N + 1; j++)
        {
            F[i][j] = 0;
            left[i][j] = 0;
            right[i][j] = 0;
            mid[i][j] = 0;
            left2[i][j] = 0;
            right2[i][j] = 0;
            mid2[i][j] = 0;
        }
    }

    F[0][0] = 1;
    F[0][N + 1] = 1;
    F[N + 1][0] = 1;
    F[N + 1][N + 1] = 1;
    for (int i = 1; i <= N; i++)
    {
        F[0][i] = 1;
        F[N + 1][i] = 1;
        F[i][0] = 1;
        F[i][N + 1] = 1;
        for (int j = 1; j <= N; j++)
            F[i][j] = FF[i - 1][j - 1];
    }

    for (int i = 1; i <= N; i++)
    {
        int start = 1;
        for (int j = 1; j <= N + 1; j++)
        {
            if (F[i][j] == 1)
            {
                if (start < j)
                {
                    int myLast = 0;
                    for (int k = start; k < j; k++)
                        myLast = std::max(myLast, last[k]);
                    int start2 = start, maxi = 0;
                    for (int k = start; k < j; k++)
                    {
                        if (F[myLast][k] == 1)
                        {
                            if (start2 < k)
                            {
                                if (start2 == start)
                                {
                                    maxi = right[myLast][start2];
                                }
                                else
                                {
                                    maxi = std::max(maxi, mid[myLast][k]);
                                }
                            }
                            start2 = k + 1;
                        }
                    }
                    if (start2 < j)
                        maxi = std::max(maxi, left[myLast][j - 1]);
                    mid[i][j] = maxi + (j - start) * (i - myLast);
                }
                start = j + 1;
            }
        }

        start = 1;
        int myLast = 0;
        for (int j = 1; j <= N + 1; j++)
        {
            if (F[i][j] == 1)
            {
                start = j + 1;
                myLast = 0;
            }
            else
            {
                left[i][j] = left[i][j - 1] - (j - start) * (i - myLast);
                if (myLast >= last[j])
                {
                    if (myLast == last[j])
                    {
                        left[i][j] = std::max(left[i][j], mid[myLast][j]);
                    }
                    else
                    {
                        left[i][j] = std::max(left[i][j], left[myLast][j]);
                    }
                }
                else
                {
                    myLast = last[j];
                    left[i][j] = right[myLast][start];
                }
                left[i][j] += (j - start + 1) * (i - myLast);
            }
        }

        start = N;
        myLast = 0;
        for (int j = N; j >= 0; j--)
        {
            if (F[i][j] == 1)
            {
                start = j - 1;
                myLast = 0;
            }
            else
            {
                right[i][j] = right[i][j + 1] - (start - j) * (i - myLast);
                if (myLast >= last[j])
                {
                    if (myLast == last[j])
                    {
                        right[i][j] = std::max(right[i][j], mid[myLast][start + 1]);
                    }
                    else
                    {
                        right[i][j] = std::max(right[i][j], right[myLast][j]);
                    }
                }
                else
                {
                    myLast = last[j];
                    right[i][j] = left[myLast][start];
                }
                right[i][j] += (start - j + 1) * (i - myLast);
            }
        }

        for (int j = 0; j <= N + 1; j++)
            if (F[i][j] == 1)
                last[j] = i;
    }

    for (int j = 0; j <= N + 1; j++)
        last2[j] = N + 1;
    for (int i = N; i >= 1; i--)
    {
        int start = 1;
        for (int j = 1; j <= N + 1; j++)
        {
            if (F[i][j] == 1)
            {
                if (start < j)
                {
                    int myLast = N + 1;
                    for (int k = start; k < j; k++)
                        myLast = std::min(myLast, last2[k]);
                    int start2 = start, maxi = 0;
                    for (int k = start; k < j; k++)
                    {
                        if (F[myLast][k] == 1)
                        {
                            if (start2 < k)
                            {
                                if (start2 == start)
                                {
                                    maxi = right2[myLast][start2];
                                }
                                else
                                {
                                    maxi = std::max(maxi, mid2[myLast][k]);
                                }
                            }
                            start2 = k + 1;
                        }
                    }
                    if (start2 < j)
                        maxi = std::max(maxi, left2[myLast][j - 1]);
                    mid2[i][j] = maxi + (j - start) * (myLast - i);
                    ans = std::max(ans, mid[i][j] + mid2[i][j] - (j - start));
                }
                start = j + 1;
            }
        }

        start = 1;
        int myLast = N + 1;
        for (int j = 1; j <= N + 1; j++)
        {
            if (F[i][j] == 1)
            {
                start = j + 1;
                myLast = N + 1;
            }
            else
            {
                left2[i][j] = left2[i][j - 1] - (j - start) * (myLast - i);
                if (myLast <= last2[j])
                {
                    if (myLast == last2[j])
                    {
                        left2[i][j] = std::max(left2[i][j], mid2[myLast][j]);
                    }
                    else
                    {
                        left2[i][j] = std::max(left2[i][j], left2[myLast][j]);
                    }
                }
                else
                {
                    myLast = last2[j];
                    left2[i][j] = right2[myLast][start];
                }
                left2[i][j] += (j - start + 1) * (myLast - i);
            }
        }

        start = N;
        myLast = N + 1;
        for (int j = N; j >= 0; j--)
        {
            if (F[i][j] == 1)
            {
                start = j - 1;
                myLast = N + 1;
            }
            else
            {
                right2[i][j] = right2[i][j + 1] - (start - j) * (myLast - i);
                if (myLast <= last2[j])
                {
                    if (myLast == last2[j])
                    {
                        right2[i][j] = std::max(right2[i][j], mid2[myLast][start + 1]);
                    }
                    else
                    {
                        right2[i][j] = std::max(right2[i][j], right2[myLast][j]);
                    }
                }
                else
                {
                    myLast = last2[j];
                    right2[i][j] = left2[myLast][start];
                }
                right2[i][j] += (start - j + 1) * (myLast - i);
            }
        }

        for (int j = 0; j <= N + 1; j++)
            if (F[i][j] == 1)
                last2[j] = i;
    }

    return ans;
}

int biggest_stadium(int N, std::vector<std::vector<int>> FF)
{
    int a = biggest_stadium_wd(N, FF);
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            std::swap(FF[i][j], FF[j][i]);
        }
    }
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N - j - 1; ++j)
        {
            std::swap(FF[i][j], FF[i][N - j - 1]);
        }
    }
    int b = biggest_stadium_wd(N, FF);
    return std::min(a, b);
}
