#include "ExecutionVisitor.h"
#include "Block.h"
#include "Statement.h"
#include "NumExpr.h"
#include "BoolExpr.h"
#include "Exceptions.h"

#include <iostream>
#include <string>
#include <sstream>



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * EXECUTIONVISITOR PER BLOCK E STATEMENTS
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

 /**
  * ExecutionVisitor PER BLOCK
  *
  * Per eseguire un blocco di statements bisogna eseguire
  * ogni statement contenuto nel blocco
  */
void ExecutionVisitor::visitBlock(Block* blockNode)
{
	//std::cout << "EXE: Executing Block " << blockNode << " that contains " << blockNode->getStatements().size() << " Statements:" << std::endl;
	for (Statement* stmt : blockNode->getStatements())
	{
		//std::cout << "EXE: Executing Statement from Block..." << std::endl;
		stmt->accept(this);
	}
}

/**
 * ExecutionVisitor PER IF-STATEMENT
 *
 * Per eseguire un if-statement bisogna:
 * - eseguire il confronto specificato
 * - eseguire il blocco corrispondente al risultato
 *   del confonto
 */
void ExecutionVisitor::visitIfStmt(IfStmt* ifStmtNode)
{
	////std::cout << "EXE: Begin IF-Statement" << std::endl;
	// valutazione della condizione
	ifStmtNode->getCondition()->accept(this);

	// il risultato dovrebbe essere in cima alla pila
	bool condition = boolStack.back();
	boolStack.pop_back();

	// eseguo il blocco corrispondente
	if (condition)
		ifStmtNode->getBlockIf()->accept(this);
	else
		ifStmtNode->getBlockElse()->accept(this);
}

/**
 * ExecutionVisitor PER WHILE-STATEMENT
 *
 * Per eseguire un while-statement bisogna:
 * - eseguire il confronto specificato
 * - se la condizione è vera eseguire il blocco e ripetere
 *   al termine dell'esecuzione
 * - se la condizione è falsa non fare niente
 */
void ExecutionVisitor::visitWhileStmt(WhileStmt* whileStmtNode)
{
	////std::cout << "EXE: Begin WHILE-Statement" << std::endl;
	while (true)
	{
		//std::cout << "EXE: New WHILE cycle..." << std::endl;
		//std::cout << "EXE: Checking WHILE condition" << std::endl;
		// valutazione della condizione
		whileStmtNode->getCondition()->accept(this);

		// il risultato dovrebbe essere in cima alla pila
		bool condition = boolStack.back();
		boolStack.pop_back();

		// se la condizione è vera eseguo il blocco, altrimenti esco
		if (condition)
			whileStmtNode->getBlock()->accept(this);
		else
			return;
	}
}

/**
 * ExecutionVisitor PER INPUT-STATEMENT
 *
 * Per eseguire un input-statement bisogna:
 * - ricevere un input da tastiera
 * - TODO: controllare l'input e gestire eccezioni
 * - (creare la variabile nel caso in cui non esista)
 * - assegnare il valore fornito in input alla variabile
 */
void ExecutionVisitor::visitInputStmt(InputStmt* inputStmtNode)
{
	//std::cout << "EXE: Begin INPUT-Statement on Variable " << inputStmtNode->getVarId()->getName() << std::endl;
	// input da tastiera
	std::string inputString;
	std::cin >> inputString;
	int inputValue;
	try
	{
		inputValue = std::stoi(inputString);
	}
	catch (std::exception e)
	{
		std::stringstream errorMessage;
		errorMessage << inputString;
		errorMessage << " is not a valid variable value, must be an integer number";
		throw InputError(errorMessage.str());
	}



	// usando una std::map non serve controllare se la variabile
	// esiste, l'operatore [] fa già quello che mi serve
	std::string variableName = inputStmtNode->getVarId()->getName();
	variables[variableName] = inputValue;

	//std::cout << "EXE: Variable " << variableName << " has been set to " << variables[variableName] << std::endl;
}

/**
 * ExecutionVisitor PER SET-STATEMENT
 *
 * Per eseguire un set-statement bisogna:
 * - calcolare il nuovo valore a partire dall'espressione
 *   numerica fornita
 * - (creare la variabile nel caso in cui non esista)
 * - assegnare il nuovo valore alla variabile
 */
