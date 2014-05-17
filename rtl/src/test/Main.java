package test;

import rtl.Lib;
import rtl.Mixed;
import rtl.Numeric;
import rtl.Integer;
import rtl.Float;
import rtl.String;
import rtl.Table;
import rtl.Function;

/*
	Translated program:
	
	a = 1
	
	function f(myarg)
	    return function(arg)
	        print(myarg + " " + arg)
	    end
	end
	
	f2 = f('test')
	f2('lua')
	
	print(f2)
 */

public class Main {
	static Table _G = new Table();
	
	public static void main(java.lang.String args[]) {
		_G.put(new String("f"), new Func1(_G));
		_G.put(new String("a"), new Integer(1));
		_G.put(  new String("f2"), ( (Func1)_G.get(new String("f")) ).value( new String("test") )  );
		((Func2)_G.get(new String("f2"))).value(new String("lua"));
		Lib.print(_G.get(new String("f2")));
	}
}
