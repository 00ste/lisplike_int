#include <iostream>
#include <string>

#include "Expression.h"
#include "ExpressionManager.h"
#include "Parser.h"

int main()
{
    ExpressionManager manager;
    ParseExpression parse{manager};

    std::string exp;
    do {
        std::cout << "Inserire un'espressione: ";
        std::cin >> exp;
        bool correct{parse(exp)};
        if (correct)
        {
            std::cout << "Espressione corretta" << std::endl;
            Expression* expr = parse.getExpression();
            std::cout << "Risultato: " << (*expr).interpret() << std::endl;
        }
        else
        {
            std::cout << "Espressione errata" << std::endl;
            manager.clearMemory();
        }
    }
    while (exp != "quit");
}



