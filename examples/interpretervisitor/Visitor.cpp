#include "Visitor.h"

void EvaluationVisitor::visitNumber(Number* numNode)
{
    accumulator.push_back((*numNode).getValue());
}

void EvaluationVisitor::visitOperator(Operator* opNode)
{
    Expression* left = (*opNode).getLeft();
    (*left).accept(this);
    Expression* right = (*opNode).getRight();
    (*right).accept(this);
    int rval = accumulator.back();  accumulator.pop_back();
    int lval = accumulator.back();  accumulator.pop_back();

    switch ((*opNode).getOp())
    {
        case Operator::PLUS:
            accumulator.push_back(lval+rval);
            return;
        case Operator::MINUS:
            accumulator.push_back(lval-rval);
            return;
        case Operator::TIMES:
            accumulator.push_back(lval*rval);
            return;
        case Operator::DIV:
            accumulator.push_back(lval/rval);
            return;
        default:
            return;
    }
}
