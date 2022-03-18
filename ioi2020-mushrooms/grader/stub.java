/*
 * Judge grader for Java
 * Task: mushrooms
 * Author: Kian Mirjalali
 */
import java.io.PrintWriter;

public class stub {

	private static int n;
	private static InputReader fifo_in = null;
	private static PrintWriter fifo_out = null;

	private static void die(int exit_code, String messageFmt, Object... messageArgs) {
		if (messageFmt != null)
			System.err.println(String.format(messageFmt, messageArgs));
		if (fifo_in != null)
			fifo_in.close();
		if (fifo_out != null)
			fifo_out.close();
		System.exit(exit_code);
	}
	private static void die(int exit_code) {
		die(exit_code, null);
	}
	private static void die() {
		die(0);
	}

	private static int readFifoInteger(String name) {
		try {
			return fifo_in.readInt();
		} catch (java.util.InputMismatchException e) {
			die(3, "Grader error: Could not read %s.", name);
			return 0;
		}
	}

	private static void wrong_if(boolean cond, String message) {
		if (cond) {
			fifo_out.printf("W %s\n", message);
			fifo_out.flush();
			die();
		}
	}

	public static synchronized int use_machine(int[] x) {
		final int k = x.length;
		wrong_if(k > n, "Too large array for query.");
		fifo_out.printf("Q %d", k);
		for (int i = 0; i < k; i++)
			fifo_out.printf(" %d", x[i]);
		fifo_out.printf("\n");
		fifo_out.flush();
		int response = readFifoInteger("query response");
		if (response < 0) // 'Die' sent from manager
			die();
		return response;
	}

	public static void main(String... args) {
		try {
			System.in.close();
		} catch (java.io.IOException e) {
		}
		System.out.close();
		if (args.length < 2)
			die(1, "Grader error: Invalid number of arguments: %d", args.length);
		try {
			fifo_in = new InputReader(new java.io.FileInputStream(args[0]));
		} catch (java.io.IOException e) {
			die(2, "Grader error: Could not open file '%s' for reading.", args[0]);
		}
		try {
			fifo_out = new PrintWriter(new java.io.FileOutputStream(args[1], true));
		} catch (java.io.IOException e) {
			die(2, "Grader error: Could not open file '%s' for writing.", args[1]);
		}

		n = readFifoInteger("'n'");
		mushrooms sol = new mushrooms();
		int answer = sol.count_mushrooms(n);
		fifo_out.printf("A %d\n", answer);
		fifo_out.flush();
		die();
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
