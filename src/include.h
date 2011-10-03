#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include "expression.h"

class IncludeExpression : public Expression {

    public:

        SymbolToken *path;

        IncludeExpression() : Expression(EXP_INCLUDE) {}

        Expression *evaluate(Environment *env) throw (SchemerException*);

        static Expression *parse(std::list<Token*> &tokens) throw (SchemerException*);
};

std::ostream & operator << (std::ostream &output, const IncludeExpression *expression);

#endif

