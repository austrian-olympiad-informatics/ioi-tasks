public class shoes {
    private class Fenwick
    {
        private int[] a;

        private int count(int right)
        {
            int cnt = 0;
            for (; right >= 0; right = (right & right + 1) - 1)
            {
                cnt += a[right];
            }
            return cnt;
        }

        public Fenwick(int n)
        {
            a = new int[n];
        }

        public int count(int left, int right)
        {
            return count(right) - count(left - 1);
        }

        public void put(int index)
        {
            for (; index < a.length; index = index | index + 1)
            {
                a[index]++;
            }
        }
    }

    private int[] create_index(int n, int[] S)
    {
        int[] index = new int[2 * n + 1];
        for (int i = 0; i < 2 * n; i++)
        {
            index[S[i] + n] = i;
        }
        return index;
    }

    private long count_adjacent(int n, int[] S)
    {
        int[] index = create_index(n, S);
        Fenwick f = new Fenwick(2 * n);
        long ans = 0;
        for (int i = 0; i < 2 * n; i++)
        {
            if (S[i] != 0)
            {
                int pos = index[-S[i] + n];
                ans += pos - i - f.count(i, pos) - (S[i] < 0 ? 1 : 0);
                S[pos] = 0;
                f.put(pos);
            }
        }
        return ans;
    }

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
        int n = S.length / 2;
        return count_adjacent(n, S);
    }
}
