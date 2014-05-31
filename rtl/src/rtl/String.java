package rtl;

/**
 * Lua string data type.
 */
public class String extends Mixed {
	public java.lang.String value = new java.lang.String();
	
	public String(java.lang.String val) {
		if (val == null) {
			throw new Error("Attempt to construct string with null initializer.");
		}
		
		value = val;
	}
	
	public String(char val) {
		value = "" + val;
	}
	
	/**
	 * Returns a string containing a character at given position.
	 * @param val Character position.
	 * @return String with one char.
	 */
	public Mixed charAt(Integer val) {
		// TODO val = 0 should return nil
		return new String(value.charAt(val.value - 1));
	}
	
	/**
	 * Concatenates this string with numeric value.
	 * @param other Number to concatenate with (second operand).
	 * @return Concatenated string.
	 */
	@Override
	public Mixed add(Mixed other) {
		if (other instanceof Integer) {
			return new String(value + ((Integer)other).value);
		
		} else if (other instanceof Float) {
			return new String(value + ((Float)other).value);
		
		} else if (other instanceof String) {
			return new String(value + ((String)other).value);
		
		} else {
			throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
		}
	}

	@Override
	public boolean equals(Object other) {
		if (other instanceof String) {
			return value.equals(((String)other).value);
		} else {
			return false;
		}
	}

	@Override
	public java.lang.String toString() {
		return new java.lang.String(value);
	}

	@Override
	public Mixed clone() {
		return new String(value);
	}

	@Override
	public int hashCode() {
		return value.hashCode();
	}
}
