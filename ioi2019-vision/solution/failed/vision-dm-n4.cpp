#include "vision.h"

using namespace std;

void construct_network(int H, int W, int K)
{
    vector<int> v;
    for (int x1 = 0; x1 < W; x1++) {
        for (int y1 = 0; y1 < H; y1++) {
            for (int x2 = 0; x2 < W; x2++) {
                for (int y2 = 0; y2 < H; y2++) {
                    if (abs(x1 - x2) + abs(y1 - y2) == K) {
                        v.push_back(add_and({y1 * W + x1, y2 * W + x2}));
                    }
                }
            }
        }
    }
    add_or(v);
}