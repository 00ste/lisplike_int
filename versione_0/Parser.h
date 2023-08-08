#ifndef PARSER_H
#define PARSER_H

#include "NodeManager.h"
#include "Block.h"
#include "Statement.h"
#include "NumExpr.h"
#include "BoolExpr.h"
#include "Token.h"

/**
 * Parser si occupa di fare il parsing di un vettore
 * di Token e di generare la struttura dati per memorizzare
 * in modo ordinato il programma.
 */
class Parser
{
public:
	Parser(NodeManager* manager) : nm{ manager } {}
	Block* operator()(const std::vector<Token>& tokenStream);
private:
	NodeManager* nm;

	void throwSyntaxError(Token failedToken);

	Block* parseBlock(std::vector<Token>::const_iterator& i);
	Statement* parseStatement(std::vector<Token>::const_iterator& i);
	NumExpr* parseNumExpr(std::vector<Token>::const_iterator& i);
	BoolExpr* parseBoolExpr(std::vector<Token>::const_iterator& i);
};

#endif