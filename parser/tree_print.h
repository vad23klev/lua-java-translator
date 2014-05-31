#include "tree_nodes.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern struct NStmtList *root;
// Counters
int while_count;
int for_count;
int expr_count;
int stmt_count;
int func_count;
int table_count;
int if_count;
FILE* output;

void print_while(char* parent, struct NWhile* while_loop);
void print_for(char* parent, struct NFor* for_loop);
char* print_expr(char* parent, struct NExpr* expr);
void print_stmt(char* parent, struct NStmt* stmt);
void print_stmt_list(char* parent, struct NStmtList* stmtlist);
void print_func(char* parent, struct NFunc* func);
void print_expr_list(char* parent, struct NExprList* exprlist);
void print_if(char* parent, struct NIf* if_tree);
void print_table(char* parent, struct NTable* table);
void print_table_elem(char* parent, struct NTblElem* elem, int num);
void print_if_list(char* parent, struct NIfList* iflist);
void print_tree(struct NStmtList* stmtlist);
void print_func_name(char* parent, struct NExprList* exprlist);


void print_while(char* parent, struct NWhile* while_loop)
{
    char* current_node,  * buffer = (char*)malloc(sizeof(char)*33);
    char* buffer1 = (char*)malloc(sizeof(char)*33);
    sprintf(buffer,"%d", while_count);
    current_node = (char*)malloc(sizeof(char)*33);
    strcpy(current_node, "while");
    strcat(current_node, buffer);
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, current_node);
    strcat(buffer, "[label = \"while\"];\n");
    fprintf(output,"%s", buffer);
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, parent);
    strcat(buffer, "--");
    strcat(buffer, current_node);
    strcat(buffer, ";\n");
    fprintf(output,"%s",buffer);
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, current_node);
    strcat(buffer, "condition");
    strcpy(buffer1, buffer);
    strcat(buffer1, "[label = \"condition\"];\n");
    fprintf(output, "%s", buffer1);
    buffer1 = (char*)malloc(sizeof(char)*33);
    strcpy(buffer1,current_node);
    strcat(buffer1,"--");
    strcat(buffer1,buffer);
    strcat(buffer1,";");
    fprintf(output, "%s", buffer1);
    while_count++;
    print_expr(buffer, while_loop->condition);
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, current_node);
    strcat(buffer, "body");
    strcpy(buffer1, buffer);
    strcat(buffer1, "[label = \"body\"];\n");
    fprintf(output, "%s", buffer1);
    buffer1 = (char*)malloc(sizeof(char)*33);
    strcpy(buffer1,current_node);
    strcat(buffer1,"--");
    strcat(buffer1,buffer);
    strcat(buffer1,";");
    fprintf(output, "%s", buffer1);
    print_stmt_list(buffer, while_loop->body);
}

