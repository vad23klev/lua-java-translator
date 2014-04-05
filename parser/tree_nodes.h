#include <stdlib.h>

enum NStmtType {
    STMT_WHILE,
    STMT_FOR,
    STMT_EXPR,
    STMT_FUNC,
    STMT_BLOCK,
    STMT_REPEAT,
    STMT_LFUNC,
    STMT_BREAK,
    STMT_RETURN,
    STMT_ASSIGN,
    STMT_LASSIGN
};

enum NExprType {
    EXPR_EQ,
    EXPR_NQ,
    EXPR_PLUS,
    EXPR_MINUS,
    EXPR_DIV,
    EXPR_MUL,
    EXPR_LE,
    EXPR_GE,
    EXPR_LT,
    EXPR_GT,
    EXPR_MOD,
    EXPR_ID,
    EXPR_INT,
    EXPR_DOUBLE,
    EXPR_CONC,
    EXPR_STR,
    EXPR_MET,
    EXPR_AND,
    EXPR_NOT,
    EXPR_OR,
    EXPR_MAS,
    EXPR_BOOL,
    EXPR_NIL,
    EXPR_UMIN,
    EXPR_FUNC_DEC_ANON,
    EXPR_TABLE,
    EXPR_ID_LIST
};

struct NWhile
{
    struct NExpr * condition;
    struct NStmtList * body;
};

struct NFor
{
    struct NExpr* name;
    struct NExpr* start;
    struct NExpr* end;
    struct NExpr* step;
    struct NStmtList * body;
};

struct NExpr
{
    char * name;
    struct NExpr * left;
    struct NExpr * right;
    int *Int;
    double *Double;
    enum NExprType type;
    struct NExpr * next;
    struct NExprList* idlist;
};

struct NStmt
{
    enum NStmtType type;
    struct NExpr * var;
    struct NExpr * expr;
    struct NFunc * func;
    struct NStmtList * list;
    struct NStmt * next;
    struct NFor * for_loop;
    struct NWhile * while_loop;
};

struct NStmtList
{
    struct NStmt * first;
    struct NStmt * last;
};

struct NFunc
{
    char* name;
    struct NExprList* args;
    struct NStmtList* body;
};

struct NExprList
{
    struct NExpr * first;
    struct NExpr * last;
};

struct NIf
{
    struct NExpr* condition;
    struct NStmtList* body;
    struct NStmtList* elsebody;
    struct NIfList* elseiflist;
    struct NIf* next;
};

struct NIfList
{
    struct NIf* first;
    struct NIf* last;
};

struct NTable
{
    struct NTblElem * first;
    struct NTblElem * last;
};

struct NTblElem
{
    struct NExpr* key;
    struct NExpr* value;
    struct NTblElem * next;
};


/*
 * Function definition
 */

void set_null_field_expr(struct NExpr* expr)
{
    expr->Int = NULL;
    expr->Double = NULL;
    expr->name = NULL;
    expr->left = NULL;
    expr->right = NULL;
    expr->type = EXPR_NIL;
    expr->next = NULL;
}

struct NExpr* create_op_expr(NExprType type,NExpr* left,NExpr* right)
{
    NExpr* result = (NExpr*)malloc(sizeof(NExpr));
    set_null_field_expr(result);
    result->left = left;
    result->right = right;
    result->type = type;
    return result;
}

struct NExpr* create_expr_id(char* id)
{
    NExpr* result = (NExpr*)malloc(sizeof(NExpr));
    set_null_field_expr(result);
    result->name = id;
    result->type = EXPR_ID;
    return result;
}

struct NExpr* create_expr_string(char* string)
{
    NExpr* result = (NExpr*)malloc(sizeof(NExpr));
    set_null_field_expr(result);
    result->name = string;
    result->type = EXPR_STR;
    return result;
}

