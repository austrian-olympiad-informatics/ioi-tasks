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
    int cnt = N * N, ans = 0;
    for (int i = 1; i < (1 << cnt); i++)
    {
        bool ok = true;

        int col_cnt = 0,
            col_mn = 100,
            col_mx = -1;

        int chosen_cells = 0;

        std::vector<std::pair<int, int>> rows;

        for (int x = 0; x < N && ok; x++)
        {
            int row_cnt = 0,
                row_mn = 100,
                row_mx = -1;
            for (int y = 0; y < N && ok; y++)
            {
                int ind = x * N + y;
                if (i >> ind & 1)
                {
                    chosen_cells++;

                    ok &= C[x][y] == 0;
                    row_cnt++;
                    row_mn = std::min(row_mn, y);
                    row_mx = std::max(row_mx, y);
                }
            }

            if (row_cnt > 0)
            {
                rows.push_back({row_mn, row_mx});

                ok &= row_cnt == row_mx - row_mn + 1;
                col_cnt++;
                col_mn = std::min(col_mn, x);
                col_mx = std::max(col_mx, x);
            }
        }

        ok &= col_cnt == col_mx - col_mn + 1;
        ok &= is_unimodal(rows);
        ok &= is_subintervaled(rows);

        if (ok)
        {
            ans = std::max(ans, chosen_cells);
#ifdef DEBUG
            if (ans == chosen_cells)
            {
                for (int x = 0; x < N; x++)
                {
                    for (int y = 0; y < N; y++)
                    {
                        int ind = x * N + y;
                        if (i >> ind & 1)
                        {
                            std::cerr << "1 ";
                        }
                        else
                        {
                            std::cerr << "0 ";
                        }
                    }
                    std::cerr << "\n";
                }
                std::cerr << "\n";
            }
#endif
        }
    }

    return ans;
}
