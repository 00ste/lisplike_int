#ifndef STATEMENT_H
#define STATEMENT_H

#include "NumExpr.h"
#include "BoolExpr.h"

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

class IfStatement : public Statement
{
public:
	IfStatement(const BoolExpr& c, const Block& b_if, const Block& b_else)
		: condition{ c }, blockIf{ b_if }, blockElse{ b_else } {};
	// facciamo che vanno bene i costruttori e distruttori di default
	IfStatement(const IfStatement& other) = default;
	~IfStatement() = default;
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
class WhileStatement : public Statement
{
public:
	WhileStatement(const BoolExpr& c, const Block& b)
		: condition{ c }, block{ b } {};
	// facciamo che vanno bene i costruttori e distruttori di default
	WhileStatement(const WhileStatement& other) = default;
	~WhileStatement() = default;
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
class InputStatement : public Statement
{
public:
	InputStatement(const Variable& var_id)
		: varId{ var_id } {};
	// facciamo che vanno bene i costruttori e distruttori di default
	InputStatement(const InputStatement& other) = default;
	~InputStatement() = default;
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
class SetStatement : public Statement
{
public:
	SetStatement(const Variable& var_id, const NumExpr& num_expr)
		: varId{ var_id }, newValue{ num_expr } {};
	// facciamo che vanno bene i costruttori e distruttori di default
	SetStatement(const SetStatement& other) = default;
	~SetStatement() = default;
	void accept(Visitor* v) override;
private:
	Variable* varId;
	NumExpr* newValue;
};

/*
 * dopo
 */
class PrintStatement : public Statement
{
public:
	PrintStatement(const NumExpr& num_expr)
		: printValue{ num_expr } {};
	// facciamo che vanno bene i costruttori e distruttori di default
	PrintStatement(const PrintStatement& other) = default;
	~PrintStatement() = default;
	void accept(Visitor* v) override;
private:
	NumExpr* printValue;
};


#endif