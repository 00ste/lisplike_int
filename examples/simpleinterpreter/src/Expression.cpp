#include "Expression.h"

int Operator::interpret() const
	{
		int lval = (*left).interpret();
		int rval = (*right).interpret();
		switch (op)
		{
			case PLUS: 	return lval+rval;
			case MINUS:	return lval-rval;
			case TIMES:	return lval*rval;
			case DIV:	return lval/rval;
			default:	return 0;
		}
