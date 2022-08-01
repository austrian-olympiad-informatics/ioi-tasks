#include "vision.h"

using namespace std;

void construct_network(int H, int W, int K)
{
    vector<int> v = {0, 1};
    if (K != 1) {
        v.push_back(2);
    }
    add_and(v);
}