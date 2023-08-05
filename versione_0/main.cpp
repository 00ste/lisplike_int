#include <iostream>
#include <stdlib.h>
#include <fstream>

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
	

	/*
	 * TOKENIZZAZIONE
	 */
	
}