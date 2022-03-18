/*
 * Near-optimal randomized solution for task mushrooms
 * 
 * Author: Ali Behjati
 */
#include "mushrooms.h"
#include <iostream>
#include <cassert>

using namespace std;
const int N = 20000 + 100;

vector<int> detected[2];
vector<int> chain[N]; 

void push(int t, int i) {
    detected[t].push_back(i);
    for (auto x : chain[i]) 
        if( x < 0 ) 
            push( t ^ 1 , -x );
        else
            push( t , x ); 
    chain[i].clear();
}

int sz() {
    return max(detected[0].size(), detected[1].size());
}

int perm[N];

int asked = 0;

int _use_machine(vector<int> a) { 
    asked++;
    vector<int> b;
    for(auto x: a)
        b.push_back(perm[x]); 
    return use_machine(b);
}


int count_mushrooms(int n) {
    detected[0].push_back(0);

    srand( 11 );
    for(int i = 1; i < n; i++) {
        perm[i] = i; 
        swap( perm[ rand() % i + 1 ] , perm[i] ); 
    }

    int i;
    for(i = 1; i < n && sz() < 2; i++) {
        int res = _use_machine({0, i});
        detected[res].push_back(i); 
    }


    int k = 140;
    int t = (detected[1].size() == 2);
    int unknown = -1;  
    for(; i < n && sz() < k && i < 2 * k ; i += 2) {
        unknown = -1;
        int res;
        if (i+2 < n) {
            res = _use_machine({i, detected[t][0], i+1, detected[t][1], i+2});
            if (res == 4) {
                push(t^1, i);
                push(t^1, i+1);
                push(t^1, i+2);
                i++;
            } else if (res == 0) {
                push(t, i);
                push(t, i+1);
                push(t, i+2);
                i++; 
            } else if (res == 2) { 
                chain[i+2].push_back(i);
                chain[i+2].push_back(-(i+1));
                unknown = i+2;
            } else if (res == 3) { 
                push(t^1, i+1);
                chain[i+2].push_back(-i);
                unknown = i+2;
            } else { 
                push(t, i+1); 
                chain[i+2].push_back(-i);
                unknown = i+2;
            }
        } else {
            if(i+1 < n)
                res = _use_machine({detected[t][0], i, detected[t][1], i+1});
            else
                res = _use_machine({detected[t][0], i, detected[t][1]});

            int not_t = (res & 2) ? 1 : 0;
            push(t ^ not_t, i);

            if(i+1 < n) {
                not_t = res & 1; 
                push(t ^ not_t, i+1);
            }
        }
    }

    if( unknown != -1 ) {
        int res = _use_machine({detected[t][0], unknown});
        push( t ^ res , unknown );
        i++; 
    }

    assert( asked < 2 * k ); 

    int more_zeros = 0;

    for(; i < n; ) {
        int size = sz();
        int t = (detected[1].size() == sz()); 
        int rem = min(size, n - i); 

        vector<int> tmp;
        for(int j = 0; j < rem; j++) {
            tmp.push_back(detected[t][j]);
            tmp.push_back(i+j); 
        }

        int res = _use_machine(tmp);
        
        if (res % 2) res++;

        more_zeros += (t ? res / 2 : rem - res / 2);

        i += rem; 
    }

    return detected[0].size() + more_zeros;
}
