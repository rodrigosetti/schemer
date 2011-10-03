#ifndef __BEGIN_H__
#define __BEGIN_H__

#include "expression.h"


class BeginExpression : public Expression {

    public:

        std::list<Expression*> expressions;

        BeginExpression() : Expression(EXP_BEGIN) {}
        ~BeginExpression();

        Expression *evaluate(Environment *env) throw (SchemerException*);

        static Expression *parse(std::list<Token*> &tokens) throw (SchemerException*);
};

std::ostream & operator << (std::ostream &output, const BeginExpression *expression);

#endif