void ExecutionVisitor::visitSetStmt(SetStmt* setStmtNode)
{
	//std::cout << "EXE: Begin SET-Statement on Variable " << setStmtNode->getVarId()->getName() << std::endl;
	// calcolo del nuovo valore
	setStmtNode->getNewValue()->accept(this);

	// il risultato dovrebbe essere in cima alla pila
	int newValue = intStack.back();
	intStack.pop_back();

	// usando una std::map non serve controllare se la variabile
	// esiste, l'operatore [] fa già quello che mi serve
	std::string variableName = setStmtNode->getVarId()->getName();
	variables[variableName] = newValue;

	//std::cout << "EXE: Variable " << variableName << " has been set to " << variables[variableName] << std::endl;
}

/**
 * ExecutionVisitor PER PRINT-STATEMENT
 *
 * Per eseguire un print-statement bisogna:
 * - calcolare il nuovo valore a partire dall'espressione
 *   numerica fornita
 * - stampare il valore su terminale
 */
void ExecutionVisitor::visitPrintStmt(PrintStmt* printStmtNode)
{
	//std::cout << "EXE: Begin PRINT-Statement" << std::endl;
	// calcolo del valore da stampare
	printStmtNode->getPrintValue()->accept(this);
	// il risultato dovrebbe essere in cima alla pila
	int printValue = intStack.back();
	intStack.pop_back();

	// stampa del valore
	std::cout << printValue << std::endl;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * EXECUTIONVISITOR PER ESPRESSIONI NUMERICHE E BOOLEANE
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

 /**
  * ExecutionVisitor PER OPERATORI NUMERICI
  *
  * Per eseguire (valutare) un'operazione aritmetica tra due espressioni
  * numeriche bisogna:
  * - eseguire (valutare) la prima espressione e scriverla sulla pila
  * - eseguire (valutare) la seconda espressione e scriverla sulla pila
  * - leggere gli operatori dalla pila e eseguire l'operazione specificata
	  sugli operatori
  * - scrivere il risultato sulla pila perché sia accessibile
  *   agli altri visitor
  *
  * Se l'operazione è matematicamente illegale, lancia un MathError
  */
void ExecutionVisitor::visitOperator(Operator* operatorNode)
{
	//std::cout << "EXE: Begin Operation between NumExpr operands" << std::endl;
	// calcolo degli operatori
	operatorNode->getLeft()->accept(this);
	operatorNode->getRight()->accept(this);

	// lettura degli operatori dalla pila
	int operandRight = intStack.back();
	intStack.pop_back();
	int operandLeft = intStack.back();
	intStack.pop_back();

	//std::cout << "EXE: Operands of " << Operator::opCodeToStr(operatorNode->getOp()) << " are " << operandLeft << " and " << operandRight << std::endl;
	//std::cout << "EXE: Numbers stack after popping operands of " << Operator::opCodeToStr(operatorNode->getOp()) << " has " << boolStack.size() << " elements:";
	//for (int x : boolStack)
	//	std::cout << x << " ";
	//std::cout << std::endl;

	// calcolo dell'operazione e scrittura sulla pila
	switch (operatorNode->getOp())
	{
	case Operator::PLUS:
		intStack.push_back(operandLeft + operandRight);
		return;
	case Operator::MINUS:
		intStack.push_back(operandLeft - operandRight);
		return;
	case Operator::TIMES:
		intStack.push_back(operandLeft * operandRight);
		return;
	case Operator::DIV:
		// La divisione per 0 non è ammessa
		if (operandRight == 0)
		{
			throw MathError("division by 0.");
		}
		intStack.push_back(operandLeft / operandRight);
		return;
	}
}

/**
 * ExecutionVisitor PER COSTANTI NUMERICHE
 *
 * Per eseguire (valutare) costanti numeriche basta scrivere sulla
 * pila il valore numerico della costante.
 */
void ExecutionVisitor::visitNumber(Number* numberNode)
{
	//std::cout << "EXE: Number eval started" << std::endl;
	intStack.push_back(numberNode->getValue());
	//std::cout << "EXE: Numbers stack after Number insertion has " << intStack.size() << " elements:";
	//for (int x : intStack)
	//	std::cout << x << "; ";
	//std::cout << std::endl;
}

/**
 * ExecutionVisitor PER VARIABILI
 *
 * Per eseguire (valutare) variabili basta scrivere sulla
 * pila il valore della variabile.
 *
 * Se la variabile non è mai stata inizializzata e non è presente
 * nella std::map variables, lancia un UndefinedReferenceError.
 */
void ExecutionVisitor::visitVariable(Variable* variableNode)
{
	//std::cout << "EXE: Variable eval started (" << variableNode->getName() << ")" << std::endl;

	if (variables.find(variableNode->getName()) == variables.end())
	{
		std::stringstream errorMessage{};
		errorMessage << "Undefined variable ";
		errorMessage << variableNode->getName();
		throw UndefinedReferenceError(errorMessage.str());
	}
	//std::cout << "EXE: variables.end() OK" << std::endl;

	intStack.push_back(variables[variableNode->getName()]);

	//std::cout << "EXE: Numbers stack after Variable insertion has " << intStack.size() << " elements:";
	//for (int x : intStack)
	//	std::cout << x << "; ";
	//std::cout << std::endl;
}

/**
 * ExecutionVisitor PER OPERATORI RELAZIONALI
 *
 * Per eseguire (valutare) un'operazione relazionale tra due espressioni
 * numeriche bisogna:
 * - eseguire (valutare) la prima espressione e scriverla sulla pila
 * - eseguire (valutare) la seconda espressione e scriverla sulla pila
 * - leggere gli operatori dalla pila e eseguire l'operazione specificata
	 sugli operatori
 * - scrivere il risultato sulla pila perché sia accessibile
 *   agli altri visitor
 */
void ExecutionVisitor::visitRelOp(RelOp* relOpNode)
{
	//std::cout << "EXE: Begin RelOp between NumExpr operands" << std::endl;
	// calcolo degli operatori

	//std::cout << "EXE: Numbers stack before operand eval has " << intStack.size() << " elements:";
	//for (int x : intStack)
	//	std::cout << x << "; ";
	//std::cout << std::endl;

	relOpNode->getLeft()->accept(this);
	relOpNode->getRight()->accept(this);

	//std::cout << "EXE: Numbers stack after operands eval has " << intStack.size() << " elements:";
	//for (int x : intStack)
	//	std::cout << x << "; ";
	//std::cout << std::endl;

	// lettura degli operatori dalla pila
	int operandRight = intStack.back();
	intStack.pop_back();
	int operandLeft = intStack.back();
	intStack.pop_back();

	//std::cout << "!!!EXE: Operands of " << RelOp::opCodeToStr(relOpNode->getOp()) << " are " << operandLeft << " and " << operandRight << std::endl;
	//std::cout << "EXE: Numbers stack after popping operands of " << RelOp::opCodeToStr(relOpNode->getOp()) << " has " << intStack.size() << " elements: ";
	//for (int x : intStack)
	//	std::cout << x << "; ";
	//std::cout << std::endl;
	//std::cout << "EXE: Bool stack before insertion has " << boolStack.size() << " elements: ";
	//for (int x : boolStack)
	//	std::cout << x << " ";
	//std::cout << std::endl;

	// calcolo dell'operazione e scrittura sulla pila
	switch (relOpNode->getOp())
	{
	case RelOp::EQ:
		boolStack.push_back(operandLeft == operandRight);
		//std::cout << "EXE: Bool stack after insertion has " << boolStack.size() << " elements: ";
		//for (int x : boolStack)
		//	std::cout << x << " ";
		//std::cout << std::endl;
		return;
	case RelOp::GT:
		boolStack.push_back(operandLeft > operandRight);
		//std::cout << "EXE: Bool stack after insertion has " << boolStack.size() << " elements: ";
		//for (int x : boolStack)
		//	std::cout << x << " ";
		//std::cout << std::endl;
		return;
	case RelOp::LT:
		boolStack.push_back(operandLeft < operandRight);
		//std::cout << "EXE: Bool stack after insertion has " << boolStack.size() << " elements: ";
		//for (int x : boolStack)
		//	std::cout << x << " ";
		//std::cout << std::endl;
		return;
	}
}

/**
 * ExecutionVisitor PER COSTANTI BOOLEANE
 *
 * Per eseguire (valutare) costanti numeriche basta scrivere sulla
 * pila il valore numerico della costante
  */
void ExecutionVisitor::visitBoolConst(BoolConst* boolConstNode)
{
	boolStack.push_back(boolConstNode->getValue());
}

/**
 * ExecutionVisitor PER OPERATORI BOOLEANI
 *
 * Per eseguire (valutare) un'operazione booleana tra due espressioni
 * booleane bisogna:
 * - eseguire (valutare) la prima espressione e scriverla sulla pila
 * - se l'operazione è un NOT, scrivere il risultato sulla pila e uscire
 * - se l'operatore è un AND, se la prima espressione è falsa, esci
 *   senza modificare la pila (cortocircuitazione)
 * - se l'operatore è un OR, se la prima espressione è vera, uscire
 *   senza modificare la pila (cortocircuitazione)
 * - eseguire (valutare) la seconda espressione e scriverla sulla pila
 * 
 * CORTOCIRCUITAZIONE
 * 
 * Gli operatori AND e NOT sono cortocircuitati, quindi:
 * Per l'operazione (AND a b)
 *   - se a è falsa, l'intera operazione è falsa, ovvero il risultato
 *     dell'operazione è il risultato di a, non serve rimuovere o aggiungere
 *     nulla alla pila.
 *   - se a è vera, allora il risultato dell'operazione è il risultato di b,
 *     una volta calcolato b non serve rimuovere o aggiungere nulla alla pila.
 * Un ragionamento analogo si può fare per OR.
 */

void ExecutionVisitor::visitBoolOp(BoolOp* boolOpNode)
{
	// Per ogni operatore si valuta il primo operando
	boolOpNode->getLeft()->accept(this);
	bool opLeft = boolStack.back();

	// Se l'operazione è un NOT basta il primo operatore
	if (boolOpNode->getOp() == BoolOp::NOT)
	{
		boolStack.pop_back();
		boolStack.push_back(opLeft);
		return;
	}
	// Se l'operatore è un AND si valuta il primo operando
	// per la cortocircuitazione
	if (boolOpNode->getOp() == BoolOp::AND)
	{
		std::cout << "EXE: AND Detected!" << std::endl;
		if (!opLeft)
		{
			std::cout << "EXE: SC AND!! First operand was " << opLeft << std::endl;
			return;
		}
		boolStack.pop_back();
		std::cout << "EXE: First operand was " << opLeft << ", need to check second operand" << std::endl;
	}
	// Se l'operatore è un OR si valuta il primo operando
	// per la cortocircuitazione
	if (boolOpNode->getOp() == BoolOp::OR)
	{
		std::cout << "EXE: OR Detected!" << std::endl;
		if (opLeft)
		{
			std::cout << "EXE: SC OR!! First operand was " << opLeft << std::endl;
			return;
		}
		boolStack.pop_back();
	}

	// Il risultato è determinato dal secondo operatore,
	// si valuta e basta, così rimane nella pila
	boolOpNode->getRight()->accept(this);
	std::cout << "EXE: Second operand was " << boolStack.back() << std::endl;
	return;
}
/*
void ExecutionVisitor::visitBoolOp(BoolOp* boolOpNode)
{
	//std::cout << "EXE: Begin BoolOp between BoolExpr operands" << std::endl;

	// se l'operazione è un not si usa solo un operatore
	if (boolOpNode->getOp() == BoolOp::NOT)
	{
		//std::cout << "EXE: NOT detected" << std::endl;
		//std::cout << "EXE: Bool stack before calculating NOT argument is " << boolStack.size() << " elements:";
		//for (int x : boolStack)
		//	std::cout << x << " ";
		//std::cout << std::endl;
		// calcolo dell'operatore
		boolOpNode->getLeft()->accept(this);
		bool op = boolStack.back();
		//std::cout << "EXE: Bool stack after calculating NOT argument is " << boolStack.size() << " elements:";
		//for (int x : boolStack)
		//	std::cout << x << " ";
		//std::cout << std::endl;
		boolStack.pop_back();
		boolStack.push_back(!op);
		//std::cout << "EXE: Bool stack after calculating NOT result is " << boolStack.size() << " elements:";
		//for (int x : boolStack)
		//	std::cout << x << " ";
		//std::cout << std::endl;
		return;
	}

	// per le operazioni di and e or
	// calcolo degli operatori
	boolOpNode->getLeft()->accept(this);
	boolOpNode->getRight()->accept(this);

	// lettura degli operatori dalla pila
	bool operatorRight = boolStack.back();
	boolStack.pop_back();
	bool operatorLeft = boolStack.back();
	boolStack.pop_back();

	// calcolo dell'operazione e scrittura sulla pila
	switch (boolOpNode->getOp())
	{
	case BoolOp::AND:
		boolStack.push_back(operatorLeft && operatorRight);
		return;
	case BoolOp::OR:
		boolStack.push_back(operatorLeft || operatorRight);
		return;
	}
}
*/