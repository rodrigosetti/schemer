#include "define.h"
#include "atom.h"
#include <sstream>

using namespace std;

Expression *DefineExpression::parse(list<Token*> &tokens) throw (SchemerException*) {
    Token *token;
    DefineExpression *expression = new DefineExpression();

    token = next_token(tokens);
    if (token->type != TOK_SYMBOL) {
        throw new SchemerException("Expected symbol");
    }

    expression->name = (SymbolToken*)token;
    expression->defined = Expression::parse(tokens);

    expectClose(tokens);

    return expression;
}

ostream & operator << (ostream &output, const DefineExpression *expression) {

    output << "(DEFINE " << expression->name << " " << expression->defined << ')';
    return output;
}

Expression* DefineExpression::evaluate(Environment *env) throw (SchemerException*) {

    env->insert(name->symbolValue, defined->evaluate( env ));

    return new Atom(new NilToken());
}

