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
    CONST_UTF8      = 1,
    
    CONST_INT       = 3,
    CONST_FLOAT     = 4,
    CONST_CLASS     = 7,
    CONST_STRING    = 8,
    CONST_FIELDREF  = 9,
    CONST_METHODREF = 10,
    CONST_NAMETYPE  = 12
};

/**
 * Filed flags.
 */
enum st_filed_flags {
    FIELD_ACCESS_PUBLIC    = 0x0001,
    FIELD_ACCESS_PRIVATE   = 0x0002,
    FIELD_ACCESS_PROTECTED = 0x0004,

    FIELD_STATIC = 0x0008
};

/**
 * Method flags.
 */
enum st_method_flags {
    METHOD_ACCESS_PUBLIC    = 0x001,
    METHOD_ACCESS_PRIVATE   = 0x002,
    METHOD_ACCESS_PROTECTED = 0x004,

    METHOD_STATIC   = 0x0008,
    METHOD_FINAL    = 0x0010,
    METHOD_ABSTRACT = 0x0400
};

/**
 * A record of constants table.
 * Singly-Linked list element.
 */
typedef struct st_const STConst;
struct st_const {
    enum st_const_types type;
    union {
        char * utf8;
        struct {
            int arg1;
            int arg2;
        } args;

        int val_int;
        double val_float;
    } value;
    
    STConst * next;
};

/**
 * Represents field record.
 * Singly-linked list element.
 */
typedef struct st_field STField;
struct st_field {
    unsigned short int flags;
    unsigned short int name;
    unsigned short int descriptor;

    STField * next;
};

/**
 * Represents method record.
 * Singly-liked list element.
 */
typedef struct st_method STMethod;
struct st_method {
    unsigned short int flags;
    unsigned short int name;
    unsigned short int descriptor;
    unsigned short int attr_num;

    STField * next;
};

/**
 * General purpose singly-linked list element.
 */
typedef struct slist SList;
struct slist {
    void  * data;
    SList * next;
};

/*
 * Table declarations.
 */
STConst *  st_const_table = NULL;
STConst *  st_const_last  = NULL;

STConst ** st_current_const_table = NULL;
STConst ** st_current_const_last  = NULL;

SList   *  func_list = NULL;
SList   *  func_last = NULL;
int        funcnum   = 0;
int        fconstnum = 0;

STConst *  st_func_handles = NULL;
STConst *  st_func_hlast   = NULL;

/*
 * Function declarations.
 */

/**
 * Creates a new constant table with first temporary UTF8 entry.
 * @return A pointer to constant list.
 */
STConst * st_new_const_table();

/**
 * Creates a new one-argument constant.
 * Accepts: CONST_UTF8, CONST_INT, CONST_FLOAT, CONST_CLASS, CONST_STRING.
 * @param [in] type Constant type.
 * @param [in] arg  Constant value.
 * @return Created constant.
 */
STConst * st_new_const(enum st_const_types type, void * arg);

/**
* Creates a new two-argument constant.
* Accepts: CONST_FIELDREF, CONST_METHODREF, CONST_NAMETYPE.
* @param [in] type Constant type.
* @param [in] arg1 Constant first value.
* @param [in] arg2 Constant second value.
* @return Created constant.
*/
STConst * st_new_const2(enum st_const_types type, int arg1, int arg2);

/**
 * Returns the number of constants of given table.
 * @param [in] table Constant table.
 * @return Number of constants.
 */
int       st_const_count(STConst * table);

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
 * Returns zero-based index of one-argument constant in table.
 * @param [in] table A constant table to search in
 * @param [in] type  Constant type. Supported types are:
 *                   CONST_INT
 *                   CONST_FLOAT
 *                   CONST_UTF8
 * @param [in] arg1  First value.
 * @param [in] arg2  Second value.
 * @return Zero-based index or -1 when constant not found.
 */
int st_constant_index(STConst * table, enum st_const_types type, const void * value);

/**
 * Returns zero-based index of two-argument constant in table.
 * @param [in] table A constant table to search in
 * @param [in] type  Constant type. Supported types are:
 *                   CONST_FIELDREF
 *                   CONST_METHODREF
 *                   CONST_NAMETYPE
 * @param [in] value Constant value.
 * @return Zero-based index or -1 when constant not found.
 */
int st_constant_index2(STConst * table, enum st_const_types type, int arg1, int arg2);

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

/**
 * Returns the number of elements in given expr list.
 * This function iterates over the list.
 * @param [in] start List head.
 * @return Number of elements.
 */
int    nexprlist_count(NExprList * start);

/**
 * Generates a function handle for the given function;
 * @param [in] f      NFunc tree node.
 * @param [in] buffer Text buffer.
 * @return Text buffer with function handle.
 */
char * st_gen_func_handle(NFunc * f, char * buffer);


//############################################################################//

STConst * st_new_const_table() {
    STConst * rtl = st_new_const(CONST_UTF8, NULL);
    rtl->value.utf8 = (char *)malloc(30);
    strcpy(rtl->value.utf8, "RTL METHODREFS WILL BE HERE");
    return rtl;
}

STConst * st_new_const(enum st_const_types type, void * arg) {
    STConst * c = (STConst *)malloc(sizeof(STConst));
    c->next = NULL;
    c->type = type;

    switch (type) {
        case CONST_UTF8:
        c->value.utf8 = (char *)arg;
        break;

        case CONST_INT:
        c->value.val_int = *((int *)arg);
        break;

        case CONST_FLOAT:
        c->value.val_float = *((double *)arg);
        break;

        case CONST_CLASS:
        c->value.args.arg1 = *((int *)arg);
        break;

        case CONST_STRING:
        c->value.args.arg1 = *((int *)arg);
        break;
    }

    return c;
}

