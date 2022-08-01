class shoes {
    private long ans = (long)1e9;
    private int n;

    private int[] inp;

    private int[] rem;
    private int p_rem;
    
    private int[] cur;
    private int p_cur = 0;

    private int[] perm;
    private void eval() {
        for (int i = 0; i != 2 * n; ++i) {
            int ptr = 0;

            while (inp[ptr] != cur[i])
                ++ptr;

            perm[i] = ptr;
        }

        int ans_this = 0;
        for (int i = 0; i != 2 * n; ++i)
            for (int j = i + 1; j != 2 * n; ++j)
                if (perm[i] > perm[j])
                    ++ans_this;

        ans = Math.min(ans, ans_this);
    }
    
    private void brute() {
        if (p_cur == 2 * n) {
            eval();
        } else {
            for (int i = 0; i != p_rem; ++i) {
                int elem = rem[i];
                rem[i] = rem[--p_rem];
                
                cur[p_cur++] = -elem;
                cur[p_cur++] = elem;

                brute();
                p_cur -= 2;

                rem[p_rem++] = rem[i];
                rem[i] = elem;
            }
        }
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

    public long count_swaps(int[] s) {
		change(s);

        inp = s;
        n = s.length / 2;
        
        rem = new int[n];
        for (int i = 0; i < n; ++i)
            rem[i] = i + 1;
        p_rem = n;
        
        cur = new int[2 * n];
        perm = new int[2 * n];
        
        brute();

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
