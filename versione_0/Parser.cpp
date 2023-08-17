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

    // Invio un messaggio di errore diverso in base
    // al token atteso
    if (expectedToken == "LP")
    {
        errorMessage << "Missing left parenthesis at token ";
        errorMessage << failedToken.word;
        throw SyntaxError(errorMessage.str());
    }

    if (expectedToken == "RP")
    {
        errorMessage << "Missing right parenthesis at token ";
        errorMessage << failedToken.word;
        throw SyntaxError(errorMessage.str());
    }

    if (expectedToken == "<opCode>")
    {
        errorMessage << "Unrecognized operator ";
        errorMessage << failedToken.word;
        throw SyntaxError(errorMessage.str());
    }

    if (expectedToken == "VAR")
    {
        errorMessage << "Expected a variable, got ";
        errorMessage << failedToken.word;
        throw SyntaxError(errorMessage.str());
    }
    if (expectedToken == "<Statement>")
    {
        throw SyntaxError("Empty BLOCK statement");
    }

    errorMessage << "Cannot parse expression at token ";
    errorMessage << failedToken.word;
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
    auto end = tokenStream.end();

    // Se non ci sono Token
    if (tokenStream.size() == 0)
        return nullptr;

    // Un intero programma è uno statement block
    return parseStmtBlock(itr, end);
}

/**
 * parseBlock
 * 
 * Effettua il parsing di un Block, i token devono essere:
 * LP BLOCK <Statement> ... <Statement> RP
 */
