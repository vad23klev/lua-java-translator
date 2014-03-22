%{
    #include <stdio.h>
    #include <tree_nodes.h>
%}

%union {
    int Int;
    char* Id;
    char* String;
    double Double;
    struct NWhile * While;
    struct NExpr * Expr;
    struct NFor * For;
    struct NStmtList * SL;
    struct NStmt * Stmt;
    struct NFunc * Func;
}

%start root
%token <Int> INT
%token <Double> DOUBLE
%token FOR
%token WHILE
%token END
%token IF
%token ELSE
%token ELSEIF
%token DO
%token THEN
%token REPEAT
%token UNTIL
%token RETURN
%token BREAK
%token FUNC
%token LOCAL
%token <String> STRING
%token <Id> ID
%token EQ
%token NQ
%token LT
%token GT
%token AND
%token NOT
%token OR
%token CONC
%token TRUE
%token FALSE
%token NIL

%type <Expr> expr;
%type <SL> stmts;
%type <Func> funct;

%%

num:    INT
        | DOUBLE
        | num '+' num
        | num '-' num
        | num '*' num
        | num '\\' num
        | num '%' num
;

expr:    expr AND expr
         | expr OR expr
         | NOT expr
         | expr EQ expr
         | expr NQ expr
         | expr '>' expr
         | expr '<' expr
         | expr GT expr
         | expr LT expr
         | ID '(' args ')'
         | ID args
         | ID
         | num
         | STRING CONC ID
         | ID CONC STRING
         | '(' expr ')'
         | STRING CONC STRING
         | ID CONC ID
         | ID '[' expr ']'
         | ID '.' ID
         | ID '.' ID '(' args ')'
;

args:
         | argsn
;

argsn:   expr
         | argsn ',' expr
;

op:      WHILE expr STMS END
         | IF expr THEN stms END
         | IF expr THEN stms ELSE stms END
         | IF expr THEN stms ELSEIF expr stms END
         | FOR ID '=' ID ',' ID DO stms END
         | FOR ID '=' ID ',' num DO stms END
         | FOR ID '=' num ',' ID DO stms END
         | FOR ID '=' num ',' NUM DO stms END
;

stms:
         | expr
	 | stms expr
         | stms op
         | op
         | funct
         | stms funct
;

root:    stms
;

funct: FUNC ID '(' fargsn ')' stms END
          | FUNC ID ':' ID '(' fargsn ')' stms END
;

fargs:  ID
        | fargs ',' ID
;

fargs: 
        | fargs
;

telem: ID '=' expr 
      | '[' expr ']' '=' expr
      | expr
      | funct
      | ID '=' funct
      | '[' expr ']' funct
;

telem_list:
	  | telem_listn
;

telem_listn: telem
	   | telem_listn ',' telem
;
