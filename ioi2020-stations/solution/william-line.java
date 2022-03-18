import java.util.ArrayList;

public class stations {
  private static final int MAXN = 1000;
  
  private ArrayList<Integer>[] adj;
  private int[] labels;
  
  private void dfs(int v, int p, int d) {
  	labels[v] = d;
  	for (Integer i : adj[v]) if (i != p) dfs(i, v, d + 1);
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
  	for (int i = 0; i < n; i++) {
  	  if(adj[i].size() == 1) {
  	    dfs(i, -1, 0);
  	    return labels;
  	  }
  	}
  	return labels;
  }

  int find_next_station(int s, int t, int[] c) {
    return s < t ? s + 1 : s - 1;
  }
}
