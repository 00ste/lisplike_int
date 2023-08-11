#ifndef TOKEN_H
#define TOKEN_H

#include <string>

struct Token
{
	enum Tag { LP, RP, BLOCK, IF, WHILE, INPUT, SET,
		PRINT, ADD, SUB, MUL, DIV, NUM, VAR, GT, LT,
		EQ, AND, OR, NOT, TRUE, FALSE };

	Token(Token::Tag t, const char* w) : tag{ t }, word{ w } {}
	Token(Token::Tag t, std::string w) : tag{ t }, word{ w } {}

	static Tag strToTag(std::string s)
	{
		if (s == "(") return LP;
		if (s == ")") return RP;
		if (s == "BLOCK") return BLOCK;
		if (s == "IF") return IF;
		if (s == "WHILE") return WHILE;
		if (s == "INPUT") return INPUT;
		if (s == "SET") return SET;
		if (s == "PRINT") return PRINT;
		if (s == "ADD") return ADD;
		if (s == "SUB") return SUB;
		if (s == "MUL") return MUL;
		if (s == "DIV") return DIV;
		if (s == "GT") return GT;
		if (s == "LT") return LT;
		if (s == "EQ") return EQ;
		if (s == "AND") return AND;
		if (s == "OR") return OR;
		if (s == "NOT") return NOT;
		if (s == "TRUE") return TRUE;
		if (s == "FALSE") return FALSE;
	}
	
	static std::string tagToStr(Tag s)
	{
		if (s == LP) return "LP";
		if (s == RP) return "RP";
		if (s == BLOCK) return "BLOCK";
		if (s == IF) return "IF";
		if (s == WHILE) return "WHILE";
		if (s == INPUT) return "INPUT";
		if (s == SET) return "SET";
		if (s == PRINT) return "PRINT";
		if (s == ADD) return "ADD";
		if (s == SUB) return "SUB";
		if (s == MUL) return "MUL";
		if (s == DIV) return "DIV";
		if (s == NUM) return "NUM";
		if (s == VAR) return "VAR";
		if (s == GT) return "GT";
		if (s == LT) return "LT";
		if (s == EQ) return "EQ";
		if (s == AND) return "AND";
		if (s == OR) return "OR";
		if (s == NOT) return "NOT";
		if (s == TRUE) return "TRUE";
		if (s == FALSE) return "FALSE";
		return "null";
	};
	
	Tag tag;
	std::string word;
};

#endif