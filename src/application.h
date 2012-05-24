#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "expression.h"
#include "procedure.h"

class ApplicationExpression : public Expression {

    public:

        Expression *function;
        std::list<Expression*> arguments;

        ApplicationExpression() : Expression(EXP_APPLICATION) {}

        Expression *evaluate(Environment *env, std::set<Expression*> &callers) throw (SchemerException*);
        std::ostream & print(std::ostream &output) const;
        void deepReach();

        static Expression *parse(std::list<Token*> &tokens) throw (SchemerException*);
};

class TailCallException {

    public:

        Procedure *procedure;
        Environment *environment;

        TailCallException(Procedure *procedure, Environment *environment) {
            this->procedure = procedure;
            this->environment = environment;
        }
};

#endif

