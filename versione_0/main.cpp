#include <iostream>
#include <fstream>
#include <sstream>

#include "Tokenizer.h"
#include "Exceptions.h"
#include "Block.h"
#include "NodeManager.h"
#include "Parser.h"
#include "Visitor.h"

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

	std::cout << "File read: " << std::endl << temp.str() << std::endl;

	/*
	 * TOKENIZZAZIONE
	 */
	Tokenizer tokenize;
	std::vector<Token> inputTokens;
	try
	{
		inputTokens = tokenize(temp.str());
		std::cout << "Tokens: ";
		for (Token token : inputTokens)
			std::cout << Token::tagToStr(token.tag) << " ";
	}
	catch (LexicalError e)
	{
		std::cerr << "Lexical Error:" << std::endl;
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (std::exception e)
	{
		std::cerr << "Generic Error" << std::endl;
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	/*
	 * PARSING
	 */
	Block* program;
	try
	{
		std::cout << std::endl << "Begin parsing..." << std::endl;
		NodeManager nm{};
		Parser parse{ &nm };
		program = parse(inputTokens);
	}
	catch (SyntaxError e)
	{
		std::cerr << "Syntax Error:" << std::endl;
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (std::exception e)
	{
		std::cerr << "Generic Error" << std::endl;
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Parsing OK" << std::endl;

	/*
	 * ESECUZIONE
	 */
	std::cout << "Begin execution..." << std::endl;
	ExecutionVisitor* ev{};

	program->accept(ev);
	std::cout << "Execution terminated!" << std::endl;
	return EXIT_SUCCESS;
}