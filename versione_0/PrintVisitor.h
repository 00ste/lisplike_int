#ifndef PRINT_VISITOR_H
#define PRINT_VISITOR_H

#include "Visitor.h"

/**
 * ExecutionVisitor si occupa di stampare i nodi su terminale.
 */
class PrintVisitor : public Visitor
{
public:
	// uso costruttori e distruttore di default
	void visitBlock(Block* blockNode) override;

	void visitPrintStmt(PrintStmt* printStmtNode) override;
	void visitSetStmt(SetStmt* setStmtNode) override;
	void visitInputStmt(InputStmt* inputStmtNode) override;
	void visitWhileStmt(WhileStmt* whileStmtNode) override;
	void visitIfStmt(IfStmt* ifStmtBlock) override;

	void visitOperator(Operator* operatorNode) override;
	void visitNumber(Number* numberNode) override;
	void visitVariable(Variable* variableNode) override;

	void visitRelOp(RelOp* relOpNode) override;
	void visitBoolConst(BoolConst* boolConstNode) override;
	void visitBoolOp(BoolOp* boolOpNode) override;
};

#endif
