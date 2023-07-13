#pragma once
#include <string>

#include "Expression.h"
#include "ExpressionManager.h"

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
	ParseExpression(ExpressionManager& manager) :
        em{manager},expression{nullptr} {}
	// metodo di accesso
    Expression* getExpression() const {return expression;}
    // funzione implementata dal function object
	bool operator()(const std::string& expression);

private:
	ExpressionManager& em;
    Expression* expression;

	bool isDigit(char c)
	{
		return ((c >= '0') && (c <= '9'));
	}


	bool isOperator(char c)
	{
		return ((c == '+') || (c == '-') || (c == '*') || (c == '/'));
	}

	int parseNumber(const std::string& expression, int index);

	int recursiveParse(const std::string& expression, int index);
};
