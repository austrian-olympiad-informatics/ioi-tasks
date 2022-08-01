#include "vision.h"

using namespace std;

void construct_network(int H, int W, int K)
{
    int x = K;
    int y = 0;
    vector<int> inputs;
    while (x >= 0)
    {
        if (x < W && y < H)
        {
            inputs.push_back(y * W + x);
        }
        x--;
        y++;
    }
    add_or(inputs);
}