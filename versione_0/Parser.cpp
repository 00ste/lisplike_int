#include <vector>
#include <iterator>
#include <string>
#include <sstream>
#include <exception>

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
void Parser::throwSyntaxError(Token failedToken,
    std::string expectedToken)
{
    std::stringstream errorMessage{};
    errorMessage << "error while parsing at token ";
    errorMessage << Token::tagToStr(failedToken.tag);
    errorMessage << ", " << expectedToken << " expected.";
    throw SyntaxError(errorMessage.str());
}

/**
 * operator()
 *
 * Effettua il parsing di un vettore di Token, e restituisce
 * il nodo iniziale della struttura dati contenente il programma.
 *
 * Per effettuare il parsing di Block, Statement, NumExpr e
 * BoolExpr vengono chiamati dei metodi appositi.
 */
Block* Parser::operator()(const std::vector<Token>& tokenStream)
{
    //std::cout << "PAR: Begin parsing" << std::endl;
    auto itr = tokenStream.begin();

    // Un intero programma è uno statement block
    return parseStmtBlock(itr);
}

/**
 * parseBlock
 * 
 * Effettua il parsing di un Block, i token devono essere:
 * LP BLOCK <Statement> ... <Statement> RP
 */
Block* Parser::parseBlock(std::vector<Token>::const_iterator& itr)
{
    //std::cout << "PAR: Inside parseBlock" << std::endl;
    Block* block;

    // controlla LP
    if (itr->tag != Token::LP)
        throwSyntaxError(*itr, "LP");
    itr++;
    
    // controlla BLOCK
    if (itr->tag != Token::BLOCK)
        throwSyntaxError(*itr, "BLOCK");
    itr++;

    //std::cout << "PAR: BLOCK OK" << std::endl;

    block = nm->makeBlock();

    // controlla ogni statement che inizia con LP
    while (itr->tag == Token::LP)
    {
        //std::cout << "PAR: Parsing new statement inside Block..." << std::endl;
        block->appendStatement(parseStatement(itr));
    }

    // controlla l'ultima RP e porta l'interatore
    // sul Token successivo
    if (itr->tag != Token::RP)
        throwSyntaxError(*itr, "RP");
    itr++;

    //std::cout << "PAR: Block ended" << std::endl;
    return block;
}

/**
 * parseStatement
 * 
 * Effettua il parsing di uno Statement (IfStmt, WhileStmt,
 * ecc...). I token per ogni Statement devono essere:
 * - IfStmt:    (LP) IF <BoolExpr> <StmtBlock> <StmtBlock> RP
 * - WhileStmt: (LP) WHILE <BoolExpr> <StmtBlock> RP
 * - InputStmt: (LP) INPUT VAR RP
 * - SetStmt:   (LP) SET VAR <NumExpr> RP
 * - PrintStmt: (LP) PRINT <NumExpr> RP
 */
Statement* Parser::parseStatement(std::vector<Token>::const_iterator& itr)
{
    //std::cout << "PAR: Inside parseStatement" << std::endl;
    // Controllo LP che apre lo statement
    if (itr->tag != Token::LP)
        throwSyntaxError(*itr, "LP");
    itr++;

    //std::cout << "PAR: LP OK" << std::endl;

    // Controllo di che Statement si tratta:
    // IfStmt:    IF <BoolExpr> <StmtBlock> <StmtBlock> RP
    if (itr->tag == Token::IF)
    {
        itr++;

        // Controllo la condizione (BoolExpr), l'iteratore
        // è già sul Token successivo
        BoolExpr* condition = parseBoolExpr(itr);

        // Controllo il primo Block, l'iteratore è già
        // sul Token successivo
        Block* blockIf = parseStmtBlock(itr);

        // Controllo il secondo Block, l'iteratore è
        // già sul Token successivo
        Block* blockElse = parseStmtBlock(itr);

        // Controllo l'ultima RP che conclude lo
        // statement e porto l'iteratore sul Token
        // successivo
        if (itr->tag != Token::RP)
            throwSyntaxError(*itr, "RP");
        itr++;
        
        return nm->makeIfStmt(condition, blockIf,
            blockElse);
    }

    // WhileStmt: WHILE <BoolExpr> <StmtBlock> RP
    if (itr->tag == Token::WHILE)
    {
        itr++;

        // Controllo la condizione (BoolExpr),
        // l'iteratore è già sul Token successivo
        BoolExpr* condition = parseBoolExpr(itr);

        // Controllo il primo Block, l'iteratore è già
        // sul Token successivo
        Block* block = parseStmtBlock(itr);

        // Controllo l'ultima RP che conclude lo
        // statement e porto l'iteratore sul Token
        // successivo
        if (itr->tag != Token::RP)
            throwSyntaxError(*itr, "RP");
        itr++;

        return nm->makeWhileStmt(condition, block);
    }

    // InputStmt: INPUT VAR RP
    if (itr->tag == Token::INPUT)
    {
        itr++;

        // Controllo VAR e costruisco il nodo
        if (itr->tag != Token::VAR)
            throwSyntaxError(*itr, "VAR");
        // TODO: Variable ha sempre meno senso
        Variable* variable = nm->makeVariable(-17,
            itr->word);
        std::cout << "PAR: Created Variable with name: " << variable->getName() << std::endl;
        itr++;

        // Controllo l'ultima RP che conclude lo
        // statement e porto l'iteratore sul token
        // successivo
        if (itr->tag != Token::RP)
            throwSyntaxError(*itr, "RP");
        itr++;

        return nm->makeInputStmt(variable);
    }

    // SetStmt:   SET VAR <NumExpr> RP
    if (itr->tag == Token::SET)
    {
        itr++;

        // Controllo VAR e costruisco il nodo relativo
        if (itr->tag != Token::VAR)
            throwSyntaxError(*itr, "VAR");
        // TODO: Variable ha sempre meno senso
        Variable* variable = nm->makeVariable(-17,
            itr->word);
        std::cout << "PAR: Created Variable with name: " << variable->getName() << std::endl;
        itr++;

        // Controllo NumExpr, l'iteratore si trova
        // già sul token successivo
        NumExpr* expression = parseNumExpr(itr);

        // Controllo l'ultima RP che conclude lo
        // statement e porto l'iteratore sul token
        // successivo
        if (itr->tag != Token::RP)
            throwSyntaxError(*itr, "RP");
        itr++;

        return nm->makeSetStmt(variable, expression);
    }

    // PrintStmt: PRINT <NumExpr> RP
    if (itr->tag == Token::PRINT)
    {
        //std::cout << "PAR: PRINT Statement recognized" << std::endl;
        itr++;

        // Controllo NumExpr, l'iteratore si trova
        // già sul token successivo
        NumExpr* expression = parseNumExpr(itr);

        // Controllo l'ultima RP che conclude lo
        // statement e porto l'iteratore sul token
        // successivo
        if (itr->tag != Token::RP)
            throwSyntaxError(*itr, "RP");
        itr++;

        return nm->makePrintStmt(expression);
    }

    //std::cout << "PAR: Unknown Statement" << std::endl;
    
    // Se non è nessuno degli Statement già controllati,
    // c'è un errore di sintassi
    throwSyntaxError(*itr, "<StmtType>");

    // Per evitare il warning. In realtà questo codice
    // è irraggiungibile perché il metodo sopra lancia
    // un'eccezione, ma il compilatore non lo sa.
    return nullptr;
}

