#ifndef EXCEPTIONS_H
#define EXEPTIONS_H

#include <stdexcept>
#include <string>


/**
 * LexicalError
 * 
 * Errore in fase di analisi lessicale, dovuto alla presenza di
 * simboli non attesi
 * 
 * ES: (WHILE (LT i 10), (ADD i 1))
 *                     ^!!
 * � un errore perch� la virgola non fa parte dei simboli terminali
 * della grammatica
 * 
 * ES: (WHILE (LT i 10) (AD i))
 *                       ^!!
 * � un errore perch� AD non � uno statement definito dal
 * linguaggio
 */
struct LexicalError : std::runtime_error
{
	LexicalError(const char* msg) : std::runtime_error(msg) {}
	LexicalError(std::string msg) : std::runtime_error(msg.c_str()) {}
};

/**
 * SyntaxError
 * 
 * Errore di sintassi, dovuto a un uso inappropriato delle
 * parole chiave del linguaggio
 * 
 * ES: (WHILE (LT i 10) (SUM a 1) (SUB b 1))
 *                                ^!!
 * � un errore di sintassi perch� WHILE ha ricevuto un blocco
 * che non si aspettava
 * 
 * ES: (WHILE (LT INPUT 10))
 *                ^!!
 * � un errore di sintassi perch� input � una parola chiave, mentre
 * WHILE si aspettava un'espressione booleana
 * 
 * ES: (WHILE (ADD a 1) (SUB a 1))
 *             ^!!
 * � un errore di sintassi perch� WHILE si aspettava un'espressione
 * booleana e non una numerica
 */
struct SyntaxError : std::runtime_error
{
	SyntaxError(const char* msg) : std::runtime_error(msg) {}
	SyntaxError(std::string msg) : std::runtime_error(msg.c_str()) {}
};

/**
 * UndefinedReferenceError
 *
 * Errore di riferimento a una variabile non definita.
 *
 * ES: (PRINT a)
 *            ^!!
 * � un errore di sintassi perch� a non � stata definita da SET o INPUT
 */
struct UndefinedReferenceError : std::runtime_error
{
	UndefinedReferenceError(const char* msg) : std::runtime_error(msg) {}
	UndefinedReferenceError(std::string msg) : std::runtime_error(msg.c_str()) {}
};

/**
 * UndefinedReferenceError
 *
 * Errore di riferimento a una variabile non definita.
 *
 * ES: (PRINT a)
 *            ^!!
 * � un errore di sintassi perch� a non � stata definita da SET o INPUT
 */
struct UndefinedReferenceError : std::runtime_error
{
	UndefinedReferenceError(const char* msg) : std::runtime_error(msg) {}
	UndefinedReferenceError(std::string msg) : std::runtime_error(msg.c_str()) {}
};

/*
 * ERRORI DI PYTHON
 * ZeroDivisionError: division by zero
 * NameError: name <name> is not defined
 * TypeError: unsupported operand type(s) for <opCode>: <type1> and <type2>
 * SyntaxError: invalid syntax
 */

#endif