#include "soccer.h"
#include <iostream>
#include <array>
#include <map>
#include <algorithm>
#include <cassert>
#define xx first
#define yy second

using namespace std;
typedef pair<int, int> pii;

int biggest_stadium(int N, vector<vector<int>> C)
{
    int x = 1, y = 1;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (C[i][j])
                x = i, y = j;
        }
    }
    int ans = 0;
    ans = max(ans, N * N - (x + 1) * (y + 1));
    ans = max(ans, N * N - (N - x) * (y + 1));
    ans = max(ans, N * N - (x + 1) * (N - y));
    ans = max(ans, N * N - (N - x) * (N - y));
    return ans;
}
