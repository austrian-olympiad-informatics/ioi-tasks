#include "soccer.h"

#define xx first
#define yy second

bool is_unimodal(std::vector<std::pair<int, int>> &ivs)
{
    bool res = true;

    int state = 0;
    for (int i = 1; i < (int)ivs.size();)
    {
        int prev_len = ivs[i - 1].yy - ivs[i - 1].xx,
            curr_len = ivs[i].yy - ivs[i].xx;

        if (state == 0)
        {
            if (prev_len <= curr_len)
            {
                i++;
            }
            else
            {
                state = 1;
            }
        }
        else
        {
            if (prev_len >= curr_len)
            {
                i++;
            }
            else
            {
                res = false;
                break;
            }
        }
    }

    return res;
}

bool is_subintervaled(std::vector<std::pair<int, int>> &ivs)
{
    bool res = true;
    for (const std::pair<int, int> &p : ivs)
    {
        for (const std::pair<int, int> &q : ivs)
        {
            res &= (p.xx <= q.xx && q.yy <= p.yy) || (q.xx <= p.xx && p.yy <= q.yy);
        }
    }

    return res;
}

int biggest_stadium(int N, std::vector<std::vector<int>> C)
{
    int cnt = 0, lens = 0;
    std::vector<std::pair<int, int>> ivs;
    int row_mn = 1e9, row_mx = -1e9;
    for (int i = 0; i < N; ++i)
    {
        int mn = 1e9, mx = -1e9;
        for (int j = 0; j < N; ++j)
        {
            cnt += C[i][j] == 0;
            if (C[i][j] == 0)
            {
                mn = std::min(mn, j);
                mx = std::max(mx, j);
            }
        }

        if (mn <= mx)
        {
            ivs.push_back({mn, mx});
            lens += mx - mn + 1;

            row_mn = std::min(row_mn, i);
            row_mx = std::max(row_mx, i);
        }
    }

    if (lens != cnt || (int)ivs.size() != row_mx - row_mn + 1)
        return 1e9; // has holes
    return is_unimodal(ivs) && is_subintervaled(ivs) ? cnt : (int)1e9;
}
