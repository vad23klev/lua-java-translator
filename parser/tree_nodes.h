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
};

struct NStmt
{
    enum NStmtType type;
    struct NExpr * expr;
    struct NStmtList * list;
    struct NStmt * next;
};

struct NStmtList
{
    struct NStmt * first;
    struct Nstmt * last;
};

struct NFunc
{
    char* name;
    char*[] args;
    struct NStmt* body;
};

enum NStmtType {OP,EXPR,FUNC};

enum NExprType {EQ,NQ,PLUS,MINUS,DIV,MUL,LE,GE,LT,GT,MOD,ID,INT,DOUBLE,CONC,STR,MET,AND,NOT,OR,MAS,BOOL,NIL};

struct NExpr* create_op_expr(NExprType type,NExpr* left,NExpr* right)
{
    NExpr* result = (NExpr*)malloc(sizeof(NExpr));
    result->left = left;
    result->right = right;
    result->type = type;
    return result;
}

struct NExpr* create_expr_id(char* id)
{
    NExpr* result = (NExpr*)malloc(sizeof(NExpr));
    result->name = id;
    result->type = ID;
    return result;
}

struct NExpr* create_expr_string(char* string)
{
    NExpr* result = (NExpr*)malloc(sizeof(NExpr));
    result->name = string;
    result->type = STR;
    return result;
}

struct NExpr* create_expr_int(int * value)
{
    NExpr* result = (NExpr*)malloc(sizeof(NExpr));
    result->Int = value;
    result->type = INT;
    return result;
}

struct NExpr* create_expr_double(double * value)
{
    NExpr* result = (NExpr*)malloc(sizeof(NExpr));
    result->Double = value;
    result->type = DOUBLE;
    return result;
}

struct NExpr* create_expr_boolean(bool* value)
{
    NExpr* result = (NExpr*)malloc(sizeof(NExpr));
    result->Bool = value;
    result->type = BOOL;
    return result;
}

struct NExpr* create_expr_nil(bool* value)
{
    NExpr* result = (NExpr*)malloc(sizeof(NExpr));
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
}
