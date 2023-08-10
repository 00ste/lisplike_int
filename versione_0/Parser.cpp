#include <vector>
#include <iterator>
#include <string>
#include <sstream>
#include <exception>

#include <iostream>

#include "Parser.h"
#include "Block.h"
#include "Statement.h"
#include "NumExpr.h"
#include "BoolExpr.h"
#include "NodeManager.h"
#include "Token.h"
#include "Exceptions.h"



/**
 * throwSyntaxError
 * 
 * Funzione di utilità per lanciare un errore di sintassi,
 * l'eccezione specifica il token che l'ha scatenata.
 */
void Parser::throwSyntaxError(Token failedToken)
{
    std::cout << "Eccezione! su " << Token::tagToStr(failedToken.tag) << std::endl;
    std::stringstream errorMessage{};
    errorMessage << "error while parsing at token";
    errorMessage << Token::tagToStr(failedToken.tag);
    throw LexicalError(errorMessage.str());
}

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
    std::cout << "PAR: Inizio parsing" << std::endl;
    auto tokenItr = tokenStream.begin();
    Block* program;

    // Nella maggior parte dei casi un programma è composto da
    // un Blocco di più statement, quindi si prova prima questo
    // caso.
    try
    {
        program = parseBlock(tokenItr);
        return program;
    }
    catch (std::exception e)
    {
        std::cout << "PAR: Il programma è un solo Statement" << std::endl;
    }
    // Tuttavia sono ammessi anche programmi composti da un
    // singolo Statement, che non fanno parte di un Block.
    // La funzione restituisce comunque un Block, in
    // memoria un singolo Statement viene comunque
    // memorizzato all'interno di un Block.
    program = nm->makeBlock();
    program->appendStatement(parseStatement(tokenItr));
    return program;
}

/**
 * parseBlock
 * 
 * Effettua il parsing di un Block, i token devono essere:
 * LP BLOCK <Statement> ... <Statement> RP
 */
Block* Parser::parseBlock(std::vector<Token>::const_iterator& itr)
{
    std::cout << "PAR: Dentro parseBlock" << std::endl;
    Block* block = nm->makeBlock();

    // controlla LP
    if (itr->tag != Token::LP)
        throwSyntaxError(*itr);
    itr++;
    
    // controlla BLOCK
    if (itr->tag != Token::BLOCK)
        throwSyntaxError(*itr);
    itr++;

    // controlla ogni statement che inizia con LP
    while (itr->tag == Token::LP)
    {
        block->appendStatement(parseStatement(itr));
    }

    // controlla l'ultima RP e porta l'interatore
    // sul Token successivo
    if (itr->tag != Token::RP)
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

        // Controllo la condizione (BoolExpr), l'iteratore
        // è già sul Token successivo
        BoolExpr* condition = parseBoolExpr(itr);

        // Controllo il primo Block, l'iteratore è già
        // sul Token successivo
        Block* blockIf = parseBlock(itr);

        // Controllo il secondo Block, l'iteratore è
        // già sul Token successivo
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

        // Controllo la condizione (BoolExpr),
        // l'iteratore è già sul Token successivo
        BoolExpr* condition = parseBoolExpr(itr);

        // Controllo il primo Block, l'iteratore è già
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

        // Controllo NumExpr, l'iteratore si trova
        // già sul token successivo
        NumExpr* expression = parseNumExpr(itr);

        // Controllo l'ultima RP che conclude lo
        // statement e porto l'iteratore sul token
        // successivo
        if (itr->tag != Token::RP)
            throwSyntaxError(*itr);
        itr++;

        return nm->makeSetStmt(variable, expression);
    }

    // PrintStmt: PRINT <NumExpr> RP
    if (itr->tag == Token::PRINT)
    {
        itr++;

        // Controllo NumExpr, l'iteratore si trova
        // già sul token successivo
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

    // Per evitare il warning. In realtà questo codice
    // è irraggiungibile perché il metodo sopra lancia
    // un'eccezione, ma il compilatore non lo sa.
    return nullptr;
}

/**
 * parseNumExpr
 * 
 * Effettua il parsing di una NumExpr (Variable, Operator,
 * Number). I token per ogni Statment devono essere:
 * - Number:    NUM
 * - Variable:  VAR
 * - Operator:  LP <opCode> <NumExpr> <NumExpr> RP
 */
