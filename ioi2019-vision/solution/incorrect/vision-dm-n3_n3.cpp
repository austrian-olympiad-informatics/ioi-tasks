#include "vision.h"

using namespace std;

inline void try_append_gate(vector<int> &gates, int x1, int y1, int x2, int y2, int W, int H)
{
    if (x2 >= 0 && y2 >= 0 && x2 < W && y2 < H)
    {
        gates.push_back(add_and({y1 * W + x1, y2 * W + x2}));
    }
}

void construct_network(int H, int W, int K)
{
    vector<int> gates;
    for (int x = 0; x < W; x++)
    {
        for (int y = 0; y < H; y++)
        {
            int dx = 0, dy = -K;
            int ddx = 1, ddy = 1;
            for (int step = 0; step <= 4 * K; step++)
            {
                try_append_gate(gates, x, y, x + dx, y + dy, W, H);
                if (step == K)
                {
                    ddx = -1;
                }
                if (step == 2 * K)
                {
                    ddy = -1;
                }
                if (step == 3 * K)
                {
                    ddx = 1;
                }
                dx += ddx;
                dy += ddy;
            }
        }
    }
    add_or(gates);
}