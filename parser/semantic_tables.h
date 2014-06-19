/**
 * @file semantic_tables.h
 * Provides functions to create various tables (e. g. constants,
 * local variables) for .class files.
 */

#ifndef _H_SEMANTIC_TABLES_
#define _H_SEMANTIC_TABLES_

#include "tree_nodes.h"

/*
 * Table structure declarations.
 */

/**
 * Constant types.
 */
enum st_const_types {
    CONST_UTF8      = 2,
    
    CONST_INT       = 3,
    CONST_FLOAT     = 4,
    CONST_CLASS     = 7,
    CONST_STRING    = 8,
    CONST_FIELDREF  = 9,
    CONST_METHODREF = 10,
    CONST_NAMETYPE  = 12
};

/**
 * A record of constants table.
 * Singly-Linked list.
 */
typedef struct st_const STConst;
struct st_const {
    enum st_const_types type;
    union {
        char * utf8;
        int arg1;
        int arg2;
        
        int val_int;
        double val_float;
    } value;
    
    STConst * next;
};

/*
 * Table declarations.
 */
STConst * st_const_table = NULL;
STConst * st_const_last  = NULL;

STConst ** st_current_const_table = NULL;
STConst ** st_current_const_last  = NULL;

/*
 * Function declarations.
 */

/**
 * Creates a new constant table with one LMixed UTF8 entry.
 * @return A pointer to constant list.
 */
STConst * st_new_const_table();

/**
 * Fills tables according to given tree.
 * @param [in] root The root of the tree.
 */
void st_fill_tables(struct NStmtList * root);

void st_stmt_list(struct NStmtList * node);
void st_stmt(struct NStmt * node);
void st_stmt_while(struct NWhile * node);
void st_stmt_for(struct NFor * node);
void st_stmt_func(struct NFunc * node);
void st_stmt_if(struct NIf * node);
void st_stmt_expr(struct NExpr * node);

/**
 * Returns zero-based index of constant in table.
 * @param [in] table A constant table to search in
 * @param [in] type  Constant type. Supported types are:
 *                   CONST_INT
 *                   CONST_FLOAT
 *                   CONST_UTF8
 * @return Zero-based index or -1 when constant not found.
 */
int st_constant_index(STConst * table, enum st_const_types type, void * value);

/**
 * Prints the constants table.
 */
void st_print_const();

/**
 * Return a string containing constant type name.
 * Just turns enum value to it's name.
 * @param [in]  type Const type.
 * @param [out] name String.
 * @return param name.
 */
char * st_type_name(enum st_const_types type, char name[10]);


//############################################################################//

STConst * st_new_const_table() {
    STConst * mixed = (STConst *)malloc(sizeof(STConst));
    mixed->type = CONST_UTF8;
    mixed->value.utf8 = (char *)malloc(7);
    strcpy(mixed->value.utf8, "LMixed;");
    mixed->next = NULL;
    return mixed;
}

void st_fill_tables(struct NStmtList * root) {
    // Set current table
    st_current_const_table = &st_const_table;
    st_current_const_last  = &st_const_last;
    
    // Append first
    *st_current_const_table = st_new_const_table();
    *st_current_const_last  = *st_current_const_table;
    
    // Append others
    st_stmt_list(root);
}

void st_stmt_list(struct NStmtList * node) {
    struct NStmt * current = node->first;
    while (current != NULL) {
        st_stmt(current);
        current = current->next;
    }
}

void st_stmt(struct NStmt * node) {
    switch (node->type) {
        case STMT_WHILE:  st_stmt_while(node->while_loop);                  break;
        case STMT_FOR:    st_stmt_for(node->for_loop);                      break;
        case STMT_EXPR:   st_stmt_expr(node->expr);                         break;
        case STMT_FUNC: {
            // Switch to local constant table and initialize it.
            st_current_const_table  = &(node->func->const_table);
            st_current_const_last   = &(node->func->const_last);
            *st_current_const_table = st_new_const_table();
            *st_current_const_last  = *st_current_const_table;
            
            // Fill table.
            st_stmt_func(node->func);
            
            // Switch to global.
            st_current_const_table = &st_const_table;
            st_current_const_last  = &st_const_last;
        }
        break;
        case STMT_BLOCK:  st_stmt_list(node->list);                         break;
        case STMT_REPEAT: st_stmt_while(node->while_loop);                  break;
        case STMT_LFUNC:  st_stmt_func(node->func);                         break;
        case STMT_RETURN: if (node->expr != NULL) st_stmt_expr(node->expr); break;
        case STMT_IF:     st_stmt_if(node->if_tree);                        break;
        
        case STMT_ASSIGN:
        case STMT_LASSIGN:
            st_stmt_expr(node->var);
            st_stmt_expr(node->expr);
            break;
    }
}

void st_stmt_while(struct NWhile * node) {
    st_stmt_expr(node->condition);
    st_stmt_list(node->body);
}

