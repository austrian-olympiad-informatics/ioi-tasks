class biscuits {

	final int x_max = (int) 1e5+1;
	long[][] dp = new long[2][2*x_max];

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
		for(int j=0; j<=2*x; j++) {
			dp[0][j] = 1;
			dp[1][j] = 1;
		}
	
		for(int i=60; i>=0; i--) {
			int parity = i%2;
			for(int j=0; j<x-a[i]; j++) {
				dp[parity][j] = dp[parity^1][(int)(j+a[i])/2];
			}
			for(int j=Math.max(0,(int)(x-a[i])); j<=2*x; j++) {
				dp[parity][j] = dp[parity^1][(int)(j+a[i])/2] 
				     + dp[parity^1][(int)(j+a[i]-x)/2];
			}
		}
		return dp[0][0];
	}
}