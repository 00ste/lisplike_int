#pragma once
#include <vector>

#include "Expression.h"


// un visitor si occupa di eseguire una specifica operazione per
// ciascun oggetto concreto di una gerarchia.
// in generale un visitor deve definire l'operazione di cui si
// occupa per ogni oggetto della gerarchia.
class Visitor
{
public:
    virtual void visitOperator(Operator* opNode) = 0;
    virtual void visitNumber(Number* numNode) = 0;
};

// un visitor concreto che si occupa di interpretare.
// in questo modo l'operazione di interpreatare, così come una
// qualsiasi altra operazione da aggiungere, viene specificata
// in un solo punto per tutti gli oggetti,
class EvaluationVisitor : public Visitor
{
public:
    EvaluationVisitor() : accumulator{} {}

    void visitOperator(Operator* opNode) override;

    void visitNumber(Number* numNode) override;

    int getValue() const
    {
        return accumulator.back();
    }
private:
    std::vector<int> accumulator;
};

// visitor concreto per la stampa delle espressioni, ogni nuova
// operazione aggiunta corrisponde a creare un nuovo visitor concreto.
// d'altronde, quando si aggiunge un nuovo oggetto concreto alla
// struttura, bisogna definire tanti nuovi metodi quanti sono i visitor
// concreti, che non è conveniente.
// il pattern visitor andrebbe applicato quando la struttura su cui si
// lavora è consolidata.
class PrintVisitor : public Visitor
{
public:
    void visitOperator(Operator* opNode) override
    {

    }

    void visitNumber(Number* numNode) override
    {

    }
};