void print_for(char* parent, struct NFor* for_loop)
{
    char* current_node, * buffer = (char*)malloc(sizeof(char)*33);
    char* buffer1 = (char*)malloc(sizeof(char)*33);
    current_node = (char*)malloc(sizeof(char)*33);
    sprintf(buffer,"%d",for_count);
    strcpy(current_node, "for");
    strcat(current_node, buffer);
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, current_node);
    fprintf(output,"%s", strcat(buffer, "[label = \"for\"];\n"));
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, parent);
    strcat(buffer, "--");
    strcat(buffer, current_node);
    strcat(buffer, ";\n");
    fprintf(output,"%s", buffer);
    for_count++;
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, current_node);
    strcat(buffer, "body");
    strcpy(buffer1, buffer);
    strcat(buffer1, "[label = \"body\"];\n");
    fprintf(output, "%s", buffer1);
    buffer1 = (char*)malloc(sizeof(char)*33);
    strcpy(buffer1,current_node);
    strcat(buffer1,"--");
    strcat(buffer1,buffer);
    strcat(buffer1,";");
    fprintf(output, "%s", buffer1);
    print_stmt_list(buffer, for_loop->body);
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, current_node);
    strcat(buffer, "varname");
    strcpy(buffer1, buffer);
    strcat(buffer1, "[label = \"var\"];\n");
    fprintf(output, "%s", buffer1);
    buffer1 = (char*)malloc(sizeof(char)*33);
    strcpy(buffer1,current_node);
    strcat(buffer1,"--");
    strcat(buffer1,buffer);
    strcat(buffer1,";");
    fprintf(output, "%s", buffer1);
    print_expr(buffer, for_loop->name);
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, current_node);
    strcat(buffer, "start");
    strcpy(buffer1, buffer);
    strcat(buffer1, "[label = \"start\"];\n");
    fprintf(output, "%s", buffer1);
    buffer1 = (char*)malloc(sizeof(char)*33);
    strcpy(buffer1,current_node);
    strcat(buffer1,"--");
    strcat(buffer1,buffer);
    strcat(buffer1,";");
    fprintf(output, "%s", buffer1);
    print_expr(buffer, for_loop->start);
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, current_node);
    strcat(buffer, "end");
    strcpy(buffer1, buffer);
    strcat(buffer1, "[label = \"end\"];\n");
    fprintf(output, "%s", buffer1);
    buffer1 = (char*)malloc(sizeof(char)*33);
    strcpy(buffer1,current_node);
    strcat(buffer1,"--");
    strcat(buffer1,buffer);
    strcat(buffer1,";");
    fprintf(output, "%s", buffer1);
    print_expr(buffer, for_loop->end);
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, current_node);
    strcat(buffer, "step");
    strcpy(buffer1, buffer);
    strcat(buffer1, "[label = \"step\"];\n");
    fprintf(output, "%s", buffer1);
    buffer1 = (char*)malloc(sizeof(char)*33);
    strcpy(buffer1,current_node);
    strcat(buffer1,"--");
    strcat(buffer1,buffer);
    strcat(buffer1,";");
    fprintf(output, "%s", buffer1);
    print_expr(buffer, for_loop->step);
}

