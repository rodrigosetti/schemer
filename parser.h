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
    EXP_SPECIAL,
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
        std::string symbolValue;
        std::list<Expression> innerExpressions;
        BuiltInMethod builtInValue;

        Environment *lambdaContext;
        std::list<std::string> lambdaFormalParameters;
        Expression *lambdaExpression;

        Expression(const ExpressionType t = EXP_SYMBOL, Token *token = NULL);

        void print(std::ostream &output);

        static Expression parse(std::list<Token> &tokens) throw (SchemerException);

        /*
        Expression evaluate(Environment *env) throw (SchemerException);
        static Environment *getGlobalEnvironment();

        static Expression buildInAdd( const Expression &a, const Expression &b) throw (SchemerException);
        static Expression buildInSub( const Expression &a, const Expression &b) throw (SchemerException);
        static Expression buildInMul( const Expression &a, const Expression &b) throw (SchemerException);
        static Expression buildInDiv( const Expression &a, const Expression &b) throw (SchemerException);
        static Expression buildInDisplay( const Expression &a ) throw (SchemerException);


    private:

        static Environment global;
        */
};

#endif

