#include <vector>
#include <sstream>
#include <iostream>

#include "Exceptions.h"
#include "Tokenizer.h"
#include "Token.h"

std::vector<Token> Tokenizer::operator()(std::string inputText)
{
	std::cout << "File received: " << std::endl << inputText << std::endl;
	std::vector<Token> inputTokens;
	tokenizeInputText(inputText, inputTokens);
	return inputTokens;
}

int Tokenizer::isKeyword(std::string word)
{
	// Parole chiave (definite in keywords)
	// std::cout << "TOK: checking for keyword \"" << word << "\"" << std::endl;
	for (int index = 0; index < keywords.size(); index++)
	{
		if (word == keywords[index])
		{
			// std::cout << "TOK: keyword found!" << std::endl;
			return index;
		}
	}
	// std::cout << "TOK: not a keyword" << std::endl;
	return -1;
}

void Tokenizer::tokenizeInputText(std::string inputText, std::vector<Token>& inputTokens)
{
	int index = 0;

	// lettura del file carattere per carattere
	while (index < inputText.size())
	{
		// std::cout << "TOK: new loop, index is: " << index << ", character is " << inputText.at(index) << std::endl;
		// Salto gli spazi bianchi
		if (std::isspace(inputText[index]))
		{
			// std::cout << "TOK: found white space" << std::endl;
			index++;
			continue;
		}

		// Parentesi aperta
		if (inputText[index] == '(')
		{
			// std::cout << "TOK: found open bracket" << std::endl;
			index++;
			inputTokens.push_back(Token{ Token::LP, "(" });
			continue;
		}

		// Parentesi chiusa
		if (inputText[index] == ')')
		{
			// std::cout << "TOK: found closed bracket" << std::endl;
			index++;
			inputTokens.push_back(Token{ Token::RP, ")" });
		}

		// Identificatori: Variabili e parole chiave
		if (std::isalpha(inputText[index]))
		{
			//std::cout << "TOK: found the start of a word" << std::endl;
			// Individuo l'intera stringa (finché ci sono lettere)
			std::stringstream temp;
			while (std::isalpha(inputText[index]))
			{
				//std::cout << "TOK: still inside the word (" << inputText[index] << " at " << index << ")" << std::endl;
				temp << inputText[index];
				index++;
				//std::cout << "TOK: temp is: " << temp.str() << std::endl;
			}
			std::string identifier = temp.str();
			//std::cout << "TOK: word ended: " << identifier << "(" << inputText[index] << "was found)" << std::endl;

			// Controllo preliminare: gli identificatori di ogni tipo
			// non possono essere seguiti da un numero
			if (std::isdigit(inputText[index]))
			{
				std::stringstream errorMessage{};
				errorMessage << "unexpected ";
				errorMessage << inputText[index];
				errorMessage << " at " << index;
				errorMessage << " after " << identifier;
				errorMessage << ", missing separator.";
				throw LexicalError(errorMessage.str());
			}

			// Distinzione tra parole chiave e variabili
			int keywordIndex = isKeyword(identifier);
			if (keywordIndex > -1)
			{
				// Parole chiave
				std::string keyword = keywords[keywordIndex];
				inputTokens.push_back(Token{ Token::strToTag(keyword), keyword });
				continue;
			}
			else
			{
				// Variabili
				//std::cout << "TOK: " << identifier << " is a variable name" << std::endl;
				inputTokens.push_back(Token{ Token::VAR, identifier });
				continue;
			}
		}

		// Costanti numeriche
		if (std::isdigit(inputText[index]))
		{
			//std::cout << "TOK: found the start of a number" << std::endl;
			// Individuo l'intero numero (finché ci sono cifre)
			std::stringstream temp;
			while (std::isdigit(inputText[index]))
			{
				//std::cout << "TOK: still inside the number (" << inputText[index] << " at " << index << ")" << std::endl;
				temp << inputText[index];
				index++;
			}
			std::string number = temp.str();
			//std::cout << "TOK: number ended (" << number << ")" << std::endl;

			inputTokens.push_back(Token{ Token::NUM, number });
		}
	}
}


