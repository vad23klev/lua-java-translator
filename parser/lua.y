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

%start ROOT
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



%type <Expr> EXPR;
%type <SL> STMS;
%type <Func> FUNCTION;

%%

NUM:    INT
        | DOUBLE
        | NUM '+' NUM
        | NUM '-' NUM
        | NUM '*' NUM
        | NUM '\\' NUM
        | NUM '%' NUM
;

EXPR:    EXPR AND EXPR
         | EXPR OR EXPR
         | NOT EXPR
         | EXPR EQ EXPR
         | EXPR NQ EXPR
         | EXPR '>' EXPR
         | EXPR '<' EXPR
         | EXPR GT EXPR
         | EXPR LT EXPR
         | ID '(' ARGS ')'
         | ID ARGS
         | ID
         | NUM
         | STRING CONC ID
         | ID CONC STRING
         | '(' EXPR ')'
         | STRING CONC STRING
         | ID CONC ID
         | ID '[' EXPR ']'
         | ID '.' ID
         | ID '.' ID '(' ARGS ')'
;

ARGS:
         | ARGSN
;

ARGSN:   EXPR
         | ARGSN ',' EXPR
;

OP:      WHILE EXPR STMS END
         | IF EXPR THEN STMS END
         | IF EXPR THEN STMS ELSE STMS END
         | IF EXPR THEN STMS ELSEIF EXPR STMS END
         | FOR ID '=' ID ',' ID DO STMS END
         | FOR ID '=' ID ',' NUM DO STMS END
         | FOR ID '=' NUM ',' ID DO STMS END
         | FOR ID '=' NUM ',' NUM DO STMS END
;

STMS:
         | EXPR
         | STMS EXPR
         | STMS OP
         | OP
;

ROOT:    STMS
;

FUNCTION: FUNC ID '(' FARGSN ')' STMS END
          | FUNC ID ':' ID '(' FARGSN ')' STMS END
;

FARGS:  ID
        | FARGS ',' ID
;

FARGSN: 
        | FARGS
;
