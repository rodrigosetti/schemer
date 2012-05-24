#include "include.h"

#include <fstream>
#include <sstream>

using namespace std;

Expression *IncludeExpression::parse(list<Token*> &tokens) throw (SchemerException*) {
    IncludeExpression* expression;
    Token *token;

    token = next_token(tokens);
    if (token->type != TOK_SYMBOL) {
        throw new SchemerException("Expected symbol", token->line, token->column);
    }

    expression = new IncludeExpression();
    expression->path = (SymbolToken*)token;

    expectClose(tokens);

    return expression;
}

ostream & IncludeExpression::print(ostream &output) const {

    output << "(INCLUDE " << path << ')';
    return output;
}

Expression* IncludeExpression::evaluate(Environment *env, set<Expression*> &callers) throw (SchemerException*) {

    ifstream file;
    list<Token*> tokens;
    Expression* expression;

    file.open(path->symbolValue.c_str(), ifstream::in);

    file >> tokens;
    expression = Expression::parse(tokens);

    return expression->evaluate( env );
}

void IncludeExpression::deepReach() {
    GarbageCollectable::deepReach();
    path->reach();
}

