package rtl;

/**
 * Lua function data type.
 */
public class Function extends Mixed {

	@Override
	public boolean equals(Object other) {
		throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
	}

	@Override
	public java.lang.String toString() {
		return "function: @" + System.identityHashCode(this);
	}

	@Override
	public Mixed clone() {
		throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
	}

	@Override
	public Mixed add(Mixed other) {
		throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
	}

	@Override
	public Mixed sub(Mixed other) {
		throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
	}

	@Override
	public Mixed mul(Mixed other) {
		throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
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
		return 0;
	}
	
}
