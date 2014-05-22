%{
    #include <stdio.h>
    #include "lua.tab.h"
    #include "tree_nodes_funcs.h"
    
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
    struct NTable* Table;
    struct NTblElem* TblElem;
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
%type <Expr> alone_id
%type <Expr> func_call
%type <SL> stmt_list
%type <SL> root
%type <SL> stmt_block
%type <While> stmt_while
%type <While> stmt_repeat
%type <Stmt> stmt
%type <If> stmt_if
%type <For> stmt_for
%type <Func> func_decl_named
%type <Func> func_decl_anon
%type <Func> func_body
%type <IfL> elseif_list
%type <Args> id_chain
%type <Args> var_list
%type <Args> arg_list
%type <Args> args
%type <Args> arg_list_decl
%type <Args> args_decl
%type <Table> tableconstructor
%type <Table> tbl_elem_list
%type <Table> tbl_elems
%type <TblElem> tbl_elem
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
                    | ';' opt_endl
;

opt_endl:             /* empty */
                    | ENDL
;

opt_endl_list:        opt_endl
                    | opt_endl_list ',' opt_endl
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
                    | RETURN expr end_expr                                      { $$ = create_stmt_return($2); }
                    | expr end_expr                                             { $$ = create_stmt_expr($1); }
                    | var '=' expr end_expr                                     { $$ = create_stmt_assign($1, $3, 0); }
                    | LOCAL var '=' expr end_expr                               { $$ = create_stmt_assign($2, $4, 1); }
                    | func_decl_named                                           { $$ = create_stmt_func($1, 0); }
                    | LOCAL func_decl_named                                     { $$ = create_stmt_func($2, 1); }
                    | ENDL                                                      { $$ = create_stmt_spec(2); }
                    | var_list '=' args                                         { $$ = create_stmt_assign(create_expr_exprlist($1), create_expr_exprlist($3), 0); }
;

stmt_block:           DO stmt_list END opt_endl                                 { $$ = $2; }
;

stmt_if:              IF expr THEN stmt_list elseif_list END opt_endl  { $$ = create_if($2, $4, $5, create_stmt_list(NULL)); }
                    | IF expr THEN stmt_list elseif_list ELSE stmt_list END opt_endl { $$ = create_if($2, $4, $5, $7); }
;

elseif_list:          /* empty */                                               { $$ = create_if_list(NULL); }
                    | elseif_list ELSEIF expr THEN stmt_list                    { $$ = add_if_to_list($1, create_if($3, $5, create_if_list(NULL), create_stmt_list(NULL))); }
;

stmt_while:           WHILE expr stmt_block                                     { $$ = create_while($2, $3); }
;

stmt_for:             FOR alone_id '=' expr ',' expr          stmt_block        { $$ = create_for($2, $4, $6, create_expr_int(1), $7); }
                    | FOR alone_id '=' expr ',' expr ',' expr stmt_block        { $$ = create_for($2, $4, $6, $8, $9); }
;

stmt_repeat:          REPEAT stmt_list UNTIL expr end_expr                      { $$ = create_while($4, $2); }
;


/* == Expressions == */
alone_id:             ID                                                        { $$ = create_expr_id(yylval.Id); }
;

id_chain:             alone_id                                                  { $$ = create_expr_list($1); }
                    | id_chain '.' alone_id                                     { $$ = add_expr_to_list($1, $3); }
;

var:                  id_chain                                                  { $$ = create_expr_exprlist($1); }
                    | var '[' expr ']'                                          { $$ = create_op_expr(EXPR_MAS, $1, $3); }
;

var_list:             var                                                       { $$ = create_expr_list($1); }
                    | var_list ',' var                                          { $$ = add_expr_to_list($1, $3); }
;

expr:                 var                                                       { $$ = $1; }
                    | INT                                                       { $$ = create_expr_int(yylval.Int); }
                    | DOUBLE                                                    { $$ = create_expr_double(yylval.Double); }
                    | STRING                                                    { $$ = create_expr_string(yylval.String); }
                    | TRUE                                                      { $$ = create_expr_boolean(1); }
                    | FALSE                                                     { $$ = create_expr_boolean(0); }
                    | NIL                                                       { $$ = create_expr_nil(); }
                    | NOT expr                                                  { $$ = create_op_expr(EXPR_NOT, $2, NULL); }
                    | '-' expr %prec UMINUS                                     { $$ = create_op_expr(EXPR_UMIN, $2, NULL); }
                    | expr AND expr                                             { $$ = create_op_expr(EXPR_AND, $1, $3); }
                    | expr OR  expr                                             { $$ = create_op_expr(EXPR_OR, $1, $3); }
                    | expr '+' expr                                             { $$ = create_op_expr(EXPR_PLUS, $1, $3); }
                    | expr '-' expr                                             { $$ = create_op_expr(EXPR_MINUS, $1, $3); }
                    | expr '*' expr                                             { $$ = create_op_expr(EXPR_MUL, $1, $3); }
                    | expr '/' expr                                             { $$ = create_op_expr(EXPR_DIV, $1, $3); }
                    | expr '%' expr                                             { $$ = create_op_expr(EXPR_MOD, $1, $3); }
                    | expr '^' expr                                             { $$ = create_op_expr(EXPR_POW, $1, $3); }
                    | expr '>' expr                                             { $$ = create_op_expr(EXPR_GT, $1, $3); }
                    | expr '<' expr                                             { $$ = create_op_expr(EXPR_LT, $1, $3); }
                    | expr GE  expr                                             { $$ = create_op_expr(EXPR_GE, $1, $3); }
                    | expr LE  expr                                             { $$ = create_op_expr(EXPR_LE, $1, $3); }
                    | expr EQ  expr                                             { $$ = create_op_expr(EXPR_EQ, $1, $3); }
                    | expr NE  expr                                             { $$ = create_op_expr(EXPR_NQ, $1, $3); }
                    | expr CONCAT expr                                          { $$ = create_op_expr(EXPR_CONC, $1, $3); }
                    | '(' expr ')'                                              { $$ = $2; }
                    | func_call                                                 { $$ = $1; }
                    | tableconstructor                                          { $$ = create_expr_table($1); }
                    | func_decl_anon                                            { $$ = create_expr_func($1); }
;


/* == Function call == */
func_call:            var '(' arg_list ')'                                      { $$ = create_op_expr(EXPR_MET, $1, create_expr_exprlist($3)); }
                    | var ':' alone_id '(' arg_list ')'                         { add_expr_to_list($1->idlist, $3); $$ = create_op_expr(EXPR_MET, $1, create_expr_exprlist($5)); }
;

arg_list:             /* empty */                                               { $$ = create_expr_list(NULL); }
                    | args                                                      { $$ = $1; }
;

args:                 expr                                                      { $$ = create_expr_list($1); }
                    | args ',' expr                                             { $$ = add_expr_to_list($1, $3); }
;


/* == Function declaration == */
func_decl_anon:       FUNCTION func_body                                       { $$ = $2; }
;

func_decl_named:      FUNCTION id_chain func_body                              { $$ = set_func_name($2, $3); }
                    | FUNCTION id_chain ':' alone_id func_body                 { $$ = set_func_name(add_expr_to_list($2, $4), $5); }
;

func_body:            '(' arg_list_decl ')' stmt_list END                      { $$ = create_func($2, $4); }
;

arg_list_decl:        /* empty */                                              { $$ = create_expr_list(NULL); }
                    | args_decl                                                { $$ = $1; }
;

args_decl:            alone_id                                                 { $$ = create_expr_list($1); }
                    | args_decl ',' alone_id                                   { $$ = add_expr_to_list($1, $3); }
;


/* == Table declaration == */
tableconstructor:     '{' opt_endl_list tbl_elem_list opt_endl_list '}'         { $$ = $3; }
;

tbl_elem_list:        /* empty */                                               { $$ = create_table(NULL); }
                    | tbl_elems                                                 { $$ = $1; }
;

tbl_elems:            tbl_elem                                                  { $$ = create_table($1); }
                    | tbl_elems opt_endl_list ',' opt_endl_list tbl_elem        { $$ = add_elem_to_table($1, $5); }
;

tbl_elem:             alone_id '=' expr                                         { $$ = create_tbl_elem($1, $3); }
                    | '[' expr ']' '=' expr                                     { $$ = create_tbl_elem($2, $5); }
                    | expr                                                      { $$ = create_tbl_elem(NULL, $1); }
;

%%
