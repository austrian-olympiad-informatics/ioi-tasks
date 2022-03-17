#include "dna.h"

#include <bits/stdc++.h>
using namespace std;

const string kDna = "ACT";
const int K = kDna.size();
vector<int> prefix_sum[sizeof(char) << 8][sizeof(char) << 8];

int cnt[sizeof(char) << 8][sizeof(char) << 8];
vector<string> dna_subsets;

constexpr int kImpossible = -1;

void init(string A, string B) {
  int N = A.size();
  for (int c1 : kDna) {
    for (int c2 : kDna) {
      prefix_sum[c1][c2].resize(N);
    }
  }
  for (int c1 : kDna) {
    for (int c2 : kDna) {
      for (int i = 0; i < N; ++i) {
        prefix_sum[c1][c2][i] = (c1 == A[i] && c2 == B[i]) ? 1 : 0;
        if (i > 0) {
          prefix_sum[c1][c2][i] += prefix_sum[c1][c2][i - 1];
        }
      }
    }
  }

  for (int bit = 1; bit < (1 << K); ++bit) {
    string dna_subset;
    for (int i = 0; i < K; ++i) {
      if (bit & (1 << i)) {
        dna_subset += kDna[i];
      }
    }
    dna_subsets.push_back(dna_subset);
  }
  sort(dna_subsets.begin(), dna_subsets.end(), [] (string a, string b) {
    return a.size() < b.size();
  });
}

int get_distance(int X, int Y) {
  for (int c1 : kDna) {
    for (int c2 : kDna) {
      cnt[c1][c2] = prefix_sum[c1][c2][Y];
      if (X > 0) {
        cnt[c1][c2] -= prefix_sum[c1][c2][X - 1];
      }
    }
  }
  
  int answer = Y - X + 1;
  for (string dna_subset : dna_subsets) {
    do {
      int res = INT_MAX;
      for (int i = 0; i < static_cast<int>(dna_subset.size()); ++i) {
        int c1 = dna_subset[i];
        int c2 = dna_subset[(i + 1) % dna_subset.size()];
        res = min(res, cnt[c1][c2]);
      }
      answer -= res;
      for (int i = 0; i < static_cast<int>(dna_subset.size()); ++i) {
        int c1 = dna_subset[i];
        int c2 = dna_subset[(i + 1) % dna_subset.size()];
        cnt[c1][c2] -= res;
      }
    } while (next_permutation(dna_subset.begin(), dna_subset.end()));
  }
  
  for (int c1 : kDna) {
    for (int c2 : kDna) {
      if (cnt[c1][c2] > 0) {
        return kImpossible;
      }
    }
  }

  return answer;
}