Block* Parser::parseBlock(std::vector<Token>::const_iterator& itr,
    const std::vector<Token>::const_iterator& end)
{
    // Controllo prima di non aver ricevuto un iteratore
    // fuori range
    if (itr == end)
        throw SyntaxError("Overflow in token stream.");

    //std::cout << "PAR: Inside parseBlock" << std::endl;
    Block* block;

    // controlla LP
    if (itr->tag != Token::LP)
        throwSyntaxError(*itr, "LP");
    itr++;
    if (itr == end)
        throw SyntaxError("Overflow in token stream.");
    
    // controlla BLOCK
    if (itr->tag != Token::BLOCK)
        throwSyntaxError(*itr, "BLOCK");
    itr++;

    //std::cout << "PAR: BLOCK OK" << std::endl;

    block = nm->makeBlock();

    // Il Block non può essere vuoto, controlla che
    // ci sia almeno uno Statement
    if (itr->tag != Token::LP)
        throwSyntaxError(*itr, "<Statement>");
    // Non incremento l'indice perché la parentesi
    // fa parte di Statement, verrà controllata da
    // parseStatement

    // controlla ogni statement che inizia con LP
    do
    {
        //std::cout << "PAR: Parsing new statement inside Block..." << std::endl;
        block->appendStatement(parseStatement(itr, end));
    }
    while (itr->tag == Token::LP);

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
Statement* Parser::parseStatement(std::vector<Token>::const_iterator& itr,
    const std::vector<Token>::const_iterator& end)
{
    //std::cout << "PAR: Inside parseStatement" << std::endl;
    
    // Controllo prima di non aver ricevuto un iteratore
    // fuori range
    if (itr == end)
        throw SyntaxError("Overflow in token stream.");

    // Controllo LP che apre lo statement
    if (itr->tag != Token::LP)
        throwSyntaxError(*itr, "LP");
    itr++;
    if (itr == end)
        throw SyntaxError("Overflow in token stream.");

    //std::cout << "PAR: LP OK" << std::endl;

    // Controllo di che Statement si tratta:
    // IfStmt:    IF <BoolExpr> <StmtBlock> <StmtBlock> RP
    if (itr->tag == Token::IF)
    {
        itr++;
        if (itr == end)
            throw SyntaxError("Overflow in token stream.");

        // Controllo la condizione (BoolExpr), l'iteratore
        // è già sul Token successivo
        BoolExpr* condition = parseBoolExpr(itr, end);

        // Controllo il primo Block, l'iteratore è già
        // sul Token successivo
        Block* blockIf = parseStmtBlock(itr, end);

        // Controllo il secondo Block, l'iteratore è
        // già sul Token successivo
        Block* blockElse = parseStmtBlock(itr, end);

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
        //PrintVisitor pv{};
        itr++;
        if (itr == end)
            throw SyntaxError("Overflow in token stream.");

        //std::cout << "PAR: Started to parse WHILE-Statement condition" << std::endl;

        // Controllo la condizione (BoolExpr),
        // l'iteratore è già sul Token successivo
        BoolExpr* condition = parseBoolExpr(itr, end);

        //std::cout << "PAR: Visiting WHILE-Statement condition:" << std::endl;
        //condition->accept(&pv);

        // Controllo il primo Block, l'iteratore è già
        // sul Token successivo
        Block* block = parseStmtBlock(itr, end);

        // Controllo l'ultima RP che conclude lo
        // statement e porto l'iteratore sul Token
        // successivo
        if (itr->tag != Token::RP)
            throwSyntaxError(*itr, "RP");
        itr++;

        WhileStmt* result = nm->makeWhileStmt(condition, block);
        //std::cout << "PAR: Visiting WHILE-Statement:" << std::endl;
        //result->accept(&pv);
        return result;
    }

    // InputStmt: INPUT VAR RP
    if (itr->tag == Token::INPUT)
    {
        itr++;
        if (itr == end)
            throw SyntaxError("Overflow in token stream.");

        // Controllo VAR e costruisco il nodo
        if (itr->tag != Token::VAR)
            throwSyntaxError(*itr, "VAR");
        // TODO: Variable ha sempre meno senso
        Variable* variable = nm->makeVariable(itr->word);
        //std::cout << "PAR: Created Variable with name: " << variable->getName() << std::endl;
        itr++;
        if (itr == end)
            throw SyntaxError("Overflow in token stream.");

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
        if (itr == end)
            throw SyntaxError("Overflow in token stream.");

        // Controllo VAR e costruisco il nodo relativo
        if (itr->tag != Token::VAR)
            throwSyntaxError(*itr, "VAR");
        // TODO: Variable ha sempre meno senso
        Variable* variable = nm->makeVariable(itr->word);
        //std::cout << "PAR: Created Variable with name: " << variable->getName() << std::endl;
        itr++;
        if (itr == end)
            throw SyntaxError("Overflow in token stream.");

        // Controllo NumExpr, l'iteratore si trova
        // già sul token successivo
        NumExpr* expression = parseNumExpr(itr, end);

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
        if (itr == end)
            throw SyntaxError("Overflow in token stream.");

        // Controllo NumExpr, l'iteratore si trova
        // già sul token successivo
        NumExpr* expression = parseNumExpr(itr, end);

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
Block* Parser::parseStmtBlock(std::vector<Token>::const_iterator& itr,
    const std::vector<Token>::const_iterator& end)
{
    // Prima si aumenta l'iteratore di due per stabilire se
    // lo statement block è un solo Statement o è un Block,
    // in modo da chiamare il metodo di parsing adeguato.
    itr += 1;
    if (itr == end)
        throw SyntaxError("Overflow in token stream.");

    if (itr->tag == Token::BLOCK)
    {
        // L'iteratore si riporta all'inizio per poter usare
        // il metodo di parsing.
        itr -= 1;
        return parseBlock(itr, end);
    }
    // L'iteratore si riporta all'inizio per poter usare
    // il metodo di parsing.
    itr -= 1;

    Block* block = nm->makeBlock();
    block->appendStatement(parseStatement(itr, end));
    //std::cout << "PAR: Block " << block << " now has " << block->getStatements().size() << " Statement(s):" << std::endl;
    //for (Statement* stmt : block->getStatements())
    //    std::cout << "at " << stmt << std::endl;
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
NumExpr* Parser::parseNumExpr(std::vector<Token>::const_iterator& itr,
    const std::vector<Token>::const_iterator& end)
{
    //std::cout << "PAR: Parsing NumExpr, received token " << Token::tagToStr(itr->tag) << std::endl;
    
    // Controllo prima di non aver ricevuto un iteratore
    // fuori range
    if (itr == end)
        throw SyntaxError("Overflow in token stream.");

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
        Variable* variable = nm->makeVariable(itr->word);
        //std::cout << "PAR: Created Variable with name: " << variable->getName() << std::endl;
        itr++;

        return variable;
    }

    // Operator

    // Controllo LP che apre ogni operazione
    if (itr->tag != Token::LP)
        throwSyntaxError(*itr, "LP");
    itr++;
    if (itr == end)
        throw SyntaxError("Overflow in token stream.");

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
    if (itr == end)
        throw SyntaxError("Overflow in token stream.");

    // Controllo NumExpr, l'iteratore si trova
    // già sul token successivo
    NumExpr* opLeft = parseNumExpr(itr, end);
    
    // Controllo NumExpr, l'iteratore si trova
    // già sul token successivo
    NumExpr* opRight = parseNumExpr(itr, end);

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
BoolExpr* Parser::parseBoolExpr(std::vector<Token>::const_iterator& itr,
    const std::vector<Token>::const_iterator& end)
{
    // Controllo prima di non aver ricevuto un iteratore
    // fuori range
    if (itr == end)
        throw SyntaxError("Overflow in token stream.");

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
    if (itr == end)
        throw SyntaxError("Overflow in token stream.");

    //std::cout << "PAR: Parsing an AND or OR BoolOp" << std::endl;
    // BoolOp:  AND <BoolExpr> <BoolExpr> RP
    //          OR <BoolExpr> <BoolExpr> RP
    if ((itr->tag == Token::AND) ||
        (itr->tag == Token::OR))
    {
        //PrintVisitor pv{};
        //std::cout << "PAR: Inside " << this << ", with token " << Token::tagToStr(itr->tag) << std::endl;

        // opCode
        BoolOp::OpCode opCode;
        opCode = BoolOp::tokenToOpCode(*itr);
        itr++;
        if (itr == end)
            throw SyntaxError("Overflow in token stream.");

        //std::cout << "PAR: Operator is " << BoolOp::opCodeToStr(opCode) << std::endl;

        // Primo operando
        BoolExpr* opLeft = parseBoolExpr(itr, end);
        //std::cout << "PAR: Visiting opLeft from parseBoolExpr:" << std::endl;
        //opLeft->accept(&pv);

        // Secondo operando
        BoolExpr* opRight = parseBoolExpr(itr, end);
        //std::cout << "PAR: Visiting opRight from parseBoolExpr:" << std::endl;
        //opRight->accept(&pv);

        //std::cout << "PAR: Created operands " << opLeft << " and " << opRight << std::endl;

        // Controllo RP che chiude ogni operazione
        if (itr->tag != Token::RP)
            throwSyntaxError(*itr, "RP");
        itr++;

        BoolOp* result = nm->makeBoolOp(opCode, opLeft, opRight);
        //std::cout << "PAR: Operands are " << result->getLeft() << " and " << result->getRight() << std::endl;
        //std::cout << "PAR: Visiting BoolOp from parseBoolExpr:" << std::endl;
        //result->accept(&pv);
        return result;
    }

    // BoolOp:  NOT <BoolExpr> RP
    if (itr->tag == Token::NOT)
    {
        itr++;
        if (itr == end)
            throw SyntaxError("Overflow in token stream.");

        // Operando
        BoolExpr* op = parseBoolExpr(itr, end);

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
        if (itr == end)
            throw SyntaxError("Overflow in token stream.");

        // Primo operando
        NumExpr* opLeft = parseNumExpr(itr, end);

        // Secondo operando
        NumExpr* opRight = parseNumExpr(itr, end);

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
