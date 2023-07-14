#include <sstream>
#include <vector>

#include "Tokenizer.h"
#include "Exceptions.h"

void Tokenizer::tokenizeInputFile(std::ifstream& inputFile,
    std::vector<Token>& inputTokens)
{

    char ch;
    ch = inputFile.get();
    // lettura del file carattere per carattere
    while (!inputFile.eof())
    {
        // salto gli spazi bianchi
        if (std::isspace(ch))
            continue;

        // praticamente un switch per tutti i tipi di carattere
        if (ch == '(')
        {
            inputTokens.push_back(Token{Token::LP,
                Token::id2word[Token::LP]});
        }
        else if (ch == ')')
        {
            inputTokens.push_back(Token{Token::RP,
                Token::id2word[Token::RP]});
        }
        else if (ch == '+')
        {
            inputTokens.push_back(Token{Token::ADD,
                Token::id2word[Token::ADD]});
        }
        else if (ch == '-')
        {
            inputTokens.push_back(Token{Token::SUB,
                Token::id2word[Token::SUB]});
        }
        else if (ch == '*')
        {
            inputTokens.push_back(Token{Token::MUL,
                Token::id2word[Token::MUL]});
        }
        else if (ch == '/')
        {
            inputTokens.push_back(Token{Token::DIV,
                Token::id2word[Token::DIV]});
        }
        else if (std::isdigit(ch))
        {
            // costante intera da leggere e accumulare
            std::stringstream tmp{};
            tmp << ch;
            do
            {
                ch = inputFile.get();
                if (std::isdigit(ch))
                    tmp << ch;
            }
            while (std::isdigit(ch));
            inputTokens.push_back(Token{Token::NUM, tmp.str()});
            continue;

        }
        else
        {
            // ci sono dei simboli non riconosciuti
            std::stringstream tmp{};
            tmp << "Errore Lessicale sul simbolo: " << ch;
            throw LexicalError(tmp.str());
        }
        ch = inputFile.get();
    }
}

std::vector<Token> Tokenizer::operator()(std::ifstream& inputFile)
{
    std::vector<Token> inputTokens;
    tokenizeInputFile(inputFile, inputTokens);
    return inputTokens;
}

