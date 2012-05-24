#include "define.h"
#include "atom.h"
#include <sstream>
#include <iostream>

using namespace std;

Expression *DefineExpression::parse(list<Token*> &tokens) throw (SchemerException*) {
    Token *token;
    DefineExpression *expression = new DefineExpression();

    token = next_token(tokens);
    if (token->type != TOK_SYMBOL) {
        throw new SchemerException("Expected symbol", token->line, token->column);
    }

    expression->name = (SymbolToken*)token;
    expression->defined = Expression::parse(tokens);

    expectClose(tokens);

    return expression;
}

ostream & DefineExpression::print(ostream &output) const {

    output << "(DEFINE " << name << " " << defined << ')';
    return output;
}

Expression* DefineExpression::evaluate(Environment *env, set<Expression*> &callers) throw (SchemerException*) {

    env->insert(name->symbolValue, defined->evaluate( env ));

    return Atom::nilExpression;
}

void DefineExpression::deepReach() {
    GarbageCollectable::deepReach();
    name->reach();
    defined->reach();
}

