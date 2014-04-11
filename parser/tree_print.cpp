#include "tree_print.h"
#include "tree_nodes.h"
#include <string.h>
#include <stdlib.h>
// Counters
int while_count;
int for_count;
int expr_count;
int stmt_count;
int func_count;
int table_count;
int if_count;
FILE* output;


void print_while(char* parent, struct NWhile* while_loop)
{
    char* current_node, * buffer = new char[33];
    current_node = strcat("while",itoa(while_count,buffer,10));
    fprintf(output,"%s",strcat(current_node, "[label = \"while\"];"));
    fprintf(output,"%s",strcat(strcat(parent,"--"),strcat(current_node,";")));
    while_count++;
    print_expr(current_node, while_loop->condition);
    print_stmt_list(current_node, while_loop->body);
}

void print_for(char* parent, struct NFor* for_loop)
{
    char* current_node, * buffer = new char[33];
    current_node = strcat("for",itoa(for_count,buffer,10));
    fprintf(output,"%s",strcat(current_node, "[label = \"for\"];"));
    fprintf(output,"%s",strcat(strcat(parent,"--"),strcat(current_node,";")));
    for_count++;
    print_stmt_list(current_node, for_loop->body);
    print_expr(current_node, for_loop->name);
    print_expr(current_node, for_loop->start);
    print_expr(current_node, for_loop->end);
    print_expr(current_node, for_loop->step);
}

