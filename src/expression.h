#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include "token.h"
#include "environment.h"
#include <set>
#include <list>
#include <istream>
#include <ostream>

typedef enum {
    EXP_ATOM,
    EXP_DEFINE,
    EXP_LAMBDA,
    EXP_BEGIN,
    EXP_IF,
    EXP_COND,
    EXP_QUOTE,
    EXP_INCLUDE,
    EXP_APPLICATION,
    EXP_PROCEDURE,
    EXP_BUILTIN
}
ExpressionType;

class Expression : public GarbageCollectable {

    public:

        ExpressionType type;

        Expression(const ExpressionType type);

        Expression *evaluate(Environment *env) throw (SchemerException*) {
            std::set<Expression*> empty;
            return evaluate( env, empty );
        }

        virtual Expression *evaluate(Environment *env, std::set<Expression*> &callers) throw (SchemerException*) {
            return this;
        }

        virtual std::ostream & print(std::ostream &output) const = 0;

        static Expression* parse(std::list<Token*> &tokens) throw (SchemerException*);
};

std::istream & operator >> (std::istream &input, Expression **expression) throw (SchemerException*);
std::ostream & operator << (std::ostream &output, const Expression *expression);

Token *next_token(std::list<Token*> &tokens);
void expectOpen(std::list<Token*> &tokens) throw (SchemerException*);
void expectClose(std::list<Token*> &tokens) throw (SchemerException*);

#endif

