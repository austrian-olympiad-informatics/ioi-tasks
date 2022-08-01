public class vision {

    private int AND(int A, int B)
    {
        int[] gates = {A, B};
        return grader.add_and(gates);
    }

    private int OR(int A, int B)
    {
        int[] gates = {A, B};
        return grader.add_or(gates);
    }

    private int[] diagonal(int H, int W, int N, boolean D)
    {
        int first, last, step;
        if (D)
        {
            first = N < W ? N : W * (N - W + 2) - 1;
            last = N < H ? N * W : (H - 1) * W + (N - H + 1);
            step = W - 1;
        }
        else
        {
            first = N < W ? W - 1 - N : W * (N - W + 1);
            last = N < H ? (N + 1) * W - 1 : H * W + (H - 2 - N);
            step = W + 1;
        }
        int[] cells = new int[(last == first ? 0 : (last - first) / step) + 1];
        int cur = first;
        int cnt = 0;
        while (true)
        {
            cells[cnt] = cur;
            if (cur == last)
            {
                break;
            }
            cnt++;
            cur += step;
        }
        return cells;
    }

    private int bottom_right_position(int H, int W)
    {
        int[] lastRow = new int[W];
        int[] conditions = new int[H * W - 1];
        int conditionsCount = 0;
        for (int y = 0; y < H; y++)
        {
            for (int x = 0; x < W; x++)
            {
                int[] gates = new int[(x > 0 ? 1 : 0) + (y > 0 ? 1 : 0)];
                int gatesCount = 0;
                if (x > 0)
                {
                    gates[gatesCount] = lastRow[x - 1];
                    gatesCount++;
                }
                if (y > 0)
                {
                    gates[gatesCount] = lastRow[x];
                }
                int gate = W * y + x;
                if (gates.length > 0)
                {
                    conditions[conditionsCount] = AND(gate, grader.add_or(gates));
                    conditionsCount++;
                }
                int[] allGates = new int[gates.length + 1];
                System.arraycopy(gates, 0, allGates, 0, gates.length);
                allGates[gates.length] = gate;
                lastRow[x] = grader.add_or(allGates);
            }
        }
        return grader.add_or(conditions);
    }

    public void construct_network(int H, int W, int K)
    {
        int pairs = H + W - 1 - K;
        int[] topRight = new int[pairs];
        int[] topLeft = new int[pairs];
        for (int i = 0; i < pairs; i++)
        {
            topRight[i] = AND(grader.add_or(diagonal(H, W, i, true)), grader.add_or(diagonal(H, W, i + K, true)));
            topLeft[i] = AND(grader.add_or(diagonal(H, W, i, false)), grader.add_or(diagonal(H, W, i + K, false)));
        }
        int bottomRightPosition = bottom_right_position(H, W);
        OR(
            AND(grader.add_or(topRight), bottomRightPosition),
            AND(grader.add_or(topLeft), grader.add_not(bottomRightPosition))
        );
    }
}
