import java.util.*;

public class shoes {
    static final int max_states = 3628800; // n=5 --> 10!
	int n;
	int[] dis = new int[max_states];
	int head, tail;
	int[] que = new int[max_states];

	boolean isGood(int[] S) {
		for(int i = 0; i < 2*n; i += 2)
			if (S[i] > 0 || S[i] + S[i + 1] != 0)
				return false;
		return true;
	}

	int getID(int[] S) {
		int res = 0;
		int f = 1;
		for(int i = 2*n; i-- > 0; ) {
			int cnt = 0;
			for(int j = i+1; j < 2*n; j++)
				cnt += (S[i] > S[j] ? 1 : 0);
			res += cnt * f;
			f *= 2*n - i;
		}
		return res;
	}

	void getArr(int id, int[] S) {
		for(int i = 2*n; i-- > 0; ) {
			int t = 2*n - i;
			S[i] = id % t;
			id /= t;
			for(int j = i+1; j < 2*n; j++)
				if(S[j] >= S[i])
					S[j]++;
		}
		for(int i = 0; i < 2*n; i++)
			S[i] = (S[i] < n ? S[i]-n : S[i]-n+1);
	}

	void push(int[] S, int distance) {
		int id = getID(S);
		if(dis[id] != -1) return;
		dis[id] = distance;
		que[tail++] = id;
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


	public long count_swaps(int[] S) {
		change(S);
		n = S.length / 2;
		if (isGood(S)) return 0;

		Arrays.fill(dis, -1);
		head = tail = 0;

		push(S, 0);
		while(head < tail) {
			int id = que[head++];
			getArr(id, S);
			for(int i = 1; i < 2*n; i++) {
				int x = S[i];
				S[i] = S[i-1];
				S[i-1] = x;
				if(isGood(S))
					return dis[id] + 1;
				push(S, dis[id] + 1);
				S[i-1] = S[i];
				S[i] = x;
			}
		}

		return -1;
	}
}
