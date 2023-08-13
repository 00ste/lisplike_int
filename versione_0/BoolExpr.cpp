#include "BoolExpr.h"
#include "Visitor.h"
#include "Token.h"
// Dentro il file *.cpp è possibile includere Visitor

void RelOp::accept(Visitor* v) { (*v).visitRelOp(this); }

void BoolConst::accept(Visitor* v) { (*v).visitBoolConst(this); }
void BoolOp::accept(Visitor* v) { (*v).visitBoolOp(this); }

BoolOp::OpCode BoolOp::tokenToOpCode(const Token& t)
{
	if (t.tag == Token::AND) return AND;
	if (t.tag == Token::NOT) return NOT;
	if (t.tag == Token::OR) return OR;
	return ERR;
}
RelOp::OpCode RelOp::tokenToOpCode(const Token& t)
{
	if (t.tag == Token::LT) return LT;
	if (t.tag == Token::GT) return GT;
	if (t.tag == Token::EQ) return EQ;
	return ERR;
}

std::string BoolOp::opCodeToStr(OpCode o)
{
	if (o == AND) return "AND";
	if (o == NOT) return "NOT";
	if (o == OR) return "OR";
	return "null";
}

std::string RelOp::opCodeToStr(OpCode o)
{
	if (o == LT) return "LT";
	if (o == GT) return "GT";
	if (o == EQ) return "EQ";
	return "null";
}
