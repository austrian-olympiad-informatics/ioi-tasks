import java.util.ArrayList;

public class vision {

    private int AND(int A, int B)
    {
        int[] gates = {A, B};
        return grader.add_and(gates);
    }

    private int OR(ArrayList<Integer> gates)
    {
        int[] gatesArray = new int[gates.size()];
        for (int i = 0; i < gates.size(); i++)
        {
            gatesArray[i] = gates.get(i);
        }
        return grader.add_or(gatesArray);
    }

    private void try_append_gate(ArrayList<Integer> gates, int x, int y, int W, int H)
    {
        if (x >= 0 && y >= 0 && x < W && y < H)
        {
            gates.add(y * W + x);
        }
    }

    public void construct_network(int H, int W, int K)
    {
        ArrayList<Integer> global = new ArrayList<>();
        for (int x = 0; x < W; x++)
        {
            for (int y = 0; y < H; y++)
            {
                ArrayList<Integer> gates = new ArrayList<>();
                int dx = 0, dy = -K;
                int ddx = 1, ddy = 1;
                for (int step = 0; step <= 4 * K; step++)
                {
                    try_append_gate(gates, x + dx, y + dy, W, H);
                    if (step == K)
                    {
                        ddx = -1;
                    }
                    if (step == 2 * K)
                    {
                        ddy = -1;
                    }
                    if (step == 3 * K)
                    {
                        ddx = 1;
                    }
                    dx += ddx;
                    dy += ddy;
                }
                if (gates.size() > 0)
                {
                    global.add(AND(y * W + x, OR(gates)));
                }
            }
        }
        OR(global);
    }
}
