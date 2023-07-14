#ifndef TOKEN_H
#define TOKEN_H

#include <string>

/*
 I token della grammatica delle espressioni numeriche sono:
 - Parentesi aperte e chiuse (, )
 - Operatori +, -, *, /
 - Numeri 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
 */

/*
 Un token è solitamente una coppia (ID, parola) dove:
 - ID è un identificativo univoco attribuito al token
 - parola è la stringa corrispondente al token
*/

struct Token
{
    static constexpr int LP = 0;    // (
    static constexpr int RP = 1;    // )
    static constexpr int ADD = 2;   // +
    static constexpr int SUB = 3;   // -
    static constexpr int MUL = 4;   // *
    static constexpr int DIV = 5;   // /
    static constexpr int NUM = 6;   // numero qualsiasi

    static constexpr const char* id2word[]
    {
        "(", ")", "+", "-", "*", "/", "NUM"
    };

    Token(int t, const char* w) : tag{t}, word{w} {};
    Token(int t, std::string w) : tag{t}, word{w} {};

    int tag;
    std::string word;
};

std::ostream& operator<<(std::ostream& os, const Token& t);

#endif


