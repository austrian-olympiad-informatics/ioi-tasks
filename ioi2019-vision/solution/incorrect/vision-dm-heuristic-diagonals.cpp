#include "vision.h"

using namespace std;

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
    vector<int> v;
    for (int direction = 0; direction < 2; direction++) {
        for (int diagonal = K; diagonal < H + W - 1; diagonal++) {
            v.push_back(add_and({
                add_or(get_diagonal(H, W, diagonal - K, direction == 0)),
                add_or(get_diagonal(H, W, diagonal, direction == 0))
            }));
        }
    }
    add_or(v);
}