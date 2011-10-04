#ifndef __COND_H__
#define __COND_H__

#include "expression.h"

class CondExpression : public Expression {

    public:

        std::list<std::pair<Expression*,Expression*> > clausures;

        CondExpression() : Expression(EXP_COND) {}
        ~CondExpression();

        Expression *evaluate(Environment *env) throw (SchemerException*);
        std::ostream & print(std::ostream &output) const;

        static Expression *parse(std::list<Token*> &tokens) throw (SchemerException*);
};

#endif

