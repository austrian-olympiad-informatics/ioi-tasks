import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.Map.Entry;

/*
 * Full solution for task mushrooms
 *
 * Author: Mikhail Tikhomirov
 * (ported from C++ by Kian)
 */
public class mushrooms {

	private static void _assert(boolean cond) {
		if (!cond)
			throw new AssertionError();
	}

	private static int bool2int(boolean b) {
		return b ? 1 : 0;
	}
	private static boolean int2bool(int i) {
		return i != 0;
	}


	private int use_machine(int[] x) {
		return stub.use_machine(x);
	}

	private int use_machine(List<Integer> x) {
		int[] xa = new int[x.size()];
		for (int i = 0; i < xa.length; i++)
			xa[i] = x.get(i);
		return use_machine(xa);
	}

	private static class IterableComparator<T extends Comparable<T>> implements Comparator<Iterable<T>> {
		@Override
		public int compare(Iterable<T> a, Iterable<T> b) {
			Iterator<T> ita = a.iterator();
			Iterator<T> itb = b.iterator();
			while (ita.hasNext()) {
				if (!itb.hasNext())
					return 1;
				T itema = ita.next();
				T itemb = itb.next();
				int c = itema.compareTo(itemb);
				if (int2bool(c))
					return c;
			}
			return itb.hasNext() ? -1 : 0;
		}
	}

	private static class LIMap<T> extends TreeMap<List<Integer>, T> {
		static final long serialVersionUID = 1L;
		public LIMap(Object... initialValues) {
			super(new IterableComparator<>());
			IllegalArgumentException ex = new IllegalArgumentException("Args must be pairs of (List<Integer>, String)");
			if (initialValues.length % 2 != 0)
				throw ex;
			for (int i = 0; i < initialValues.length; i+=2) {
				if (!(initialValues[i] instanceof List))
					throw ex;
				@SuppressWarnings("unchecked")
				List<Integer> k = (List<Integer>)initialValues[i];
				@SuppressWarnings("unchecked")
				T v = (T)initialValues[i+1];
				this.put(k, v);
			}
		}
	}


	int[] labels;
	List<List<Integer>> lst;
	int K = 110;

	int ans;

	void label(int p, int z) {
		labels[p] = z;
		lst.get(z).add(p);
		if (!int2bool(z)) ++ans;
	}

	int p = 1;
	int z;
	List<Integer> toQ = new ArrayList<>();

	String subs(String s0, int mask) {
		char[] s = s0.toCharArray();
		for (int i = 0; i < s.length; i++){
			char c = s[i];
			if (c >= 'A' && c <= 'Z')
				s[i] = (char)('0' + ((mask >> (c - 'A')) & 1));
		}
		return new String(s);
	}

	int eval(final String s) {
		int ret = 0;
		for (int i = 0; i < s.length() - 1; i++)
			ret += bool2int(s.charAt(i) != s.charAt(i + 1));
		return ret;
	}

	int query(String s) {
		List<Integer> v = new ArrayList<>();
		int[] ptr = new int[2];
		for (char c: s.toCharArray()) {
			if (c == '0' || c == '1') {
				int d = (c - '0') ^ z;
				_assert(ptr[d] < lst.get(d).size());
				v.add(lst.get(d).get(ptr[d]++));
			} else {
				int l = c - 'A';
				v.add(toQ.get(l));
			}
		}
		return use_machine(v);
	}

	LIMap<String> prec1 = new LIMap<>(
		Arrays.asList(), "A0B0C0DE",
		Arrays.asList(1), "CED",
		Arrays.asList(2), "DBE0A",
		Arrays.asList(3), "CB0E0DA1",
		Arrays.asList(4), "D0A0E0CB1",
		Arrays.asList(5), "DBCE1",
		Arrays.asList(6), "CED"
	);

	LIMap<String> prec2 = new LIMap<>(
		Arrays.asList(), "ABCDE0",
		Arrays.asList(1), "B0",
		Arrays.asList(1, 1), "EADC",
		Arrays.asList(2), "CB0D",
		Arrays.asList(2, 1), "EADB",
		Arrays.asList(2, 2), "DAE0CB",
		Arrays.asList(3), "CB0D",
		Arrays.asList(3, 1), "EDB0",
		Arrays.asList(3, 2), "AED",
		Arrays.asList(3, 3), "ED",
		Arrays.asList(4), "B0",
		Arrays.asList(4, 1), "0EACD"
	);


	void magic(LIMap<String> m) {
		toQ.clear();
		for (int i = 0; i < 5; i++)
			toQ.add(p + i);
		List<Integer> seq = new ArrayList<>();
		Map<String, Integer> res = new TreeMap<>();
		while (m.containsKey(seq)) {
			String s = m.get(seq);
			int x = query(s);
			res.put(s, x);
			seq.add(x);
		}
		List<Integer> goodM = new ArrayList<>();
		for (int mask = 0; mask < 32; mask++) {
			boolean ok = true;
			for (Entry<String, Integer> w : res.entrySet())
				ok &= eval(subs(w.getKey(), mask)) == w.getValue();
			if (ok) goodM.add(mask);
		}
		_assert(goodM.size() == 1);
		int mask = goodM.get(0);
		for (int i = 0; i < 5; i++)
			label(p++, ((mask >> i) & 1) ^ z);
	}

	public int count_mushrooms(int n) {
		labels = new int[n];
		Arrays.fill(labels, -1);
		lst = new ArrayList<>();
		for (int i = 0; i<2; i++)
			lst.add(new ArrayList<>());
		p = 1;
		ans = 0;
		label(0, 0);
		while (p < n) {
			z = lst.get(0).size() > lst.get(1).size() ? 0 : 1;
			if (n - p < 5 || lst.get(z).size() >= K) {
				List<Integer> m = new ArrayList<>();
				int i = 0;
				while (p < n && i < lst.get(z).size()) {
					m.add(p++);
					m.add(lst.get(z).get(i++));
				}
				int x = use_machine(m);
				lst.get(z ^ (x & 1)).add(m.get(0));
				x = (x + 1) / 2;
				ans += (int2bool(z) ? x : i - x);
			} else magic(!lst.get(z ^ 1).isEmpty() && lst.get(z).size() >= 3 ? prec1 : prec2);
		}
		return ans;
	}
}
