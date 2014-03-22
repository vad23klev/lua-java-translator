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
%token CONCAT
%token TRUE
%token FALSE
%token NIL

%type <Expr> expr;
%type <SL> stmts;
%type <Func> func;

%%

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
         | INT
         | DOUBLE
         | expr '+' expr
         | expr '-' expr
         | expr '*' expr
         | expr '\\' expr
         | expr '%' expr
         | STRING
         | STRING CONC ID
         | ID CONC STRING
         | '(' expr ')'
         | STRING CONC STRING
         | ID CONC ID
         | ID '[' expr ']'
         | ID '.' ID
         | ID '.' ID '(' args ')'
;

args:    /* empty */
         | argsn
;

argsn:   expr
         | argsn ',' expr
;

stmt:    WHILE expr stmts END
         | IF expr THEN stmts END
         | IF expr THEN stmts ELSE stmts END
         | IF expr THEN stmts ELSEIF expr stmts END
         | FOR ID '=' expr ',' expr DO stmts END
         | FOR ID '=' expr ',' expr ',' expr DO stmts END
         | expr
         | func
;

stmts:   /* empty */
         | stmt
         | stmts stmt
;

root:    stmts
;

func:    FUNC ID '(' func_args_e ')' stmts END
         | FUNC ID ':' ID '(' func_args_e ')' stmts END
;

func_args:  ID
            | func_args ',' ID
;

func_args_e: /* empty */
             | func_args
;

tbl_elem:   ID '=' expr 
            | '[' expr ']' '=' expr
            | expr
            | func
            | ID '=' func
            | '[' expr ']' func
;

tbl_elem_list: /* empty */
               | tbl_elem_listn
;

tbl_elem_listn: tbl_elem
	            | tbl_elem_listn ',' tbl_elem
;
