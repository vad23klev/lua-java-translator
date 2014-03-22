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
%token FUNCTION
%token LOCAL
%token <String> STRING
%token <Id> ID
%token EQ
%token NE
%token LE
%token GE
%token CONCAT
%token TRUE
%token FALSE
%token NIL
%token ENDL

%type <Expr> expr
%type <SL> stmts
%type <Func> func

%left  OR
%left  AND
%left  '<' '>' LE GE EQ NE
%right CONCAT
%left  '+' '-'
%left  '*' '/' '%'
%left  NOT UMINUS
%right '^'
%nonassoc ')'

%%

end_expr:             ENDL
                    | ';'
;

root:                 stmt_list
;


/* == Statements == */
stmt_list:            stmt
                    | stmt_list stmt
;

stmt:                 stmt_if
                    | stmt_while
                    | stmt_for
                    | expr end_expr
                    | var '=' expr end_expr
                    | func_decl_named
;

stmt_if:              IF expr THEN stmt_list END
                    | IF expr THEN stmt_list ELSE stmt_list END
;

stmt_while:           WHILE expr DO stmt_list END
;

stmt_for:             FOR ID '=' expr ',' expr          DO stmt_list END
                    | FOR ID '=' expr ',' expr ',' expr DO stmt_list END
;


/* == Expressions == */
/* Костыль? В ANTLR-грамматике lua разделяют lvalue (var) и expr... */
var:                  ID
                    | var '[' expr ']'
;

expr:                 var
                    | INT
                    | DOUBLE
                    | STRING
                    | NOT expr
                    | expr AND expr
                    | expr OR  expr
                    | expr '+' expr
                    | expr '-' expr
                    | expr '*' expr
                    | expr '/' expr
                    | expr '%' expr
                    | expr '>' expr
                    | expr '<' expr
                    | expr GE  expr
                    | expr LE  expr
                    | expr EQ  expr
                    | expr NE  expr
                    | expr CONCAT expr
                    | '(' expr ')'
                    | func_call
                    | tableconstructor
                    | func_decl_anon
;


/* == Function call == */
func_call:            var '(' arg_list ')'
;

arg_list:             /* empty */
                    | args
;

args:                 expr
                    | args ',' expr
;


/* == Function declaration == */
func_decl_anon:       FUNCTION func_body
;

func_decl_named:      FUNCTION ID func_body
;

func_body:            '(' arg_list ')' stmt_list END
;


/* == Table declaration == */
tableconstructor:     '{' tbl_elem_list '}'
;

tbl_elem_list:        /* empty */
                    | tbl_elems
;

tbl_elems:            tbl_elem
                    | tbl_elems ',' tbl_elem
;

tbl_elem:             STRING '=' expr
                    | '[' expr ']' '=' expr
;
