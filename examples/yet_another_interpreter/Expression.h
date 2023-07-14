#ifndef EXPRESSION_H
#define EXPRESSION_H

// non si può includere Visitor.h all'interno di Expression.h,
// perché Epression.h è già incluso all'interno di Visitor.h
// si definisce semplicemente la classe
class Visitor;

// Classe base astratta Expression per espressioni generiche
class Expression
{
public:
	virtual ~Expression() {};
	virtual void accept(Visitor* v);
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

	// mentre i visitor sono molteplici, il metodo accept è unico.
	// in effetti l'oggetto non è in grado né di interpretarsi e
	// né di stampare sé stesso, ma "chiama" un visitor esterno
	// specializzato in quell'operazione in modo che la faccia al
	// suo posto.
	void accept(Visitor* v) override;

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

	void accept(Visitor* v) override;

	// metodo di utilità per convertire i caratteri in OpCode
    static OpCode charToOpCode(char c)
    {
        switch (c)
        {
        case '+': return PLUS;
        case '-': return MINUS;
        case '*': return TIMES;
        case '/': return DIV;
        }
    }

    // metodo di utilità per convertire i caratteri in OpCode
    static char opCodeToChar(OpCode op)
    {
        switch (op)
        {
        case PLUS: 	return '+';
        case MINUS: return '-';
        case TIMES: return '*';
        case DIV: 	return '/';
        }
    }


private:
	OpCode		op;
	Expression* left;
	Expression* right;
};

#endif
