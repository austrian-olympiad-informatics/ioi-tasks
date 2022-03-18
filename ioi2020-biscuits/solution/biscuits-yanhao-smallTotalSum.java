class biscuits {

	boolean is_valid(long y, long x, long[] a) {
		long carry = 0;
		for(int i=0; y>0; i++) {
			if(y % 2 == 1) {
				if(carry + a[i] < x) return false;
				carry = (carry + a[i] - x)/2;
			} else {
				carry = (carry + a[i])/2;
			}
			y = y/2;
		}
		return true;
	}

	long count_tastiness(long x, long[] a) {
		long s = 0;
		for(int i=0; i<a.length; i++) {
			s += (a[i]<<i);
		}
		long[] b = new long[61];
		for(int i=0; i<=60; i++) {
			b[i] = (i < a.length ? a[i] : 0);
		}
		a = b;
		long ans = 0;
		for(int i=0; i<=s; i++) {
			if(is_valid(i, x, a)) ans++;
		}
		return ans;
	}
}