#include <bits/stdc++.h>
#ifdef LOCAL
#include <prettyprint.h>
#define print(x) cerr << x << "\n";
#else
#define print(x)
#endif
#define int int64_t
using namespace std;

template<typename T>
struct Segtree {

    Segtree(vector<T> init) {
        int n = 1;
        while (n < init.size()) n*=2;
        data.resize(2*n);
        for (int i = 0; i < init.size(); i++) {
            set(i, init[i]);
        }
    }
    
    T query(int l, int r, int i = 1, int _l = 0, int _r = -1) {
        if (_r < 0) _r = data.size() / 2;
        if (l <= _l && r >= _r) return data[i];
        if (_l >= r || _r <= l) return 0;
        const T m = (_l + _r) / 2;
        return query(l, r, 2*i, _l, m) + query(l, r, 2*i+1, m, _r);
    }

    // Find first position with left sum lower or equal to sum
    // Return remaining sum, pos
    pair<T, int> search(int sum, int i = 1, int _l = 0, int _r = -1) {
        if (_r < 0) _r = data.size() / 2;
        assert(sum >= 0);
        if (_r == _l + 1) return {sum, i - data.size() / 2};
        const T m = (_l + _r) / 2;
        if (sum - data[2*i] >= 0) return search(sum - data[2*i], 2*i+1, m, _r);
        return search(sum, 2*i, _l, m);
    }
    
    void rebuild(int i) {
        data[i] = data[i*2] + data[i*2+1];
        if (i > 1) rebuild(i/2);
    }
    
    void set(int pos, T val) {
        pos += data.size() / 2;
        data[pos] = val;
        rebuild(pos/2);
    }

    void add(int pos, T val) {
        pos += data.size() / 2;
        data[pos] += val;
        rebuild(pos/2);
    }

    void debug(int l = 0, int r = -1, int i = 1, int depth = 0) {
        if (r < 0) r = data.size() / 2;
        int m = (l + r) / 2;
        if (r - l > 1) debug(l, m, 2*i, depth + 1);
        cerr << string(depth * 4, ' ') << data[i] << "\n";
        if (r - l > 1) debug(m, r, 2*i + 1, depth + 1);
    }
    
    vector<T> data;
};

struct Solver {

    Solver(vector<int> nums) : N(nums.size()), segte(vector<int>(nums.size(), 0)) {

        // do a manual riffle first
        vector<int> a(nums.begin(), nums.begin() + nums.size() / 2), b(nums.begin() + nums.size() / 2, nums.end());
        nums.clear();
        reverse(a.begin(), a.end());
        reverse(b.begin(), b.end());
        while (!a.empty() || !b.empty()) {
            if (a.empty() || !b.empty() && b.back() < a.back()) {
                nums.push_back(b.back());
                b.pop_back();
            } else {
                nums.push_back(a.back());
                a.pop_back();
            }
        }
        
        // now nums are sorted
        int big = 0;
        for (int i = 0; i < nums.size(); i++) {
            big = max(nums[i], big);
            following[big].push_back(nums[i]);
            segte.set(big, following[big].size());
        }
    }

    int getValue(int position) {
        print("Searching for " << position << " in " << segte.data);
        auto [remSum, pos] = segte.search(position);
        print("Found " << pos << " with su " << remSum);
        return following[pos].at(remSum);
    }
    
    void riffle() {
        print("Round " << round);
        auto [remSum, pos] = segte.search(N / 2);
        print("N/2 is at " << pos << " with sum " << remSum);

        if (remSum > 0) {
            print("Splitting at " << following[pos][remSum]);
            int big = 0;
            for (int i = remSum; i < following[pos].size(); i++) {
                big = max(following[pos][i], big);
                following[big].push_back(following[pos][i]);
                segte.set(big, following[big].size());
            }
            following[pos].erase(following[pos].begin() + remSum, following[pos].end());
            segte.set(pos, following[pos].size());
        }
        round++;
        print("Round " << round);
        print(segte.data);
    }

    int N;
    int round = 1;
    Segtree<int> segte;
    unordered_map<int, vector<int>> following;
};

struct Query {
    int round;
    int position;
    int queryIndex;
    int result;
};
    
signed main() {
    int N, Q;
    cin >> N >> Q;
    vector<int> nums(N);
    for (int i = 0; i < N; i++) {
        cin >> nums[i];
    }
    vector<Query> queries(Q); //round, index, query index
    for (int i = 0; i < Q; i++) {
        cin >> queries[i].round >> queries[i].position;
        queries[i].position--;
        queries[i].round = min(queries[i].round, int(210000));
        queries[i].queryIndex = i;
    }
    const auto byRound = [](Query& a, Query& b) {return a.round < b.round;};
    const auto byIndex = [](Query& a, Query& b) {return a.queryIndex < b.queryIndex;};
    sort(queries.begin(), queries.end(), byRound);
    Solver solver(nums);
    for (Query& query : queries) {
        while (solver.round < query.round) {
            solver.riffle();
        }
        if (query.round == 0) query.result = nums[query.position];
        else query.result = solver.getValue(query.position);
        
    }
    
    sort(queries.begin(), queries.end(), byIndex);
    for (Query& query : queries) {
        cout << query.result << "\n";
    }
}
