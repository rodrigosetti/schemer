#ifndef __COND_H__
#define __COND_H__

#include "expression.h"

class CondExpression : public Expression {

    public:

        std::list<std::pair<Expression*,Expression*> > clausures;

        CondExpression() : Expression(EXP_COND) {}
        ~CondExpression();

        Expression *evaluate(Environment *env) throw (SchemerException*);

        static Expression *parse(std::list<Token*> &tokens) throw (SchemerException*);
};

std::ostream & operator << (std::ostream &output, const CondExpression *expression);

#endif

