#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include <iterator>
#include "Statement.h"

// Visitor include già Block, quindi Block non può includere
// a sua volta Visitor, ma la classe deve comunque essere dichiarata
class Visitor;


class Block
{
public:
	Block() {};
	Block(const Block& other) = delete;
	~Block() = default;
	
	void accept(Visitor* v);
	
	void appendStatement(Statement* statement)
	{
		statements.push_back(statement);
	}
	const std::vector<Statement*>& getStatements() const
	{
		return statements;
	}
	
	
private:
	std::vector<Statement*> statements;
};

#endif