/*
 * Two-by-one solution, just computing the counts
 *
 * Author: Kian Mirjalali
 */
public class mushrooms {

	private int use_machine(int[] x) {
		return stub.use_machine(x);
	}

	private static final int TYPE_0 = 0;
	private static final int TYPE_1 = 1;

	private static class SpeciesCounts {
		private int[] counts;
		public SpeciesCounts() {
			counts = new int[] {0, 0};
		}
		public void add(int type, int count, int otherCount) {
			counts[type] += count;
			counts[1-type] += otherCount;
		}
		public void add(int type, int count) {
			add(type, count, 0);
		}
		public void add(int type) {
			add(type, 1);
		}
		public int get(int type) {
			return counts[type];
		}
	}

	private int getType(int i) {
		return use_machine(new int[] {0, i});
	}

	int a, b;
	int ta;
	private int[] getTypes(int i, int j) {
		final int d = use_machine(new int[] {a, i, b, j});
		return new int[] {(d>>1)^ta, (d&1)^ta};
	}

	public int count_mushrooms(int n) {
		final SpeciesCounts speciesCounts = new SpeciesCounts();
		int head = 0;
		if (n >= 1) {
			speciesCounts.add(TYPE_0);
			head++;
			if (n >= 2) {
				final int t1 = getType(head);
				speciesCounts.add(t1);
				head++;
				if (n == 2 || t1 == TYPE_0) {
					a = 0;
					b = 1;
					ta = TYPE_0;
				} else {//n >= 3 && t1 == TYPE_1
					final int t2 = getType(head);
					speciesCounts.add(t2);
					head++;
					if (t2 == TYPE_0) {
						a = 0;
						b = 2;
						ta = TYPE_0;
					} else {//t2 == TYPE_1
						a = 1;
						b = 2;
						ta = TYPE_1;
					}
				}
			}
		}

		// head <= n
		while (head+2 <= n) {
			final int[] p = getTypes(head, head+1);
			speciesCounts.add(p[0]);
			speciesCounts.add(p[1]);
			head += 2;
		}
		// n-2 < head <= n
		if (head < n) { // head == n-1
			final int t = getType(head);
			speciesCounts.add(t);
			head++;
		}
		// head == n
		return speciesCounts.get(TYPE_0);
	}
}
