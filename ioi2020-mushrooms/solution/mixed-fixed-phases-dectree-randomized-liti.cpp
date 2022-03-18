/*
 * Optimal Semi-Randomized solution for task mushrooms
 * Multiple heuristics included in the first phase
 * Including a decision tree to determine what to do based on the result of
 * {a, 0, b, 0, c, 0, d, 0, e} 
 * Decision tree is calculated in a manner that for each answer to the above query
 * Hold all different cases of a..e and see which query makes distinct answers
 * 
 * Note that randomized solutions won't help a lot because grader is adaptive
 *
 * Author: Ali Behjati
 */
#include "mushrooms.h"
#include <iostream>
#include <cassert>
#include <map>

using namespace std;
const int N = 20000 + 100;

vector<map<int,int>> dec_tree = {
    {{2, 2},
        {1, 4},
        {0, 8},
        {3, 17}
    },
    {{4, 3},
        {6, 5},
        {2, 9},
        {5, 18},
        {3, 20},
        {1, 24}
    },
    {{1, 6},
        {3, 10},
        {2, 12},
        {5, 19},
        {6, 21},
        {4, 25}
    },
    {{1, 7},
        {3, 11},
        {5, 13},
        {2, 22},
        {6, 26},
        {4, 28}
    },
    {{3, 14},
        {1, 23},
        {2, 27},
        {4, 29}
    }};

vector<vector<int>> dec_ques = {
    { 0, 1, 4, 2 },
    { 0, 1, 2, 4, -1, 3, -1 },
    { 1, 2, 3, 0, -2, 4, -2 },
    { 0, 1, 2, 4, -2, 3, -2 },
    { 0, 1, 2, -2, 3 }
};

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


    int k = 100;
    int t = (detected[1].size() == 2);
    int unknown = -1;  
    for(; i < n && sz() < k && i < 2*k ; i += 2) {
        unknown = -1;
        int res;
        if (i+4 < n && sz() >= 4 && detected[0].size() >= 2 && detected[1].size() >= 2) {
            int t = (detected[1].size() == sz());
            int res = _use_machine({i, detected[t][0], i+1, detected[t][1], i+2, detected[t][2], i+3, detected[t][3], i+4});
            if (res ==0) {
                push(t, i);
                push(t, i+1);
                push(t, i+2);
                push(t, i+3);
                push(t, i+4);
                i += 3;
            } else if (res == 8) {
                push(t^1, i);
                push(t^1, i+1);
                push(t^1, i+2);
                push(t^1, i+3);
                push(t^1, i+4);
                i += 3;
            } else if (res == 1) {
                push(t, i+1);
                push(t, i+2);
                push(t, i+3);
                chain[i+4].push_back(-i);
                i += 2;
            } else if (res == 7) {
                push(t^1, i+1);
                push(t^1, i+2);
                push(t^1, i+3);
                chain[i+4].push_back(-i);
                i += 2;
            } else { 
                assert(2 <= res && res <= 6);
                res -= 2;

                vector<int> ask;
                int ptr[2] = {0,0};
                for(auto x: dec_ques[res]) {
                    if (x >= 0)
                        ask.push_back(i+x);
                    else {
                        int tpe = (x+2) ^ t;
                        ask.push_back(detected[tpe][ptr[tpe]++]);
                    }
                }

                int new_res = _use_machine(ask);
                int mask = dec_tree[res][new_res];

                for(int j = 0; j < 5; j++) 
                    push(((mask>>j) & 1)^t, i+j);
                i += 3;
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

    assert( asked < 2 * k ); 

    int more_zeros = 0;

    for(; i < n; ) {
        int size = sz();
        int t = (detected[1].size() == sz()); 
        int rem = min(size, n - i); 

        vector<int> tmp;
        for(int j = 0; j < rem; j++) {
            tmp.push_back(i+j); 
            tmp.push_back(detected[t][j]);
        }

        int res = _use_machine(tmp);

        more_zeros += (t ? res / 2 : rem - 1 - res / 2);

        int not_t = (res & 1);
        push(t ^ not_t, i);

        i += rem; 
    }

    return detected[0].size() + more_zeros;
}
