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
	public abstract Mixed add(Mixed other);
	
	/**
	 * Subtraction operation;
	 * @param other Second operand.
	 * @return Operation result.
	 */
	public abstract Mixed sub(Mixed other);
	
	/**
	 * Multiplication operation;
	 * @param other Second operand.
	 * @return Operation result.
	 */
	public abstract Mixed mul(Mixed other);
	
	/**
	 * Division operation;
	 * @param other Second operand.
	 * @return Operation result.
	 */
	public abstract Mixed div(Mixed other);
	
	/**
	 * Modulo operation;
	 * @param other Second operand.
	 * @return Operation result.
	 */
	public abstract Mixed mod(Mixed other);
}
