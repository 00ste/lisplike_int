#ifndef PARSER_H
#define PARSER_H

#include <string>

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

	void throwSyntaxError(Token failedToken, std::string expectedToken);

	Block* parseBlock(std::vector<Token>::const_iterator& itr);
	Statement* parseStatement(std::vector<Token>::const_iterator& itr);
	Block* parseStmtBlock(std::vector<Token>::const_iterator& itr);
	NumExpr* parseNumExpr(std::vector<Token>::const_iterator& itr);
	BoolExpr* parseBoolExpr(std::vector<Token>::const_iterator& itr);
};

#endif