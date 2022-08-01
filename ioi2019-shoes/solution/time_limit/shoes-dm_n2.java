public class shoes {

	private void change(int[] v) {
		int n = v.length / 2;

		int[] cnt = new int[n];
		for(int i = 0; i < n; i++)
			cnt[i] = 0;

		for(int i = 0; i < 2*n; i++)
			if(v[i] > 0)
				cnt[v[i] - 1]++;

		for(int i = 1; i < n; i++)
			cnt[i] += cnt[i-1];


		int[] cntl = new int[n];
		int[] cntr = new int[n];
		for(int i = 0; i < n; i++) {
			cntl[i] = cnt[i];
			cntr[i] = cnt[i];
		}

		for(int i = 0; i < 2*n; i++)
			if(v[i] > 0)
				v[i] = (--cntr[v[i] - 1]) + 1;
			else
				v[i] = -((--cntl[-v[i] - 1]) + 1);
	}


    public long count_swaps(int[] S)
    {
		change(S);
        long ans = 0;
        for (int i = 0; i < S.length; i += 2)
        {
            int pos = i + 1;
            while (S[pos] != -S[i]) {
                pos++;
            }
            ans += pos - i - (S[i] < 0 ? 1 : 0);
            for (int j = pos; j >= i + 2; j--)
            {
                S[j] = S[j - 1];
            }
        }
        return ans;
    }
}
