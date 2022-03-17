#include "registers.h"

#include <bits/stdc++.h>
using namespace std;

constexpr int M = 100;
constexpr int B = 2000;

constexpr int kOne = M - 1;
constexpr int kOneTimesK = kOne - 1;

constexpr int kFirstBuffer = kOneTimesK - 1;
constexpr int kSecondBuffer = kFirstBuffer - 1;

// r[c] = min(r[a], r[b])
void take_min(int K, int a, int b, int c) {
  append_not(kFirstBuffer, b);
  append_add(kFirstBuffer, kFirstBuffer, kOne);  // r[kFirstBuffer] is 2s complement
                                          // of r[b]
  append_add(kFirstBuffer, a, kFirstBuffer);
  append_right(kFirstBuffer, kFirstBuffer, B - 1);  // r[kFirstBuffer] is 0 if
                                              // r[a] >= r[b], else 1
  for (int i = 1; i < K; ++i) {
    append_left(kSecondBuffer, kFirstBuffer, 1);
    append_or(kFirstBuffer, kFirstBuffer, kSecondBuffer);
  }

  // r[kFirstBuffer] is 0 if r[a] >= r[b], else 1 * K
  append_xor(kSecondBuffer, kFirstBuffer, kOneTimesK);
  // r[kSecondBuffer] is 0 if r[a] < r[b], else 1 * K

  append_and(kFirstBuffer, kFirstBuffer, a);  // r[kFirstBuffer] is 0 if
                                        // r[a] >= r[b], else r[a]
  append_and(kSecondBuffer, kSecondBuffer, b);  // r[kSecondBuffer] is 0 if
                                          // r[a] < r[b], else r[b]
  append_or(c, kFirstBuffer, kSecondBuffer);
}

void construct_instructions(int S, int N, int K, int Q) {
  vector<bool> v(B, false);
  v[0] = true;
  append_store(kOne, v);

  for (int i = 0; i < K; ++i) {
    v[i] = true;
  }
  append_store(kOneTimesK, v);

  append_move(2, 0);
  append_and(0, 0, kOneTimesK);
  for (int i = 1; i < N; ++i) {
    append_right(1, 2, i * K);
    append_and(1, 1, kOneTimesK);
    take_min(K, 0, 1, 0);
  }
}
