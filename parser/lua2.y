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
%type <SL> stmt_list
/* %type <Func> func    какое именно объявление тут должно быть? */

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
stmt_list:            /* empty */
                    | stmt_list stmt
;

stmt:                 stmt_block
                    | stmt_if
                    | stmt_while
                    | stmt_for
                    | stmt_repeat
                    | BREAK end_expr
                    | RETURN end_expr
                    | expr end_expr
                    | var '=' expr end_expr
                    | LOCAL var '=' expr end_expr
                    | func_decl_named
                    | LOCAL func_decl_named
;

stmt_block:           DO stmt_list END
;

stmt_if:              IF expr THEN stmt_list elseif_list END
                    | IF expr THEN stmt_list ELSE stmt_list elseif_list END
;

elseif_list:          /* empty */
                    | elseif_list ELSEIF expr THEN stmt_list
;

stmt_while:           WHILE expr block
;

stmt_for:             FOR ID '=' expr ',' expr          block
                    | FOR ID '=' expr ',' expr ',' expr block
;

stmt_repeat:          REPEAT stmt_list UNTIL expr end_expr
;


/* == Expressions == */
var:                  ID
                    | var '[' expr ']'
                    | var '.' ID
;

expr:                 var
                    | INT
                    | DOUBLE
                    | STRING
                    | TRUE
                    | FALSE
                    | NIL
                    | NOT expr
                    | '-' expr %prec UMINUS
                    | expr AND expr
                    | expr OR  expr
                    | expr '+' expr
                    | expr '-' expr
                    | expr '*' expr
                    | expr '/' expr
                    | expr '%' expr
                    | expr '^' expr
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
                    | var ':' ID '(' arg_list ')'
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

func_decl_named:      FUNCTION var func_body
                    | FUNCTION var ':' ID func_body
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

tbl_elem:             ID '=' expr
                    | '[' expr ']' '=' expr
                    | expr
;
