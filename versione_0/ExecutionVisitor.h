#ifndef EXECUTION_VISITOR_H
#define EXECUTION_VISITOR_H

#include "Visitor.h"

/**
 * ExecutionVisitor si occupa di eseguire gli statement e
 * valutare le espressioni numeriche e booleane.
 * La classe possiede come attributi:
 * - Tre buffer per memorizzare i risutlati delle operazioni
 *   numeriche e booleane e i nomi di variabili
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
	std::vector<std::string> varStack;
	std::map<std::string, int> variables;
};

#endif