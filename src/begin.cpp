#include "begin.h"
#include "atom.h"

#include <sstream>

using namespace std;

BeginExpression::~BeginExpression() {
    for (std::list<Expression*>::iterator i = expressions.begin();
         i != expressions.end();
         i++) {
        delete *i;
    }
}

Expression *BeginExpression::parse(list<Token*> &tokens) throw (SchemerException*) {
    Token *token;
    BeginExpression *expression = new BeginExpression();

    while (true) {
        token = tokens.front();

        if (token && token->type == TOK_CLOSE) {
            break;
        } else {
            expression->expressions.push_back(Expression::parse(tokens));
        }
    }
    expectClose(tokens);

    return expression;
}

ostream & BeginExpression::print(ostream &output) const {

    output << "(BEGIN ";
    bool separate = false;
    for (list<Expression*>::const_iterator i = expressions.begin();
         i != expressions.end();
         i++) {
        if (separate) output << ' ';
        output << *i;
        separate = true;
    }
    output << ')';
    return output;
}

Expression* BeginExpression::evaluate(Environment *env) throw (SchemerException*) {
    Expression *lastEvaluated =  new Atom(new NilToken());

    for (list<Expression*>::const_iterator i = expressions.begin();
         i != expressions.end();
         i++) {
        lastEvaluated = (*i)->evaluate(env);
    }
    return lastEvaluated;
}

