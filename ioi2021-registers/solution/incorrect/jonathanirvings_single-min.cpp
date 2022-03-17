#include "registers.h"

#include <bits/stdc++.h>
using namespace std;

constexpr int M = 100;
constexpr int B = 2000;

constexpr int kOne = M - 1;
constexpr int kOneTimesK = kOne - 1;

void construct_instructions(int S, int N, int K, int Q) {
  vector<bool> v(B, false);
  v[0] = true;
  append_store(kOne, v);

  for (int i = 0; i < K; ++i) {
    v[i] = true;
  }
  append_store(kOneTimesK, v);

  append_right(1, 0, K);
  append_and(0, 0, kOneTimesK);

  append_not(2, 1);
  append_add(2, 2, kOne);  // r[2] is 2s complement of a[1].
  append_add(2, 0, 2);
  append_right(2, 2, B - 1);  // r[2] is 0 if a[0] >= a[1], else 1

  for (int i = 1; i < K; ++i) {
    append_left(3, 2, 1);
    append_or(2, 2, 3);
  }

  append_xor(3, 2, kOneTimesK);
  // r[2] is 0 if a[0] >= a[1], else 1 * K
  // r[3] is 0 if a[1] > a[0], else 1 * K

  append_and(2, 2, 0);  // r[2] is 0 if a[0] >= a[1], else a[0]
  append_and(3, 3, 1);  // r[3] is 0 if a[1] > a[0], else a[1]
  append_or(0, 2, 3);
}
