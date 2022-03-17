#include <bits/stdc++.h>
#include "registers.h"
using namespace std;

const int b = 2000;
void construct_instructions(int s, int n, int k, int q) {
    vector<bool> v(b,0);
    v[0] = 1;
    append_store(3, v);
    vector<bool> all_ones(b,1);
    append_store(4, all_ones);
    vector<bool> last_k(b,0);
    for(int i=0; i<k; i++) {
        last_k[i] = 1;
    }
    append_store(5, last_k);

    append_right(1, 0, k);


    append_not(8, 1);
    append_and(8, 8, 5);
    append_and(0, 0, 5);
    append_add(2, 0, 8);
    append_right(2, 2, k);

    append_and(2, 2, 3); // 2 should be 0...01 or 0....00
    append_add(2, 2, 4); // now 2 is 0....00 or 1....11
    append_and(6,2,5);
    append_and(6,6,0);
    append_not(7,2);
    append_and(7,7,5);
    append_and(7,1,7);
    append_or(0,6,7);

    return;
}
