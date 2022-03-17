#include "dungeons.h"
#include <vector>
#include <assert.h>
#include <cstdio>
#define get_phase(x) min((63-__builtin_clzll(x)), maxphase-1)
#define win(node, phase) (get_phase(s[node]) < phase)
#define draw(node, phase) (get_phase(s[node]) == phase)
#define loc_out(x) (win(x, phase) ? w[x] : l[x])
#define sz_out(x) (win(x, phase) ? s[x] : p[x])

using namespace std;

const int maxphase = 25; // number of phases
const int maxpath = 25; // log2(maximum path length)
const int maxn = 4e5+5;
const long long inf = 1LL<<60;
int n;
vector<long long> s,p;
vector<int>w,l;

long long sze1[maxphase][maxn];
long long mx1[maxphase][maxn];
int loc1[maxphase][maxn];

long long sze2[maxpath][maxn];
long long mx2[maxpath][maxn];
int loc2[maxpath][maxn];

void init(int _n, vector<int> _s, vector<int> _p, vector<int> _w, vector<int> _l) {
    n = _n; s = vector<long long>(begin(_s), end(_s)); p = vector<long long>(begin(_p), end(_p)); w = _w; l = _l;

    int dfs[n+1];
    int curr = 0;
    for(int phase=0; phase<maxphase; phase++) {
        // begin dfs to look for node with the correct phase
        for(int node=0; node<n; node++) {
            sze1[phase][node] = -1;
        }
        for(int node=0; node<n; node++) {
            if(sze1[phase][node] != -1) continue; // -1 for unvisited
            dfs[curr++] = node;
             do {
                sze1[phase][dfs[curr-1]] = -2;
                if(win(dfs[curr-1], phase)) {
                    dfs[curr] = w[dfs[curr-1]]; // win
                } else {
                    dfs[curr] = l[dfs[curr-1]]; // lose
                }
                curr++;
            } while(dfs[curr-1]!=n && sze1[phase][dfs[curr-1]] == -1 && !draw(dfs[curr-1], phase));

            if(dfs[curr-1]==n || draw(dfs[curr-1], phase)) {
                sze1[phase][dfs[curr-2]] = sz_out(dfs[curr-2]);
                mx1[phase][dfs[curr-2]] = win(dfs[curr-2], phase) ? inf : s[dfs[curr-2]];
                loc1[phase][dfs[curr-2]] = dfs[curr-1];
                for(int i=(int)curr-2; i>=1; i--) {
                    sze1[phase][dfs[i-1]] = min(sze1[phase][dfs[i]] + sz_out(dfs[i-1]), inf);
                    mx1[phase][dfs[i-1]] = min(win(dfs[i-1], phase) ? inf : s[dfs[i-1]],
                                               mx1[phase][dfs[i]] - sz_out(dfs[i-1]));
                    loc1[phase][dfs[i-1]] = loc1[phase][dfs[i]];
                }
            } else if(sze1[phase][dfs[curr-1]] >= 0) { // has a legit outgoing edge
                for(int i =(int) curr-1; i>=1; i--) {
                    sze1[phase][dfs[i-1]] = min(sze1[phase][dfs[i]] + sz_out(dfs[i-1]), inf);
                    mx1[phase][dfs[i-1]] = min(win(dfs[i-1], phase) ? inf : s[dfs[i-1]],
                                               mx1[phase][dfs[i]] - sz_out(dfs[i-1]));
                    loc1[phase][dfs[i-1]] = loc1[phase][dfs[i]];

                }
            }
            curr = 0;
        }
    }

    for(int node=0; node<n; node++) {
        sze2[0][node] = sze1[get_phase(s[node])][node];
        mx2[0][node] = mx1[get_phase(s[node])][node];
        loc2[0][node] = loc1[get_phase(s[node])][node];
    }

    sze2[0][n] = 0;
    mx2[0][n] = inf;
    loc2[0][n] = n;

    for(int len=1; len<maxpath; len++) {
        for(int node=0; node<=n; node++) {
            int l2 = loc2[len-1][node];
            if(l2!=-1) {
                sze2[len][node] = min(sze2[len-1][node] + sze2[len-1][l2],inf);
                mx2[len][node] = min(mx2[len-1][node],  mx2[len-1][l2]-sze2[len-1][node]);
                loc2[len][node] = loc2[len-1][l2];
            } else {
                sze2[len][node] = sze2[len-1][node];
                mx2[len][node] = mx2[len-1][node];
                loc2[len][node] = loc2[len-1][node];
            }
        }
    }
}

void step_forward(int &x, long long &z) {
    if(z >= s[x]) {
        z += s[x];
        x = w[x];
    } else {
        z += p[x];
        x = l[x];
    }
}

long long simulate(int x, int _z) {
    long long z = _z;
    int phase = get_phase(z);
    while(x<n) {
        assert(phase<maxphase);
        if(z < mx1[phase][x] && sze1[phase][x] != inf) {
            z += sze1[phase][x];
            x = loc1[phase][x];

            if(x==n) return z;
            for(int i=min(maxpath-1, 1+get_phase(s[x])); i>=0; i--) {
                if(z < mx2[i][x] && sze2[i][x] != inf) {
                    z += sze2[i][x];
                    x = loc2[i][x];
                }
            }
            if(x==n) return z;
            assert(z >= mx1[phase][x] || sze1[phase][x] == inf);
        }
        if (phase == get_phase(s[x])) {
            step_forward(x,z);
            phase = max(phase, get_phase(z));
        } else {
            phase++;
        }
    }
    return z;
}
