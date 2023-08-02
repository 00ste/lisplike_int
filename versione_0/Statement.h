#ifndef STATEMENT_H
#define STATEMENT_H

#include "Block.h"
#include "NumExpr.h"
#include "BoolExpr.h"

// Visitor include già Statement, quindi Statement non può includere
// a sua volta Visitor, ma la classe deve comunque essere dichiarata
class Visitor;

/*
 * Statement generico all'interno del programma
 */
class Statement
{
public:
	virtual ~Statement() {};
	virtual void accept(Visitor* v) = 0;
};

/*
 * IF-Statement (scelta condizionata)
 * 
 * Corrisponde a:
 * (IF boolExpr, stmtBlockTrue, stmtBlockFalse)
 * 
 * Esegue stmtBlockTrue se boolExpr è vera
 * Esegue stmtBlockFalse se boolExpr è falsa
 * 
 * Possono verificarsi gli errori:
 * - la dichiarazione non è sintatticamente corretta
 * - boolExpr non è sintatticamente corretta o contiene variabili non
 *   definite in precedenza
 * - uno dei due stmtBlock non è sintatticamente corretto
 * - uno dei due stmtBlock, se eseguito, contiene errori semantici (ad
 *   esempio divisione per 0)
 */

class IfStmt : public Statement
{
public:
	IfStmt(BoolExpr* c, Block* b_if, Block* b_else)
		: condition{ c }, blockIf{ b_if }, blockElse{ b_else } {};
	// facciamo che vanno bene i costruttori e distruttori di default
	IfStmt(const IfStmt& other) = default;
	~IfStmt() = default;
	void accept(Visitor* v) override;
private:
	BoolExpr* condition;
	Block* blockIf;
	Block* blockElse;
};

/*
 * WHILE-Statement (iterazione)
 *
 * Corrisponde a:
 * (WHILE boolExpr, stmtBlock)
 *
 * Esegue stmtBlock finché boolExpr è vera. L'esecuzione non avviene
 * se smtBlock è falsa in partenza.
 *
 * Possono verificarsi gli errori:
 * - la dichiarazione non è sintatticamente corretta
 * - boolExpr non è sintatticamente corretta o contiene variabili non
 *   definite in precedenza
 * - lo stmtBlock non è sintatticamente corretto
 * - lo stmtBlock, se eseguito, contiene errori semantici (ad
 *   esempio divisione per 0)
 */
class WhileStmt : public Statement
{
public:
	WhileStmt(BoolExpr* c, Block* b)
		: condition{ c }, block{ b } {};
	// facciamo che vanno bene i costruttori e distruttori di default
	WhileStmt(const WhileStmt& other) = default;
	~WhileStmt() = default;
	void accept(Visitor* v) override;
private:
	BoolExpr* condition;
	Block* block;
};

/*
 * INPUT-Statement
 *
 * Corrisponde a:
 * (INPUT id_variabile)
 *
 * Crea la variabile con id id_variabile se non era già presente, e assegna
 * ad essa un valore ottenuto in input da terminale.
 *
 * Possono verificarsi gli errori:
 * - l'istruzione non è sintatticamente corretta
 * - id_variabile è una parola chiave
 * - viene inserito da console un valore illecito
 */
class InputStmt : public Statement
{
public:
	InputStmt(Variable* var_id)
		: varId{ var_id } {};
	// facciamo che vanno bene i costruttori e distruttori di default
	InputStmt(const InputStmt& other) = default;
	~InputStmt() = default;
	void accept(Visitor* v) override;
private:
	Variable* varId;
};

/*
 * SET-Statement (dichiarazione di variabili)
 *
 * Corrisponde a:
 * (SET id_variabile, num_expr)
 *
 * Crea la variabile con id id_variabile se non era già presente, e assegna
 * ad essa il valore dell'espressione num_expr
 *
 * Possono verificarsi gli errori:
 * - la dichiarazione non è sintatticamente corretta
 * - id_variabile è una parola chiave
 * - la num_expr non è sintatticamente corretta
 * - la num_expr, se eseguita, contiene errori semantici (ad
 *   esempio divisione per 0)
 */
class SetStmt : public Statement
{
public:
	SetStmt(Variable* var_id, NumExpr* num_expr)
		: varId{ var_id }, newValue{ num_expr } {};
	// facciamo che vanno bene i costruttori e distruttori di default
	SetStmt(const SetStmt& other) = default;
	~SetStmt() = default;
	void accept(Visitor* v) override;
private:
	Variable* varId;
	NumExpr* newValue;
};

/*
 * dopo
 */
class PrintStmt : public Statement
{
public:
	PrintStmt(NumExpr* num_expr)
		: printValue{ num_expr } {};
	// facciamo che vanno bene i costruttori e distruttori di default
	PrintStmt(const PrintStmt& other) = default;
	~PrintStmt() = default;
	void accept(Visitor* v) override;
private:
	NumExpr* printValue;
};


#endif