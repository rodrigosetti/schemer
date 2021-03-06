#ifndef __ATOM_H__
#define __ATOM_H__

#include "expression.h"

class Atom : public Expression {

    public:

        Token *token;

        Atom() : Expression(EXP_ATOM) {}
        Atom(Token *token) : Expression(EXP_ATOM) {
            this->token = token;
        }

        Expression *evaluate(Environment *env, std::set<Expression*> &callers) throw (SchemerException*);
        std::ostream & print(std::ostream &output) const;
        void deepReach();

        static Atom *nilExpression;
};


#endif

