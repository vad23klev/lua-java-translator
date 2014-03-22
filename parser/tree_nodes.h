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
    int num;
    enum NExprType type;
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

enum NExprType {EQ,NQ,PLUS,MINUS,DIV,MUL,LE,GE,LT,GT,MOD,ID,NUM,CONC,STR,MET,AND,NOT,OR,MAS};

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
