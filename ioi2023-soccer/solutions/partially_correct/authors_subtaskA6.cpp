// ##################################
// ##                              ##
// ##   Solution for Subtask A6    ##
// ##         (25.00 pts)          ##
// ##                              ##
// ##################################

#include <iostream>
#include <vector>
#include <algorithm>

#include "soccer.h"

using namespace std;

int biggest_stadium(int N, vector<vector<int>> C)
{
    vector<int> l(N, N);
    vector<int> r(N, -1);
    vector<int> h(N, 0);

    // Condition 1: All rows are interval
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (C[i][j] == 1)
                continue;
            l[i] = min(l[i], j);
            r[i] = max(r[i], j);
        }
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = l[i]; j <= r[i]; j++)
        {
            if (C[i][j] == 1)
                return -1; // Failed
        }
    }

    // Condition 2: All two ranges are "overlap"
    for (int i = 0; i < N; i++)
    {
        for (int j = i + 1; j < N; j++)
        {
            bool flag = false;
            if (l[i] <= l[j] && r[j] <= r[i])
                flag = true;
            if (l[j] <= l[i] && r[i] <= r[j])
                flag = true;
            if (flag == false)
                return -1; // Failed
        }
    }

    // Condition 3: Histogram of empty cells is "mountain"
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (C[i][j] == 0)
                h[i] += 1;
        }
    }
    int Max_Value = -1, Max_Index = -1;
    for (int i = 0; i < N; i++)
    {
        if (Max_Value < h[i])
        {
            Max_Value = h[i];
            Max_Index = i;
        }
    }
    for (int i = Max_Index - 1; i >= 0; i--)
    {
        if (h[i] > h[i + 1])
            return -1; // Failed
    }
    for (int i = Max_Index + 1; i < N; i++)
    {
        if (h[i - 1] < h[i])
            return -1; // Failed
    }

    // Success: If entire empty cells are regular
    int cnt = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (C[i][j] == 0)
                cnt++;
        }
    }
    return cnt;
}