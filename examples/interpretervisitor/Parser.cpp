#include <sstream>
#include "Parser.h"

// Analisi di un numero definito dall'espressione regolare
// 0 + (1+...+9).(0+...+9)* e creazione del nodo
int ParseExpression::parseNumber(const std::string& expression, int index)
{
    // buffer per memorizzare il valore del numero
    std::stringstream temp{};

    // caso 0
	if (expression[index] == '0')
	{
	    if (index < expression.size() && isDigit(expression[index+1]))
	        return -1;
	    else
            index ++;
	}
	// caso (0+...+9)*
	// il primo carattere (1,...,9) non è 0 perché altrimenti
	// sarebbe stato vero l'if sopra
	else
	    while(index < expression.size() && isDigit(expression[index]))
	    {
	        temp << expression[index];
	        index ++;
        }
	// espressione sicuramente corretta
	// conversione dello stream in valore
	int value;
    temp >> value;
	(*this).expression = em.makeNumber(value);
	return index;
}

// Analisi di un'espressione definita dalla grammatica
// <exp> ::= <number> | (<exp>@<exp>)
// dove @ è un operatore aritmetico di base +,-,*,/
int ParseExpression::recursiveParse(const std::string& expression, int index)
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

bool ParseExpression::operator()(const std::string& expression)
{
	int i = recursiveParse(expression, 0);
	return (i == expression.size());
}
