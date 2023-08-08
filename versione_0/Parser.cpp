#include <vector>
#include <iterator>
#include <string>
#include <sstream>

#include "Parser.h"
#include "Block.h"
#include "Token.h"
#include "Exceptions.h"


/**
 * operator()
 * 
 * Effettua il parsing di un vettore di Token, e restituisce
 * il nodo iniziale della struttura dati contenente il programma.
 * 
 * Per effettuare il parsing di Block, Statement, NumExpr e
 * BoolExpr vengono chiamati dei metodi appositi.
 * Ogni metodo che effettua il parsing (incluso operator()) legge
 * anche la parentesi relativa al prossimo Token.
 */
Block* Parser::operator()(const std::vector<Token>& tokenStream)
{
    auto tokenItr = tokenStream.begin();
    
    if ((*tokenItr++).tag == Token::LP)
    {
        Block* program = parseBlock(tokenItr);
        return program;
    }
    else
        throwSyntaxError(*tokenItr);
}

void Parser::throwSyntaxError(Token failedToken)
{
    std::stringstream errorMessage{};
    errorMessage << "error while parsing at token";
    errorMessage << Token::tagToStr(failedToken.tag);
    throw LexicalError(errorMessage.str());
}

/**
 * parseBlock
 * 
 * Effettua il parsing di un Block, i token devono essere:
 * LP BLOCK <Statement> ... <Statement> RP
 */
Block* Parser::parseBlock(std::vector<Token>::const_iterator& itr)
{
    Block* block = (*nm).makeBlock();
    
    // controlla BLOCK
    if ((*itr).tag != Token::BLOCK)
        throwSyntaxError(*itr);
    itr++;

    // controlla ogni statement che inizia con LP
    while ((*itr).tag == Token::LP)
    {
        (*block).appendStatement(parseStatement(itr));
    }

    // controlla l'ultima RP
    if ((*itr).tag != Token::RP)
        throwSyntaxError(*itr);
    itr++;

    return block;
}

/**
 * parseStatement
 * 
 * Effettua il parsing di uno Statement (IfStmt, WhileStmt,
 * ecc...). I token per ogni Statement devono essere:
 * - IfStmt:    (LP) IF <BoolExpr> <Block> <Block> RP
 * - WhileStmt: (LP) WHILE <BoolExpr> <Block> RP
 * - InputStmt: (LP) INPUT VAR RP
 * - SetStmt:   (LP) SET VAR <NumExpr> RP
 * - PrintStmt: (LP) PRINT <NumExpr> RP
 */
Statement* Parser::parseStatement(std::vector<Token>::const_iterator& itr)
{
    // Controllo LP che apre lo statement
    if (itr->tag != Token::LP)
        throwSyntaxError(*itr);

    // Controllo di che Statement si tratta:
    // IfStmt:    IF <BoolExpr> <Block> <Block> RP
    if (itr->tag == Token::IF)
    {
        itr++;

        // Controllo la condizione (BoolExpr), comprese
        // LP che la precede e RP che la segue, l'iteratore
        // è già sul Token successivo
        BoolExpr* condition = parseBoolExpr(itr);

        // Controllo il primo Block, comprese LP che la
        // precede e RP che la segue, l'iteratore è già
        // sul Token successivo
        Block* blockIf = parseBlock(itr);

        // Controllo il secondo Block e la RP che lo
        // segue, l'iteratore è già sul Token successivo
        Block* blockElse = parseBlock(itr);

        // Controllo l'ultima RP che conclude lo
        // statement e porto l'iteratore sul Token
        // successivo
        if (itr->tag != Token::RP)
            throwSyntaxError(*itr);
        itr++;
        
        return nm->makeIfStmt(condition, blockIf,
            blockElse);
    }

    // WhileStmt: WHILE <BoolExpr> <Block> RP
    if (itr->tag == Token::WHILE)
    {
        itr++;

        // Controllo la condizione (BoolExpr), comprese
        // LP che la precede e RP che la segue,
        // l'iteratore è già sul Token successivo
        BoolExpr* condition = parseBoolExpr(itr);

        // Controllo il primo Block, comprese LP che la
        // precede e RP che la segue, l'iteratore è già
        // sul Token successivo
        Block* block = parseBlock(itr);

        // Controllo l'ultima RP che conclude lo
        // statement e porto l'iteratore sul Token
        // successivo
        if (itr->tag != Token::RP)
            throwSyntaxError(*itr);
        itr++;

        return nm->makeWhileStmt(condition, block);
    }

    // InputStmt: INPUT VAR RP
    if (itr->tag == Token::INPUT)
    {
        itr++;

        // Controllo VAR e costruisco il nodo
        if (itr->tag != Token::VAR)
            throwSyntaxError(*itr);
        itr++;
        // TODO: Variable ha sempre meno senso
        Variable* variable = nm->makeVariable(-17,
            itr->word);

        // Controllo l'ultima RP che conclude lo
        // statement e porto l'iteratore sul token
        // successivo
        if (itr->tag != Token::RP)
            throwSyntaxError(*itr);
        itr++;

        return nm->makeInputStmt(variable);
    }

    // SetStmt:   SET VAR <NumExpr> RP
    if (itr->tag == Token::SET)
    {
        itr++;

        // Controllo VAR e costruisco il nodo relativo
        if (itr->tag != Token::VAR)
            throwSyntaxError(*itr);
        itr++;
        // TODO: Variable ha sempre meno senso
        Variable* variable = nm->makeVariable(-17,
            itr->word);

        // Controllo RP che segue VAR
        if (itr->tag != Token::RP)
            throwSyntaxError(*itr);
        itr++;

        // Controllo LP che segue NumExpr
        if (itr->tag != Token::LP)
            throwSyntaxError(*itr);
        itr++;

        // Controllo NumExpr e la RP che la segue,
        // l'iteratore si trova già sul token
        // successivo
        NumExpr* expression = parseNumExpr(itr);

        // Controllo l'ultima RP che conclude lo
        // statement e porto l'iteratore sul token
        // successivo
        if (itr->tag != Token::RP)
            throwSyntaxError(*itr);
        itr++;

        return nm->makeSetStmt(variable, expression);
    }

    // PrintStmt: (PRINT) LP <NumExpr> RP RP
    if (itr->tag == Token::PRINT)
    {
        itr++;

        // Controllo LP che precede NumExpr
        if (itr->tag != Token::LP)
            throwSyntaxError(*itr);
        itr++;

        // Controllo NumExpr e la RP che la segue,
        // l'iteratore si trova già sul token
        // successivo
        NumExpr* expression = parseNumExpr(itr);

        // Controllo l'ultima RP che conclude lo
        // statement e porto l'iteratore sul token
        // successivo
        if (itr->tag != Token::RP)
            throwSyntaxError(*itr);
        itr++;

        return nm->makePrintStmt(expression);
    }
    
    // Se non è nessuno degli Statement già controllati,
    // c'è un errore di sintassi
    throwSyntaxError(*itr);
}

/**
 * parseNumExpr
 * 
 * Effettua il parsing di una NumExpr (Variable, Operator,
 * Number). I token per ogni Statment devono essere:
 * - Variable:  (LP) VAR RP
 * - Number:    (LP) NUM RP
 * - Operator:  (LP) <opCode> <NumExpr> RP LP <NumExpr> RP
 */
void Parser::parseNumExpr(std::vector<Token>::const_iterator& itr)
{
    if (itr->tag == Token::VAR)
    {

    }
}

void Parser::parseBoolExpr(std::vector<Token>::const_iterator& i)
{
}
