#ifndef __DEFINE_H__
#define __DEFINE_H__

#include "expression.h"

class DefineExpression : public Expression {

    public:

        SymbolToken *name;
        Expression *defined;

        DefineExpression() : Expression(EXP_DEFINE) {}
        ~DefineExpression() { delete defined; }

        Expression *evaluate(Environment *env) throw (SchemerException*);
        std::ostream & print(std::ostream &output) const;

        static Expression *parse(std::list<Token*> &tokens) throw (SchemerException*);
};

#endif

