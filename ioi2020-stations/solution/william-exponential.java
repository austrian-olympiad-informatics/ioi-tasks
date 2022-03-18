import java.util.ArrayList;

public class stations {
  private static final int MAXN = 1000;
  
  private ArrayList<Integer>[] adj;
  private int[] labels;
  
  private void dfs(int v, int p, int d) {
    int cnt = 0;
  	labels[v] = d;
  	for (Integer i : adj[v]) if (i != p) dfs(i, v, (d << 3) + (++cnt));
  }

  int[] label(int n, int k, int[] u, int[] v) {
    adj = new ArrayList[n];
    for (int i = 0; i < n; i++) {
      adj[i] = new ArrayList<Integer>();
    }
  	for (int i = 0; i < n - 1; i++) {
  		adj[u[i]].add(v[i]);
  		adj[v[i]].add(u[i]);
  	}
  	labels = new int[n];
  	dfs(0, -1, 0);
  	return labels;
  }

  int find_next_station(int s, int t, int[] c) {
  	if (t < s) {
  		return s >> 3;
  	}
  	while (s < t) {
  		if ((t >> 3) == s) {
  			return t;
  		}
  		t >>= 3;
  	}
  	return s >> 3;
  }
}
