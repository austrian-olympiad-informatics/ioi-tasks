#include "vision.h"

using namespace std;

inline int num(int x, int y, int W)
{
    return y * W + x;
}

vector<int> get_diagonal(int H, int W, int N, bool D)
{
    vector<int> cells;
    int first, last, step;
    if (D)
    {
        first = N < W ? N : W * (N - W + 2) - 1;
        last = N < H ? N * W : (H - 1) * W + (N - H + 1);
        step = W - 1;
    }
    else
    {
        first = N < W ? W - 1 - N : W * (N - W + 1);
        last = N < H ? (N + 1) * W - 1 : H * W + (H - 2 - N);
        step = W + 1;
    }
    int cur = first;
    while (true)
    {
        cells.emplace_back(cur);
        if (cur == last)
        {
            break;
        }
        cur += step;
    }
    return cells;
}

void construct_network(int H, int W, int K)
{
    vector<int> _01 = {0, add_not(0)};

    vector<int> rows;
    for (int y = 0; y < H - 1; y++)
    {
        vector<int> gates;
        for (int x = 0; x < W; x++)
        {
            gates.push_back(num(x, y, W));
            gates.push_back(num(x, y + 1, W));
        }
        rows.push_back(add_and({add_or(gates), add_not(add_xor(gates))}));
    }
    int row = add_or(H > 1 ? rows : _01);

    vector<int> cols;
    for (int x = 0; x < W - 1; x++)
    {
        vector<int> gates;
        for (int y = 0; y < H; y++)
        {
            gates.push_back(num(x, y, W));
            gates.push_back(num(x + 1, y, W));
        }
        cols.push_back(add_and({add_or(gates), add_not(add_xor(gates))}));
    }
    int col = add_or(W > 1 ? cols : _01);

    vector<int> diags;
    for (int direction = 0; direction < 2; direction++)
    {
        for (int diagonal = 0; diagonal < H + W - 1; diagonal++)
        {
            vector<int> gates = get_diagonal(H, W, diagonal, direction == 0);
            diags.push_back(add_and({add_or(gates), add_not(add_xor(gates))}));
        }
    }
    int diag = add_or(diags);

    add_and({row, col, add_not(diag)});
}