#ifndef __BEGIN_H__
#define __BEGIN_H__

#include "expression.h"


class BeginExpression : public Expression {

    public:

        std::list<Expression*> expressions;

        BeginExpression() : Expression(EXP_BEGIN) {}
        ~BeginExpression();

        Expression *evaluate(Environment *env) throw (SchemerException*);
        std::ostream & print(std::ostream &output) const;

        static Expression *parse(std::list<Token*> &tokens) throw (SchemerException*);
};

#endif

