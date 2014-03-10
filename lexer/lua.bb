%{
    #include <stdio.h>
	
%}

%token NUM
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
%token STRING
%token ID

%%

NUM:      DIGIT
        | NUM DIGIT    { $$ = $1*10+$2; }
        | NUM '+' NUM
        | NUM '-' NUM
        | NUM '*' NUM
        | NUM '\' NUM
        | NUM '%' NUM
;

DIGIT:    '0' { $$=0; } | '1' { $$=1; } | '2' { $$=2; } | '3' { $$=3; }
        | '4' { $$=4; } | '5' { $$=5; } | '6' { $$=6; } | '7' { $$=7; }
        | '8' { $$=8; } | '9' { $$=9; }
;

EXPR:
           EXPR 'and' EXPR
         | EXPR 'or' EXPR
         | 'not' EXPR
         | EXPR '==' EXPR
         | EXPR '~=' EXPR
         | EXPR '>' EXPR
         | EXPR '<' EXPR
         | EXPR '>=' EXPR
         | EXPR '<=' EXPR
         | ID (ARGS)
         | ID
         | NUM
         | STRING '..' ID
         | ID '..' STRING
         | (EXPR)
;

ARGS:      EXPR
         | ARGS ',' EXPR
;

OP:
           WHILE EXPR STMS
         | IF EXPR STMS
;

STM:
           OP
         | ID
         | ID (ARGS)
;

STMS:
           STM
         | STMS STM
;