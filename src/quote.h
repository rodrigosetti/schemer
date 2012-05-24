#ifndef __QUOTE_H__
#define __QUOTE_H__

#include "expression.h"

class QuoteExpression : public Expression {

    public:

        Expression *quoted;

        QuoteExpression() : Expression(EXP_QUOTE) {}

        Expression *evaluate(Environment *env, std::set<Expression*> &callers) throw (SchemerException*);
        std::ostream & print(std::ostream &output) const;
        void deepReach();

        static Expression *parse(std::list<Token*> &tokens) throw (SchemerException*);
};

#endif

