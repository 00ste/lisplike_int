#include <iostream>
#include <string>

#include "ExpressionInterpreter.cpp"

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
	ParseExpression(ExpressionManager& manager) : em{manager} {}

	bool operator()(const std::string& expression)
	{
		int i = recursiveParse(expression, 0);
		return (i == expression.size());
	}
private:
	// la lezione è finita e non l'ha più usato
	ExpressionManager& em;

	bool isDigit(char c)
	{
		return ((c >= '0') && (c <= '9'));
	}

	bool isOperator(char c)
	{
		if (c == '+') return true;
		if (c == '-') return true;
		if (c == '*') return true;
		if (c == '/') return true;
		return false;
	}

	int parseNumber(const std::string& expression, int index)
	{
		// Un numero si può scrivere con l'espressione regolare:
		// 0 + (1+...+9).(0+...+9)*
		if (expression[index ++] == '0')
		{
			// Il numero è un solo 0
			return index;
		}
		// Il numero continua per un numero indeterminato di cifre
		for (; index < expression.size(); index ++)
		{
			if (!isDigit(expression[index]))
				return index;
		}
		return index;
	}

	int recursiveParse(const std::string& expression, int index)
	{
		// Controllo che l'indice sia lecito
		if ((index < 0) || (index >= expression.size()))
			return -1;

		// Un'espressione corretta inizia con una parentesi oppure è un numero
		if (expression[index] == '(')
		{
			// L'espressione è composta da più espressioni
			// Un'espressione composta è del tipo (<exp>@<exp>)
			if (expression[index] != '(') return -1;

			index = recursiveParse(expression, index + 1);
			if (index < 0) return -1;

			if (!isOperator(expression[index])) return -1;

			index = recursiveParse(expression, index + 1);
			if (index < 0) return -1;

			if (expression[index] != ')') return -1;

			return index + 1;
		}
		else if (isDigit(expression[index]))
		{
			// L'espressione è un numero
			return parseNumber(expression, index);
		}
		else
		{
			// L'espressione è errata e restituisco -1
			return -1;
		}
	}
};

int main()
{
	ExpressionManager manager;
	ParseExpression parse{manager};

	std::string exp;
	do {
		std::cout << "Inserire un'espressione: ";
		std::cin >> exp;
		std::cout << parse(exp) << std::endl;
	}
	while (exp != "quit");
}
