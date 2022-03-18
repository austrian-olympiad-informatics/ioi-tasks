/*
 * Trivial one-by-one solution
 *
 * Author: Kian Mirjalali
 */
public class mushrooms {

	private int use_machine(int[] x) {
		return stub.use_machine(x);
	}

	public int count_mushrooms(int n) {
		int s = 1;
		for (int i=1; i<n; i++)
			s += 1 - use_machine(new int[] {0, i});
		return s;
	}
}
