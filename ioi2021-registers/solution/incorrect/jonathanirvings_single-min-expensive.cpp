#include "registers.h"

#include <bits/stdc++.h>
using namespace std;

constexpr int M = 100;
constexpr int B = 2000;

constexpr int kFinalAnswer = M - 1;
constexpr int kExpectedInput = kFinalAnswer - 1;
constexpr int kExpectedOutput = kExpectedInput - 1;
constexpr int kFirstBuffer = kExpectedOutput - 1;
constexpr int kSecondBuffer = kFirstBuffer - 1;

void load(int K, int val, int starting_index, vector<bool> &v) {
  for (int i = 0; i < K; ++i) {
    if (val & (1 << i)) {
      v[starting_index + i] = true;
    }
  }
}

void construct_instructions(int S, int N, int K, int Q) {
  for (int i = 0; i < (1 << K); ++i) {
    for (int j = 0; j < (1 << K); ++j) {
      vector<bool> expected_input(B, false);
      vector<bool> expected_output(B, false);

      load(K, i, 0, expected_input);
      load(K, j, K, expected_input);
      load(K, min(i, j), 0, expected_output);

      append_store(kExpectedInput, expected_input);
      append_store(kExpectedOutput, expected_output);

      append_xor(kFirstBuffer, 0, kExpectedInput);
      for (int bit = 1; bit < 2 * K; ++bit) {
        append_right(kSecondBuffer, kFirstBuffer, 1);
        append_or(kFirstBuffer, kFirstBuffer, kSecondBuffer);
      }

      for (int bit = 1; bit < K; ++bit) {
        append_left(kSecondBuffer, kFirstBuffer, 1);
        append_or(kFirstBuffer, kFirstBuffer, kSecondBuffer);
      }
      append_not(kFirstBuffer, kFirstBuffer);
      append_and(kFirstBuffer, kFirstBuffer, kExpectedOutput);
      append_or(kFinalAnswer, kFinalAnswer, kFirstBuffer);
    }
  }
  append_move(0, kFinalAnswer);
}
