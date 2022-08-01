class shoes {

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


    public long count_swaps(int[] in) {
		change(in);

        int n = in.length / 2;
        
        int where[] = new int[n + 1];
        for (int i = 1; i <= n; ++i)
            where[i] = -1;

        for (int i = 0; i != 2 * n; ++i)
            if (where[Math.abs(in[i])] == -1)
                where[Math.abs(in[i])] = i;
        
        long ans = 0;

        int fenw[] = new int[2 * n];
        for (int i = 0; i != 2 * n; ++i)
            fenw[i] = (i + 1) - (i & (i + 1));
        
        for (int pos = 2 * n - 1; pos >= 0; --pos) {
            if (where[Math.abs(in[pos])] == pos)
                continue;

            int i = where[Math.abs(in[pos])];

            int dist = 0;

            for (int p = pos; p >= 0; p = (p & (p + 1)) - 1)
                dist += fenw[p];
            for (int p = i; p >= 0; p = (p & (p + 1)) - 1)
                dist -= fenw[p];
            
            if (in[pos] > 0)
                --dist;

            ans += dist;
            for (int p = i; p < 2 * n; p = p | (p + 1))
                fenw[p] -= 1;
        }
        
        return ans;
    }

    // public static void main(String[] argv) {
    //     shoe sh = new shoe();
    //     int[] test = new int[4];
    //     test[0] = -2;
    //     test[1] = -1;
    //     test[2] = 1;
    //     test[3] = 2;
    //     sh.count_swaps(test);
    // }
}
