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
    vector<int> emptyvec(N, 0);
    vector<vector<int>> emptymat(N, emptyvec);
    if (C == emptymat)
    {
        return N * N;
    }
    return C[0][0] || C[0].back() || C.back()[0] || C.back().back() ? N * N - 1 : 0;
}
