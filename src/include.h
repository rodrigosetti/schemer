#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include "expression.h"

class IncludeExpression : public Expression {

    public:

        SymbolToken *path;

        IncludeExpression() : Expression(EXP_INCLUDE) {}

        Expression *evaluate(Environment *env) throw (SchemerException*);
        std::ostream & print(std::ostream &output) const;

        static Expression *parse(std::list<Token*> &tokens) throw (SchemerException*);
};

#endif

