#ifndef BOOL_EXPR_H
#define BOOL_EXPR_H

#include "NumExpr.h"
#include "Token.h"

// Visitor include già BoolExpr, quindi BoolExpr non può includere
// a sua volta Visitor, ma la classe deve comunque essere dichiarata
class Visitor;

/*
 * Espressione booleana (true/false) generica
 */
class BoolExpr
{
public:
	virtual ~BoolExpr() {};
	virtual void accept(Visitor* v) = 0;
};

/*
 * Rappresenta un'operazione di confronto (GT, LS, EQ)
 * tra due espressioni numeriche
 */
class RelOp : public BoolExpr
{
public:
	enum OpCode { GT, LT, EQ, ERR };

	RelOp(OpCode o, NumExpr* lop, NumExpr* rop) :
		operation{ o }, left{ lop }, right{ rop } {}
	RelOp(const RelOp& other) = default;
	~RelOp() = default;
	RelOp& operator=(const RelOp& other) = default;

	void accept(Visitor* v) override;

	OpCode getOp() const { return operation; }
	NumExpr* getLeft() const { return left; }
	NumExpr* getRight() const { return right; }

	static OpCode tokenToOpCode(const Token& t);
private:
	OpCode operation;
	NumExpr* left;
	NumExpr* right;
};

/*
 * Rappresenta un'operazione booleana (AND, OR, NOT)
 * tra due espressioni booleane
 */
class BoolOp : public BoolExpr
{
public:
	enum OpCode { AND, OR, NOT, ERR };

	BoolOp(OpCode o, BoolExpr* lop, BoolExpr* rop) :
		operation{ o }, left{ lop }, right{ rop } {}
	BoolOp(const BoolOp& other) = default;
	~BoolOp() = default;
	BoolOp& operator=(const BoolOp& other) = default;

	void accept(Visitor* v) override;

	OpCode getOp() const { return operation; }
	BoolExpr* getLeft() const { return left; }
	BoolExpr* getRight() const
	{
		if (operation == NOT)
			return nullptr;
		return right;
	}

	static OpCode tokenToOpCode(const Token& t);
private:
	OpCode operation;
	BoolExpr* left;
	BoolExpr* right;
};

/**
 * Rappresenta una costante booleana (true/false)
 */
class BoolConst : public BoolExpr
{
public:
	BoolConst(bool v) : value{ v } {}
	BoolConst(const BoolConst& other) = default;
	~BoolConst() {}
	BoolConst& operator=(const BoolConst& other) = default;

	void accept(Visitor* v) override;

	int getValue() const { return value; }
private:
	bool value;
};

#endif