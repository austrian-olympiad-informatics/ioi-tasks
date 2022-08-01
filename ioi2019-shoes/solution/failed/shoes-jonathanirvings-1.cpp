#include "shoes.h"

#include <bits/stdc++.h>
using namespace std;

namespace testcaseCheck {

const int SUBTASK = 1;

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

long long count_swaps(std::vector<int> S) {
  testcaseCheck::run(S);  
	
  return S[0] < S[1] ? 0 : 1;
}
