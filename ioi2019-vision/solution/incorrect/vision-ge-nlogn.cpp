#include "vision.h"

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

int const_one;
int const_zero;

int proxy_and(vector<int> nrs) {
    if (nrs.size() == 1u) {
        return nrs[0];
    } else {
        return add_and(nrs);
    }
}

int proxy_or(vector<int> nrs) {
    if (nrs.size() == 1u) {
        return nrs[0];
    } else {
        return add_or(nrs);
    }
}

int proxy_xor(vector<int> nrs) {
    if (nrs.size() == 1u) {
        return nrs[0];
    } else {
        return add_xor(nrs);
    }
}

vector <int> get_diag_ones(int n, int m, const vector<vector<int>>& idx) {
    vector <vector <int> > diags(n + m - 1);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            diags[i + j].pb(idx[i][j]);
        }

    vector <int> neurons;
    for (int i = 0; i < n + m - 1; i++) {
        neurons.pb(proxy_or(diags[i]));
    }

    return neurons;
}

vector <int> get_first_one(vector <int> ones) {
    vector <int> pref;
    pref.pb(add_not(ones[0]));

    for (int i = 1; i < (int) ones.size(); i++) {
        pref.pb(proxy_and({pref.back(), add_not(ones[i])}));
    }

    return pref;
}

vector <int> get_last_one(vector <int> ones) {
    vector <int> suf(ones.size());
    suf[(int) ones.size() - 1] = ones.back();

    for (int i = (int) ones.size() - 2; i >= 0; i--) {
        suf[i] = proxy_or({suf[i + 1], ones[i]});
    }

    return suf;
}

int differ_by_pw(vector <int> mn, vector <int> mx, int pw) {
    int len = (int) mn.size();
    vector <int> indicators;
    indicators.pb(proxy_and({add_not(mn[0]), mx[pw - 1]}));
    for (int i = 0; i < len - pw; i++) {
        indicators.pb(proxy_and({mn[i], add_not(mn[i + 1]), mx[i + pw]}));        
    }

    return proxy_or(indicators);
}

vector <int> conditional_add(vector <int> num, int val, int c) {
    int len = (int) num.size();
    vector <int> res;

    for (int i = 0; i < val; i++) {        
        res.pb(proxy_or({num[i], c}));
    }
    for (int i = val; i < len; i++) {
        res.pb(proxy_or({num[i], proxy_and({c, num[i - val]})}));
    }

    return res;
}

vector <int> get_abs_diff(vector <int> mn, vector <int> mx) {
    vector <int> res;
    for (int i = 0; i < (int) mn.size(); i++) {
        res.pb(const_zero);
    }
    
    int pw = 1;
    while (pw * 2 <= (int) mn.size()) {
        pw *= 2;
    }

    while (pw > 0) {
        int h = differ_by_pw(mn, mx, pw);

        mn = conditional_add(mn, pw, h);
        res = conditional_add(res, pw, h);

        pw /= 2;
    }    

    return res;
}

vector <int> get_max(vector <int> a, vector <int> b) {
    vector <int> res;
    for (int i = 0; i < (int) a.size(); i++) {
        res.pb(proxy_or({a[i], b[i]}));
    }

    return res;
}

vector <int> get_ans(int n, int m,  const vector<vector<int>>& idx) {
    vector <int> ones = get_diag_ones(n, m, idx);

    vector <int> mn_one = get_first_one(ones);
    vector <int> mx_one = get_last_one(ones);
    vector <int> diff = get_abs_diff(mn_one, mx_one);

    return diff;
}

void construct_network(int n, int m, int k) {
    vector <vector<int>> idx(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            idx[i][j] = i * m + j;
        }
    }

    const_one = proxy_or({0, add_not(0)});
    const_zero = proxy_and({0, add_not(0)});

    vector <int> xpy_ans = get_ans(n, m, idx);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m / 2; j++) {
            swap(idx[i][j], idx[i][m - 1 - j]);
        }
    }
    vector <int> xmy_ans = get_ans(n, m, idx);
    vector <int> mx_ans = get_max(xpy_ans, xmy_ans);

    if (k == n + m - 2) {
        proxy_xor({mx_ans[n + m - 2]});
    } else {
        proxy_xor({mx_ans[k], mx_ans[k + 1]});
    }
}