char* print_expr(char* parent, struct NExpr* expr)
{
    char* current;
    char* current_node;
    char* buffer;
    switch (expr->type)
    {
        case EXPR_EQ:
            current = (char*)malloc(sizeof(char)*3);
            strcpy(current,"==");
            break;
        case EXPR_NQ:
            current = (char*)malloc(sizeof(char)*3);
            strcpy(current,"!=");
            break;
        case EXPR_PLUS:
            current = (char*)malloc(sizeof(char)*2);
            strcpy(current,"+");
            break;
        case EXPR_MINUS:
            current = (char*)malloc(sizeof(char)*2);
            strcpy(current,"-");
            break;
        case EXPR_DIV:
            current = (char*)malloc(sizeof(char)*2);
            strcpy(current,"/");
            break;
        case EXPR_MUL:
            current = (char*)malloc(sizeof(char)*2);
            strcpy(current,"*");
            break;
        case EXPR_LE:
            current = (char*)malloc(sizeof(char)*3);
            strcpy(current,"<=");
            break;
        case EXPR_GE:
            current = (char*)malloc(sizeof(char)*3);
            strcpy(current,">=");
            break;
        case EXPR_LT:
            current = (char*)malloc(sizeof(char)*2);
            strcpy(current,"<");
            break;
        case EXPR_GT:
            current = (char*)malloc(sizeof(char)*2);
            strcpy(current,">");
            break;
        case EXPR_MOD:
            current = (char*)malloc(sizeof(char)*2);
            strcpy(current,"%");
            break;
        case EXPR_ID:
            current = (char*)malloc(sizeof(char)*33);
            strcpy(current,expr->name);
            break;
        case EXPR_INT:
            current = (char*)malloc(sizeof(char)*33);
            sprintf(current, "%d", expr->Int);
            break;
        case EXPR_DOUBLE:
            current = (char*)malloc(sizeof(char)*33);
            sprintf(current, "%g", expr->Double);
            break;
        case EXPR_CONC:
            current = (char*)malloc(sizeof(char)*3);
            strcpy(current,"..");
            break;
        case EXPR_STR:
            current = (char*)malloc(sizeof(char)*33);
            strcpy(current, expr->name);
            break;
        case EXPR_MET:
            current = (char*)malloc(sizeof(char)*5);
            strcpy(current,"fcall");
            break;
        case EXPR_AND:
            current = (char*)malloc(sizeof(char)*3);
            strcpy(current,"&&");
            break;
        case EXPR_NOT:
            current = (char*)malloc(sizeof(char)*2);
            strcpy(current,"!");
            break;
        case EXPR_OR:
            current = (char*)malloc(sizeof(char)*3);
            strcpy(current,"||");
            break;
        case EXPR_MAS:
            current = (char*)malloc(sizeof(char)*3);
            strcpy(current,"[]");
            break;
        case EXPR_BOOL:
            current = (char*)malloc(sizeof(char)*6);
            if(expr->Int)
                strcpy(current,"true");
            else
                strcpy(current,"false");
            break;
        case EXPR_NIL:
            current = (char*)malloc(sizeof(char)*4);
            strcpy(current,"nil");
            break;
        case EXPR_UMIN:
            current = (char*)malloc(sizeof(char)*4);
            strcpy(current,"(-)");
            break;
        case EXPR_ID_LIST:
            current = (char*)malloc(sizeof(char)*3);
            strcpy(current,"id");
            break;
        case EXPR_TABLE:
            current = (char*)malloc(sizeof(char)*3);
            strcpy(current,"tableconstruct");
            break;
    }
    current_node = (char*)malloc(sizeof(char)*33);
    buffer = (char*)malloc(sizeof(char)*33);
    sprintf(buffer, "%d", expr_count);
    strcpy(current_node,"expr");
    strcat(current_node, buffer);
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, current_node);
    strcat(buffer, "[label = \"");
    strcat(buffer, current);
    strcat(buffer, "\"];\n");
    fprintf(output,"%s", buffer);
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, parent);
    strcat(buffer, "--");
    strcat(buffer, current_node);
    strcat(buffer, ";\n");
    fprintf(output,"%s",buffer);
    expr_count++;
    if (expr->type == EXPR_EQ || expr->type == EXPR_NQ || expr->type == EXPR_PLUS || expr->type == EXPR_MINUS || expr->type == EXPR_DIV || expr->type == EXPR_MUL
        || expr->type == EXPR_LE || expr->type == EXPR_GE || expr->type == EXPR_LT || expr->type == EXPR_GT || expr->type == EXPR_MOD || expr->type == EXPR_CONC
        || expr->type == EXPR_AND || expr->type == EXPR_OR)
    {
        print_expr(current_node,expr->left);
        print_expr(current_node,expr->right);
    } else if (expr->type == EXPR_MET)
    {
        char* buffer1;
        buffer = (char*)malloc(sizeof(char)*33);
        buffer1 = (char*)malloc(sizeof(char)*33);
        strcpy(buffer, current_node);
        strcat(buffer, "name");
        strcpy(buffer1, buffer);
        strcat(buffer1, "[label = \"name\"];\n");
        fprintf(output,"%s", buffer1);
        print_func_name(buffer, expr->left->idlist);
        buffer1 = (char*)malloc(sizeof(char)*33);
        strcpy(buffer1, current_node);
        strcat(buffer1, "--");
        strcat(buffer1, buffer);
        strcat(buffer1, ";\n");
        fprintf(output,"%s", buffer1);
        buffer = (char*)malloc(sizeof(char)*33);
        strcpy(buffer, current_node);
        strcat(buffer, "args[label = \" args \"];\n");
        fprintf(output,"%s", buffer);
        buffer1 = (char*)malloc(sizeof(char)*33);
        strcpy(buffer1, current_node);
        strcat(buffer1, " -- ");
        strcat(buffer1, current_node);
        strcat(buffer1, "args;\n");
        fprintf(output,"%s", buffer1);
        buffer = (char*)malloc(sizeof(char)*33);
        strcpy(buffer, current_node);
        strcat(buffer, "args");
        print_expr_list(buffer,expr->right->idlist);
    } else if (expr->type == EXPR_MAS)
    {
        char* buffer1;
        buffer = (char*)malloc(sizeof(char)*33);
        buffer1 = (char*)malloc(sizeof(char)*33);
        strcpy(buffer, current_node);
        strcat(buffer, "name");
        strcpy(buffer1, buffer);
        strcat(buffer1, "[label = \"name\"];\n");
        fprintf(output,"%s", buffer1);
        print_func_name(buffer, expr->left->idlist);
        buffer1 = (char*)malloc(sizeof(char)*33);
        strcpy(buffer1, current_node);
        strcat(buffer1, "--");
        strcat(buffer1, buffer);
        strcat(buffer1, ";\n");
        fprintf(output,"%s", buffer1);
        buffer = (char*)malloc(sizeof(char)*33);
        strcpy(buffer, current_node);
        strcat(buffer, "args[label = \" element \"];\n");
        fprintf(output,"%s", buffer);
        buffer1 = (char*)malloc(sizeof(char)*33);
        strcpy(buffer1, current_node);
        strcat(buffer1, " -- ");
        strcat(buffer1, current_node);
        strcat(buffer1, "args;\n");
        fprintf(output,"%s", buffer1);
        buffer = (char*)malloc(sizeof(char)*33);
        strcpy(buffer, current_node);
        strcat(buffer, "args");
        print_func_name(buffer,expr->right->idlist);
    } else if (expr->type == EXPR_NOT || expr->type == EXPR_UMIN)
    {
        print_expr(current_node,expr->left);
    } else if (expr->type == EXPR_FUNC_DEC_ANON)
    {
        print_func(current_node, expr->func);
    } else if (expr->type == EXPR_TABLE)
    {
        print_table(current_node, expr->table);
    } else if (expr->type == EXPR_ID_LIST)
    {
        print_expr_list(current_node, expr->idlist);
    }
    return current_node;
}

