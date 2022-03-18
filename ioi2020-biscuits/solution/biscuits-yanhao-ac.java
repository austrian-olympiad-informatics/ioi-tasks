import java.util.HashMap;

class biscuits {
	HashMap<Long, Long> m = new HashMap<>();
	long f(long[] s, long x, long n) {
		if(n<=0) return 0;
		if(n==1) return 1;
		if(m.containsKey(n)) {
			return m.get(n);
		}
		int a = 63-Long.numberOfLeadingZeros(n-1);
		long b = ((long)1)<<a;
		long c = Math.min(n,1+s[a]/x) - b;
		long ans = f(s,x,b) + f(s,x,c);
		m.put(n, f(s,x,b) + f(s,x,c));
		return ans;
	}
	long count_tastiness(long x, long[] a) {
		m = new HashMap<>();
		long[] b = new long[61];
		for(int i=0; i<a.length; i++) {
			b[i] = a[i];
		}
		for(int i=1; i<a.length; i++) {
			b[i] = b[i-1] + (b[i]<<i);
		}
		for(int i=a.length; i<=60; i++) {
			b[i] = b[i-1];
		}
		return f(b, x, 1+b[60]);
	}
}