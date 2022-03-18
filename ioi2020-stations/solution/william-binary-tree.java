import java.util.ArrayList;

public class stations {
  private static final int MAXN = 1000;
  
  private int[] labels;

  int[] label(int n, int k, int[] u, int[] v) {
  	labels = new int[n];
  	for (int i = 0; i < n; i++) {
  	  labels[i] = i;
  	}
  	return labels;
  }

  int find_next_station(int s, int t, int[] c) {
    ++s; ++t;
    while (s < t) {
      if (t / 2 == s) return t - 1;
      t /= 2;
    }
    return s / 2 - 1;
  }
}
