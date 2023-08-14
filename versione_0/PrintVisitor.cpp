#include <sstream>

#include "PrintVisitor.h"

/**
 * Per stampare un Block si stampa:
 * - "(BLOCK"
 * - Ciascuno Statement
 * - ")"
 */
void PrintVisitor::visitBlock(Block* blockNode)
{
	std::cout << "(BLOCK ";
	for (Statement* stmt : blockNode->getStatements())
		stmt->accept(this);
	std::cout << ") ";
}

/**
 * Per stampare un PRINT-Statement si stampa:
 * - "(PRINT "
 * - La NumExpr
 * - ")"
 */
void PrintVisitor::visitPrintStmt(PrintStmt* printStmtNode)
{
	std::cout << "(PRINT ";
	printStmtNode->getPrintValue()->accept(this);
	std::cout << ") ";
}

/**
 * Per stampare un SET-Statement si stampa:
 * - "(SET"
 * - La Variable
 * - " "
 * - La NumExpr
 * - ")"
 */
void PrintVisitor::visitSetStmt(SetStmt* setStmtNode)
{
	std::cout << "(SET ";
	setStmtNode->getVarId()->accept(this);
	std::cout << " ";
	setStmtNode->getNewValue()->accept(this);
	std::cout << ") ";
}

/**
 * Per stampare un INPUT-Statement si stampa:
 * - "(INPUT"
 * - La variabile
 * - ")"
 */
void PrintVisitor::visitInputStmt(InputStmt* inputStmtNode)
{
	std::cout << "(INPUT ";
	inputStmtNode->getVarId()->accept(this);
	std::cout << ") ";
}

/**
 * Per stampare un WHILE-Statement si stampa:
 * - "(WHILE "
 * - La BoolExpr
 * - " "
 * - Il Block
 * - ") "
 */
void PrintVisitor::visitWhileStmt(WhileStmt* whileStmtNode)
{
	std::cout << "(WHILE ";
	whileStmtNode->getCondition()->accept(this);
	std::cout << " ";
	whileStmtNode->getBlock()->accept(this);
	std::cout << ") ";
}

/**
 * Per stampare un IF-Statement si stampa:
 * - "(IF "
 * - La BoolExpr
 * - " "
 * - Il Block se BoolExpr è vera
 * - " "
 * - Il Block se BoolExpr è falsa
 * - ") "
 */
void PrintVisitor::visitIfStmt(IfStmt* ifStmtBlock)
{
	std::cout << "(IF ";
	ifStmtBlock->getCondition()->accept(this);
	std::cout << " ";
	ifStmtBlock->getBlockIf()->accept(this);
	std::cout << " ";
	ifStmtBlock->getBlockElse()->accept(this);
	std::cout << ") ";	
}

/**
 * Per stampare un Operator si stampa:
 * - "("
 * - L'OpCode
 * - " "
 * - La prima NumExpr
 * - " "
 * - La seconda NumExpr
 * - ")"
 */
void PrintVisitor::visitOperator(Operator* operatorNode)
{
	std::cout << "(";
	std::cout << Operator::opCodeToStr(operatorNode->getOp());
	std::cout << " ";
	operatorNode->getLeft()->accept(this);
	std::cout << " ";
	operatorNode->getRight()->accept(this);
	std::cout << ") ";
}

/**
 * Per stampare un Numero si stampa il valore
 */
void PrintVisitor::visitNumber(Number* numberNode)
{
	std::cout << numberNode->getValue();
}

/**
 * Per stapmare una Variabile si stampa il nome
 */
void PrintVisitor::visitVariable(Variable* variableNode)
{
	std::cout << variableNode->getName();
}

/**
 * Per stampare un RelOp si stampa:
 * - "("
 * - L'OpCode
 * - " "
 * - La prima NumExpr
 * - " "
 * - La seconda NumExpr
 * - ")"
 */
void PrintVisitor::visitRelOp(RelOp* relOpNode)
{
	std::cout << "(";
	std::cout << RelOp::opCodeToStr(relOpNode->getOp());
	std::cout << " ";
	relOpNode->getLeft()->accept(this);
	std::cout << " ";
	relOpNode->getRight()->accept(this);
	std::cout << ") ";
}

/**
 * Per stampare una BoolConst si stampa il valore
 */
void PrintVisitor::visitBoolConst(BoolConst* boolConstNode)
{
	if (boolConstNode->getValue())
		std::cout << "TRUE";
	else
		std::cout << "FALSE";
}

/**
 * Per stampare un BoolOp si stampa:
 * - "("
 * - L'OpCode
 * - " "
 * - La prima BoolExpr
 * - " "
 * - La seconda BoolExpr
 * - ")"
 * 
 * oppure se l'operazione è un NOT:
 * - "(NOT "
 * - La BoolExpr
 * - ")"
 */
void PrintVisitor::visitBoolOp(BoolOp* boolOpNode)
{
	std::cout << "(";
	std::cout << BoolOp::opCodeToStr(boolOpNode->getOp());
	std::cout << " ";
	boolOpNode->getLeft()->accept(this);
	if (boolOpNode->getOp() != BoolOp::NOT)
	{
		std::cout << " ";
		std::cout << "PRN: Getting second operator: " << boolOpNode->getRight() << std::endl;
		boolOpNode->getRight()->accept(this);
	}
	std::cout << ") ";
}
