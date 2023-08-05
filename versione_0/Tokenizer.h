#ifndef	TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

#include "Token.h"

class Tokenizer
{
public:
	// utilizzo costruttori e distruttore di default
	std::vector<Token> operator()(std::string inputText);
private:
	void tokenizeInputText(std::string inputText,
		std::vector<Token>& inputTokens);
};

#endif