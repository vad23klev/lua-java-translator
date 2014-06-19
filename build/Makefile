all: clean lualp

lualp: lua.tab.o lex.yy.o
	g++ lex.yy.o lua.tab.o -o lualp

lua.tab.o: lua.tab.c tree_nodes.h tree_nodes_funcs.h tree_print.h semantic_tables.h
	g++ -c lua.tab.c

lex.yy.o: lex.yy.c
	g++ -c lex.yy.c

lex.yy.c:
	flex ../lexer/lua.yy

lua.tab.c:
	bison -d ../parser/lua.y

tree_nodes.h:
	cp ../parser/tree_nodes.h ./

tree_nodes_funcs.h:
	cp ../parser/tree_nodes_funcs.h ./

tree_print.h:
	cp ../parser/tree_print.h ./

semantic_tables.h:
	cp ../parser/semantic_tables.h ./

clean:
	rm -rf *.o *.h *.c lualp
