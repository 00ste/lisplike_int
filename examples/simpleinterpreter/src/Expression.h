#pragma once

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
	static OpCode getOp() const { return op; }
	Expression* getLeft() const { return left; }
	Expression* getRight() const { return right; }

	// override del metodo interpret():
	// - i singoli operandi sono espressioni e vanno interpretati
	// - deve essere svolta l'operazione specificata da op sui due
	//   operandi
	int interpret() const override;
	}


private:
	OpCode		op;
	Expression* left;
	Expression* right;
};
)
