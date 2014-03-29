struct NWhile
{
    struct NExpr * condition;
    struct NStmt * body;
};

struct NFor
{
    char* name;
    double start;
    double end;
    double step;
    struct NStmt * body;
};

struct NExpr
{
    char * name;
    struct NExpr * left;
    struct NExpr * right;
    int *Int;
    double *Double;
    enum NExprType type;
    bool *Bool;
    struct NExpr * next;
};

struct NStmt
{
    enum NStmtType type;
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
    struct Nstmt * last;
};

struct NFunc
{
    char* name;
    struct NExprList* args;
    struct NStmt* body;
};

struct NExprList
{
    struct NExpr * first;
    struct NExpr * last;
}

enum NStmtType {WHILE,FOR,EXPR,FUNC};

enum NExprType {EQ,NQ,PLUS,MINUS,DIV,MUL,LE,GE,LT,GT,MOD,ID,INT,DOUBLE,CONC,STR,MET,AND,NOT,OR,MAS,BOOL,NIL};

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
    result->type = ID;
    return result;
}

struct NExpr* create_expr_string(char* string)
{
    NExpr* result = (NExpr*)malloc(sizeof(NExpr));
    set_null_field_expr(result);
    result->name = string;
    result->type = STR;
    return result;
}

struct NExpr* create_expr_int(int * value)
{
    NExpr* result = (NExpr*)malloc(sizeof(NExpr));
    set_null_field_expr(result);
    result->Int = value;
    result->type = INT;
    return result;
}

struct NExpr* create_expr_double(double * value)
{
    NExpr* result = (NExpr*)malloc(sizeof(NExpr));
    set_null_field_expr(result);
    result->Double = value;
    result->type = DOUBLE;
    return result;
}

struct NExpr* create_expr_boolean(bool* value)
{
    NExpr* result = (NExpr*)malloc(sizeof(NExpr));
    set_null_field_expr(result);
    result->Bool = value;
    result->type = BOOL;
    return result;
}

struct NExpr* create_expr_nil(bool* value)
{
    NExpr* result = (NExpr*)malloc(sizeof(NExpr));
    set_null_field_expr(result);
    result->type = NIL;
    return result;
}

void set_null_field_expr(struct NExpr* expr)
{
    expr->Int = NULL;
    expr->Double = NULL;
    expr->Bool = NULL;
    expr->name = NULL;
    expr->left = NULL;
    expr->right = NULL;
    expr->type = NIL;
    expr->next = NULL;
}

struct NStmt* create_stmt_func(struct NFunc* func)
{
    struct NStmt* result = (NStmt*)malloc(sizeof(NStmt));
    result->func = func;
    result->type = FUNC;
    return result;
}

struct NStmt* create_stmt_expr(struct NExpr* expr)
{
    struct NStmt* result = (NStmt*)malloc(sizeof(NStmt));
    result->expr = expr;
    result->type = EXPR;
    return result;
}

struct NStmt* create_stmt_while(struct NWhile* While)
{
    struct NStmt* result = (NStmt*)malloc(sizeof(NStmt));
    result->while_loop = While;
    result->type = WHILE;
    return result;
}