void print_stmt(char* parent, struct NStmt* stmt)
{
    char* current_node, * buffer = (char*)malloc(sizeof(char)*33);
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
        case STMT_LFUNC:
            print_func(parent,stmt->func);
            break;
        case STMT_REPEAT:
            print_while(parent,stmt->while_loop);
            break;
        case STMT_ASSIGN:
            current_node = (char*)malloc(sizeof(char)*33);
            strcpy(current_node, "assign");
            sprintf(buffer, "%d", stmt_count);
            strcat(current_node,buffer);
            buffer = (char*)malloc(sizeof(char)*33);
            strcpy(buffer, current_node);
            fprintf(output,"%s",strcat(buffer, "[label = \"=\"];\n"));
            buffer = (char*)malloc(sizeof(char)*33);
            strcpy(buffer, parent);
            strcat(buffer, "--");
            strcat(buffer, current_node);
            strcat(buffer, ";\n");
            fprintf(output,"%s",buffer);
            print_expr(current_node,stmt->var);
            print_expr(current_node,stmt->expr);
            stmt_count++;
            break;
        case STMT_LASSIGN:
            current_node = (char*)malloc(sizeof(char)*33);
            strcpy(current_node, "lassign");
            sprintf(buffer, "%d", stmt_count);
            strcat(current_node, buffer);
            buffer = (char*)malloc(sizeof(char)*33);
            strcpy(buffer, current_node);
            fprintf(output,"%s",strcat(buffer, "[label = \"=\"];\n"));
            buffer = (char*)malloc(sizeof(char)*33);
            strcpy(buffer, parent);
            strcat(buffer, "--");
            strcat(buffer, current_node);
            strcat(buffer, ";");
            fprintf(output,"%s",buffer);
            print_expr(current_node,stmt->var);
            print_expr(current_node,stmt->expr);
            stmt_count++;
            break;
        case STMT_RETURN:
            current_node = (char*)malloc(sizeof(char)*33);
            strcpy(current_node, "return");
            sprintf(buffer, "%d", stmt_count);
            current_node = strcat(current_node,buffer);
            buffer = (char*)malloc(sizeof(char)*33);
            strcpy(buffer, current_node);
            fprintf(output,"%s",strcat(buffer, "[label = \"return\"];\n"));
            buffer = (char*)malloc(sizeof(char)*33);
            strcpy(buffer, parent);
            strcat(buffer, "--");
            strcat(buffer, current_node);
            strcat(buffer, ";\n");
            fprintf(output,"%s",buffer);
            if (stmt->expr!=NULL)
            {
                print_expr(current_node, stmt->expr);
            }
            stmt_count++;
            break;
        case STMT_BREAK:
            current_node = (char*)malloc(sizeof(char)*33);
            strcpy(current_node, "break");
            sprintf(buffer, "%d", stmt_count);
            strcat(current_node,buffer);
            buffer = (char*)malloc(sizeof(char)*33);
            strcpy(buffer, current_node);
            fprintf(output,"%s",strcat(buffer, "[label = \"break\"];\n"));
            buffer = (char*)malloc(sizeof(char)*33);
            strcpy(buffer, parent);
            strcat(buffer, "--");
            strcat(buffer, current_node);
            strcat(buffer, ";\n");
            fprintf(output,"%s",buffer);
            stmt_count++;
            break;
        case STMT_IF:
            print_if(parent, stmt->if_tree);
            break;
    }
}

