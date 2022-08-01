#include "vision.h"

using namespace std;

vector<int> diagonal(int H, int W, int N, bool D)
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

int bottom_right_position(int H, int W)
{
    vector<int> lastRow(W, 0);
    vector<int> conditions;
    for (int y = 0; y < H; y++)
    {
        for (int x = 0; x < W; x++)
        {
            vector<int> gates;
            if (x > 0)
            {
                gates.push_back(lastRow[x - 1]);
            }
            if (y > 0)
            {
                gates.push_back(lastRow[x]);
            }
            int gate = W * y + x;
            if (gates.size() > 0)
            {
                conditions.push_back(add_and({gate, add_or(gates)}));
            }
            gates.push_back(gate);
            lastRow[x] = add_or(gates);
        }
    }
    return add_or(conditions);
}

void construct_network(int H, int W, int K)
{
    vector<int> topRight, topLeft;
    for (int i = 0; i + K < H + W - 1; i++)
    {
        topRight.push_back(add_and({add_or(diagonal(H, W, i, true)), add_or(diagonal(H, W, i + K, true))}));
        topLeft.push_back(add_and({add_or(diagonal(H, W, i, false)), add_or(diagonal(H, W, i + K, false))}));
    }
    int bottomRightPosition = bottom_right_position(H, W);
    add_or({
        add_and({add_or(topRight), bottomRightPosition}),
        add_and({add_or(topLeft), add_not(bottomRightPosition)})
    });
}