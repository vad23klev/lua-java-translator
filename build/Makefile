all: clean lualp

lualp: lua2.tab.o lex.yy.o
	g++ lex.yy.o lua2.tab.o -o lualp

lua2.tab.o: lua2.tab.c
	g++ -c lua2.tab.c

lex.yy.o: lex.yy.c
	g++ -c lex.yy.c

lex.yy.c:
	flex ../lexer/lua.yy

lua2.tab.c:
	cp ../parser/tree_nodes.h ./
	bison -d ../parser/lua2.y



clean:
	rm -rf *.o *.h *.c lualp