void print_expr(char* parent, struct NExpr* expr)
{
    char* current;
    char* current_node;
    char buffer [33];
    switch (expr->type)
    {
        case EXPR_EQ:
            current = new char[3];
            current = "==";
            break;
        case EXPR_NQ:
            current = new char[3];
            current = "!=";
            break;
        case EXPR_PLUS:
            current = new char[2];
            current = "+";
            break;
        case EXPR_MINUS:
            current = new char[2];
            current = "-";
            break;
        case EXPR_DIV:
            current = new char[2];
            current = "/";
            break;
        case EXPR_MUL:
            current = new char[2];
            current = "*";
            break;
        case EXPR_LE:
            current = new char[3];
            current = "<=";
            break;
        case EXPR_GE:
            current = new char[3];
            current = ">=";
            break;
        case EXPR_LT:
            current = new char[2];
            current = "<";
            break;
        case EXPR_GT:
            current = new char[2];
            current = ">";
            break;
        case EXPR_MOD:
            current = new char[2];
            current = "%";
            break;
        case EXPR_ID:
            current = expr->name;
            break;
        case EXPR_INT:
            current = new char[33];
            current = itoa(expr->Int,buffer,10);
            break;
        case EXPR_DOUBLE:
            current = new char[33];
            sprintf(current, sizeof(current), "%g", expr->Double);
            break;
        case EXPR_CONC:
            current = new char[3];
            current = "..";
            break;
        case EXPR_STR:
            current = expr->name;
            break;
        case EXPR_MET:
            current = new char[5];
            current = "fcall";
            break;
        case EXPR_AND:
            current = new char[3];
            current = "&&";
            break;
        case EXPR_NOT:
            current = new char[2];
            current = "!";
            break;
        case EXPR_OR:
            current = new char[3];
            current = "||";
            break;
        case EXPR_MAS:
            current = new char[3];
            current = "[]";
            break;
        case EXPR_BOOL:
            current = new char[6];
            if(expr->Int)
            {
                current = "true";
            } else
            {
                current = "false";
            break;
        case EXPR_NIL:
            current = new char[4];
            current = "nil";
            break;
        case EXPR_UMIN:
            current = new char[4];
            current = "(-)";
            break;
    }
    current_node = strcat("expr",itoa(expr_count,buffer,10));
    fprintf(output,"%s",strcat(strcat(current_node,strcat(strcat("[label = \"",current), "\"]"),";"));
    fprintf(output,"%s",strcat(strcat(parent,"--"),strcat(current_node,";")));
    expr_count++;
    if (expr->type == EQ || expr->type == NQ || expr->type == PLUS || expr->type == MINUS || expr->type == DIV || expr->type == MUL
        || expr->type == LE || expr->type == GE || expr->type == LT || expr->type == GT || expr->type == MOD || expr->type == CONC
        || expr->type == MET || expr->type == AND || expr->type == OR || expr->type == MAS)
    {
        print_expr(&current_node,expr->left);
        print_expr(&current_node,expr->right);
    } else if (expr->type == NOT || expr->type == UMIN)
    {
        print_expr(&current_node,expr->left);
    }
}

void print_stmt(char* parent, struct NStmt* stmt)
{
    char* current_node, * buffer = new char[33];
    switch(stmt->type)
    {
        case STMT_WHILE:
            print_while(parent,stmt->while_loop);
            break;
        case STMT_FOR:
            print_for(parent,stmt->for_loop);
            break;
        case STMT_EXPR:
            print_expr(parent,stmt->expr);
            break;
        case STMT_FUNC:
            print_func(parent,stmt->func);
            break;
        case STMT_BLOCK:
            print_stmt_list(parent,stmt->list);
            break;
        case STMT_STMT_LFUNC:
            print_fuc(parent,stmt->func);
            break;
        case STMT_REPEAT:
            print_while(parent,stmt->while_loop);
            break;
        case STMT_REPEAT:
            print_while(parent,stmt->while_loop);
            break;
        case STMT_ASSIGN:
            current_node = strcat("assign",itoa(stmt_count,buffer,10));
            fprintf(output,"%s",strcat(current_node, "[label = \"=\"];"));
            fprintf(output,"%s",strcat(strcat(parent,"--"),strcat(current_node,";")));
            print_expr(current_node,stmt->var);
            print_expr(current_node,stmt->expr);
            stmt_count++;
            break;
        case STMT_LASSIGN:
            current_node = strcat("lassign",itoa(stmt_count,buffer,10));
            fprintf(output,"%s",strcat(current_node, "[label = \"=\"];"));
            fprintf(output,"%s",strcat(strcat(parent,"--"),strcat(current_node,";")));
            print_expr(current_node,stmt->var);
            print_expr(current_node,stmt->expr);
            stmt_count++;
            break;
        case STMT_RETURN:
            current_node = strcat("return",itoa(stmt_count,buffer,10));
            fprintf(output,"%s",strcat(current_node, "[label = \"return\"];"));
            fprintf(output,"%s",strcat(strcat(parent,"--"),strcat(current_node,";")));
            stmt_count++;
            break;
        case STMT_BREAK:
            current_node = strcat("break",itoa(stmt_count,buffer,10));
            fprintf(output,"%s",strcat(current_node, "[label = \"break\"];"));
            fprintf(output,"%s",strcat(strcat(parent,"--"),strcat(current_node,";")));
            stmt_count++;
            break;
    }
}

void print_stmt_list(char* parent, struct NStmtList* stmtlist)
{
    struct NStmt current = stmtlist->first;
    while (current != NULL)
    {
        print_stmt(parent,current);
        current = current->next;
    }
}

void print_func(char* parent, struct NFunc* func)
{
    char* current_node, * buffer = new char[33];
    current_node = strcat("func",itoa(func_count,buffer,10));
    fprintf(output,"%s",strcat(current_node, "[label = \"func\"];"));
    fprintf(output,"%s",strcat(strcat(parent,"--"),strcat(current_node,";")));
    func_count++;
    if( func->name != NULL)
    {
        fprintf(output,"%s",strcat(strcat(current_node, "name"), strcat(strcat("[label = \"name =", func->name),"\"];")));
        fprintf(output,"%s",strcat(strcat(parent,"--"),strcat(strcat(current_node, "name"),";")));
    }
    print_expr_list(current_node,func->args);
    print_stmt_list(current_node, func->body);
}

void print_expr_list(char* parent, struct NExprList* exprlist)
{
    struct NExpr current = exprlist->first;
    while (current != NULL)
    {
        print_expr(parent,current);
        current = current->next;
    }
}

void print_if(char* parent, struct NIf* if_tree)
{
    char* current_node, * buffer = new char[33];
    current_node = strcat("if",itoa(if_count,buffer,10));
    fprintf(output,"%s",strcat(current_node, "[label = \"if\"];"));
    fprintf(output,"%s",strcat(strcat(parent,"--"),strcat(current_node,";")));
    if_count++;
    print_expr(current_node,if_tree->condition);
    print_if_list(current_node,if_tree->elseiflist);
    print_stmt_list(current_node, if_tree->elsebody);
}

void print_table(char* parent, struct NTable* table)
{
    char* current_node, * buffer = new char[33];
    current_node = strcat("table",itoa(table_count,buffer,10));
    fprintf(output,"%s",strcat(current_node, "[label = \"table\"];"));
    fprintf(output,"%s",strcat(strcat(parent,"--"),strcat(current_node,";")));
    table_count++;
    struct NExpr current = table->first;
    int i = 0;
    while (current != NULL)
    {
        print_table_elem(current_node,current, i);
        current = current->next;
    }
}

void print_table_elem(char* parent, struct NTblElem elem, int num)
{
    char* current_node, * buffer = new char[33];
    current_node = strcat(parent,itoa(num,buffer,10));
    fprintf(output,"%s",strcat(current_node, "[label = \"tblelem\"];"));
    fprintf(output,"%s",strcat(strcat(parent,"--"),strcat(current_node,";")));
    table_count++;
    if(key != NULL) {
        print_expr(current_node,elem->key);
    }
    print_expr(current_node,elem->value);
}
void print_if_list(char* parent, struct NIfList* iflist)
{
    struct NIf current = exprlist->first;
    while (current != NULL)
    {
        print_if(parent,current);
        current = current->next;
    }
}

void print_tree(struct NStmtList* stmtlist)
{
    while_count = 0;
    for_count = 0;
    expr_count = 0;
    stmt_count = 0;
    func_count = 0;
    table_count = 0;
    if_count = 0;
    char* start = "graph graphname {";
    char* end = "}";
    output = fopen("file.dot","w");
    fprintf(output, "%s\n", start);
    fprintf(output, "%s\n", "root;");
    print_stmt_list("root",root);
    fprintf(output, "%s\n", end);
    close(output);
}
