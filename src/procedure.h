#ifndef __PROCEDURE_H__
#define __PROCEDURE_H__

#include "expression.h"

class Procedure : public Expression {

    public:

        std::list<SymbolToken*> formalParameters;
        Expression *procedureExpression;
        Environment *environment;

        Procedure() : Expression(EXP_PROCEDURE) {}
        ~Procedure() { delete procedureExpression; }

        std::ostream & print(std::ostream &output) const;
        void reach();
};

#endif

