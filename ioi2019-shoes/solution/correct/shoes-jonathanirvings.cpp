#include "shoes.h"

#include <bits/stdc++.h>
using namespace std;

namespace testcaseCheck {

const int SUBTASK = 6;

bool permutationsOfShoes(vector<int> S, int N) {
  vector<int> V;
  for (int i = 1; i <= N; ++i) {
    V.push_back(-i);
    V.push_back(i);
  }

  sort(S.begin(), S.end());
  sort(V.begin(), V.end());
  for (int i = 0; i < 2 * N; ++i) {
    if (S[i] != V[i]) {
      return false;
    }
  }

  return true;
}

void run(std::vector<int> S) {
  assert(1 <= SUBTASK && SUBTASK <= 6);

  assert(S.size() % 2 == 0);
  int N = S.size() / 2;
  assert(1 <= N && N <= 100000);
  
  map<int, int> sizeToOccurences;
  for (int s : S) {
    sizeToOccurences[abs(s)] += s / abs(s);
  }
  for (pair<int, int> occurences : sizeToOccurences) {
    assert(occurences.second == 0);
  }

  if (SUBTASK == 1) {
    assert(N == 1);
  }

  if (SUBTASK == 2) {
    assert(N <= 8);
  }

  if (SUBTASK == 3) {
    for (int s : S) {
      assert(abs(s) == abs(S[0]));
    }
  }

  if (SUBTASK == 4) {
    for (int i = 0; i < N; ++i) {
      assert(abs(S[i]) == S[i + N]);
    }
  }

  if (SUBTASK == 5) {
    assert(N <= 1000);
  }
}


}  // namespace testcaseCheck

struct FenwickTree
{
  vector<int> arr;
  
  FenwickTree(int n) {
    arr.resize(n + 1, 0);
  }
  
  void update(int x,int y) {
    for (int i = x + 1; i < arr.size(); i += (i & -i)) {
      arr[i] += y;
    }
  }

  int query(int x) {
    int ret = 0;
    for (int i = x + 1; i > 0; i -= (i & -i)) {
      ret += arr[i];
    }
    return ret;
  }
};

long long count_swaps(std::vector<int> S) {
  testcaseCheck::run(S);  
  
  long long answer = 0;
  int N = S.size() / 2;
  
  FenwickTree fenwickTree(2 * N);
  for (int i = 0; i < 2 * N; ++i) {
    fenwickTree.update(i, 1);
  }

  map<int, vector<int>> positions;
  for (int i = 2 * N - 1; i >= 0; --i) {
    positions[S[i]].push_back(i);
  }

  for (int i = 0; i < 2 * N; ++i) {
    if (fenwickTree.query(i) == fenwickTree.query(i - 1)) {
      continue;
    }
    if (S[i] > 0) {
      ++answer;
    }
    int pos = positions[-S[i]][positions[-S[i]].size() - 1];
    answer += fenwickTree.query(pos - 1) - fenwickTree.query(i);
    fenwickTree.update(i, -1);
    fenwickTree.update(pos, -1);

    positions[S[i]].pop_back();
    positions[-S[i]].pop_back();
  }

  return answer;
}
