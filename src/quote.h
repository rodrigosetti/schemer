#ifndef __QUOTE_H__
#define __QUOTE_H__

#include "expression.h"

class QuoteExpression : public Expression {

    public:

        Expression *quoted;

        QuoteExpression() : Expression(EXP_QUOTE) {}
        ~QuoteExpression() { delete quoted; }

        Expression *evaluate(Environment *env) throw (SchemerException*);
        std::ostream & print(std::ostream &output) const;

        static Expression *parse(std::list<Token*> &tokens) throw (SchemerException*);
};

#endif

