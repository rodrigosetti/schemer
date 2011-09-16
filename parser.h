#ifndef __PARSER_H__
#define __PARSER_H__

#include "tokenizer.h"
#include "environment.h"
#include <list>
#include <ostream>

typedef enum {
    EXP_ATOM,
    EXP_DEFINE,
    EXP_LAMBDA,
    EXP_IF,
    EXP_QUOTE,
    EXP_APPLICATION,
    EXP_PROCEDURE,
    EXP_BUILTIN
}
ExpressionType;

class Expression {

    public:

        ExpressionType type;

        Expression(const ExpressionType type) {
            this->type = type;
        }

        Expression *evaluate(Environment *env) {
            return this;
        }

        static Expression* parse(std::list<Token*> &tokens) throw (SchemerException);
};

class Atom : public Expression {

    public:

        Token *token;

        Atom(Token *token) : Expression(EXP_ATOM) {
            this->token = token;
        }

};

class DefineExpression : public Expression {

    public:

        Token *name;
        Expression *defined;

        static Expression *parse(std::list<Token*> &tokens) throw (SchemerException);
};

class LambdaExpression : public Expression {

    public:

        std::list<SymbolToken*> formalParameters;
        Expression *lambdaExpression;

        static Expression *parse(std::list<Token*> &tokens) throw (SchemerException);
};

class IfExpression : public Expression {

    public:

        Expression *condition;
        Expression *conseq;
        Expression *otherwise;

        static Expression *parse(std::list<Token*> &tokens) throw (SchemerException);
};

class QuoteExpression : public Expression {

    public:

        Expression *quoted;

        static Expression *parse(std::list<Token*> &tokens) throw (SchemerException);
};

class BeginExpression : public Expression {

    public:

        std::list<Expression*> expressions;

        static Expression *parse(std::list<Token*> &tokens) throw (SchemerException);
};

class ApplicationExpression : public Expression {

    public:

        Expression *function;
        std::list<Expression*> arguments;

        static Expression *parse(std::list<Token*> &tokens) throw (SchemerException);
};

class Procedure : public Expression {

    public:

        std::list<SymbolToken*> formalParameters;
        Expression *procedureExpression;
        Environment *environment;

};

class BuiltInProcedure : public Expression {

    public:
        std::string name;

        Expression *apply(const std::list<Expression*> &arguments);
};

std::ostream & operator << (std::ostream &output, const Atom &expression);
std::ostream & operator << (std::ostream &output, const DefineExpression &expression);
std::ostream & operator << (std::ostream &output, const LambdaExpression &expression);
std::ostream & operator << (std::ostream &output, const IfExpression &expression);
std::ostream & operator << (std::ostream &output, const QuoteExpression &expression);
std::ostream & operator << (std::ostream &output, const BeginExpression &expression);
std::ostream & operator << (std::ostream &output, const ApplicationExpression &expression);
std::ostream & operator << (std::ostream &output, const Procedure &expression);
std::ostream & operator << (std::ostream &output, const BuiltInProcedure &expression);

#endif

