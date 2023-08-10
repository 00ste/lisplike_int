#include "NumExpr.h"
#include "Visitor.h"
// Dentro il file *.cpp è possibile includere Visitor

void Operator::accept(Visitor* v) { (*v).visitOperator(this); }
Operator::OpCode Operator::TokenToOpCode(const Token& t)
{
    if (t.tag == Token::ADD) return PLUS;
    if (t.tag == Token::SUB) return MINUS;
    if (t.tag == Token::MUL) return TIMES;
    if (t.tag == Token::DIV) return DIV;
}
void Number::accept(Visitor* v) { v->visitNumber(this); }
// void Variable::accept(Visitor* v) { v->visitVariable(this); }