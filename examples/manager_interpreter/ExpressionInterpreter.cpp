// GRAMMATICA CONTIENE SOLO ESPRESSIONI ARITMETICHE CON
// NUMERI INTERI E OPERATORI ARITMETICI DI BASE

#include <iostream>
#include <vector>

// Classe base astratta Expression per espressioni generiche
class Expression
{
public:
	virtual ~Expression() {};
	virtual int interpret() const = 0;
};

// Classe derivata per numeri interi
class Number : public Expression
{
public:
	Number(int v) : value{v} {}
	~Number() {}

	// costruttore di copia
	Number(const Number& other) = default;
	// costruttore di assegnamento
	Number& operator=(const Number& other) = default;

	int getValue() const { return value; }

	// override di interpret()
	int interpret() const override { return value; }
private:
	int value;
};

// Classe derivata per gli operatori
// ovvero espressioni numeriche contenenti un operatore
class Operator : public Expression
{
public:
	enum OpCode { PLUS, MINUS, TIMES, DIV };

	Operator(OpCode o, Expression* lop, Expression* rop) :
		op{o}, left{lop}, right{rop} {}

	// gli operatori sono dei puntatori, vengono forniti dall'esterno
	// quindi gli oggetti esistono già, e non dovrebbe essere compito
	// della classe deallocarli (distruggerli)
	~Operator() = default;
	// costruttore di copia, non è una copia "profonda" perché copia
	// solo i puntatori e non gli oggetti in sé. È come se fosse un
	// alias dello stesso oggetto fornito
	Operator(const Operator& other) = default;
	// costruttore di assegnamento
	Operator& operator=(const Operator& other) = default;

	// metodi di accesso
	OpCode getOp() const { return op; }
	Expression* getLeft() const { return left; }
	Expression* getRight() const { return right; }

	// override del metodo interpret():
	// - i singoli operandi sono espressioni e vanno interpretati
	// - deve essere svolta l'operazione specificata da op sui due
	//   operandi
	int interpret() const override
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
	}


private:
	OpCode		op;
	Expression* left;
	Expression* right;
};


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


int main()
{
	ExpressionManager em;

	// Espressione di prova: (5+7)*12 = 12*12 = 144
	Expression* a = em.makeNumber(5);
	Expression* b = em.makeNumber(7);
	Expression* c = em.makeNumber(12);
	Expression* sum = em.makeOperator(Operator::PLUS, a, b);
	Expression* prod = em.makeOperator(Operator::TIMES, sum, c);


	std::cout << "(5+7)*12 = " << (*prod).interpret() << std::endl;
}
