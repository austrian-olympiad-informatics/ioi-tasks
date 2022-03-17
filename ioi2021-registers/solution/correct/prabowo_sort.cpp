// Number of instructions: O(log^2(n))
//   For s = 1, n = 100, and k = 10, number of instructions: 926

#include "registers.h"

#include <vector>

const int M = 100;
const int B = 2000;

// Categorize the smaller numbers from (evenMask U evenMask + len) to evenMask,
//   and larger numbers to evenMask + len.
// Result is in register t
void sort_category(int t, int k, std::vector<bool> evenMask, int len) {
  const int kEvenReg = M - 3;
  append_store(kEvenReg, evenMask);

  const int kEvenPos = M - 4, kOddPos = M - 5;
  append_and(kEvenPos, kEvenReg, 0);
  append_xor(kOddPos, kEvenPos, 0); append_right(kOddPos, kOddPos, k * len); append_and(kOddPos, kOddPos, kEvenReg);

  // Create negative of odd positions
  const int kOddNeg = M - 6;
  append_xor(kOddNeg, kOddPos, kEvenReg);

  // Even - Odd; if a[i] < a[i + len] then the "Add register" at the corresponding position is 0, otherwise 1
  const int kAddPos = M - 7;
  append_add(kAddPos, kEvenPos, kOddNeg); append_right(kAddPos, kAddPos, k);

  // Filter the min pos and max pos
  const int kMinPos = M - 8, kMaxPos = M - 9;
  append_add(kAddPos, kAddPos, kEvenReg); append_and(kMinPos, kEvenPos, kAddPos); append_and(kMaxPos, kOddPos, kAddPos);
  append_xor(kOddPos, kOddPos, kMaxPos); append_xor(kEvenPos, kEvenPos, kMinPos);

  append_or(kMinPos, kMinPos, kOddPos); append_or(kMaxPos, kMaxPos, kEvenPos);
  append_left(kMaxPos, kMaxPos, k * len);

  append_or(t, kMinPos, kMaxPos);
}

void construct_instructions(int s, int n, int k, int) {
  // Append a with (2^k - 1) until n is power of 2
  std::vector<bool> v(B);
  for (; (n & -n) != n; ++n) {
    for (int i = 0; i < k; ++i) {
      v[n*k + i] = true;
    }
  }
  append_store(1, v); append_or(0, 0, 1);

  for (int len = 1; len < n; len <<= 1) {
    for (int step = len; step > (s == 0 ? len - 1 : 0); step >>= 1) {
      std::vector<bool> firstEven(B);
      std::vector<bool> secondEven(B);
      for (int i = 0; i + step < n; ++i) {
        if ((step == len && (i & step)) || (step != len && !(i & step))) continue;
        if (i / (len * 2) != (i + step) / (len * 2)) continue;
        if ((i & 1) && step != 1) {
          for (int j = 0; j < k; ++j) {
            secondEven[i * k + j] = true;
          }
        } else {
          for (int j = 0; j < k; ++j) {
            firstEven[i * k + j] = true;
          }
        }
      }

      std::vector<bool> neitherMask(B);
      for (int i = 0; i < n; ++i) {
        if (firstEven[i * k] || secondEven[i * k]) continue;
        if (i >= step && (firstEven[(i - step) * k] || secondEven[(i - step) * k])) continue;
        for (int j = 0; j < k; ++j) {
          neitherMask[i * k + j] = true;
        }
      }

      sort_category(1, k, firstEven, step);
      if (s != 0 && step != 1) {
        sort_category(2, k, secondEven, step);
      }
      if (step != len) {
        append_store(3, neitherMask); append_and(3, 3, 0);
        append_or(1, 1, 3);
      }
      append_or(0, 1, s != 0 && step != 1 ? 2 : 1);
    }
  }
}