void print_stmt_list(char* parent, struct NStmtList* stmtlist)
{
    struct NStmt* current = stmtlist->first;
    while (current != NULL)
    {
        print_stmt(parent,current);
        current = current->next;
    }
}

void print_func(char* parent, struct NFunc* func)
{
    char* current_node = (char*)malloc(sizeof(char)*33);
    char* buffer = (char*)malloc(sizeof(char)*33);
    char* buffer1 = (char*)malloc(sizeof(char)*33);
    strcpy(current_node, "func");
    sprintf(buffer, "%d", func_count);
    strcat(current_node, buffer);
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, current_node);
    fprintf(output,"%s",strcat(buffer, "[label = \"func\"];\n"));
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, parent);
    strcat(buffer, "--");
    strcat(buffer, current_node);
    strcat(buffer, ";\n");
    fprintf(output,"%s", buffer);
    func_count++;
    if( func->name != NULL)
    {
        buffer = (char*)malloc(sizeof(char)*33);
        strcpy(buffer, current_node);
        strcat(buffer, "name");
        strcpy(buffer1, buffer);
        strcat(buffer1, "[label = \"func_name\"];\n");
        fprintf(output,"%s", buffer1);
        print_func_name(buffer, func->name);
        buffer1 = (char*)malloc(sizeof(char)*33);
        strcpy(buffer1, current_node);
        strcat(buffer1, "--");
        strcat(buffer1, buffer);
        strcat(buffer1, ";\n");
        fprintf(output,"%s", buffer1);
    }
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, current_node);
    strcat(buffer, "args[label = \" args \"];\n");
    fprintf(output,"%s", buffer);
    buffer1 = (char*)malloc(sizeof(char)*33);
    strcpy(buffer1, current_node);
    strcat(buffer1, " -- ");
    strcat(buffer1, current_node);
    strcat(buffer1, "args;\n");
    fprintf(output,"%s", buffer1);
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, current_node);
    strcat(buffer, "args");
    print_expr_list(buffer,func->args);
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, current_node);
    strcat(buffer, "body[label = \" body \"];\n");
    fprintf(output,"%s", buffer);
    buffer1 = (char*)malloc(sizeof(char)*33);
    strcpy(buffer1, current_node);
    strcat(buffer1, " -- ");
    strcat(buffer1, current_node);
    strcat(buffer1, "body;\n");
    fprintf(output,"%s", buffer1);
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, current_node);
    strcat(buffer, "body");
    print_stmt_list(buffer, func->body);
}

void print_expr_list(char* parent, struct NExprList* exprlist)
{
    struct NExpr* current = exprlist->first;
    while (current != NULL)
    {
        print_expr(parent,current);
        current = current->next;
    }
}

void print_func_name(char* parent, struct NExprList* exprlist)
{
    struct NExpr* current = exprlist->first;
    char* current_parent = parent;
    while (current != NULL)
    {
        current_parent = print_expr(current_parent ,current);
        current = current->next;
    }
}

