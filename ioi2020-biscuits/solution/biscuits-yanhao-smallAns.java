class biscuits {
	long[] arr = new long[(int) 2e5+5];

	long count_tastiness(long x, long[] a) {
		long[] s = new long[61];
		s[0] = a[0];
		for(int i=1; i<a.length; i++) {
			s[i] = s[i-1] + (a[i]<<i);
		}
		for(int i=a.length; i<=60; i++) {
			s[i] = s[i-1];
		}
		arr[0] = 0;
		int sz = 1;
		for(int i=0; i<s.length; i++) {
			long threshold = Math.min(1L<<i, s[i]/x-(1L<<i)+1);
			for(int j=0; arr[j]<threshold; j++) {
				arr[sz++] = arr[j]+(1L<<i);
			}
		}
		return sz;
	}
}
