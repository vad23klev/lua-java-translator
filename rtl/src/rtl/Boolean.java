package rtl;

public class Boolean extends Mixed {
	boolean value;
	
	public Boolean(boolean val) {
		value = val;
	}
	
	@Override
	public boolean equals(Object other) {
		if (other instanceof Boolean) {
			return value == ((Boolean)other).value;
		} else {
			return false;
		}
	}

	@Override
	public java.lang.String toString() {
		return value ? "true" : "false";
	}

	@Override
	public Mixed clone() {
		return new Boolean(value);
	}

	@Override
	public Mixed add(Mixed other) {
		if (other instanceof Boolean) {
			return new Boolean(value || ((Boolean)other).value);
		} else {
			throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
		}
	}

	@Override
	public Mixed sub(Mixed other) {
		throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
	}

	@Override
	public Mixed mul(Mixed other) {
		if (other instanceof Boolean) {
			return new Boolean(value && ((Boolean)other).value);
		} else {
			throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
		}
	}

	@Override
	public Mixed div(Mixed other) {
		throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
	}

	@Override
	public Mixed mod(Mixed other) {
		throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
	}

	@Override
	public int hashCode() {
		return value ? 1 : 0;
	}

}
