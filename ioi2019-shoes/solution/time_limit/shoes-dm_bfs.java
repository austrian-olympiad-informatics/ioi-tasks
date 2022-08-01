import java.util.HashMap;
import java.util.Queue;
import java.util.LinkedList;
import java.util.ArrayList;
import java.util.stream.Collectors;

public class shoes {
	private HashMap<String, Integer> m = new HashMap<>();
	private Queue<ArrayList<Integer>> q = new LinkedList<>();

	private Boolean isAcceptable(ArrayList<Integer> S)
	{
		for (int i = 0; i < S.size(); i += 2)
		{
			if (S.get(i) > 0 || S.get(i) + S.get(i + 1) != 0)
			{
				return false;
			}
		}
		return true;
	}

	private String listToString(ArrayList<Integer> l)
	{
		return l.stream().map(Object::toString).collect(Collectors.joining("."));
	}

	private void change(int[] v) {
		int n = v.length / 2;

		int[] cnt = new int[n];
		for(int i = 0; i < n; i++)
			cnt[i] = 0;

		for(int i = 0; i < 2*n; i++)
			if(v[i] > 0)
				cnt[v[i] - 1]++;

		for(int i = 1; i < n; i++)
			cnt[i] += cnt[i-1];


		int[] cntl = new int[n];
		int[] cntr = new int[n];
		for(int i = 0; i < n; i++) {
			cntl[i] = cnt[i];
			cntr[i] = cnt[i];
		}

		for(int i = 0; i < 2*n; i++)
			if(v[i] > 0)
				v[i] = (--cntr[v[i] - 1]) + 1;
			else
				v[i] = -((--cntl[-v[i] - 1]) + 1);
	}


	public long count_swaps(int[] S)
	{
		change(S);
		ArrayList<Integer> l = new ArrayList<>();
		for (int value : S) {
			l.add(value);
		}
		if (isAcceptable(l))
		{
			return 0;
		}
		q.add(l);
		m.put(listToString(l), 0);
		while (true)
		{
			ArrayList<Integer> initial = q.remove();
			int curSwaps = m.get(listToString(initial));
			for (int i = 1; i < l.size(); i++)
			{
				ArrayList<Integer> cur = (ArrayList<Integer>)initial.clone();
				int t = cur.get(i - 1);
				cur.set(i - 1, cur.get(i));
				cur.set(i, t);
				if (!m.containsKey(listToString(cur)))
				{
					if (isAcceptable(cur))
					{
						return curSwaps + 1;
					}
					q.add(cur);
					m.put(listToString(cur), curSwaps + 1);
				}
			}
		}
	}
}
