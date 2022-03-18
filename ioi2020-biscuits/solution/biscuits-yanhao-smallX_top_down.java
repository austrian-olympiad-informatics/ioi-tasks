class biscuits {

	final int x_max = (int)1e5+1;
	long[][] dp = new long[61][2*x_max];

	long f(int idx, long w, long x, long[] a) {
		if(idx==60) return 1;
		if(dp[idx][(int)w]!=-1) return dp[idx][(int)w];
		return dp[idx][(int)w] = f(idx+1, (w+a[idx])/2, x, a) + (w+a[idx]>=x ? f(idx+1, (w+a[idx]-x)/2, x, a) : 0);
	}
	
	long[] merge_biscuits(long x, long[] a) {
		long[] b = new long[61];
		for(int i=0; i<=60; i++) {
			b[i] = (i < a.length ? a[i] : 0);
		}
		for(int i=0; i<b.length; i++) {
			if(b[i]>x+1) {
				b[i+1] += (b[i]-x)/2;
				b[i] -= (b[i]-x)&(-2);
			}
		}
		return b;
	}

	long count_tastiness(long x, long[] a) {
		a = merge_biscuits(x, a);
		for(int i=0; i<=60; i++) {
			for(int j=0; j<=2*x; j++) {
				dp[i][j] = -1;
			}
		}
		return f(0, 0, x, a);
	}
}