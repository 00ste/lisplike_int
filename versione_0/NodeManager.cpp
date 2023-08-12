#include <iostream>

#include "NodeManager.h"
#include "Block.h"
#include "Statement.h"
#include "NumExpr.h"
#include "BoolExpr.h"

/**
 * Dealloca tutti i nodi da tutte le liste e ripulisce la
 * memoria.
 */
void NodeManager::clearMemory()
{
	std::cout << "MAN: Clearing memory..." << std::endl;
	for (Block* block : blockNodes)
		delete(block);
	for (Statement* statement : statementNodes)
		delete(statement);
	for (NumExpr* numExpr : numExprNodes)
		delete(numExpr);
	for (BoolExpr* boolExpr : boolExprNodes)
		delete(boolExpr);
}


/**
 * FACTORY METHOD PER TUTTI GLI OGGETTI DA GESTIRE
 * 
 * Per ogni oggetto:
 * - l'oggetto viene creato chiamando il suo costruttore
 * - l'oggetto viene aggiunto alla lista corrispondente
 * - viene restituito il puntatore all'oggetto
 */
Block* NodeManager::makeBlock()
{
	Block* x = new Block();
	blockNodes.push_back(x);
	//std::cout << "MAN: New Block created at " << x << std::endl;
	return x;
}

IfStmt* NodeManager::makeIfStmt(BoolExpr* c, Block* b_if, Block* b_else)
{
	IfStmt* x = new IfStmt(c, b_if, b_else);
	statementNodes.push_back(x);
	//std::cout << "MAN: New IfStmt created at " << x << std::endl;
	return x;
}
WhileStmt* NodeManager::makeWhileStmt(BoolExpr* c, Block* b)
{
	WhileStmt* x = new WhileStmt(c, b);
	statementNodes.push_back(x);
	//std::cout << "MAN: New WhileStmt created at " << x << std::endl;
	return x;
}
InputStmt* NodeManager::makeInputStmt(Variable* var_id)
{
	InputStmt* x = new InputStmt(var_id);
	statementNodes.push_back(x);
	//std::cout << "MAN: New InputStmt created at " << x << std::endl;
	return x;
}
SetStmt* NodeManager::makeSetStmt(Variable* var_id, NumExpr* num_expr)
{
	SetStmt* x = new SetStmt(var_id, num_expr);
	statementNodes.push_back(x);
	//std::cout << "MAN: New SetStmt created at " << x << std::endl;
	return x;
}
PrintStmt* NodeManager::makePrintStmt(NumExpr* num_expr)
{
	PrintStmt* x = new PrintStmt(num_expr);
	statementNodes.push_back(x);
	//std::cout << "MAN: New PrintStmt created at " << x << std::endl;
	return x;
}

Operator* NodeManager::makeOperator(Operator::OpCode o, NumExpr* lop, NumExpr* rop)
{
	Operator* x = new Operator(o, lop, rop);
	numExprNodes.push_back(x);
	return x;
}
Number* NodeManager::makeNumber(int v)
{
	Number* x = new Number(v);
	numExprNodes.push_back(x);
	return x;
}
Variable* NodeManager::makeVariable(int v, const std::string& var_id)
{
	Variable* x = new Variable(v, var_id);
	numExprNodes.push_back(x);
	return x;
}

RelOp* NodeManager::makeRelOp(RelOp::OpCode o, NumExpr* lop, NumExpr* rop)
{
	RelOp* x = new RelOp(o, lop, rop);
	boolExprNodes.push_back(x);
	return x;
}
BoolOp* NodeManager::makeBoolOp(BoolOp::OpCode o, BoolExpr* lop, BoolExpr* rop)
{
	BoolOp* x = new BoolOp(o, lop, rop);
	boolExprNodes.push_back(x);
	return x;
}
BoolConst* NodeManager::makeBoolConst(bool v)
{
	BoolConst* x = new BoolConst(v);
	boolExprNodes.push_back(x);
	return x;
}






