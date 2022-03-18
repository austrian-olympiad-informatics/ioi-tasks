/*
 * Model solution for task mushrooms
 *
 * Author: Mikhail Tikhomirov
 */

#include "mushrooms.h"
#include <vector>
#include <string>
#include <map>
#include <cassert>

#define mp make_pair
#define mt make_tuple
#define fi first
#define se second
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define forn(i, n) for (int i = 0; i < (int)(n); ++i)
#define for1(i, n) for (int i = 1; i <= (int)(n); ++i)
#define ford(i, n) for (int i = (int)(n) - 1; i >= 0; --i)
#define fore(i, a, b) for (int i = (int)(a); i <= (int)(b); ++i)

using namespace std;

typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<pii> vpi;
typedef vector<vi> vvi;
typedef long long i64;
typedef vector<i64> vi64;
typedef vector<vi64> vvi64;
typedef pair<i64, i64> pi64;
typedef double ld;

template<class T> bool uin(T &a, T b) { return a > b ? (a = b, true) : false; }
template<class T> bool uax(T &a, T b) { return a < b ? (a = b, true) : false; }

vi labels;
vvi lst;
int K = 110;

int ans;

void label(int p, int z) {
//    cerr << p << ' ' << z << '\n';
    labels[p] = z;
    lst[z].pb(p);
    if (!z) ++ans;
}

int p;
int z;
vi toQ;

string subs(string s, int mask) {
    for (char &c: s) if (c >= 'A' && c <= 'Z') c = (char)('0' + ((mask >> (c - 'A')) & 1));
    return s;
}

int eval(const string &s) {
    int ret = 0;
    forn(i, (int)s.size() - 1) ret += int(s[i] != s[i + 1]);
    return ret;
}

int query(string s) {
    vi v;
    vi ptr(2);
    for (char c: s) {
        if (c == '0' || c == '1') {
            int d = (c - '0') ^ z;
            assert(ptr[d] < (int)lst[d].size());
            v.pb(lst[d][ptr[d]++]);
        } else {
            int l = c - 'A';
            v.pb(toQ[l]);
        }
    }
    return use_machine(v);
}

map<vi, string> prec1 = {
    {{}, "A0B0C0DE"},
    {{1}, "CED"},
    {{2}, "DBE0A"},
    {{3}, "CB0E0DA1"},
    {{4}, "D0A0E0CB1"},
    {{5}, "DBCE1"},
    {{6}, "CED"}
};

map<vi, string> prec2 = {
    {{}, "ABCDE0"},
    {{1}, "B0"},
    {{1, 1}, "EADC"},
    {{2}, "CB0D"},
    {{2, 1}, "EADB"},
    {{2, 2}, "DAE0CB"},
    {{3}, "CB0D"},
    {{3, 1}, "EDB0"},
    {{3, 2}, "AED"},
    {{3, 3}, "ED"},
    {{4}, "B0"},
    {{4, 1}, "0EACD"}
};


void magic(map<vi, string> &m) {
    toQ.clear();
    forn(i, 5) toQ.pb(p + i);
    vi seq;
    map<string, int> res;
    while (m.count(seq)) {
        string s = m[seq];
        int x = query(s);
        res[s] = x;
        seq.pb(x);
    }
    vi goodM;
    forn(mask, 32) {
        bool ok = true;
        for (auto w: res) ok &= eval(subs(w.fi, mask)) == w.se;
        if (ok) goodM.pb(mask);
    }
    assert(goodM.size() == 1);
    int mask = goodM[0];
    forn(i, 5) label(p++, ((mask >> i) & 1) ^ z);
}

int count_mushrooms(int n) {
    labels = vi(n, -1);
    lst = vvi(2);
    p = 1;
    ans = 0;
    label(0, 0);
    while (p < n) {
        z = lst[0].size() > lst[1].size() ? 0 : 1;
        if (n - p < 5 || (int)lst[z].size() >= K) {
            vi m;
            int i = 0;
            while (p < n && i < (int)lst[z].size()) {
                m.pb(p++);
                m.pb(lst[z][i++]);
            }
            int x = use_machine(m);
            lst[z ^ (x & 1)].pb(m[0]);
            x = (x + 1) / 2;
            ans += (z ? x : i - x);
        } else magic(!lst[z ^ 1].empty() && lst[z].size() >= 3 ? prec1 : prec2);
    }
    return ans;
}

