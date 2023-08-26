#include "validate.h"

#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef long double ld;

int main(int argc, char **argv) {
    init_io(argc, argv);

    int n;
    judge_in >> n;

    vector<vector<int> > OK(n, vector<int>(n, 0));

    for(int c1 = 1; c1 < n; c1++){
        for(int c2 = 0; c2 < c1; c2++){
            int a;
            judge_in >> a;
            if(2*c2 < c1){
                OK[a][c1] = 1;
                OK[c1][a] = 1;
            }
        }
    }
    vector<int> ans, ans_sorted;
    for(int c1 = 0; c1 < n; c1++){
        int a;
        if(!(author_out >> a)){
            wrong_answer("Could not read %d:th number", c1+1);
        }
        ans.push_back(a);
        ans_sorted.push_back(a);
    }
    sort(all(ans_sorted));
    for(int c1 = 0; c1 < n; c1++){
        if(ans_sorted[c1] != c1){
            wrong_answer("Person %d missing from ranking", c1+1);
        }
    }

    for(int c1 = 0; c1 < n-1; c1++){
        if(!OK[ans[c1]][ans[c1+1]]){
            wrong_answer("Enemies %d and %d are next to each other", ans[c1]+1, ans[c1+1]+1);
        }
    }

    string garbage;
    if(author_out >> garbage) {
        wrong_answer("trailing output, expected eof");
    }

    accept();
}
