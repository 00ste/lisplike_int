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


/**
 * ExecutionVisitor si occupa di eseguire gli statement e
 * valutare le espressioni numeriche e booleane.
 * La classe possiede come attributi:
 * - Due buffer per memorizzare i risutlati delle operazioni
 *   numeriche e booleane
 * - Una mappa che contiene le variabili definite e il loro
 *   valore
 */
class ExecutionVisitor : public Visitor
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
private:
	std::vector<int> intStack;
	std::vector<bool> boolStack;
	std::map<std::string, int> variables;
};

#endif