#include <vector>

/**
 * Manager fornisce un'interfaccia per la creazione di Statement, NumExpr e
 * BoolExpr, e per facilitare la gestione della memoria.
 */
class Manager
{
public:
    // COSTRUTTORE E DISTRUTTORE
    Manager() = default;
    Manager(const Manager& other) = delete;
    ~Manager();

    // OPERATORI
    Manager& operator=(const Manager& other) = delete;

    // FACTORY METHODS -- metodi per la creazione degli oggetti concreti, che
    // derivano dalle classi astratte Statement, NumExpr e BoolExpr
    Statement*  makePrintStatement  (const std::string& variable);
    Statement*  makeSetStatement    (const std::string& variable,       const NumExpr&  value);
    Statement*  makeInputStatement  (const std::string& variable);
    Statement*  makeWhileStatement  (const boolExpr&    condition,      const Block&    block);             // TODO: gestire meglio la suddivisione del programma in blocchi
    Statement*  makeIfStatement     (const boolExpr&    condition,      const Block&    blockTrue,          const Block& blockFalse);
    
    NumExpr*    makeOperator        (const NumExpr&     operatorLeft,   const NumExpr&  operatorRight);
    NumExpr*    makeNumber          (const int          number);
    NumExpr*    makeVariable        (const std::string& name,           const int       value);             // TODO: la variabile contiene numeri interi o espressioni??
    
    BoolExpr*   makeRelOp           (const NumExpr&     operatorLeft,   const NumExpr&  operatorRight);
    BoolExpr*   makeBoolConst       (const boolExpr     value);
    BoolExpr*   makeBoolOp          (const BoolExpr&    operatorLeft,   const BoolExpr& operatorRight);


private:
    std::vector<Statement*> allocatedStatements;
    std::vector<NumExpr*>   allocatedNumExprs;
    std::vector<BoolExpr*>  allocatedBoolExprs;
};