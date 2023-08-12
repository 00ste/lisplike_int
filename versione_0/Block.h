#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include <iterator>

// DEBUG
#include <iostream>

#include "Statement.h"

// Visitor include già Block, quindi Block non può includere
// a sua volta Visitor, ma la classe deve comunque essere dichiarata
class Visitor;


class Block
{
public:
	Block() = default;
	Block(const Block& other) = delete;
	~Block() = default;
	
	void accept(Visitor* v);
	
	void appendStatement(Statement* statement)
	{
		//std::cout << "BLK: appending Statement to Block " << this << std::endl;
		statements.push_back(statement);
		//std::cout << "BLK: Statements are now " << statements.size() << std::endl;
	}
	const std::vector<Statement*>& getStatements() const
	{
		return statements;
	}
	
	
private:
	std::vector<Statement*> statements;
};

#endif