class biscuits {
	long[] merge_biscuits(long[] a) {
		long[] b = new long[61];
		for(int i=0; i<=60; i++) {
			b[i] = (i < a.length ? a[i] : 0);
		}
		for(int i=0; i<b.length; i++) {
			if(b[i]>2) {
				b[i+1] += (b[i]-1)/2;
				b[i] = (b[i]%2==1 ? 1 : 2);
			}
		}
		return b;
	}

	long count_tastiness(long x, long[] a) {
		a = merge_biscuits(a);
		long ans = 1;
		long current = 0;
		for(int i=60; i>=0; i--) {
			if(a[i]==0) {
				ans *= (current+1);
				current = 0;
			} else {
				current = current*2 + a[i];
			}
		}
		ans *= (current+1);
		return ans;
	}
}