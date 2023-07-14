#include "Expression.h"
// in questo file posso includere Visitor.h perché Expression.cpp
// non è incluso da nessuna parte
#include "Visitor.h"


void Number::accept(Visitor* v)
{
	(*v).visitNumber(this);
}

void Operator::accept(Visitor* v)
{
	(*v).visitOperator(this);
}

