#include <sstream>
#include <vector>

#include "Parser.h"
#include "Token.h"

// Analisi di un'espressione definita dalla grammatica
// <exp> ::= <number> | (<exp>@<exp>)
// dove @ è un operatore aritmetico di base +,-,*,/
Expression* recursiveParse(std::vector<Token>::const_iterator& itr)
{
	// Un'espressione corretta inizia con una parentesi oppure con un numero
	if ((*itr).tag == Token::LP)
	{
		// L'espressione è composta da più espressioni
		// Un'espressione composta è del tipo (<exp>@<exp>)
		// Parentesi già controllata, manca verificare <exp>@<exp>)

		index = recursiveParse(expression, index + 1);
		if (index < 0) return -1;
		// memorizzo il puntatore all'oggetto Expression risultato dal parsing
		// dell'operando sinistro
        Expression* left = (*this).expression;

		if (!isOperator(expression[index])) return -1;
		// memorizzo l'operatore
		char op = expression[index];

		index = recursiveParse(expression, index + 1);
		if (index < 0) return -1;

		if (expression[index] != ')') return -1;
		// memorizzo il puntatore all'oggetto Expression risultato dal parsing
		// dell'operando destro
		Expression* right = (*this).expression;

        (*this).expression = em.makeOperator(Operator::charToOpCode(op),
            left, right);

		return index + 1;
	}
	else if (isDigit(expression[index]))
	{
		// L'espressione è un numero
		return parseNumber(expression, index);
	}
	else
	{bool ParseExpression::operator()(const std::string& expression)
{
	int i = recursiveParse(expression, 0);
	return (i == expression.size());
}
		// L'espressione è errata e restituisco -1
		return -1;
	}
}

Expression* ParseExpression::operator()(const std::vector<Token>& tokenStream)
{
	// gestione con iteratori anziché una variabile di indice
	auto tokenItr = tokenStream.begin();
	Expression* expr = recursiveParse(tokenItr);
	// controllare un attimo se sono arrivato in fondo al tokenStream
	return expr;
}
