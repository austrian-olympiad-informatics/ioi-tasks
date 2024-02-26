// ##################################
// ##                              ##
// ##   Solution for Subtask B6    ##
// ##        (100.00 pts)          ##
// ##                              ##
// ##################################

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

#include "soccer.h"

using namespace std;

class RangeMin
{
public:
    int size_ = 1;
    vector<int> dat;

    void init(int sz)
    {
        while (size_ <= sz)
            size_ *= 2;
        dat.resize(size_ * 2, 0);
    }
    void update(int pos, int x)
    {
        pos += size_;
        dat[pos] = x;
        while (pos >= 2)
        {
            pos >>= 1;
            dat[pos] = min(dat[pos * 2], dat[pos * 2 + 1]);
        }
    }
    int query_(int l, int r, int a, int b, int u)
    {
        if (r <= a || b <= l)
            return 1000000000;
        if (l <= a && b <= r)
            return dat[u];
        int v1 = query_(l, r, a, ((a + b) >> 1), u * 2);
        int v2 = query_(l, r, ((a + b) >> 1), b, u * 2 + 1);
        return min(v1, v2);
    }
    int query(int l, int r)
    {
        return query_(l, r, 0, size_, 1);
    }
};

struct Rectangle
{
    int xl, xr, yl, yr;
};

bool operator<(const Rectangle &a1, const Rectangle &a2)
{
    if (a1.xr - a1.xl < a2.xr - a2.xl)
        return true;
    if (a1.xr - a1.xl > a2.xr - a2.xl)
        return false;
    if (a1.xl < a2.xl)
        return true;
    if (a1.xl > a2.xl)
        return false;
    if (a1.xr < a2.xr)
        return true;
    if (a1.xr > a2.xr)
        return false;
    if (a1.yl < a2.yl)
        return true;
    if (a1.yl > a2.yl)
        return false;
    if (a1.yr < a2.yl)
        return true;
    return false;
}

bool operator==(const Rectangle &a1, const Rectangle &a2)
{
    if (a1.xl == a2.xl && a1.xr == a2.xr && a1.yl == a2.yl && a1.yr == a2.yr)
        return true;
    return false;
}

const int MAXN = 3209;

// Variables for Rectangles & DP
int NumRects = 0;
Rectangle Rect[MAXN * MAXN];
vector<int> Next[MAXN * MAXN];
int dp[MAXN * MAXN];

// Other Variables
int EmptyUp[MAXN][MAXN];
int EmptyDn[MAXN][MAXN];
RangeMin UpMax[MAXN];
RangeMin DnMin[MAXN];
vector<int> List[MAXN];

// Enumerate Rectangles
void Enumerate(int N, int bottom, vector<int> Up, vector<int> Dn)
{
    stack<pair<int, int>> Stack;
    Stack.push(make_pair(-1, bottom + 2));
    Up.push_back(bottom);

    // Start Simulation
    int LatestBottom = -1;
    for (int i = 0; i <= N; i++)
    {
        while (Stack.top().second <= Up[i])
        {
            int val = Stack.top().second;
            Stack.pop();
            if (val == Up[i])
                continue;

            // Add Rectangle
            Rectangle New;
            New.yl = Stack.top().first + 1;
            New.yr = i - 1;
            New.xl = val + 1;
            New.xr = bottom;
            if (New.yl <= LatestBottom)
            {
                Rect[NumRects] = New;
                NumRects += 1;
            }
        }
        if (i == N)
            break;

        // Add Stack & Update LatestBottom
        Stack.push(make_pair(i, Up[i]));
        if (Dn[i] == bottom + 1)
            LatestBottom = i;
    }
}

void add_edge_real(int N, int idx, vector<int> CutPoint)
{
    for (int j = 0; j < (int)CutPoint.size() - 1; j++)
    {
        if (CutPoint[j + 1] - CutPoint[j] <= 1)
            continue;
        Rectangle New;
        New.yl = CutPoint[j + 0] + 1;
        New.yr = CutPoint[j + 1] - 1;
        New.xl = 0;
        New.xr = N - 1;
        if (Rect[idx].xl >= 1)
            New.xl = -UpMax[Rect[idx].xl - 1].query(New.yl, New.yr + 1) + 1;
        if (Rect[idx].xr <= N - 2)
            New.xr = DnMin[Rect[idx].xr + 1].query(New.yl, New.yr + 1) - 1;
        int pos1 = lower_bound(Rect, Rect + NumRects, New) - Rect;
        if (pos1 < NumRects && Rect[pos1] == New)
        {
            Next[idx].push_back(pos1);
        }
    }
}

