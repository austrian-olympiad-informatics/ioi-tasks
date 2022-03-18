/*
 * Two-by-one solution, computing the types for all indices
 *
 * Author: Kian Mirjalali
 */
public class mushrooms {

	private int use_machine(int[] x) {
		return stub.use_machine(x);
	}

	private static final int TYPE_0 = 0;
	private static final int TYPE_1 = 1;

	private static class Indices extends java.util.ArrayList<Integer> {
		private static final long serialVersionUID = 1L;
	}
	private static class SpeciesTypes {
		private Indices[] typesIndices;
		public SpeciesTypes() {
			typesIndices = new Indices[2];
			for (int i = 0; i < typesIndices.length; i++)
				typesIndices[i] = new Indices();
		}
		public void add(int type, int index) {
			typesIndices[type].add(index);
		}
		public Indices getIndices(int type) {
			return typesIndices[type];
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
		final SpeciesTypes knownSpeciesTypes = new SpeciesTypes();
		int head = 0;
		if (n >= 1) {
			knownSpeciesTypes.add(TYPE_0, head);
			head++;
			if (n >= 2) {
				final int t1 = getType(head);
				knownSpeciesTypes.add(t1, head);
				head++;
				if (n == 2 || t1 == TYPE_0) {
					a = 0;
					b = 1;
					ta = TYPE_0;
				} else {//n >= 3 && t1 == TYPE_1
					final int t2 = getType(head);
					knownSpeciesTypes.add(t2, head);
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
			knownSpeciesTypes.add(p[0], head);
			knownSpeciesTypes.add(p[1], head+1);
			head += 2;
		}
		// n-2 < head <= n
		if (head < n) { // head == n-1
			final int t = getType(head);
			knownSpeciesTypes.add(t, head);
			head++;
		}
		// head == n
		return knownSpeciesTypes.getIndices(TYPE_0).size();
	}
}
