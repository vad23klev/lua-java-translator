
extern "C" struct NStmtList *root;

void print_while(char* parent, struct NWhile* while_loop);
void print_for(char* parent, struct NFor* for_loop);
void print_expr(char* parent, struct NExpr* expr);
void print_stmt(char* parent, struct NStmt* stmt);
void print_stmt_list(char* parent, struct NStmtList* stmtlist);
void print_func(char* parent, struct NFunc* func);
void print_expr_list(char* parent, struct NExprList* exprlist);
void print_if(char* parent, struct NIf* if_tree);
void print_table(char* parent, struct NTable* table);
void print_table_elem(char* parent, struct NTblElem elem, int num);
void print_if_list(char* parent, struct NIfList* iflist);
void print_tree(struct NStmtList* stmtlist);
