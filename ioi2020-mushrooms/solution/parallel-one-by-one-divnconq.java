/*
 * A parallel implementation of the one-by-one solution (creating threads through divide and conquer).
 * The purpose of the parallel solutions is to verify if the graders have the correct behavior
 *   when contestants use parallelism in their solutions.
 *
 * Author: Kian Mirjalali
 */
public class mushrooms {

	private int use_machine(int[] x) {
		return stub.use_machine(x);
	}

	public int count_mushrooms(int n) {
		final int[] species = new int[n];
		species[0] = 0;
		final int max_threads = 200;
		final int block_size = Math.max(1, n/max_threads);
		new Object() {
			public void handle_range(int begin, int end) {
				final int l = end - begin;
				if (l <= 0)
					return;
				if (l <= block_size) {
					for (int i = begin; i < end; i++)
						species[i] = use_machine(new int[] {0, i});
					return;
				}
				// l >= 2
				final int mid = begin + l/2;
				Thread th = new Thread(()-> { handle_range(begin, mid); });
				th.start();
				handle_range(mid, end);
				try {
					th.join();
				} catch(InterruptedException e) {
					th.interrupt();
				}
			}
		}.handle_range(1, n);
		return (int)(java.util.Arrays.stream(species).filter(t -> (t == 0)).count());
	}
}
