package rtl;

import java.util.Hashtable;

/**
 * Lua table type.
 */
public class Table extends Mixed {
	private Hashtable<Mixed, Mixed> value = new Hashtable<>();
	
	/**
	 * Returns a value associated with the given key.
	 * @param key Pair key.
	 * @return Value or null if no value associated with the given key.
	 */
	public Mixed get(Mixed key) {
		return value.get(key);
	}
	
	/**
	 * Adds a key-value pair to this table.
	 * @param key Pair key.
	 * @param val Pair value.
	 */
	public void put(Mixed key, Mixed val) {
		value.put(key, val);
	}
	
	/**
	 * Removes a key-value pair by the given key.
	 * @param key Pair key.
	 */
	public void remove(Mixed key) {
		value.remove(key);
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
		return value.hashCode();
	}
}
