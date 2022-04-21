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

    continue_answer(mp(0, 0));
    for (int i = 0; i < n; i++) {
        continue_answer(input[i]);
    }

    finalize();

    return 0;
}
