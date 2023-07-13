#include <iostream>
#include <string>

#include "Expression.h"
#include "ExpressionManager.h"
#include "Parser.h"
#include "Visitor.h"

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
            EvaluationVisitor* v = new EvaluationVisitor();
            (*expr).accept(v);
            std::cout << "Risultato: " << (*v).getValue() << std::endl;
        }
        else
        {
            std::cout << "Espressione errata" << std::endl;
            manager.clearMemory();
        }
    }
    while (exp != "quit");
}


