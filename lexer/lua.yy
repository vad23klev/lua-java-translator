%{
  #include<stdio.h>
  #include<string.h>
  char strconst[81] = {0};
%}

%option noyywrap

%x COMMENT
%x STRING
%x SHORTSTRING
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

\"                            { BEGIN(STRING); strcpy(strconst, ""); }
<STRING>\\\"                  strcat(strconst, "\"");
<STRING>\\n                   strcat(strconst, "\n"); 
<STRING>\\\\                  strcat(strconst, "\\");
<STRING>[^\\\n\"]+            strcat(strconst,yytext);
<STRING>\"                    { printf("Found string: \"%s\"\n",strconst); BEGIN(INITIAL); }

[']                           { BEGIN(SHORTSTRING); strcpy(strconst,""); }
<SHORTSTRING>\\\"             strcat(strconst, "\"");
<SHORTSTRING>\\n              strcat(strconst, "\n"); 
<SHORTSTRING>\\\\             strcat(strconst, "\\");
<SHORTSTRING>[^\\\n\\"']+     strcat(strconst,yytext);
<SHORTSTRING>\'               { printf("Found string: \"%s\"\n",strconst); BEGIN(INITIAL); }

"[["                          { BEGIN(MULTISTRING); strcpy(strconst,""); }
<MULTISTRING>\\\"             strcat(strconst, "\"");
<MULTISTRING>\\n              strcat(strconst, "\n"); 
<MULTISTRING>\\\\             strcat(strconst, "\\");
<MULTISTRING>[^\\\n\\"\]\]]+  strcat(strconst,yytext);
<MULTISTRING>"]]"             { printf("Found string: \"%s\"\n",strconst); BEGIN(INITIAL); }

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
"true"                        printf("Found \"true\" boolean value\n");
"false"                       printf("Found \"false\" boolean value\n");

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
"=>"                          printf("Found \"=>\"\n");
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
[ \f\n\r\t\v]                 ;

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
