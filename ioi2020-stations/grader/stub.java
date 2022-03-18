import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.util.InputMismatchException;

public class stub {
	
	private static InputReader inputReader;
	private static PrintWriter outputWriter; 

	public static void main(String[] args) {
		if (args.length != 3) {
			System.err.println("grader error");
			System.err.println("number of arguments isn't 3");
			System.exit(0);
		}
		try {
			inputReader = new InputReader(new FileInputStream(args[0]));
			outputWriter = new PrintWriter(new File(args[1]));
		} catch (FileNotFoundException e) {
			System.err.println("grader error");
			e.printStackTrace(System.err);
			System.exit(0);
		}
		stations solver = new stations();
		if (args[2].equals("0")) { // grader A
			int r = 0;
			try {
				r = inputReader.readInt();
			} catch (InputMismatchException e) {
				System.err.println("grader A error");
				System.err.println("could not read 'r'");
				System.exit(0);
			}
			for (int i = 0; i < r; i++) {
				int n = 0, k = 0;
				try {
					n = inputReader.readInt();
					k = inputReader.readInt();
				} catch (InputMismatchException e) {
					System.err.println("grader A error");
					System.err.println("could not read 'n' and 'k'");
					System.exit(0);
				}
				if (n < 0 || k < 0) { // error in labels
					System.exit(0);
				}
				int[] u = new int[n - 1];
				int[] v = new int[n - 1];
				for (int j = 0; j < n - 1; j++) {
					try {
						u[j] = inputReader.readInt();
						v[j] = inputReader.readInt();
					} catch (InputMismatchException e) {
						System.err.println("grader A error");
						System.err.println("could not read edges");
						System.exit(0);
					}
				}
				int[] labels = solver.label(n, k, u, v);
				outputWriter.println(labels.length);
				if (labels.length != n) {
					outputWriter.flush();
					System.exit(0);
				}
				for (int j : labels) {
					outputWriter.println(j);
				}
				outputWriter.flush();
			}
		} else if (args[2].equals("1")) { 
			int q = 0;
			try {
				q = inputReader.readInt();
			} catch (InputMismatchException e) {
				System.err.println("grader B error");
				System.err.println("could not read 'q'");
				System.exit(0);
			}
			if (q < 0) { // error in labels
				System.exit(0);
			}
			for (int i = 0; i < q; i++) {
				int s = 0, t = 0, l = 0;
				try {
					s = inputReader.readInt();
					t = inputReader.readInt();
					l = inputReader.readInt();
				} catch (InputMismatchException e) {
					System.err.println("grader B error");
					System.err.println("could not read query");
					System.exit(0);
				}
				if (s < 0 || t < 0 || l < 0) { // error in answer
					System.exit(0);
				}
				int[] c = new int[l];
				for (int j = 0; j < l; j++) {
					try {
						c[j] = inputReader.readInt();
					} catch (InputMismatchException e) {
						System.err.println("grader B error");
						System.err.println("could not read adjacency list");
						System.exit(0);
					}
				}
				outputWriter.println(solver.find_next_station(s, t, c));
				outputWriter.flush();
			}
		} else {
			System.err.println("grader error");
			System.err.println("unknown grader type " + args[2]);
			System.exit(0);
		}
	}
}

class InputReader {
	private InputStream stream;
	private byte[] buf = new byte[1024];
	private int curChar;
	private int numChars;

	public InputReader(InputStream stream) {
		this.stream = stream;
	}

	public int read() {
		if (numChars == -1) {
			throw new InputMismatchException();
		}
		if (curChar >= numChars) {
			curChar = 0;
			try {
				numChars = stream.read(buf);
			} catch (IOException e) {
				throw new InputMismatchException();
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
				throw new InputMismatchException();
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
