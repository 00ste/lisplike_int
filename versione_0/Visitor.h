#ifndef VISITOR_H
#define VISITOR_H

#include <map>

#include "Block.h"
#include "Statement.h"
#include "NumExpr.h"
#include "BoolExpr.h"

class Visitor
{
public:
	virtual void visitBlock(Block* blockNode) = 0;

	virtual void visitPrintStmt(PrintStmt* printStmtNode) = 0;
	virtual void visitSetStmt(SetStmt* setStmtNode) = 0;
	virtual void visitInputStmt(InputStmt* inputStmtNode) = 0;
	virtual void visitWhileStmt(WhileStmt* whileStmtNode) = 0;
	virtual void visitIfStmt(IfStmt* ifStmtBlock) = 0;

	virtual void visitOperator(Operator* operatorNode) = 0;
	virtual void visitNumber(Number* numberNode) = 0;
	virtual void visitVariable(Variable* variableNode) = 0;

	virtual void visitRelOp(RelOp* relOpNode) = 0;
	virtual void visitBoolConst(BoolConst* boolConstNode) = 0;
	virtual void visitBoolOp(BoolOp* boolOpNode) = 0;
};

#endif