struct NExpr* create_expr_int(int * value)
{
    NExpr* result = (NExpr*)malloc(sizeof(NExpr));
    set_null_field_expr(result);
    result->Int = value;
    result->type = EXPR_INT;
    return result;
}

struct NExpr* create_expr_double(double * value)
{
    NExpr* result = (NExpr*)malloc(sizeof(NExpr));
    set_null_field_expr(result);
    result->Double = value;
    result->type = EXPR_DOUBLE;
    return result;
}

struct NExpr* create_expr_boolean(int* value)
{
    NExpr* result = (NExpr*)malloc(sizeof(NExpr));
    set_null_field_expr(result);
    result->Int = value;
    result->type = EXPR_BOOL;
    return result;
}

struct NExpr* create_expr_nil()
{
    NExpr* result = (NExpr*)malloc(sizeof(NExpr));
    set_null_field_expr(result);
    result->type = EXPR_NIL;
    return result;
}

struct NStmt* create_stmt_func(struct NFunc* func)
{
    struct NStmt* result = (NStmt*)malloc(sizeof(NStmt));
    result->func = func;
    result->type = STMT_FUNC;
    return result;
}

struct NStmt* create_stmt_expr(struct NExpr* expr)
{
    struct NStmt* result = (NStmt*)malloc(sizeof(NStmt));
    result->expr = expr;
    result->type = STMT_EXPR;
    return result;
}

struct NStmt* create_stmt_while(struct NWhile* While)
{
    struct NStmt* result = (NStmt*)malloc(sizeof(NStmt));
    result->while_loop = While;
    result->type = STMT_WHILE;
    return result;
}

struct NStmt* create_stmt_for(struct NFor* For)
{
    struct NStmt* result = (NStmt*)malloc(sizeof(NStmt));
    result->for_loop = For;
    result->type = STMT_FOR;
    return result;
}

struct  NExprList* create_expr_list(struct NExpr* first)
{
    struct NExprList* result = (NExprList*)malloc(sizeof(NExprList));
    result->first = first;
    result->last = result->first;
    return result;
}

struct NExprList* add_expr_to_list(struct NExprList* list, struct NExpr* element )
{
    list->last->next = element;
    list->last = element;
    return list;
}

struct NStmtList* create_stmt_list(struct NStmt* first)
{
    struct NStmtList* result = (NStmtList*)malloc(sizeof(NStmtList));
    result->first = first;
    result->last = first;
    return result;
}

struct NStmtList* add_stmt_to_list(struct NStmtList* list, struct NStmt* element)
{
    if(list->first == NULL)
    {
        list->first = element;
    }
    else
    {
        list->last->next = element;
    }
    list->last = element;
    return list;
}

struct NFor* create_for(struct NExpr* operand, struct NExpr* start, struct NExpr* end, struct NExpr* step, struct NStmt* body)
{
    struct NFor* result = (NFor*)malloc(sizeof(NFor));
    result->name = operand;
    result->start = start;
    result->end = end;
    result->step = step;
    result->body = body;
    return result;
}

struct NWhile* create_while(struct NExpr* condition, struct NStmt* body)
{
    struct NWhile* result = (NWhile*)malloc(sizeof(NWhile));
    result->condition = condition;
    result->body = body;
    return result;
}

struct NTblElem* create_tbl_elem(struct NExpr* key, struct NExpr* value)
{
    struct NTblElem* result = (NTblElem*)malloc(sizeof(NTblElem));
    result->key = key;
    result->value = value;
    result->next = NULL;
    return result;
}

struct NTable* create_table(struct NTblElem* first)
{
    struct NTable* result = (NTable*)malloc(sizeof(NTable));
    result->first = first;
    result->last = first;
    return result;
}

struct NTable* add_elem_to_table(struct NTable* list, struct NTblElem* element)
{
    if(list->first == NULL)
    {
        list->first = element;
    }
    else
    {
        list->last->next = element;
    }
    list->last = element;
    return list;
}
