#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>

#include "Expression.h"
#include "ExpressionManager.h"
#include "Parser.h"
#include "Visitor.h"
#include "Token.h"
#include "Tokenizer.h"
#include "Exceptions.h"

int main(int argc, char* argv[])
{
    // controllo di avere abbastanza parametri
    // argv[0] è il nome dell'eseguibile
    // argv[1] è il primo parametro
    if (argc < 2)
    {
        std::cerr << "File non specificato" << std::endl;
        std::cerr << "Utilizzo: " << argv[0] << " <filename>" << std::endl;
        return EXIT_FAILURE;
    }

    // controllo sui file
    std::ifstream inputFile;

    try
    {
        inputFile.open(argv[1]);
    }
    catch (LexicalError exc)
    {
        std::cerr << "Errore in fase di analisi lessicale" << std::endl;
        std::cerr << "Eccezione dice: " << exc.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::exception& exc)
    {
        std::cerr << "Impossibile aprire il file " << argv[1] << std::endl;
        std::cerr << "Eccezione dice: " << exc.what() << std::endl;
        return EXIT_FAILURE;
    }

    // fase di tokenizzazione
    Tokenizer tokenize;
    std::vector<Token> inputTokens;

    try
    {
        inputTokens = std::move(tokenize(inputFile));
        inputFile.close();
    }
    catch (std::exception& exc)
    {
        std::cerr << "Impossibile leggere il file " << argv[1] << std::endl;
        std::cerr << "Eccezione dice: " << exc.what() << std::endl;
        return EXIT_FAILURE;
    }

    /*
    for (Token t : inputTokens)
    {
        std::cout << t << std::endl;
    }
    */

    // fase di parsing
    ExpressionManager manager;
    ParseExpression parse{manager};

    try
    {
        Expression* expr = parse(inputTokens);
        PrintVisitor* p = new PrintVisito();
        std::cout << "Espressione letta: ";
        (*expr).accept(p);
        std::cout << std::endl;
        EvaluationVisitor* v = new EvaluationVisitor();
        (*expr).accept(v);
        std::cout << "Risultato: " << (*v).getValue() << std::endl;
    }
    catch (ParseError& exc)
    {
        std::cerr << "Errore in parsing" << std::endl;
        std::cerr << "Eccezione dice: " << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::exception& exc)
    {
        std::cerr << "Errore misterioso" << std::endl;
        std::cerr << "Eccezione dice: " << exc.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

