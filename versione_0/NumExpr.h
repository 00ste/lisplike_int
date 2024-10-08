#ifndef NUM_EXPR_H
#define NUM_EXPR_H

#include <string>

#include "Token.h"

// Visitor include gi� NumExpr, quindi NumExpr non pu� includere
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
	enum OpCode { PLUS, MINUS, TIMES, DIV, ERR };

	Operator(OpCode o, NumExpr* lop, NumExpr* rop) :
		operation{ o }, left{ lop }, right{ rop } {}
	Operator(const Operator& other) = default;
	~Operator() = default;

	void accept(Visitor* v) override;

	OpCode getOp() const { return operation; }
	NumExpr* getLeft() const { return left; }
	NumExpr* getRight() const { return right; }

	static OpCode tokenToOpCode(const Token& t);
	static std::string opCodeToStr(OpCode o);
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

	// mentre i visitor sono molteplici, il metodo accept � unico.
	// in effetti l'oggetto non � in grado n� di interpretarsi e
	// n� di stampare s� stesso, ma "chiama" un visitor esterno
	// specializzato in quell'operazione in modo che la faccia al
	// suo posto.
	void accept(Visitor* v) override;

private:
	int value;
};

/*
 * Rappresenta variabile con un id a valore intero.
 * Il valore non viene memorizzato all'interno della
 * struttura dati, poich� la variabile assume dei
 * valori solamente a runtime.
 */
class Variable : public NumExpr
{
public:
	Variable(const std::string& var_id) :
		name{ var_id } {}
	Variable(const Variable& other) = default;
	~Variable() = default;

	void accept(Visitor* v) override;

	std::string getName() const { return name; }
private:
	std::string name;
};

#endif