void print_if(char* parent, struct NIf* if_tree)
{
    char* current_node, * buffer = (char*)malloc(sizeof(char)*33), *buffer1;
    current_node = (char*)malloc(sizeof(char)*33);
    sprintf(buffer, "%d", if_count);
    strcpy(current_node, "if");
    strcat(current_node, buffer);
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, current_node);
    strcat(buffer, "[label = \"if\"];\n");
    fprintf(output,"%s", buffer);
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, parent);
    strcat(buffer, "--");
    strcat(buffer, current_node);
    strcat(buffer, ";");
    fprintf(output,"%s",buffer);
    if_count++;
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, current_node);
    strcat(buffer, "condition[label = \" condition \"];\n");
    fprintf(output,"%s", buffer);
    buffer1 = (char*)malloc(sizeof(char)*33);
    strcpy(buffer1, current_node);
    strcat(buffer1, " -- ");
    strcat(buffer1, current_node);
    strcat(buffer1, "condition;\n");
    fprintf(output,"%s", buffer1);
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, current_node);
    strcat(buffer, "condition");
    print_expr(buffer, if_tree->condition);
    if (if_tree->body->first!=NULL)
    {
        buffer = (char*)malloc(sizeof(char)*33);
        strcpy(buffer, current_node);
        strcat(buffer, "body[label = \" body \"];\n");
        fprintf(output,"%s", buffer);
        buffer1 = (char*)malloc(sizeof(char)*33);
        strcpy(buffer1, current_node);
        strcat(buffer1, " -- ");
        strcat(buffer1, current_node);
        strcat(buffer1, "body;\n");
        fprintf(output,"%s", buffer1);
        buffer = (char*)malloc(sizeof(char)*33);
        strcpy(buffer, current_node);
        strcat(buffer, "body");
        print_stmt_list(buffer, if_tree->body);
    }
    if(if_tree->elseiflist->first!=NULL)
    {
        buffer = (char*)malloc(sizeof(char)*33);
        strcpy(buffer, current_node);
        strcat(buffer, "elseif[label = \" elseif_list \"];\n");
        fprintf(output,"%s", buffer);
        buffer1 = (char*)malloc(sizeof(char)*33);
        strcpy(buffer1, current_node);
        strcat(buffer1, " -- ");
        strcat(buffer1, current_node);
        strcat(buffer1, "elseif;\n");
        fprintf(output,"%s", buffer1);
        buffer = (char*)malloc(sizeof(char)*33);
        strcpy(buffer, current_node);
        strcat(buffer, "elseif");
        print_if_list(buffer, if_tree->elseiflist);
    }
    if(if_tree->elsebody->first!=NULL)
    {
        buffer = (char*)malloc(sizeof(char)*33);
        strcpy(buffer, current_node);
        strcat(buffer, "elsebody[label = \" Else_body \"];\n");
        fprintf(output,"%s", buffer);
        buffer1 = (char*)malloc(sizeof(char)*33);
        strcpy(buffer1, current_node);
        strcat(buffer1, " -- ");
        strcat(buffer1, current_node);
        strcat(buffer1, "elsebody;\n");
        fprintf(output,"%s", buffer1);
        buffer = (char*)malloc(sizeof(char)*33);
        strcpy(buffer, current_node);
        strcat(buffer, "elsebody");
        print_stmt_list(buffer, if_tree->elsebody);
     }
}

void print_table(char* parent, struct NTable* table)
{
    char* current_node, * buffer = (char*)malloc(sizeof(char)*33);
    current_node = (char*)malloc(sizeof(char)*33);
    sprintf(buffer, "%d", table_count);
    strcpy(current_node, "table");
    strcat(current_node, buffer);
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, current_node);
    strcat(buffer, "[label = \"table\"];\n");
    fprintf(output,"%s", buffer);
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, parent);
    strcat(buffer, "--");
    strcat(buffer, current_node);
    strcat(buffer, ";\n");
    fprintf(output,"%s",buffer);
    table_count++;
    struct NTblElem* current = table->first;
    int i = 0;
    while (current != NULL)
    {
        print_table_elem(current_node,current, i);
        i++;
        current = current->next;
    }
}

void print_table_elem(char* parent, struct NTblElem* elem, int num)
{
    char* current_node, * buffer = (char*)malloc(sizeof(char)*33);
    sprintf(buffer, "%d", num);
    current_node = (char*)malloc(sizeof(char)*33);
    strcpy(current_node, parent);
    strcat(current_node, buffer);
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, current_node);
    strcat(buffer, "[label = \"tblelem\"];\n");
    fprintf(output,"%s", buffer);
    buffer = (char*)malloc(sizeof(char)*33);
    strcpy(buffer, parent);
    strcat(buffer, "--");
    strcat(buffer, current_node);
    strcat(buffer, ";\n");
    fprintf(output,"%s",buffer);
    table_count++;
    if(elem->key != NULL) {
        print_expr(current_node,elem->key);
    }
    print_expr(current_node,elem->value);
}
void print_if_list(char* parent, struct NIfList* iflist)
{
    struct NIf* current = iflist->first;
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
    char* start = (char*)malloc(sizeof(char)*5);
    strcpy(start, "root");
    output = fopen("file.dot","w");
    fprintf(output, "%s\n", "graph graphname {");
    fprintf(output, "%s\n", "root;");
    print_stmt_list(start,root);
    fprintf(output, "%s\n", "}");
    fclose(output);
}
