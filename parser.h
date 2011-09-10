#ifndef __PARSER_H__
#define __PARSER_H__

#include "tokenizer.h"
#include "environment.h"
#include <list>
#include <ostream>

typedef enum {
    EXP_INT,
    EXP_FLOAT,
    EXP_SYMBOL,
    EXP_COMPOSITE,
    EXP_LAMBDA,
    EXP_BUILTIN
}
ExpressionType;

typedef enum {
    BI_ADD,
    BI_SUB,
    BI_MUL,
    BI_DIV,
    BI_DISPLAY
}
BuiltInMethod;

class Expression {

    public:

        ExpressionType type;
        Token *token;

        long double floatValue;
        long int intValue;
        std::list<Expression> innerExpressions;
        BuiltInMethod builtInValue;

        Environment *lambdaContext;
        std::list<std::string> lambdaFormalParameters;
        Expression *lambdaExpression;

        Expression(const long double floatValue, Token *token = NULL);
        Expression(const long int intValue, Token *token = NULL);
        Expression(Token *token = NULL);
        Expression(const std::list<Expression> &innerExpressions, Token *token = NULL);
        Expression(const BuiltInMethod builtInValue, Token *token = NULL);

        void print(std::ostream &output);

        static Expression parse(std::list<Token*> &tokens) throw (SchemerException);

        //Expression evaluate(Environment *env) throw (SchemerException);
        static Environment *getGlobalEnvironment();

};

#endif

