package rtl;

/**
 * Lua integer data type.
 */
public class Integer extends Numeric {
	public int value;
	
	public Integer(int val) {
		value = val;
	}

	@Override
	public boolean equals(Object other) {
		if (other instanceof Integer) {
			return this.value == ((Integer)other).value;
		} else if (other instanceof Float) {
			return this.value == ((Float)other).value;
		} else {
			return false;
		}
	}

	@Override
	public Mixed gr(Mixed other) {
		if (other instanceof Integer) {
			return new Boolean(this.value > ((Integer)other).value);
		} else if (other instanceof Float) {
			return new Boolean(this.value > ((Float)other).value);
		} else {
			throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
		}
	}
	
	@Override
	public java.lang.String toString() {
		return "" + value;
	}

	@Override
	public Mixed clone() {
		return new Integer(value);
	}
	
	@Override
	public Mixed add(Mixed other) {
		if (other instanceof Float) {
			return new Float(value + ((Float)other).value);
		
		} else if (other instanceof Integer) {
			return new Integer(value + ((Integer)other).value);
		
		} else {
			throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
		}
	}

	@Override
	public Mixed sub(Mixed other) {
		if (other instanceof Float) {
			return new Float(value - ((Float)other).value);
		
		} else if (other instanceof Integer) {
			return new Integer(value - ((Integer)other).value);
		
		} else {
			throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
		}
	}

	@Override
	public Mixed mul(Mixed other) {
		if (other instanceof Float) {
			return new Float(value * ((Float)other).value);
		
		} else if (other instanceof Integer) {
			return new Integer(value * ((Integer)other).value);
		
		} else {
			throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
		}
	}

	@Override
	public Mixed div(Mixed other) {
		if (other instanceof Float) {
			return new Float(value / ((Float)other).value);
		
		} else if (other instanceof Integer) {
			return new Integer(value / ((Integer)other).value);
		
		} else {
			throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
		}
	}

	@Override
	public Mixed mod(Mixed other) {
		if (other instanceof Float) {
			return new Float(value % ((Float)other).value);
		
		} else if (other instanceof Integer) {
			return new Integer(value % ((Integer)other).value);
		
		} else {
			throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
		}
	}

	@Override
	public int hashCode() {
		return value;
	}
}