STConst * st_new_const2(enum st_const_types type, int arg1, int arg2) {
    STConst * c = (STConst *)malloc(sizeof(STConst));
    c->next = NULL;
    c->type = type;
    c->value.args.arg1 = arg1;
    c->value.args.arg2 = arg2;
    return c;
}

int st_const_count(STConst * table) {
    STConst * cur = table;
    int count = 0;

    while (cur != NULL) {
        count++;
        cur = cur->next;
    }

    return count;
}

void st_fill_tables(struct NStmtList * root) {
    st_func_handles = st_new_const_table();
    st_func_hlast   = st_func_handles;

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
        case STMT_BLOCK:  st_stmt_list(node->list);                         break;
        case STMT_REPEAT: st_stmt_while(node->while_loop);                  break;
        case STMT_LFUNC:  st_stmt_func(node->func);                         break;
        case STMT_RETURN: if (node->expr != NULL) st_stmt_expr(node->expr); break;
        case STMT_IF:     st_stmt_if(node->if_tree);                        break;

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
    int arg1;
    char buf[512];
    STConst * c = NULL;
    SList * f = (SList *)malloc(sizeof(SList));

    // Add function to list
    f->next = NULL;
    f->data = (void *)(node);
    if (func_list == NULL) {
        func_list = f;
        func_last = f;
    } else {
        func_last->next = f;
        func_last = f;
    }

    // Set function classname
    node->classname = (char *)malloc(10);
    sprintf(node->classname, "func%d", funcnum);
    funcnum++;

    // Create methodref

    // Method name UTF8
    if (st_constant_index(st_func_handles, CONST_UTF8, "value") == -1) {
        c = st_new_const(CONST_UTF8, NULL);
        c->value.utf8 = (char *)malloc(6);
        strcpy(c->value.utf8, "value");
        st_func_hlast->next = c;
        st_func_hlast = c;
        fconstnum++;
    }

    // Function handle UTF8
    st_gen_func_handle(node, buf);
    int fh = st_constant_index(st_func_handles, CONST_UTF8, buf);
    if (fh == -1) {
        c = st_new_const(CONST_UTF8, NULL);
        c->value.utf8 = (char *)malloc(512);
        strcpy(c->value.utf8, buf);
        st_func_hlast->next = c;
        st_func_hlast = c;
        fh = ++fconstnum;
    }

    // Class name UTF8
    c = st_new_const(CONST_UTF8, (void *)node->classname);
    st_func_hlast->next = c;
    st_func_hlast = c;
    fconstnum++;

    // Class
    c = st_new_const(CONST_CLASS, (void *)&fconstnum);
    st_func_hlast->next = c;
    st_func_hlast = c;
    fconstnum++;

    // Name and type
    arg1 = st_constant_index(st_func_handles, CONST_UTF8, "value");
    int nt = st_constant_index2(st_func_handles, CONST_NAMETYPE, arg1, fh);
    if (nt == -1) {
        c = st_new_const2(CONST_NAMETYPE, arg1, fh);
        st_func_hlast->next = c;
        st_func_hlast = c;
        nt = ++fconstnum;
    }

    // Methodref
    arg1 = fconstnum - 1;
    c = st_new_const2(CONST_METHODREF, arg1, nt);
    st_func_hlast->next = c;
    st_func_hlast = c;
    fconstnum++;

    // Set methodref attribute
    node->methodref = fconstnum;

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
            cstr->value.args.arg1 = st_constant_index(*st_current_const_table, CONST_UTF8, utf8->value.utf8);

            (*st_current_const_last)->next = cstr;
            *st_current_const_last = cstr;
        }
        break;

        case EXPR_MET: {
            struct NExpr * cur = node->right->idlist->first;
            while (cur != NULL) {
                st_stmt_expr(cur);
                cur = cur->next;
            }
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

int st_constant_index(STConst * table, enum st_const_types type, const void * value) {
    STConst * cur = table;
    int index = 0;
    while (cur != 0) {
        if (cur->type == type) {
            switch (type) {
                case CONST_INT:
                    if (cur->value.val_int == *((int *)value)) {
                        return index;
                    }
                break;
                
                case CONST_FLOAT:
                    if (cur->value.val_float == *((double *)value)) {
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

int st_constant_index2(STConst * table, enum st_const_types type, int arg1, int arg2) {
    STConst * cur = table;
    int index = 0;
    while (cur != NULL) {
        if (cur->type == type
            && cur->value.args.arg1 == arg1
            && cur->value.args.arg2 == arg2) {

            return index;
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
            case CONST_UTF8:      printf("'%s'", cur->value.utf8);      break;
            case CONST_INT:       printf("%d", cur->value.val_int);   break;
            case CONST_FLOAT:     printf("%f", cur->value.val_float); break;

            case CONST_CLASS:
            case CONST_STRING:    printf("%d", cur->value.args.arg1); break;
            
            case CONST_FIELDREF:
            case CONST_METHODREF:
            case CONST_NAMETYPE: printf("%d %d", cur->value.args.arg1, cur->value.args.arg2); break;
            
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

int nexprlist_count(NExprList * start) {
    int count = 0;
    NExpr * cur = start->first;
    while (cur != NULL) {
        count++;
        cur = cur->next;
    }
    return count;
}

char * st_gen_func_handle(NFunc * f, char * buffer) {
    int i;

    strcpy(buffer, "(");
    for (i = 0; i < nexprlist_count(f->args); i++) {
        strcat(buffer, "Lrtl/Mixed;");
    }
    strcat(buffer, ")Lrtl/Mixed;");

    return buffer;
}

#endif
