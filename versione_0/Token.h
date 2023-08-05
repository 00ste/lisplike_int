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
	
	Tag tag;
	std::string word;
};

#endif