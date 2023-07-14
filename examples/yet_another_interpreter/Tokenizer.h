#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
#include <fstream>

#include "Token.h"

class Tokenizer {
    // vanno bene costruttori e distruttori di default
public:
    // Il file viene preso per riferimento perché la lettura
    // ne modifico lo stato (avanza il puntatore interno)
    std::vector<Token> operator()(std::ifstream& inputFile);

private:
    // il metodo privato prende il file e il vettore di token
    // per riferimento non costante: il file viene letto e il
    // vettore di token viene riempito
    void tokenizeInputFile(std::ifstream& inputFile,
        std::vector<Token>& inputTokens);
};

#endif
