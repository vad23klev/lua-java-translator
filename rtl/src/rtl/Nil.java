package rtl;

public class Nil extends Mixed {

	@Override
	public boolean equals(Object other) {
		return other instanceof Nil;
	}

	@Override
	public java.lang.String toString() {
		return "nil";
	}

	@Override
	public Mixed clone() {
		return new Nil();
	}

	@Override
	public int hashCode() {
		return 0;
	}
}
