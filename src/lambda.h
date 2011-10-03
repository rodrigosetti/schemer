#ifndef __LAMBDA_H__
#define __LAMBDA_H__

#include "expression.h"

class LambdaExpression : public Expression {

    public:

        std::list<SymbolToken*> formalParameters;
        Expression *lambdaExpression;

        LambdaExpression() : Expression(EXP_LAMBDA) {}
        ~LambdaExpression() { delete lambdaExpression; }

        Expression *evaluate(Environment *env) throw (SchemerException*);

        static Expression *parse(std::list<Token*> &tokens) throw (SchemerException*);
};

std::ostream & operator << (std::ostream &output, const LambdaExpression *expression);

#endif

