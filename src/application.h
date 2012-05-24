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

        Procedure *functor;
        Environment *environment;

        TailCallException(Procedure *functor, Environment *environment) {
            this->functor = functor;
            this->environment = environment;
        }
};

#endif

