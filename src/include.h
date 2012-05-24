#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include "expression.h"

class IncludeExpression : public Expression {

    public:

        SymbolToken *path;

        IncludeExpression() : Expression(EXP_INCLUDE) {}

        Expression *evaluate(Environment *env, std::set<Expression*> &callers) throw (SchemerException*);
        std::ostream & print(std::ostream &output) const;
        void deepReach();

        static Expression *parse(std::list<Token*> &tokens) throw (SchemerException*);
};

#endif

