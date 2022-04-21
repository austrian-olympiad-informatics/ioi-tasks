#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <string>
#include <cmath>
#include <cassert>
#include <ctime>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

using namespace std;

#define pb push_back
#define mp make_pair
#define fs first
#define sc second

int n;
vector <pair <int, int> > input;
vector <pair <int, int> > answer;

void continue_answer(pair <int, int> point) {
    if (answer.empty()) {
        answer.pb(point);

        return;
    }

    if (point.fs != answer.back().fs) {
        answer.pb(mp(point.fs, answer.back().sc));
    }
    if (point.sc != answer.back().sc) {
        answer.pb(point);
    }
}

void add_spiral(vector <pair <pair <int, int>, pair <int, int> > > spiral) {
    for (int i = 0; i < (int) spiral.size(); i++) {
        int lft = spiral[i].fs.fs;
        int up = spiral[i].fs.sc;
        int rgt = spiral[i].sc.fs;
        int down = spiral[i].sc.sc;

        pair <int, int> ldc = mp(input[lft].fs, input[down].sc);
        pair <int, int> luc = mp(input[lft].fs, input[up].sc);
        pair <int, int> rdc = mp(input[rgt].fs, input[down].sc);
        pair <int, int> ruc = mp(input[rgt].fs, input[up].sc);

        if (i == 0) {
            continue_answer(ldc);
            continue_answer(luc);
            continue_answer(ruc);
            continue_answer(rdc);
            continue_answer(ldc);
        } else {
            answer.back().fs = input[lft].fs;
            if (input[lft].sc >= answer.back().sc) {
                continue_answer(luc);
                continue_answer(ruc);
                continue_answer(rdc);
                continue_answer(ldc);
            } else {
                continue_answer(ldc);
                continue_answer(rdc);
                continue_answer(ruc);
                continue_answer(luc);
            }
        }
    }
}

void add_chain(vector <pair <int, int> > chain, bool order) {
    if (chain.empty())
        return;
    int len = (int) chain.size();
    if (order) {
        for (int i = 0; i < len; i++) {
            chain[i].sc *= -1;
        }
    }
    sort(chain.begin(), chain.end());
    if (order) {
        for (int i = 0; i < len; i++) {
            chain[i].sc *= -1;
        }
    }

    continue_answer(chain[0]);
    for (int i = 1; i < len; i++) {
        if (i % 2) {
            continue_answer(mp(chain[i].fs, chain[i - 1].sc));
        } else {
            continue_answer(mp(chain[i - 1].fs, chain[i].sc));
        }
    }
    if (len > 1) {
        continue_answer(chain.back());
    }
}

void finalize() {
    cout << answer.size()-1 << '\n';
    for (int i = 1; i < (int) answer.size(); i++) {
        cout << answer[i].fs << ' ' << answer[i].sc << '\n';
    }
}

int main () {
    int n;
    cin >> n;

    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        input.pb(mp(x, y));
    }

    vector <pair <int, int> > chain1, chain2;
    vector <pair <pair <int, int>, pair <int, int> > > spiral;
    set <pair <int, int> > xs;
    set <pair <int, int> > ys;
    vector <bool> used(n, false);

    for (int i = 0; i < n; i++) {
        xs.insert(mp(input[i].fs, i));
        ys.insert(mp(input[i].sc, i));
    }

    while (true) {
        int lft = -1;
        while (!xs.empty() && used[xs.begin()->sc]) {
            xs.erase(xs.begin());
        }
        if (!xs.empty()) {
            lft = xs.begin()->sc;
        }

        int rgt = -1;
        while (!xs.empty() && used[xs.rbegin()->sc]) {
            xs.erase(*xs.rbegin());
        }
        if (!xs.empty()) {
            rgt = xs.rbegin()->sc;
        }

        int down = -1;
        while (!ys.empty() && used[ys.begin()->sc]) {
            ys.erase(ys.begin());
        }
        if (!ys.empty()) {
            down = ys.begin()->sc;
        }

        int up = -1;
        while (!ys.empty() && used[ys.rbegin()->sc]) {
            ys.erase(*ys.rbegin());
        }
        if (!ys.empty()) {
            up = ys.rbegin()->sc;
        }

        if (lft == -1 || rgt == -1 || down == -1 || up == -1) {
            break;
        }

        if (lft == down) {
            chain1.pb(input[lft]);
            used[lft] = true;
            continue;
        }
        if (lft == up) {
            chain2.pb(input[lft]);
            used[lft] = true;
            continue;
        }
        if (rgt == down) {
            chain2.pb(input[rgt]);
            used[rgt] = true;
            continue;
        }
        if (rgt == up) {
            chain1.pb(input[rgt]);
            used[rgt] = true;
            continue;
        }
        spiral.pb(mp(mp(lft, up), mp(rgt, down)));
        used[lft] = true;
        used[rgt] = true;
        used[down] = true;
        used[up] = true;
    }

    reverse(spiral.begin(), spiral.end());
    continue_answer(mp(0, 0));
    add_spiral(spiral);
    add_chain(chain1, false);
    add_chain(chain2, true);

    finalize();

    return 0;
}
