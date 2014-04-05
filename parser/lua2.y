%{
    #include <stdio.h>
    #include "tree_nodes.h"
    
    extern int yylex(void);
    
    void yyerror(const char *s)
    {
        fprintf(stderr, "Line %d: %s\n", yylloc.first_line, s);
    }

    struct NStmtList* root;
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
    struct NExprList * Args;
    struct NIf * If;
    struct NIfList * IfL;
}

%locations

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
%type <Expr> var
%type <SL> stmt_list
%type <SL> root
%type <SL> stmt_block
%type <While> stmt_while
%type <While> stmt_repeat
%type <Stmt> stmt
%type <If> stmt_if
%type <For> stmt_for
%type <Func> func_decl_named
%type <IfL> elseif_list
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

root:                 stmt_list                                                 { root=$1; $$=$1; }
;


/* == Statements == */
stmt_list:            /* empty */                                               { $$ = create_stmt_list(NULL); }
                    | stmt_list stmt                                            { $$ = add_stmt_to_list($1, $2); }
;

stmt:                 stmt_block                                                { $$ = create_stmt_block($1); }
                    | stmt_if                                                   { $$ = create_stmt_if($1); }
                    | stmt_while                                                { $$ = create_stmt_while($1, 0); }
                    | stmt_for                                                  { $$ = create_stmt_for($1); }
                    | stmt_repeat                                               { $$ = create_stmt_while($1, 1); }
                    | BREAK end_expr                                            { $$ = create_stmt_spec(0); }
                    | RETURN end_expr                                           { $$ = create_stmt_spec(1); }
                    | expr end_expr                                             { $$ = create_stmt_expr($1); }
                    | var '=' expr end_expr                                     { $$ = create_stmt_assign($1, $3, 0); }
                    | LOCAL var '=' expr end_expr                               { $$ = create_stmt_assign($2, $4, 1); }
                    | func_decl_named                                           { $$ = create_stmt_func($1, 0); }
                    | LOCAL func_decl_named                                     { $$ = create_stmt_func($2, 1); }
;

stmt_block:           DO stmt_list END                                          { $$ = $2; }
;

stmt_if:              IF expr THEN stmt_list elseif_list END                    { $$ = create_if($2, $4, $5, NULL); }
                    | IF expr THEN stmt_list elseif_list ELSE stmt_list END     { $$ = create_if($2, $4, $5, $7); }
;

elseif_list:          /* empty */                                               { $$ = create_if_list(NULL); }
                    | elseif_list ELSEIF expr THEN stmt_list                    { $$ = add_if_to_list($1, create_if($3, $5, NULL, NULL)); }
;

stmt_while:           WHILE expr stmt_block                                     { $$ = create_while($2, $3); }
;

stmt_for:             FOR ID '=' expr ',' expr          stmt_block              { $$ = create_for(create_expr_id(yyval.Id), $4, $6, create_expr_int(yyval.Int), $7); }
                    | FOR ID '=' expr ',' expr ',' expr stmt_block              { $$ = create_for(create_expr_id(yyval.Id), $4, $6, $8, $9); }
;

stmt_repeat:          REPEAT stmt_list UNTIL expr end_expr                      { $$ = create_while($4, $2); }
;


/* == Expressions == */
id_chain:             ID
                    | id_chain '.' ID
;

var:                  id_chain
                    | var '[' expr ']'
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

func_decl_named:      FUNCTION id_chain func_body
                    | FUNCTION id_chain ':' ID func_body
;

func_body:            '(' arg_list_decl ')' stmt_list END
;

arg_list_decl:        /* empty */
                    | args_decl
;

args_decl:            ID
                    | args_decl ',' ID
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

%%
