package rtl;

/**
 * Lua float data type.
 */
public class Float extends Numeric {
	public double value;
	
	public Float(double val) {
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
	public java.lang.String toString() {
		return "" + value;
	}

	@Override
	public Mixed clone() {
		return new Float(value);
	}
	
	@Override
	public Mixed add(Mixed other) {
		if (other instanceof Float) {
			return new Float(value + ((Float)other).value);
		} else {
			return new Float(value + ((Integer)other).value);
		}
	}

	@Override
	public Mixed sub(Mixed other) {
		if (other instanceof Float) {
			return new Float(value - ((Float)other).value);
		} else {
			return new Float(value - ((Integer)other).value);
		}
	}

	@Override
	public Mixed mul(Mixed other) {
		if (other instanceof Float) {
			return new Float(value * ((Float)other).value);
		} else {
			return new Float(value * ((Integer)other).value);
		}
	}

	@Override
	public Mixed div(Mixed other) {
		if (other instanceof Float) {
			return new Float(value / ((Float)other).value);
		} else {
			return new Float(value / ((Integer)other).value);
		}
	}

	@Override
	public Mixed mod(Mixed other) {
		if (other instanceof Float) {
			return new Float(value % ((Float)other).value);
		} else {
			return new Float(value % ((Integer)other).value);
		}
	}

	@Override
	public int hashCode() {
		return (int)value;
	}
}
