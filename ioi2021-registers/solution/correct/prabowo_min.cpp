// Number of instructions: 2 + 12 * log(n)
//   For n = 100 and k = 100, number of instructions: 86

#include "registers.h"

#include <vector>

const int M = 100;
const int B = 2000;

void construct_instructions(int, int n, int k, int) {
  // Append a with (2^k - 1) until n is power of 2
  std::vector<bool> v(B);
  for (; (n & -n) != n; ++n) {
    for (int i = 0; i < k; ++i) {
      v[n*k + i] = true;
    }
  }
  append_store(1, v); append_or(0, 0, 1);

  for (int len = 1; len < n; len <<= 1) {
    // In this iteration, this will happen:
    //   a[i] := min(a[i], a[i + len]) for all i = 0 (mod 2*len)

    std::vector<bool> evenMask(B);
    for (int i = 0; i < n; ++i) {
      if (i % len != 0) continue;
      if (!(i & len)) {
        for (int j = 0; j < k; ++j) evenMask[i*k + j] = true;
      }
    }

    // Initialize some constants (e.g. k=2, len=2):
    //   EvenReg: 110000001100000011000...
    const int kEvenReg = M - 3, kOddReg = M - 2;
    append_store(kEvenReg, evenMask);

    // Separate a[x] where x/(2^len) is odd/even
    const int kEvenPos = 1, kOddPos = 2;
    append_and(kEvenPos, kEvenReg, 0); append_xor(kOddPos, kEvenPos, 0); append_right(kOddPos, kOddPos, k * len);

    // Create negative of odd positions
    const int kOddNeg = 3;
    append_xor(kOddNeg, kOddPos, kEvenReg);

    // Even - Odd; if a[i] < a[i + len] then the "Add register" at position i*k is 0, otherwise 1
    const int kAddPos = 4;
    append_add(kAddPos, kEvenPos, kOddNeg); append_right(kAddPos, kAddPos, k);

    // Filter the even (then odd) positions that are the "min"
    append_add(kAddPos, kAddPos, kEvenReg); append_and(kEvenPos, kEvenPos, kAddPos);
    append_xor(kAddPos, kAddPos, kEvenReg); append_and(kOddPos, kOddPos, kAddPos);

    append_or(0, kOddPos, kEvenPos);
  }
}
