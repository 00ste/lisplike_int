#include <vector>

#include "Tokenizer.h"
#include "Token.h"

std::vector<Token> Tokenizer::operator()(std::string inputText)
{
	std::vector<Token> inputTokens;
	tokenizeInputText(inputText, inputTokens);
	return inputTokens;
}

void Tokenizer::tokenizeInputText(std::string inputText, std::vector<Token>& inputTokens)
{
	int index = 0;
	while (index < inputText.size())
	{
		// Salto gli spazi bianchi
		if (std::isspace(inputText[index]))
		{
			index++;
			continue;
		}

		// Parentesi aperta
		if (inputText[index] == '(')
		{
			index++;
			inputTokens.push_back(Token{ Token::LP, "(" });
			continue;
		}

		// Parentesi chiusa
		if (inputText[index] == ')')
		{
			index++;
			inputTokens.push_back(Token{ Token::RP, ")" });
			continue;
		}
	}
}