NumExpr* Parser::parseNumExpr(std::vector<Token>::const_iterator& itr)
{
    // Number
    if (itr->tag == Token::NUM)
        return nm->makeNumber(std::stoi(itr->word));

    // Variable
    if (itr->tag == Token::VAR)
        return nm->makeVariable(-17, itr->word);

    // Operator

    // Controllo LP che apre ogni operazione
    if (itr->tag != Token::LP)
        throwSyntaxError(*itr);
    itr++;

    // Controllo <opCode> e creo l'OpCode
    // corrispondente
    if ((itr->tag != Token::ADD) &&
        (itr->tag != Token::SUB) &&
        (itr->tag != Token::MUL) &&
        (itr->tag != Token::DIV))
        throwSyntaxError(*itr);
    Operator::OpCode opCode;
    opCode = Operator::TokenToOpCode(*itr);
    itr++;

    // Controllo NumExpr, l'iteratore si trova
    // già sul token successivo
    NumExpr* opLeft = parseNumExpr(itr);
    
    // Controllo NumExpr, l'iteratore si trova
    // già sul token successivo
    NumExpr* opRight = parseNumExpr(itr);

    // Controllo RP che chiude ogni operazione
    if (itr->tag != Token::RP)
        throwSyntaxError(*itr);
    itr++;

    return nm->makeOperator(opCode, opLeft, opRight);
}

/**
 * parseBoolExpr
 *
 * Effettua il parsing di una BoolExpr (BoolConst, BoolOp,
 * RelOp). I token per ogni Statment devono essere:
 * - BoolConst: TRUE / FALSE
 * - BoolOp:    LP <opCode> <BoolExpr> <BoolExpr> RP
 * - RelOp:     LP <opCode> <NumExpr> <NumExpr> RP
 */
BoolExpr* Parser::parseBoolExpr(std::vector<Token>::const_iterator& itr)
{
    // BoolConst
    if (itr->tag == Token::TRUE)
        return nm->makeBoolConst(true);
    if (itr->tag == Token::FALSE)
        return nm->makeBoolConst(false);

    // BoolOp e RelOp

    // Controllo LP che apre ogni operazione
    if (itr->tag != Token::LP)
        throwSyntaxError(*itr);
    itr++;

    // BoolOp:  AND <BoolExpr> <BoolExpr> RP
    //          OR <BoolExpr> <BoolExpr> RP
    if ((itr->tag == Token::AND) ||
        (itr->tag == Token::OR))
    {
        // opCode
        BoolOp::OpCode opCode;
        opCode = BoolOp::tokenToOpCode(*itr);
        itr++;

        // Primo operando
        BoolExpr* opLeft = parseBoolExpr(itr);

        // Secondo operando
        BoolExpr* opRight = parseBoolExpr(itr);

        // Controllo RP che chiude ogni operazione
        if (itr->tag != Token::RP)
            throwSyntaxError(*itr);
        itr++;

        return nm->makeBoolOp(opCode, opLeft, opRight);
    }

    // BoolOp:  NOT <BoolExpr> RP
    if (itr->tag == Token::NOT)
    {
        itr++;

        // Operando
        BoolExpr* op = parseBoolExpr(itr);

        // Controllo RP che chiude ogni operazione
        if (itr->tag != Token::RP)
            throwSyntaxError(*itr);
        itr++;

        return nm->makeBoolOp(BoolOp::NOT, op, nullptr);
    }

    // RelOp:   LT <NumExpr> <NumExpr> RP
    //          GT <NumExpr> <NumExpr> RP
    //          EQ <NumExpr> <NumExpr> RP
    if ((itr->tag == Token::LT) ||
        (itr->tag == Token::GT) ||
        (itr->tag == Token::EQ))
    {
        // opCode
        RelOp::OpCode opCode;
        opCode = RelOp::tokenToOpCode(*itr);
        itr++;

        // Primo operando
        NumExpr* opLeft = parseNumExpr(itr);

        // Secondo operando
        NumExpr* opRight = parseNumExpr(itr);

        // Controllo RP che chiude ogni operazione
        if (itr->tag != Token::RP)
            throwSyntaxError(*itr);
        itr++;

        return nm->makeRelOp(opCode, opLeft, opRight);
    }

    // Se non è nessuno degli OpCode già controllati,
    // c'è un errore di sintassi
    throwSyntaxError(*itr);

    // Per evitare il warning. In realtà questo codice
    // è irraggiungibile perché il metodo sopra lancia
    // un'eccezione, ma il compilatore non lo sa.
    return nullptr;
}
