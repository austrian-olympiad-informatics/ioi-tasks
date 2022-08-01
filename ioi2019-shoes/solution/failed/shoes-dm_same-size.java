public class shoes {
    public long count_swaps(int[] S)
    {
        int current = 0, opposite = 0;
        long swaps = 0;
        while (current < S.length) {
            while (S[current] + S[opposite] != 0) {
                opposite++;
            }
            int newSwaps = opposite - current - (S[current] < 0 ? 1 : 0);
            swaps += newSwaps;
            if (newSwaps > 0) {
                S[opposite] = S[opposite - 1];
            }
            current += 2;
            opposite++;
        }
        return swaps;
    }
}
