#ifndef NUM_EXPR_H
#define NUM_EXPR_H

#include <string>

// Visitor include già NumExpr, quindi NumExpr non può includere
// a sua volta Visitor, ma la classe deve comunque essere dichiarata
class Visitor;

/*
 * Espressione numerica generica
 */
class NumExpr
{
public:
	virtual ~NumExpr() {};
	virtual void accept(Visitor* v) = 0;
};

/*
 * Rappresenta un'operazione aritmetica (ADD, SUB, MUL, DIV)
 * tra due espressioni numeriche
 */
class Operator : public NumExpr
{
public:
	enum OpCode { PLUS, MINUS, TIMES, DIV };

	Operator(OpCode o, NumExpr* lop, NumExpr* rop) :
		operation{ o }, left{ lop }, right{ rop } {}
	Operator(const Operator& other) = default;
	~Operator() = default;

	void accept(Visitor* v) override;

	OpCode getOp() const { return operation; }
	NumExpr* getLeft() const { return left; }
	NumExpr* getRight() const { return right; }
private:
	OpCode operation;
	NumExpr* left;
	NumExpr* right;
};

/*
 * Rappresenta un numero intero costante
 */
class Number : public NumExpr
{
public:
	Number(int v) : value{ v } {}
	Number(const Number& other) = default;
	~Number() {}
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

/*
 * Rappresenta variabile con un id a valore intero
 */
class Variable : public NumExpr
{
public:
	Variable(int v, std::string var_id) : name{ var_id } {}
	Variable(const Variable& other) = default;
	~Variable() = default;

	void accept(Visitor* v) override;

	std::string getName() const { return name; }
private:
	// il valore direi che non serve, perché viene memorizzato
	// all'interno del visitor di esecuzione in fase di esecuzione
	// int value;
	std::string name;
};

#endif