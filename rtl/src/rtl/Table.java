package rtl;

import java.util.Hashtable;

/**
 * Lua table type.
 */
public class Table extends Mixed {
	private Hashtable<Mixed, Mixed> value = new Hashtable<>();
	
	@Override
	public Mixed get(Mixed key) {
		return value.get(key);
	}
	
	@Override
	public Mixed put(Mixed key, Mixed val) {
		value.put(key, val);
		return this;
	}
	
	@Override
	public Mixed remove(Mixed key) {
		value.remove(key);
		return this;
	}

	@Override
	public boolean equals(Object other) {
		throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
	}

	@Override
	public java.lang.String toString() {
		return "table: @" + System.identityHashCode(this);
	}

	@Override
	public Mixed clone() {
		throw new Error(Mixed.EXCEPTION_TYPE_NOT_ALLOWED);
	}

	@Override
	public int hashCode() {
		return value.hashCode();
	}
}
