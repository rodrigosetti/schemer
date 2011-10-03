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

        static Expression *parse(std::list<Token*> &tokens) throw (SchemerException*);
};

std::ostream & operator << (std::ostream &output, const DefineExpression *expression);

#endif

