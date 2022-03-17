#include "registers.h"

#include <vector>

const int M = 100;
const int B = 2000;

void construct_instructions(int, int n, int k, int) {
  std::vector<bool> v(B);
  for (; (n & -n) != n; ++n) {
    for (int i = 0; i < k; ++i) {
      v[n*k + i] = true;
    }
  }
  append_store(1, v); append_or(0, 0, 1);

  for (int len = 1; len < n; len <<= 1) {
    std::vector<bool> oddMask(B), evenMask(B), ones(B);
    for (int i = 0; i < n; ++i) {
      if (i % len != 0) continue;
      if (i & len) {
        for (int j = 0; j < k; ++j) oddMask[i*k + j] = true;
      } else {
        for (int j = 0; j < k; ++j) evenMask[i*k + j] = true;
        ones[i*k] = 1;
      }
    }
    const int kEvenReg = M - 3, kOddReg = M - 2, kOnesReg = M - 1;
    append_store(kEvenReg, evenMask); append_store(kOddReg, oddMask); append_store(kOnesReg, ones);

    const int kEvenPos = 1, kOddPos = 2;
    append_and(kEvenPos, kEvenReg, 0); append_and(kOddPos, kOddReg, 0); append_right(kOddPos, kOddPos, k * len);

    const int kOddNeg = 3;
    append_xor(kOddNeg, kOddPos, kEvenReg); append_add(kOddNeg, kOnesReg, kOddNeg);

    const int kAddPos = 4;
    // The bug is here, missing one more AND command
    append_add(kAddPos, kEvenPos, kOddNeg); append_right(kAddPos, kAddPos, k);
    
    append_add(kAddPos, kAddPos, kEvenReg); append_and(kEvenPos, kEvenPos, kAddPos);
    append_xor(kAddPos, kAddPos, kEvenReg); append_and(kOddPos, kOddPos, kAddPos);

    append_or(0, kOddPos, kEvenPos);
  }
}
