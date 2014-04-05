all: clean lualp

lualp: lua2.tab.o lex.yy.o
	g++ lex.yy.o lua2.tab.o -o lualp

lua2.tab.o: lua2.tab.c tree_nodes.h
	g++ -c lua2.tab.c

lex.yy.o: lex.yy.c
	g++ -c lex.yy.c

lex.yy.c:
	flex ../lexer/lua.yy

lua2.tab.c:
	bison -d ../parser/lua2.y

tree_nodes.h:
	cp ../parser/tree_nodes.h ./


clean:
	rm -rf *.o *.h *.c lualp
