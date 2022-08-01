#include "vision.h"

using namespace std;

void construct_network(int H, int W, int K)
{
    int N = H > W ? H : W;
    vector<int> gates;
    for (int i = 0; i + K < N; i++) {
        gates.push_back(add_and({i, i + K}));
    }
    add_or(gates);
}