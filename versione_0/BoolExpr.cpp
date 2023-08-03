#include "BoolExpr.h"
#include "Visitor.h"
// Dentro il file *.cpp è possibile includere Visitor

void RelOp::accept(Visitor* v) { (*v).visitRelOp(this); }
void BoolConst::accept(Visitor* v) { (*v).visitBoolConst(this); }
void BoolOp::accept(Visitor* v) { (*v).visitBoolOp(this); }