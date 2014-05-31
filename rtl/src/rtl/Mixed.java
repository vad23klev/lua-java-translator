package rtl;

/**
 * Lua mixed data type.
 */
public abstract class Mixed {
	public static final java.lang.String EXCEPTION_TYPE_NOT_ALLOWED = "Operation not allowed with these types.";
	
	@Override
	public abstract boolean equals(Object other);
	
	@Override
	public abstract java.lang.String toString();
	
	@Override
	public abstract Mixed clone();
	
	@Override
	public abstract int hashCode();
	
	/**
	 * Addition operation;
	 * @param other Second operand.
	 * @return Operation result.
	 */
	public Mixed add(Mixed other) {
		throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
	}
	
	/**
	 * Subtraction operation;
	 * @param other Second operand.
	 * @return Operation result.
	 */
	public Mixed sub(Mixed other) {
		throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
	}
	
	/**
	 * Multiplication operation;
	 * @param other Second operand.
	 * @return Operation result.
	 */
	public Mixed mul(Mixed other) {
		throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
	}
	
	/**
	 * Division operation;
	 * @param other Second operand.
	 * @return Operation result.
	 */
	public Mixed div(Mixed other) {
		throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
	}
	
	/**
	 * Modulo operation;
	 * @param other Second operand.
	 * @return Operation result.
	 */
	public Mixed mod(Mixed other) {
		throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
	}
	
	/**
	 * Returns a value associated with the given key.
	 * @param key Pair key.
	 * @return Value or null if no value associated with the given key.
	 */
	public Mixed get(Mixed key) {
		throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
	}
	
	/**
	 * Adds a key-value pair to this table.
	 * @param key Pair key.
	 * @param val Pair value.
	 */
	public Mixed put(Mixed key, Mixed val) {
		throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
	}
	
	/**
	 * Removes a key-value pair by the given key.
	 * @param key Pair key.
	 */
	public Mixed remove(Mixed key) {
		throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
	}
	
	
	/* Logical */
	
	public Mixed not() {
		throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
	}
	
	public Mixed and(Mixed other) {
		throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
	}
	
	public Mixed or(Mixed other) {
		throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
	}
	
	
	
	public Mixed eq(Mixed other) {
		return new Boolean(this.equals(other));
	}
	
	public Mixed neq(Mixed other) {
		return this.eq(other).not();
	}
	
	public Mixed gr(Mixed other) {
		throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
	}
	
	public Mixed greq(Mixed other) {
		return this.eq(other).and(this.gr(other));
	}
	
	public Mixed loeq(Mixed other) {
		return this.gr(other).not();
	}
	
	public Mixed lo(Mixed other) {
		return this.greq(other).not();
	}
}
