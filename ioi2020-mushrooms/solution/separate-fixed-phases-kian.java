/*
 * Solution with two separate phases of fixed size
 * By "separate" we mean that species types are only detected in the first phase.
 * By "fixed" we mean that #queries in the first phase is fixed from the beginning.
 *
 * Author: Kian Mirjalali
 */
import java.util.function.IntToDoubleFunction;

public class mushrooms {

	private int use_machine(int[] x) {
		return stub.use_machine(x);
	}

	private static <C extends java.util.Collection<?>> int largest_element_index(C[] v) {
		int bestIndex = -1;
		int bestSize = -1;
		for (int i = 0; i < v.length; i++)
			if (bestSize < v[i].size()) {
				bestIndex = i;
				bestSize = v[i].size();
			}
		return bestIndex;
	}

	/**
	 * @returns ceil(a/b) for integers a, b
	 */
	private static int ceildiv(int a, int b) {
		return (a+b-1)/b;
	}

	/**
	 * Finds minimum input of a function in range [begin, end).
	 * @param f a function : Integer -> Real
	 * @param begin
	 * @param end
	 * @return Integer input which minimizes f
	 */
	public static int functionMinInput(IntToDoubleFunction f, int begin, int end) {
		int best = begin;
		double fbest = f.applyAsDouble(best);
		for (int i = begin+1; i < end; i++) {
			final double fi = f.applyAsDouble(i);
			if (fbest > fi) {
				best = i;
				fbest = fi;
			}
		}
		return best;
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
		public int getLargestType() {
			return largest_element_index(typesIndices);
		}
	}

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

		final int head0 = head;
		final IntToDoubleFunction queries_func =
				k -> head0-1+k+ceildiv(n-head0-2*k, Math.max(k, 1));
		final int k = functionMinInput(queries_func, 0, (int)(2*Math.sqrt(n)));
		for (int j = 0; j < k && head+2 <= n; j++) {
			final int[] p = getTypes(head, head+1);
			knownSpeciesTypes.add(p[0], head);
			knownSpeciesTypes.add(p[1], head+1);
			head += 2;
		}

		final SpeciesCounts speciesCounts = new SpeciesCounts();
		final int majType = knownSpeciesTypes.getLargestType();
		final Indices majIndices = knownSpeciesTypes.getIndices(majType);
		final int majSize = majIndices.size();
		while (head < n) {
			final int blockSize = Math.min(majSize, n-head);
			final int[] v = new int[2*blockSize];
			for (int i = 0, j = 0; i < blockSize;) {
				v[j++] = head++;
				v[j++] = majIndices.get(i++);
			}
			final int differences = use_machine(v);
			final int differents = (differences&1)+(differences>>1);
			final int same = blockSize-differents;
			speciesCounts.add(majType, same, differents);
		}
		return knownSpeciesTypes.getIndices(TYPE_0).size()+speciesCounts.get(TYPE_0);
	}
}
