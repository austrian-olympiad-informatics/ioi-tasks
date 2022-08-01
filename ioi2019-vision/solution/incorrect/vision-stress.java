import java.util.Arrays;

public class vision {

    public void construct_network(int H, int W, int K) {
    	int maxGates = 10000;
    	int maxInputs = 1000000;
    	int cur = H * W - 1;
    	for (int i = 0; i < maxGates - 1; i++) {
            int[] u = new int[1];
            u[0] = cur;
    	    grader.add_or(u);
    	    cur++;
        }
        int inputs = maxInputs - maxGates + 1;
    	int[] a = new int[inputs];
    	Arrays.fill(a, cur);
    	grader.add_or(a);
    }
}