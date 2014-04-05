%{
  #include <stdio.h>
  #include <string.h>
  
  #include "lua2.tab.h"
  
  char strconst[1281] = {0};
  int line = 0;
  
  extern int yyparse(void);
%}

%option noyywrap
%location

%x COMMENT
%x STRING_A
%x STRING_Q
%x MULTISTRING
%x SHORTCOMMENT

%%

"--"                          BEGIN(SHORTCOMMENT);
<SHORTCOMMENT>[^\n]*          ;
<SHORTCOMMENT>"\n"            {
                                  line++;
                                  BEGIN(INITIAL);
                              }

"--[["                        BEGIN(COMMENT);
<COMMENT>[^--]*               ;
<COMMENT>--[^\]\]]            ;
<COMMENT>--]]                 BEGIN(INITIAL);
<COMMENT>.|[\r\f\t\v]         ;
<COMMENT>[\n]                 line++;

\"                            { BEGIN(STRING_Q); strcpy(strconst, ""); }
<STRING_Q>\\\"                strcat(strconst, "\"");
<STRING_Q>\\n                 strcat(strconst, "\n"); 
<STRING_Q>\\\\                strcat(strconst, "\\");
<STRING_Q>[^\\\n\"]+          strcat(strconst,yytext);
<STRING_Q>\"                  {
                                  yylval.String = (char *)malloc(strlen(strconst) + 1);
                                  strcpy(yylval.String, strconst);
                                  BEGIN(INITIAL);
                                  yyloc.first_line = line;
                                  return STRING;
                              }
<STRING_Q>.|[\r\f\t\v]        ;
<STRING_Q>[\n]                line++;

\'                            { BEGIN(STRING_A); strcpy(strconst, ""); }
<STRING_A>\\\"                strcat(strconst, "\"");
<STRING_A>\\n                 strcat(strconst, "\n"); 
<STRING_A>\\\\                strcat(strconst, "\\");
<STRING_A>[^\\\n\']+          strcat(strconst,yytext);
<STRING_A>\'                  {
                                  yylval.String = (char *)malloc(strlen(strconst) + 1);
                                  strcpy(yylval.String, strconst);
                                  BEGIN(INITIAL);
                                  yyloc.first_line = line;
                                  return STRING;
                              }
<STRING_A>.|[\r\f\t\v]        ;
<STRING_A>[\n]                line++;


"[["                          { BEGIN(MULTISTRING); strcpy(strconst,""); }
<MULTISTRING>[^\]\n]*         strcat(strconst,yytext);
<MULTISTRING>\][^\]\n]*       strcat(strconst,yytext);
<MULTISTRING>[\n]             {
                                  strcat(strconst, "\n");
                                  line++;
                              }
<MULTISTRING>"]]"             {
                                  yylval.String = (char *)malloc(strlen(strconst) + 1);
                                  strcpy(yylval.String, strconst);
                                  BEGIN(INITIAL);
                                  yyloc.first_line = line;
                                  return STRING;
                              }

"while"                       { yyloc.first_line = line; return WHILE; }
"end"                         { yyloc.first_line = line; return END; }
"for"                         { yyloc.first_line = line; return FOR; }
"if"                          { yyloc.first_line = line; return IF; }
"elseif"                      { yyloc.first_line = line; return ELSEIF; }
"else"                        { yyloc.first_line = line; return ELSE; }
"do"                          { yyloc.first_line = line; return DO; }
"then"                        { yyloc.first_line = line; return THEN; }
"repeat"                      { yyloc.first_line = line; return REPEAT; }
"until"                       { yyloc.first_line = line; return UNTIL; }
"return"                      { yyloc.first_line = line; return RETURN; }
"break"                       { yyloc.first_line = line; return BREAK; }

"not"                         { yyloc.first_line = line; return NOT; }
"and"                         { yyloc.first_line = line; return AND; }
"or"                          { yyloc.first_line = line; return OR; }

"nil"                         { yyloc.first_line = line; return NIL; }
"true"                        { yyloc.first_line = line; return TRUE; }
"false"                       { yyloc.first_line = line; return FALSE; }

"local"                       { yyloc.first_line = line; return LOCAL; }
"function"                    { yyloc.first_line = line; return FUNCTION; }


".."                          { yyloc.first_line = line; return CONCAT; }
">"                           { yyloc.first_line = line; return '>'; }
"<"                           { yyloc.first_line = line; return '<'; }
"-"                           { yyloc.first_line = line; return '-'; }
"\*"                          { yyloc.first_line = line; return '*'; }
"/"                           { yyloc.first_line = line; return '/'; }
"%"                           { yyloc.first_line = line; return '%'; }
"=="                          { yyloc.first_line = line; return EQ; }
">="                          { yyloc.first_line = line; return GE; }
"<="                          { yyloc.first_line = line; return LE; }
"~="                          { yyloc.first_line = line; return NE; }
"="                           { yyloc.first_line = line; return '='; }
","                           { yyloc.first_line = line; return ','; }
"+"                           { yyloc.first_line = line; return '+'; }
"."                           { yyloc.first_line = line; return '.'; }
"("                           { yyloc.first_line = line; return '('; }
")"                           { yyloc.first_line = line; return ')'; }
"{"                           { yyloc.first_line = line; return '{'; }
"}"                           { yyloc.first_line = line; return '}'; }
"["                           { yyloc.first_line = line; return '['; }
"]"                           { yyloc.first_line = line; return ']'; }
";"                           { yyloc.first_line = line; return ';'; }
"#"                           { yyloc.first_line = line; return '#'; }
":"                           { yyloc.first_line = line; return ':'; }

"0x"[A-F0-9]+                 {
                                  yylval.Int = strtol(yytext,NULL,16);
                                  yyloc.first_line = line;
                                  return INT;
                              }

[+-]?[0-9]+                   {
                                  yylval.Int = atoi(yytext);
                                  yyloc.first_line = line;
                                  return INT;
                              }

([_]|[a-z])+[a-zA-Z0-9_]*     {
                                  yylval.Id = (char *)malloc(sizeof(yytext)+1);
                                  strcpy(yylval.Id, yytext);
                                  yyloc.first_line = line;
                                  return ID;
                              }

[+-]?[0-9]+\.[0-9eE-]+        {
                                  yylval.Double = atof(yytext);
                                  yyloc.first_line = line;
                                  return DOUBLE;
                              }

[^ \f\n\r\t\v]                printf("Unknown symbol: %s\n",yytext);

.|[\r\t\f\v]                  ;
[\n]                          line++;

<<EOF>>                       return 0;

%%

int main(int argc,char* argv[])
{
    if (argc > 1) 
    {
        yyin = fopen(argv[1], "r");
        yyparse();
    }
    return 0;
}
