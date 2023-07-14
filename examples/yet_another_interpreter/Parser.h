#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

#include "Expression.h"
#include "ExpressionManager.h"
#include "Token.h"

/*
 Parser che riconosce le stringhe della CFG:
 G = (V, T, P, S)
 dove:
 V = {<exp>, <number>, <first>, <digit>, <remainder>}
 T = {0,1,2,3,4,5,6,7,8,9,+,-,*,/,(,)}
 P = {
	<exp>		::= (<exp>+<exp>) | (<exp>-<exp>) | (<exp>*<exp>) | (<exp>/<exp>) ,
	<exp>		::= <number> ,
	<number>	::= <first><remainder> | 0 ,
	<first>		::= 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 ,
	<digit>		::= <first> | 0 ,
	<remainder>	::= <digit><remainder> | epsilon
 }
 S = <exp>
 */

// function object per il parsing di espressioni
class ParseExpression
{
public:
	// costruttore
	ParseExpression(ExpressionManager& manager) : em{manager} {}
	// metodo di accesso
    Expression* getExpression() const {return expression;}
    // funzione implementata dal function object
	Expression* operator()(const std::vector<Token>& tokenStream);

private:
	ExpressionManager& em;

	Expression* recursiveParse(std::vector<Token>::const_iterator& itr);
};

#endif
