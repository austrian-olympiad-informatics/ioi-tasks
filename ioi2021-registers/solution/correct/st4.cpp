#include <bits/stdc++.h>
#include "registers.h"
using namespace std;

const int b = 2000;
void construct_instructions(int s, int n, int k, int q) {
    vector<bool> v(b,0);
    for(int i=0; i<b; i++) {
        if(i%(2*k)==0) {
            v[i] = 1;
        }
    }
    append_store(3, v);
    vector<bool> mask_ones(b);
    for(int i=0; i<b; i++) {
        if((i/k)%2==0) {
            mask_ones[i] = 1;
        }
    }
    append_store(4, mask_ones);
    vector<bool> fill_up(b);
    for(int i=n*k; i<b; i++) {
        // ensure that we do not
        // accidentally take min with 0
        fill_up[i] = 1;
    }
    append_store(5, fill_up);
    append_or(0, 0, 5);
    int iter = 1 + (int)log2(n-0.001); // ceil(log2(n))
    int a = k;
    while(iter--) {
        append_right(1, 0, a); // we need to shift twice as many bits each time
        a *= 2;
        append_not(8, 1);
        append_and(8, 8, 4);
        append_and(0, 0, 4);
        append_add(2, 0, 8);
        append_right(2, 2, k); // always k bits, do not change

        append_and(2, 2, 3); // 2 should be 0...01 or 0....00
        append_add(2, 2, 4); // now 2 is 0....00 or 1....11
        append_and(6,2,0);
        append_not(7,2);
        append_and(7,1,7);
        append_or(0,6,7);
        n = n/2;
    }

    // so now we have min(a0,a1), blank, min(a2,a3), blank etc


    return;
}
