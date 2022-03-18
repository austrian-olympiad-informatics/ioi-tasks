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
  	int centre = 0;
  	for (int i = 0; i < n; i++) {
  	  if(adj[i].size() > 2) {
  	    centre = i;
  	  }
  	}
  	labels[centre] = 0;
  	for (int i = 0; i < adj[centre].size(); i++) {
  	  dfs(adj[centre].get(i), centre, i * MAXN + 1); // encode with branch * MAXN + index
  	}
  	return labels;
  }

  int find_next_station(int s, int t, int[] c) {
    if (s == 0) { // special case for centre
      return t / MAXN * MAXN + 1;
    } else if (s / MAXN == t / MAXN) { // same branch
      return s < t ? s + 1 : s - 1;
    } else return c[0]; // different branch, move towards smaller index or centre
  }
}