// Add Edges from rectangle idx
void add_edge(int N, int idx)
{
    // Part 1
    if (Rect[idx].xl >= 1)
    {
        int BlockPlace = Rect[idx].xl - 1;
        vector<int> UpList;
        UpList.push_back(Rect[idx].yl - 1);
        int pos = lower_bound(List[BlockPlace].begin(), List[BlockPlace].end(), Rect[idx].yl) - List[BlockPlace].begin();
        while (pos < (int)List[BlockPlace].size() && List[BlockPlace][pos] <= Rect[idx].yr)
        {
            UpList.push_back(List[BlockPlace][pos]);
            pos += 1;
        }
        UpList.push_back(Rect[idx].yr + 1);
        add_edge_real(N, idx, UpList);
    }

    // Part 2
    if (Rect[idx].xr <= N - 2)
    {
        int BlockPlace = Rect[idx].xr + 1;
        vector<int> DnList;
        DnList.push_back(Rect[idx].yl - 1);
        int pos = lower_bound(List[BlockPlace].begin(), List[BlockPlace].end(), Rect[idx].yl) - List[BlockPlace].begin();
        while (pos < (int)List[BlockPlace].size() && List[BlockPlace][pos] <= Rect[idx].yr)
        {
            DnList.push_back(List[BlockPlace][pos]);
            pos += 1;
        }
        DnList.push_back(Rect[idx].yr + 1);
        add_edge_real(N, idx, DnList);
    }
}

int biggest_stadium(int N, vector<vector<int>> C)
{
    // [1] Calculate EmptyUp, EmptyDn
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (C[i][j] == 1)
                EmptyUp[i][j] = i;
            else if (i == 0)
                EmptyUp[i][j] = -1;
            else
                EmptyUp[i][j] = EmptyUp[i - 1][j];
        }
    }
    for (int i = N - 1; i >= 0; i--)
    {
        for (int j = 0; j < N; j++)
        {
            if (C[i][j] == 1)
                EmptyDn[i][j] = i;
            else if (i == N - 1)
                EmptyDn[i][j] = N;
            else
                EmptyDn[i][j] = EmptyDn[i + 1][j];
        }
    }

    // [2] Enumerate Rectangle
    for (int i = 0; i < N; i++)
    {
        vector<int> Up(N, -1), Dn(N, N);
        for (int j = 0; j < N; j++)
            Up[j] = EmptyUp[i][j];
        if (i < N - 1)
        {
            for (int j = 0; j < N; j++)
                Dn[j] = EmptyDn[i + 1][j];
        }
        Enumerate(N, i, Up, Dn);
    }
    sort(Rect, Rect + NumRects); // sort by xr-xl

    // [3] Prepare SegmentTree
    for (int i = 0; i < N; i++)
    {
        UpMax[i].init(N + 1);
        DnMin[i].init(N + 1);
        for (int j = 0; j < N; j++)
        {
            UpMax[i].update(j, -EmptyUp[i][j]);
            DnMin[i].update(j, EmptyDn[i][j]);
        }
    }

    // [4] Add "Edges" on graph
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (C[i][j] == 1)
                List[i].push_back(j);
        }
    }
    for (int i = 0; i < NumRects; i++)
    {
        add_edge(N, i);
    }

    // [5] Dynamic Programming
    for (int i = 0; i < NumRects; i++)
    {
        dp[i] = max(dp[i], (Rect[i].xr - Rect[i].xl + 1) * (Rect[i].yr - Rect[i].yl + 1));
        for (int to : Next[i])
        {
            int AddArea_X = (Rect[to].xr - Rect[to].xl + 1) - (Rect[i].xr - Rect[i].xl + 1);
            int AddArea_Y = (Rect[to].yr - Rect[to].yl + 1);
            dp[to] = max(dp[to], dp[i] + AddArea_X * AddArea_Y);
        }
    }

    // [6] Return Answer
    int Answer = 0;
    for (int i = 0; i < NumRects; i++)
        Answer = max(Answer, dp[i]);
    return Answer;
}