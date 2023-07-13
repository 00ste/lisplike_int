#pragma once
#include <vector>

#include "Expression.h"

// ExpressionManager è responsabile della creazione delle
// espressioni, specialmente per la creazione e la memorizzazione
// in modo ordinato dei nodi.
class ExpressionManager
{
public:
	// il costruttore di default di ExpressionManager chiama
	// il costruttore di default di std::vector e inizializza
	// allocated come un vettore vuoto
	ExpressionManager() = default;
	// quando si elimina il manager, questo è responsabile di deallocare
	// tutti i nodi che ha allocato
	~ExpressionManager() {
		for (Expression* node : allocated)
			delete(node);
	}

	// disabilitate copia e assegnamento per evitare problemi di
	// ownership dei puntatori
	// ad esempio: se viene effettuata una copia dell'oggetto
	// (che comunque si riferisce agli stessi puntatori), una volta
	// deallocata una delle copie gli oggetti sono stati deallocati
	// e la seconda copia diventa inutilizzabile
	ExpressionManager(const ExpressionManager& other) = delete;

	// Factory method per creare i tipi di oggetti necessari
	Expression* makeNumber(int value)
	{
		Expression* n = new Number(value);
		allocated.push_back(n);
		return n;
	}

	Expression* makeOperator(Operator::OpCode op, Expression* l,
		Expression* r)
	{
		Expression* o = new Operator(op, l, r);
		allocated.push_back(o);
		return o;
	}
private:
	std::vector<Expression*> allocated;
};

