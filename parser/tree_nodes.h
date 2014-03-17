struct NWhile 
{
	struct NExpr * condition;
	struct NStmt * body;
        double step;
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

enum NStmtType {OP,EXPR};

enum NExprType {EQ,NQ,PLUS,MINUS,DIV,MUL,LT,GT,MOD};
