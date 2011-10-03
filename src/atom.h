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
        bool boolValue();

        Expression *evaluate(Environment *env) throw (SchemerException*);
};

std::ostream & operator << (std::ostream &output, const Atom *expression);

#endif

