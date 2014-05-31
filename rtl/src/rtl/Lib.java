package rtl;

public class Lib {
	private static final java.lang.String EXCEPTION_TYPE_MISMATCH = "[ERROR] Type mismatch."; 
	
	public static Mixed lassert(Mixed v, Mixed msg) {
		if (v.equals(new Boolean(false))) {
			error(msg);
			return null;
		} else {
			return new String("assertion " + msg.toString() + " passed");
		}
	}
	
	public static Mixed lassert(Mixed v) {
		return lassert(v, new String("assertion failed!"));
	}
	
	public static void error(Mixed msg, Mixed level) {
		throw new Error(msg.toString());
	}
	
	public static void error(Mixed msg) {
		error(msg, new Integer(0));
	}
	
	public static Mixed tonumber(Mixed e) {
		if (e instanceof Numeric) {
			return e
		} else if (e instanceof String) {
			return new Integer(java.lang.Integer.parseInt( ((String)e).value ));
		} else {
			return new Nil();
		}
	}
	
	public static Mixed tonumber(Mixed e, Mixed base) {
		throw new Error("[ERROR] tonumber: base not supported.");
	}
	
	public static Mixed tostring(Mixed e) {
		return new String(e.toString());
	}
	
	public static void print(Mixed val) {
		System.out.println(val.toString());
	}
	
	public static Mixed ioread(Mixed format) {
		java.util.Scanner scanner = new java.util.Scanner(System.in);
		java.lang.String vf = null;
		
		if (format instanceof String) {
			vf = ((String)format).value;
			if (vf == "*n") {
				return new Float(scanner.nextDouble());
			} else if (vf == "*l") {
				return new String(scanner.nextLine());
			} else {
				throw new Error("[ERROR] io.read: Quantifier not supported.");
			}
		} else if (format instanceof Integer) {
			return new String(scanner.next(".{" + ((Integer)format).value + "}"));
		} else {
			throw new Error(EXCEPTION_TYPE_MISMATCH);
		}
	}
}
