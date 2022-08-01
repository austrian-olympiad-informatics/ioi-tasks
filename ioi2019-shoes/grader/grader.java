
public class grader {
	public static void main(String[] args) {
		InputReader inputReader = new InputReader(System.in);
		// BEGIN SECRET
		final String input_secret = "08f55b3f-c300-4051-a472-59ca2a776178";
		final String output_secret = "9ce55564-5404-428a-8d2e-0d809c85101e";
		String secret = inputReader.readString();
		if (!input_secret.equals(secret)) {
			System.out.println(output_secret);
			System.out.println("SV");
			System.out.close();
			System.exit(0);
		}
		// END SECRET
		int n = inputReader.readInt();
		int[] S = new int[2 * n];
		for (int i = 0; i < 2 * n; i++)
			S[i] = inputReader.readInt();
		inputReader.close();
		
		shoes solver = new shoes();
		long result = solver.count_swaps(S);
		
		// BEGIN SECRET
		System.out.println(output_secret);
		System.out.println("OK");
		// END SECRET
		System.out.println(result);
		System.out.close();
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
