#include "vision.h"

using namespace std;

void construct_network(int H, int W, int K)
{
    vector<int> v;
    for (int x = 0; x < W; x++) {
        for (int y = 0; y < H; y++) {
            if (x + K < W) {
                v.push_back(add_and({y * W + x, y * W + (x + K)}));
            }
            if (y + K < H) {
                v.push_back(add_and({y * W + x, (y + K) * W + x}));
            }
        }
    }
    add_or(v);
}