%{
  #include<stdio.h>
  #include<string.h>
  char strconst[1281] = {0};
%}

%option noyywrap

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
<STRING_Q>\"                  { printf("Found string: %s\n",strconst); BEGIN(INITIAL); }
<STRING_Q>.|[\n\r\f\t\v]      ;

\'                            { BEGIN(STRING_A); strcpy(strconst, ""); }
<STRING_A>\\\"                strcat(strconst, "\"");
<STRING_A>\\n                 strcat(strconst, "\n"); 
<STRING_A>\\\\                strcat(strconst, "\\");
<STRING_A>[^\\\n\']+          strcat(strconst,yytext);
<STRING_A>\'                  { printf("Found string: %s\n",strconst); BEGIN(INITIAL); }
<STRING_A>.|[\n\r\f\t\v]      ;


"[["                          { BEGIN(MULTISTRING); strcpy(strconst,""); }
<MULTISTRING>[^\]]*           strcat(strconst,yytext);
<MULTISTRING>\][^\]]*         strcat(strconst,yytext);
<MULTISTRING>"]]"             { printf("Found string: %s\n",strconst); BEGIN(INITIAL); }

"while"                       printf("Found \"while\"\n");
"end"                         printf("Found \"end\"\n");
"for"                         printf("Found \"for\"\n");
"if"                          printf("Found \"if\"\n");
"elseif"                      printf("Found \"elseif\"\n");
"else"                        printf("Found \"else\"\n");
"do"                          printf("Found \"do\"\n");
"then"                        printf("Found \"then\"\n");
"repeat"                      printf("Found \"repeat\"\n");
"until"                       printf("Found \"until\"\n");
"return"                      printf("Found \"return\"\n");
"break"                       printf("Found \"break\"\n");

"not"                         printf("Found logical \"not\"\n");
"and"                         printf("Found logical \"and\"\n");
"or"                          printf("Found logical \"or\"\n");

"nil"                         printf("Found \"nil\"\n");
"true"                        printf("Found \"true\" constant\n");
"false"                       printf("Found \"false\" constant\n");

"self"                        printf("Found \"self\" (like this pointer)");

"assert"                      printf("Found \"assert\" call\n");
"collectgarbage"              printf("Found \"collectgarbage\" call\n");
"dofile"                      printf("Found \"dofile\" (include)\n");
"error"                       printf("Found \"error\" call\n");
"getmetatable"                printf("Found \"getmetatable\" call\n");
"ipairs"                      printf("Found \"ipairs\" call (foreach)\n");
"next"                        printf("Found \"next\" call (like foreach)\n");
"pairs"                       printf("Found \"pairs\" call\n");
"print"                       printf("Found \"print\" call\n");
"setmetatable"                printf("Found \"setmetatable\" call\n");
"tonumber"                    printf("Found \"tonumber\" call\n");
"tostring"                    printf("Found \"tostring\" call\n");

"local"                       printf("Found \"local\" (variable declaration)\n");
"function"                    printf("Found \"function\" (function declaration)\n");
"require"                     printf("Found \"require\" (include)\n");


".."                          printf("Found \"..\" like strcat \n");
">"                           printf("Found \">\"\n");
"<"                           printf("Found \"<\"\n");
"-"                           printf("Found \"-\"\n");
"\*"                          printf("Found \"*\"\n");
"/"                           printf("Found \"/\"\n");
"%"                           printf("Found \"%\"\n");
"=="                          printf("Found \"==\"\n");
">="                          printf("Found \"=>\"\n");
"<="                          printf("Found \"<=\"\n");
"~="                          printf("Found \"~=\"\n");
"="                           printf("Found \"=\"\n");
","                           printf("Found \",\"\n");
"+"                           printf("Found \"+\"\n");
"."                           printf("Found \".\"\n");
"("                           printf("Found \"(\"\n");
")"                           printf("Found \")\"\n");
"{"                           printf("Found \"{\"\n");
"}"                           printf("Found \"}\"\n");
"["                           printf("Found \"[\"\n");
"]"                           printf("Found \"]\"\n");
";"                           printf("Found \";\"\n");
"#"                           printf("Found \"#\"\n");
":"                           printf("Found \":\"\n");

"0x"[A-F0-9]+                 printf("Found hexadecimal: %d\n",strtol(yytext,NULL,16));
[+-]?[0-9]+                   printf("Found decimal: %d\n",atoi(yytext));
([_]|[a-z])+[a-zA-Z0-9_]*     printf("Found identifier: %s\n",yytext);
[+-]?[0-9]+\.[0-9eE-]+        printf("Found double: %f\n",atof(yytext));
[^ \f\n\r\t\v]                printf("Unknown symbol: %s\n",yytext);

.|[\n\r\t\f\v]                ;

<<EOF>>                       { printf("<<END OF FILE>>\n"); return 0; }

%%

int main(int argc,char* argv[])
{
    if (argc > 1) 
    {
        yyin = fopen(argv[1], "r");
        yylex();
    }
    return 0;
}
