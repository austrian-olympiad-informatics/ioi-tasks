import java.util.ArrayList;

public class stations {
  private static final int MAXN = 1000;
  
  private int cnt;
  private ArrayList<Integer>[] adj;
  private int[] labels;
  
  private void dfs(int v, int p, int d) {
  	if(d == 0) labels[v] = cnt++;
  	for (Integer i : adj[v]) if (i != p) dfs(i, v, d ^ 1);
  	if(d == 1) labels[v] = cnt++;
  }

  int[] label(int n, int k, int[] u, int[] v) {
    cnt = 0;
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
  	if (s < c[0]) {
  		if (t < s || t >= c[c.length - 1]) return c[c.length - 1];
  		for (int i : c) if (i >= t) return i;
    	return c[c.length - 1];
  	}
  	if (t > s || t <= c[0]) return c[0];
  	int ans = c[0];
  	for (int i : c) if (i <= t) ans = i;
  	return ans;
  }
}
