#ifndef PARSER_H
#define PARSER_H

#include "NodeManager.h"
#include "Block.h"
#include "Token.h"

/**
 * Parser si occupa di fare il parsing di un vettore
 * di Token e di generare la struttura dati per memorizzare
 * in modo ordinato il programma.
 */
class Parser
{
public:
	Parser(NodeManager& manager) : nm{ manager },
		program{} {}
	Block* getProgram() const { return program; }
	Block* operator()(const std::vector<Token>& tokenStream);
private:
	NodeManager& nm;
	Block* program;

	void parseBlock(std::vector<Token>::const_iterator& i);
	void parseStatement(std::vector<Token>::const_iterator& i);
	void parseNumExpr(std::vector<Token>::const_iterator& i);
	void parseBoolExpr(std::vector<Token>::const_iterator& i);
};

#endif