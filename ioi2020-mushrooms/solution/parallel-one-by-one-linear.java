/*
 * A parallel implementation of the one-by-one solution (creating threads linearly).
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
		java.util.List<Thread> threads = new java.util.ArrayList<>();
		for (int begin_ = 1; begin_ < n; begin_ += block_size) {
			final int begin = begin_;
			final int end = Math.min(n, begin+block_size);
			threads.add(new Thread(() -> {
				for (int i = begin; i < end; i++)
					species[i] = use_machine(new int[] {0, i});
			}));
		}
		for (Thread th : threads)
			th.start();
		for (Thread th : threads)
			try {
				th.join();
			} catch(InterruptedException e) {
			}
		return (int)(java.util.Arrays.stream(species).filter(t -> (t == 0)).count());
	}
}