void st_stmt_for(struct NFor * node) {
    st_stmt_expr(node->name);
    st_stmt_expr(node->start);
    st_stmt_expr(node->end);
    st_stmt_expr(node->step);
    st_stmt_list(node->body);
}

void st_stmt_func(struct NFunc * node) {
    st_stmt_list(node->body);
}

void st_stmt_if(struct NIf * node) {
    st_stmt_list(node->body);
    
    struct NIf* current = node->elseiflist->first;
    while (current != NULL)
    {
        st_stmt_if(current);
        current = current->next;
    }
    
    st_stmt_list(node->elsebody);
}

void st_stmt_expr(struct NExpr * node) {
    switch (node->type) {
        case EXPR_INT: {
            if (st_constant_index(*st_current_const_table, CONST_INT, (void *)&(node->Int)) == -1) {
                STConst * cint = (STConst *)malloc(sizeof(STConst));
                cint->next = NULL;
                
                cint->type = CONST_INT;
                cint->value.val_int = node->Int;
                
                (*st_current_const_last)->next = cint;
                *st_current_const_last = cint;
            }
        }
        break;
        
        case EXPR_DOUBLE: {
            if (st_constant_index(*st_current_const_table, CONST_FLOAT, (void *)&(node->Double)) == -1) {
                STConst * cfloat = (STConst *)malloc(sizeof(STConst));
                cfloat->next = NULL;
                
                cfloat->type = CONST_FLOAT;
                cfloat->value.val_float = node->Double;
                
                (*st_current_const_last)->next = cfloat;
                *st_current_const_last = cfloat;
            }
        }
        break;
        
        case EXPR_STR: {
            // Make UTF-8
            STConst * utf8 = (STConst *)malloc(sizeof(STConst));
            utf8->next = NULL;
            
            utf8->type = CONST_UTF8;
            utf8->value.utf8 = node->name; // `strcpy` it in case of problems
            
            (*st_current_const_last)->next = utf8;
            *st_current_const_last = utf8;
            
            // Make constant
            STConst * cstr = (STConst *)malloc(sizeof(STConst));
            cstr->next = NULL;
            
            cstr->type = CONST_STRING;
            cstr->value.arg1 = st_constant_index(*st_current_const_table, CONST_UTF8, utf8->value.utf8);
            
            (*st_current_const_last)->next = cstr;
            *st_current_const_last = cstr;
        }
        break;
    }
    
    if (node->left != NULL) {
        st_stmt_expr(node->left);
    }
    
    if (node->right != NULL) {
        st_stmt_expr(node->right);
    }
}

int st_constant_index(STConst * table, enum st_const_types type, void * value) {
    STConst * cur = table;
    int index = 0;
    while (cur != 0) {
        if (cur->type == type) {
            switch (type) {
                case CONST_INT:
                    if (cur->value.val_int == *((int *)&value)) {
                        return index;
                    }
                break;
                
                case CONST_FLOAT:
                    if (cur->value.val_float == *((double *)&value)) {
                        return index;
                    }
                break;
                
                case CONST_UTF8:
                    if (strcmp(cur->value.utf8, (const char *)value) == 0) {
                        return index;
                    }
                break;
                
                default:
                    return -1;
                break;
            }
        }
        
        cur = cur->next;
        index++;
    }
    return -1;
}

void st_print_const(STConst * table) {
    char name[10] = "";
    STConst * cur = table;
    int index = 0;
    while (cur != 0) {
        printf("%5d:  %9s  ", index, st_type_name(cur->type, name));
        switch (cur->type) {
            case CONST_UTF8:      printf("%s", cur->value.utf8);      break;
            case CONST_INT:       printf("%d", cur->value.val_int);   break;
            case CONST_FLOAT:     printf("%f", cur->value.val_float); break;
            case CONST_STRING:    printf("%d", cur->value.arg1);      break;
            
            case CONST_FIELDREF:
            case CONST_METHODREF:
            case CONST_NAMETYPE:
            case CONST_CLASS:     printf("%d %d", cur->value.arg1, cur->value.arg2); break;
            
            
            default:              printf("==WTF?=="); break;
        }
        printf("\n");
        
        cur = cur->next;
        index++;
    }
}

char * st_type_name(enum st_const_types type, char name[10]) {
    switch (type) {
        case CONST_UTF8:      strcpy(name, "UTF8");      break;
        case CONST_INT:       strcpy(name, "INT");       break;
        case CONST_FLOAT:     strcpy(name, "FLOAT");     break;
        case CONST_CLASS:     strcpy(name, "CLASS");     break;
        case CONST_STRING:    strcpy(name, "STRING");    break;
        case CONST_FIELDREF:  strcpy(name, "FIELDREF");  break;
        case CONST_METHODREF: strcpy(name, "METHODREF"); break;
        case CONST_NAMETYPE:  strcpy(name, "NAMETYPE");  break;
        default:              strcpy(name, "==WTF?==");  break;
    }
    return name;
}

#endif
