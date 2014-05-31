package test;

import rtl.Lib;
import rtl.Mixed;
import rtl.Nil;
import rtl.Numeric;
import rtl.Integer;
import rtl.Float;
import rtl.String;
import rtl.Table;
import rtl.Function;

public class Func2 extends Function {
	private Table _G;
	private Mixed cv1;
	
	public Func2(Table _G, Mixed cv1) {
		this._G = _G;
		this.cv1 = cv1;
	}
	
	public Mixed value(Mixed arg1) {
		Lib.print( cv1.clone().add(new String(" ")).add(arg1) );
		return new Nil();
	}
}
