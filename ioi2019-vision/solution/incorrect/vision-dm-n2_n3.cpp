#include "vision.h"

using namespace std;

inline void try_append_gate(vector<int> &gates, int x, int y, int W, int H)
{
    if (x >= 0 && y >= 0 && x < W && y < H)
    {
        gates.push_back(y * W + x);
    }
}

void construct_network(int H, int W, int K)
{
    vector<int> global;
    for (int x = 0; x < W; x++)
    {
        for (int y = 0; y < H; y++)
        {
            vector<int> gates;
            int dx = 0, dy = -K;
            int ddx = 1, ddy = 1;
            for (int step = 0; step <= 4 * K; step++)
            {
                try_append_gate(gates, x + dx, y + dy, W, H);
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
            if (gates.size() > 0)
            {
                global.push_back(add_and({y * W + x, add_or(gates)}));
            }
        }
    }
    add_or(global);
}