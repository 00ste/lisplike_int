#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include <iostream>

#include "Block.h"
#include "Statement.h"
#include "NumExpr.h"
#include "BoolExpr.h"


/**
 * Manager si occupa di allocare, memorizzare e deallocare
 * gli oggetti che compongono l'albero sintattico del programma.
 */
class NodeManager
{
public:
	NodeManager() : blockNodes{}, statementNodes{},
		numExprNodes{}, boolExprNodes{} {}
	NodeManager(const NodeManager& other) = delete;
	~NodeManager() { clearMemory(); }

	void clearMemory();

	Block* makeBlock();

	IfStmt* makeIfStmt(BoolExpr* c, Block* b_if, Block* b_else);
	WhileStmt* makeWhileStmt(BoolExpr* c, Block* b);
	InputStmt* makeInputStmt(Variable* var_id);
	SetStmt* makeSetStmt(Variable* var_id, NumExpr* num_expr);
	PrintStmt* makePrintStmt(NumExpr* num_expr);

	Operator* makeOperator(Operator::OpCode o, NumExpr* lop, NumExpr* rop);
	Number* makeNumber(int v);
	Variable* makeVariable(const std::string& var_id);

	RelOp* makeRelOp(RelOp::OpCode o, NumExpr* lop, NumExpr* rop);
	BoolOp* makeBoolOp(BoolOp::OpCode o, BoolExpr* lop, BoolExpr* rop);
	BoolConst* makeBoolConst(bool v);
private:
	std::vector<Block*> blockNodes;
	std::vector<Statement*> statementNodes;
	std::vector<NumExpr*> numExprNodes;
	std::vector<BoolExpr*> boolExprNodes;
};

#endif