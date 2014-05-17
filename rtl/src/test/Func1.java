package test;

import rtl.Mixed;
import rtl.Numeric;
import rtl.Integer;
import rtl.Float;
import rtl.String;
import rtl.Table;
import rtl.Function;

public class Func1 extends Function {
	private Table _G;
	
	public Func1(Table _G) {
		this._G = _G;
	}
	
	public Mixed value(Mixed arg1) {
		return new Func2(_G, arg1);
	}
}
