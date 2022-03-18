import java.util.ArrayList;

public class stations {
  private static final int MAXN = 1000;
  
  private int cnt;
  private ArrayList<Integer>[] adj;
  private int[] labels;
  
  private void dfs(int v, int p) {
	  labels[v] = (++cnt) * MAXN;
  	for (Integer i : adj[v]) if (i != p) dfs(i, v);
  	labels[v] += cnt;
  }

  int[] label(int n, int k, int[] u, int[] v) {
    cnt = -1;
    adj = new ArrayList[n];
    for (int i = 0; i < n; i++) {
      adj[i] = new ArrayList<Integer>();
    }
  	for (int i = 0; i < n - 1; i++) {
  		adj[u[i]].add(v[i]);
  		adj[v[i]].add(u[i]);
  	}
  	labels = new int[n];
  	dfs(0, -1);
  	return labels;
  }

  int find_next_station(int s, int t, int[] c) {
  	int s_in = s / MAXN, s_out = s % MAXN, t_in = t / MAXN;
  	if (t_in < s_in || t_in > s_out) return c[0];
  	for (int i = 0; i < c.length; i++) {                     
		  if (c[i] / MAXN < s_in) continue; // do not process parent                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
  		if (t_in <= c[i] % MAXN) return c[i];
  	}
  	return -1;
  }
}
