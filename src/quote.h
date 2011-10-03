#ifndef __QUOTE_H__
#define __QUOTE_H__

#include "expression.h"

class QuoteExpression : public Expression {

    public:

        Expression *quoted;

        QuoteExpression() : Expression(EXP_QUOTE) {}
        ~QuoteExpression() { delete quoted; }

        Expression *evaluate(Environment *env) throw (SchemerException*);

        static Expression *parse(std::list<Token*> &tokens) throw (SchemerException*);
};

std::ostream & operator << (std::ostream &output, const QuoteExpression *expression);

#endif

