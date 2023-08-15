#include <iostream>
#include <fstream>
#include <sstream>

#include "Tokenizer.h"
#include "Exceptions.h"
#include "Block.h"
#include "NodeManager.h"
#include "Parser.h"
#include "ExecutionVisitor.h"
#include "PrintVisitor.h"

/*
 * 
 * FAIL_DivisionByZero				OK
 * FAIL_EmptyBlock					OK
 * FAIL_EmptyProgram				OK
 * FAIL_LexicalError				OK
 * FAIL_MismatchedParenthesis1		OK
 * FAIL_MismatchedParenthesis2		OK ??
 * FAIL_MisplacedBool				OK
 * FAIL_MissingElse					OK ??
 * FAIL_MissingGuard				OK
 * FAIL_MissingOperand				ok *
 * FAIL_MultipleErrors1				OK
 * FAIL_MultipleErrors2				ok *
 * FAIL_UndeclaredVariable			OK
 * FAIL_WrongIdentifier1			ok ???
 * FAIL_WrongIdentifier2			OK
 * PASS_BoolExpr					OK
 * PASS_Factorial					OK
 * PASS_Fibonacci1					OK
 * PASS_Fibonacci2					OK
 * PASS_Fibonacci3					OK
 * PASS_Logarithm1					OK
 * PASS_Logarithm2					OK
 * PASS_Logarithm3					OK
 * PASS_Logarithm4					OK
 * PASS_ManyPrimes					OK
 * PASS_Modulo						OK
 * PASS_PolyRoots1					OK
 * PASS_PolyRoots2					OK
 * PASS_Prime						OK
 * PASS_ShortCircuit				OK
 * PASS_SimpleSequence				OK
 * PASS_Sum							OK
 */

int main(int argc, char* argv[])
{
	/*
	 * LETTURA DEL FILE A PARTIRE DA PARAMETRI DA TERMINALE
	 */
	 // controllo numero parametri
	if (argc < 2)
	{
		std::cerr << "Error: FILENAME not specified" << std::endl;
		std::cerr << "Usage: " << argv[0] << " FILENAME" << std::endl;
		return EXIT_FAILURE;
	}

	// controllo apertura file
	std::ifstream inputFile;
	try
	{
		inputFile.open(argv[1]);
	}
	catch (std::exception& e)
	{
		std::cerr << "Error: could not open file " << argv[1] << std::endl;
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	// lettura dell'intero file in una stringa
	std::stringstream temp{};
	temp << inputFile.rdbuf();
	inputFile.close();

	//std::cout << "File read: " << std::endl << temp.str() << std::endl;

	/*
	 * TOKENIZZAZIONE
	 */
	Tokenizer tokenize;
	std::vector<Token> inputTokens;
	try
	{
		inputTokens = tokenize(temp.str());
		/*
		std::cout << "Tokens: ";
		for (Token token : inputTokens)
			std::cout << Token::tagToStr(token.tag) << " ";
		std::cout << std::endl;
		for (Token token : inputTokens)
			std::cout << token.word << " ";
		std::cout << std::endl;
		*/
	}
	catch (LexicalError e)
	{
		std::cerr << "(ERROR in tokenizer: ";
		std::cerr << e.what() << " )" << std::endl;
		return EXIT_FAILURE;
	}
	catch (std::exception e)
	{
		std::cerr << "(ERROR: ";
		std::cerr << e.what() << " )" << std::endl;
		return EXIT_FAILURE;
	}

	/*
	 * PARSING
	 */
	Block* program;
	NodeManager nm{};
	Parser parse{ &nm };
	try
	{
		//std::cout << std::endl << "Begin parsing..." << std::endl;
		program = parse(inputTokens);
		if (program == nullptr)
			return EXIT_FAILURE;
	}
	catch (SyntaxError e)
	{
		std::cerr << "(ERROR in parser: ";
		std::cerr << e.what() << " )" << std::endl;
		return EXIT_FAILURE;
	}
	catch (std::exception e)
	{
		std::cerr << "(ERROR: ";
		std::cerr << e.what() << " )" << std::endl;
		return EXIT_FAILURE;
	}

	/*
	std::cout << "Parsing OK" << std::endl;

	std::cout << "Generated program Block " << program << " containing " << program->getStatements().size() << " Statement(s):" << std::endl;
	for (Statement* stmt : program->getStatements())
		std::cout << "at " << stmt << std::endl;
	*/

	/*
	 * PRINT (DEBUG)
	 */
	/*
	PrintVisitor pv{};
	try
	{
		std::cout << "Generated program: " << std::endl;
		program->accept(&pv);
	}
	catch (std::exception e)
	{
		std::cerr << "Error while printing generated program: ";
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	*/

	/*
	 * ESECUZIONE
	 */
	//std::cout << "Begin execution..." << std::endl;
	ExecutionVisitor ev{};

	try
	{
		program->accept(&ev);
		//std::cout << "Execution terminated!" << std::endl;
		return EXIT_SUCCESS;
	}
	catch (UndefinedReferenceError e)
	{
		std::cerr << "(ERROR in evaluator: ";
		std::cerr << e.what() << " )" << std::endl;
		return EXIT_FAILURE;
	}
	catch(MathError e)
	{
		std::cerr << "(ERROR in evaluator: ";
		std::cerr << e.what() << " )" << std::endl;
		return EXIT_FAILURE;
	}
	catch (std::exception e)
	{
		std::cerr << "(ERROR: ";
		std::cerr << e.what() << " )" << std::endl;
		return EXIT_FAILURE;
	}
}