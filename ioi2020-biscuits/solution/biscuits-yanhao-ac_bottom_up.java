class biscuits {	
	long[] dp = new long[61];
	long[] b = new long[61];
	
	long count_tastiness(long x, long[] a) {
		long sum = 0;
		int last = Long.numberOfLeadingZeros(x)-3;
		for(int i=0; i<=last; i++) {
			dp[i] = 0;
			long j = (1L<<i);
			for(int h=i-1; h>=0; h--) {
				if((j>>h) > 0) {
					dp[i] += dp[h];
					j = (j < b[h] ? j : b[h])-(1L<<h);
				}
			}
			if(j==1) dp[i]++;
			sum += (i<a.length ? (a[i]<<i) : 0);
			b[i] = sum/x+1;
		}
		return dp[last];
	}
}