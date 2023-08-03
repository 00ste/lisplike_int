#include "Block.h"
#include "Visitor.h"
// Dentro il file *.cpp è possibile includere Visitor

void Block::accept(Visitor* v)
{
	(*v).visitBlock(this);
}