/**
 * parseStmtBlock
 *
 * Effettua il parsing di uno Statement oppure di un Block,
 * utilizza parseStatement e parseB
 */
Block* Parser::parseStmtBlock(std::vector<Token>::const_iterator& itr)
{
    // Prima si aumenta l'iteratore di due per stabilire se
    // lo statement block è un solo Statement o è un Block,
    // in modo da chiamare il metodo di parsing adeguato.
    itr += 1;
    if (itr->tag == Token::BLOCK)
    {
        // L'iteratore si riporta all'inizio per poter usare
        // il metodo di parsing.
        itr -= 1;
        return parseBlock(itr);
    }
    // L'iteratore si riporta all'inizio per poter usare
    // il metodo di parsing.
    itr -= 1;

    Block* block = nm->makeBlock();
    block->appendStatement(parseStatement(itr));
    std::cout << "PAR: Block " << block << " now has " << block->getStatements().size() << " Statement(s):" << std::endl;
    for (Statement* stmt : block->getStatements())
        std::cout << "at " << stmt << std::endl;
    return block;
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
    //std::cout << "PAR: Parsing NumExpr, received token " << Token::tagToStr(itr->tag) << std::endl;
    // Number
    if (itr->tag == Token::NUM)
    {
        //std::cout << "PAR: Parsing Number, received word " << itr->word << std::endl;

        // conversione creativa
        std::stringstream buffer;
        int value;
        buffer << itr->word;
        buffer >> value;

        itr++;
        return nm->makeNumber(value);
    }

    // Variable
    if (itr->tag == Token::VAR)
    {
        Variable* variable = nm->makeVariable(-17, itr->word);
        std::cout << "PAR: Created Variable with name: " << variable->getName() << std::endl;
        itr++;
        return variable;
    }

    // Operator

    // Controllo LP che apre ogni operazione
    if (itr->tag != Token::LP)
        throwSyntaxError(*itr, "LP");
    itr++;

    // Controllo <opCode> e creo l'OpCode
    // corrispondente
    if ((itr->tag != Token::ADD) &&
        (itr->tag != Token::SUB) &&
        (itr->tag != Token::MUL) &&
        (itr->tag != Token::DIV))
        throwSyntaxError(*itr, "<opCode>");
    Operator::OpCode opCode;
    opCode = Operator::tokenToOpCode(*itr);
    itr++;

    // Controllo NumExpr, l'iteratore si trova
    // già sul token successivo
    NumExpr* opLeft = parseNumExpr(itr);
    
    // Controllo NumExpr, l'iteratore si trova
    // già sul token successivo
    NumExpr* opRight = parseNumExpr(itr);

    // Controllo RP che chiude ogni operazione
    if (itr->tag != Token::RP)
        throwSyntaxError(*itr, "RP");
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
    {
        itr++;
        return nm->makeBoolConst(true);
    }
        
    if (itr->tag == Token::FALSE)
    {
        itr++;
        return nm->makeBoolConst(false);
    }

    // BoolOp e RelOp

    // Controllo LP che apre ogni operazione
    if (itr->tag != Token::LP)
        throwSyntaxError(*itr, "LP");
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
            throwSyntaxError(*itr, "RP");
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
            throwSyntaxError(*itr, "RP");
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
            throwSyntaxError(*itr, "RP");
        itr++;

        return nm->makeRelOp(opCode, opLeft, opRight);
    }

    // Se non è nessuno degli OpCode già controllati,
    // c'è un errore di sintassi
    throwSyntaxError(*itr, "<opCode>");

    // Per evitare il warning. In realtà questo codice
    // è irraggiungibile perché il metodo sopra lancia
    // un'eccezione, ma il compilatore non lo sa.
    return nullptr;
}
