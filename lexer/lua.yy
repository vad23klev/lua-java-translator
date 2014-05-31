%{

    #include <stdio.h>
    #include <string.h>

    #include "lua.tab.h"
    #include "tree_print.h"

    #define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;

    char strconst[1281] = {0};

    extern int yyparse(void);
%}

%option noyywrap
%option yylineno

%x COMMENT
%x STRING_A
%x STRING_Q
%x MULTISTRING
%x SHORTCOMMENT

%%

"--"                          BEGIN(SHORTCOMMENT);
<SHORTCOMMENT>[^\n]*          ;
<SHORTCOMMENT>"\n"            BEGIN(INITIAL);

"--[["                        BEGIN(COMMENT);
<COMMENT>[^--]*               ;
<COMMENT>--[^\]\]]            ;
<COMMENT>--]]                 BEGIN(INITIAL);
<COMMENT>.|[\n\r\f\t\v]       ;

\"                            { BEGIN(STRING_Q); strcpy(strconst, ""); }
<STRING_Q>\\\"                strcat(strconst, "\"");
<STRING_Q>\\n                 strcat(strconst, "\n");
<STRING_Q>\\\\                strcat(strconst, "\\");
<STRING_Q>[^\\\n\"]+          strcat(strconst,yytext);
<STRING_Q>\"                  {
                                  yylval.String = (char *)malloc(strlen(strconst) + 1);
                                  strcpy(yylval.String, strconst);
                                  BEGIN(INITIAL);
                                  return STRING;
                              }
<STRING_Q>.|[\n\r\f\t\v]      ;

\'                            { BEGIN(STRING_A); strcpy(strconst, ""); }
<STRING_A>\\\"                strcat(strconst, "\"");
<STRING_A>\\n                 strcat(strconst, "\n");
<STRING_A>\\\\                strcat(strconst, "\\");
<STRING_A>[^\\\n\']+          strcat(strconst,yytext);
<STRING_A>\'                  {
                                  yylval.String = (char *)malloc(strlen(strconst) + 1);
                                  strcpy(yylval.String, strconst);
                                  BEGIN(INITIAL);
                                  return STRING;
                              }
<STRING_A>.|[\n\r\f\t\v]      ;


"[["                          { BEGIN(MULTISTRING); strcpy(strconst,""); }
<MULTISTRING>[^\]]*           strcat(strconst,yytext);
<MULTISTRING>\][^\]]*         strcat(strconst,yytext);
<MULTISTRING>"]]"             {
                                  yylval.String = (char *)malloc(strlen(strconst) + 1);
                                  strcpy(yylval.String, strconst);
                                  BEGIN(INITIAL);
                                  return STRING;
                              }

"while"                       return WHILE;
"end"                         return END;
"for"                         return FOR;
"if"                          return IF;
"elseif"                      return ELSEIF;
"else"                        return ELSE;
"do"                          return DO;
"then"                        return THEN;
"repeat"                      return REPEAT;
"until"                       return UNTIL;
"return"                      return RETURN;
"break"                       return BREAK;

"not"                         return NOT;
"and"                         return AND;
"or"                          return OR;

"nil"                         return NIL;
"true"                        return TRUE;
"false"                       return FALSE;

"local"                       return LOCAL;
"function"                    return FUNCTION;


".."                          return CONCAT;
">"                           return '>';
"<"                           return '<';
"-"                           return '-';
"\*"                          return '*';
"/"                           return '/';
"%"                           return '%';
"=="                          return EQ;
">="                          return GE;
"<="                          return LE;
"~="                          return NE;
"="                           return '=';
","                           return ',';
"+"                           return '+';
"."                           return '.';
"("                           return '(';
")"                           return ')';
"{"                           return '{';
"}"                           return '}';
"["                           return '[';
"]"                           return ']';
";"                           return ';';
"#"                           return '#';
":"                           return ':';

"0x"[A-F0-9]+                 {
                                  yylval.Int = strtol(yytext,NULL,16);
                                  return INT;
                              }

[0-9]+                        {
                                  yylval.Int = atoi(yytext);
                                  return INT;
                              }

([_]|[A-Za-z])+[a-zA-Z0-9_]*     {
                                  yylval.Id = (char *)malloc(sizeof(yytext)+1);
                                  strcpy(yylval.Id, yytext);
                                  return ID;
                              }

[+-]?[0-9]+\.[0-9eE-]+        {
                                  yylval.Double = atof(yytext);
                                  return DOUBLE;
                              }

[^ \f\n\r\t\v]                printf("Unknown symbol: %s\n",yytext);

.|[\r\t\f\v]                  ;
\n                            return ENDL;

<<EOF>>                       return 0;

%%

int main(int argc,char* argv[])
{
    if (argc > 1)
    {
        yyin = fopen(argv[1], "r");
        yyparse();
        print_tree(root);
    }
    return 0;
}
