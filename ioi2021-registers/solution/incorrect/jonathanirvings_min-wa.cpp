#include "registers.h"

#include <bits/stdc++.h>
using namespace std;

constexpr int M = 100;
constexpr int B = 2000;

constexpr int kToComplete = M - 1;

constexpr int kSecondElem = M - 1;
constexpr int kSecondElemComplement = kSecondElem - 1;
constexpr int kFilterBits = kSecondElemComplement - 1;
constexpr int kLsb = kFilterBits - 1;
constexpr int kMsb = kLsb - 1;
constexpr int kTakeFromFirst = kMsb - 1;
constexpr int kTakeFromSecond = kTakeFromFirst - 1;

constexpr int kFirstElemMsb = kTakeFromSecond - 1;
constexpr int kSecondElemMsb = kFirstElemMsb - 1;
constexpr int kOverrideToZero = kSecondElemMsb - 1;
constexpr int kOverrideToOne = kOverrideToZero - 1;
constexpr int kBuffer = kOverrideToOne - 1;
constexpr int kFilterBitsPlusOne = kBuffer - 1;

void compute_min(int diff, const int N, const int K) {
  if (diff == N) {
    return;
  }

  vector<bool> filter_bits(B, false);
  vector<bool> filter_bits_plus_one(B, false);
  vector<bool> lsb(B, false);
  for (int i = 0; i < N; i += 2 * diff) {
    lsb[i * K] = true;
    fill(filter_bits.begin() + i * K, filter_bits.begin() + (i + 1) * K, true);
    fill(filter_bits_plus_one.begin() + i * K,
         filter_bits_plus_one.begin() + (i + 1) * K + 1, true);
  }
  append_store(kFilterBitsPlusOne, filter_bits_plus_one);
  append_store(kFilterBits, filter_bits);
  append_store(kLsb, lsb);
  
  append_right(kSecondElem, 0, diff * K);
  append_and(kSecondElem, kSecondElem, kFilterBits);
  append_and(0, 0, kFilterBits);

  append_and(kFirstElemMsb, 0, kMsb);
  append_and(kSecondElemMsb, kSecondElem, kMsb);

  append_not(kOverrideToZero, kFirstElemMsb);
  append_and(kOverrideToOne, kOverrideToZero, kSecondElemMsb);
  append_or(kOverrideToZero, kOverrideToZero, kSecondElemMsb);

  append_not(kSecondElemComplement, kSecondElem);
  append_add(kSecondElemComplement, kSecondElemComplement, kLsb);
  append_and(kSecondElemComplement, kSecondElemComplement, kFilterBits);

  append_add(kSecondElemComplement, 0, kSecondElemComplement);
  append_and(kSecondElemComplement, kSecondElemComplement, kOverrideToZero);
  append_or(kSecondElemComplement, kSecondElemComplement, kOverrideToOne);
  append_right(kSecondElemComplement, kSecondElemComplement, K - 1);
  append_and(kSecondElemComplement, kSecondElemComplement, kLsb);

  append_add(kTakeFromSecond, kSecondElemComplement, kFilterBits);
  append_not(kTakeFromFirst, kTakeFromSecond);

  append_and(kTakeFromSecond, kTakeFromSecond, kSecondElem);
  append_and(kTakeFromFirst, kTakeFromFirst, 0);
  append_or(0, kTakeFromSecond, kTakeFromFirst);

  compute_min(diff << 1, N, K);
}

void construct_instructions(int S, int N, int K, int Q) {
  vector<bool> to_complete(B, false);
  vector<bool> msb(B, false);

  while (N != (1 << __builtin_ctz(N))) {
    fill(to_complete.begin() + N * K, to_complete.begin() + (N + 1) * K, true);
    ++N;
  }

  for (int i = 0; i < N; ++i) {
    msb[(i + 1) * K - 1] = true;
  }

  append_store(kMsb, msb);
  append_store(kToComplete, to_complete);
  append_or(0, 0, kToComplete);

  compute_min(1, N, K);
}
