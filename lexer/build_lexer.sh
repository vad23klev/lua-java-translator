#!/bin/bash
flex lua.yy
g++ -c lex.yy.c
g++ lex.yy.o -o lexer
