#ifndef __PROCEDURE_H__
#define __PROCEDURE_H__

#include "expression.h"

class Procedure : public Expression {

    public:

        std::list<SymbolToken*> formalParameters;
        Expression *procedureExpression;
        Environment *environment;
        bool tailCall;

        Procedure() : Expression(EXP_PROCEDURE), tailCall(false) {}

        std::ostream & print(std::ostream &output) const;
        void deepReach();
};

#endif

