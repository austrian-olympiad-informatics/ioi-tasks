import java.util.ArrayList;

public class grader {
	private static final int MAX_GATES = 10_000;
	private static final int MAX_INPUTS = 1_000_000;

	private static final int AND = 0;
	private static final int OR = 1;
	private static final int XOR = 2;
	private static final int NOT = 3;

	private static boolean increasing(int a, int b, int c) {
		return a <= b && b <= c;
	}

	private static class Network {

		private static class Gate {
			private int type;
			private int[] input_indexes;

			private Gate(int type, int[] input_indexes) {
				this.type = type;
				this.input_indexes = input_indexes.clone();
			}
		}
		
		private int input_size;
		private int total_inputs;
		private ArrayList<Gate> gates;

		private Network(int input_size) {
			this.input_size = input_size;
			this.total_inputs = 0;
			this.gates = new ArrayList<Gate>();
		}
		
		private int add_gate(int type, int[] input_indexes) {
			if (input_indexes.length == 0)
				WA("Instruction with no inputs");
			
			if (gates.size() + 1 > MAX_GATES)
				WA("Too many instructions");
			
			if (total_inputs + input_indexes.length > MAX_INPUTS)
				WA("Too many inputs");
			
			gates.add(new Gate(type, input_indexes));
			total_inputs += input_indexes.length;
			int new_index = input_size + gates.size() - 1;
			
			for (int input_index : input_indexes)
				if (!increasing(0, input_index, new_index-1))
					WA("Invalid index");
			
			return new_index;
		}
		
		private void print() {
			System.out.println(gates.size());
			StringBuilder sb = new StringBuilder();
			for (Gate gate : gates) {
				sb.append(gate.type);
				sb.append(" ");
				if (gate.type == NOT) {
					sb.append(gate.input_indexes[0]);
				} else {
					sb.append(gate.input_indexes.length);
					for (int input : gate.input_indexes) {
						sb.append(" ");
						sb.append(input);
					}
				}
				sb.append('\n');
			}
			System.out.print(sb.toString());
		}
	}


	private static Network network;

	private static void general_output(String verdict, String reason) {
		final String output_secret = "b17553fd-ba5a-4140-836c-491f938c515b";
		System.out.println(output_secret);
		System.out.println(verdict);
		if (!"OK".equals(verdict)) {
			if (reason != null)
				System.out.println(reason);
		} else {
			network.print();
		}
		System.out.close();
		System.exit(0);
	}

	private static void WA(String reason) {
		general_output("WA", reason);
	}

	public static void main(String[] args) {
		int H, W, K;
		try (InputReader inputReader = new InputReader(System.in)) {
			final String input_secret = "c2675211-ade0-44b0-8c15-741dd835f3d2";
			String secret = inputReader.readString();
			if (!input_secret.equals(secret))
				general_output("SV", null);
			H = inputReader.readInt();
			W = inputReader.readInt();
			K = inputReader.readInt();
		}
		
		network = new Network(H * W);
		vision solver = new vision();
		solver.construct_network(H, W, K);
		
		general_output("OK", null);
	}

	public static int add_and(int[] Ns) {
		return network.add_gate(AND, Ns);
	}

	public static int add_or(int[] Ns) {
		return network.add_gate(OR, Ns);
	}

	public static int add_xor(int[] Ns) {
		return network.add_gate(XOR, Ns);
	}

	public static int add_not(int N) {
		int[] Ns = {N};
		return network.add_gate(NOT, Ns);
	}
}


class InputReader implements java.io.Closeable {
	private java.io.InputStream stream;
	private byte[] buf = new byte[1024];
	private int curChar;
	private int numChars;

	public InputReader(java.io.InputStream stream) {
		this.stream = stream;
	}
	
	@Override
	public void close() {
		try {
			this.stream.close();
		} catch (java.io.IOException e) {
		}
	}

	public int read() {
		if (numChars == -1) {
			throw new java.util.InputMismatchException();
		}
		if (curChar >= numChars) {
			curChar = 0;
			try {
				numChars = stream.read(buf);
			} catch (java.io.IOException e) {
				throw new java.util.InputMismatchException();
			}
			if (numChars <= 0) {
				return -1;
			}
		}
		return buf[curChar++];
	}

	public int readInt() {
		int c = eatWhite();
		int sgn = 1;
		if (c == '-') {
			sgn = -1;
			c = read();
		}
		int res = 0;
		do {
			if (c < '0' || c > '9') {
				throw new java.util.InputMismatchException();
			}
			res *= 10;
			res += c - '0';
			c = read();
		} while (!isSpaceChar(c));
		return res * sgn;
	}

	public String readString() {
		int c = eatWhite();
		StringBuilder res = new StringBuilder();
		do {
			if (Character.isValidCodePoint(c))
				res.appendCodePoint(c);
			c = read();
		} while (!isSpaceChar(c));
		return res.toString();
	}

	public String readLine() {
		StringBuilder res = new StringBuilder();
		while (true) {
			int c = read();
			if (c == '\n' || c == '\r' || c == -1)
				break;
			if (Character.isValidCodePoint(c))
				res.appendCodePoint(c);
		}
		return res.toString();
	}

	private int eatWhite() {
		int c = read();
		while (isSpaceChar(c)) {
			c = read();
		}
		return c;
	}

	public static boolean isSpaceChar(int c) {
		return c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == -1;
	}
}
