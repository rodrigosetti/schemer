#include "cond.h"
#include "atom.h"
#include <sstream>

using namespace std;

Expression *CondExpression::parse(list<Token*> &tokens) throw (SchemerException*) {
    Token *token;
    CondExpression *expression = new CondExpression();

    while (true) {
        token = tokens.front();

        if (token && token->type == TOK_CLOSE) {
            break;
        } else {
            expectOpen(tokens);

            Expression *condition = Expression::parse(tokens);
            Expression *body = Expression::parse(tokens);

            expression->clausures.push_back(
                    pair<Expression*,Expression*>(condition, body));

            expectClose(tokens);
        }
    }

    expectClose(tokens);

    return expression;
}

ostream & CondExpression::print(ostream &output) const {

    output << "(COND ";

    bool separate = false;
    for (list<pair<Expression*,Expression*> >::const_iterator i = clausures.begin();
         i != clausures.end();
         i++) {
        if (separate) output << ' ';
        output << '(' << i->first << ' ' << i->second << ')';
        separate = true;
    }
    return output;
}

Expression* CondExpression::evaluate(Environment *env) throw (SchemerException*) {

    Expression *cond;

    for (list<pair<Expression*,Expression*> >::const_iterator i = clausures.begin();
         i != clausures.end();
         i++) {

        cond = i->first->evaluate(env);

        if (cond->type != EXP_ATOM || ((Atom*)cond)->token->type != TOK_BOOL ) {
            throw new SchemerException("Cond form conditionals should evaluate to bool");
        }

        if ( ((BoolToken*)((Atom*)cond)->token)->boolValue ) {
            return i->second->evaluate(env);
        }
    }

    return Atom::nilExpression;
}

void CondExpression::deepReach() {
    GarbageCollectable::deepReach();
    for (std::list<pair<Expression*,Expression*> >::iterator i = clausures.begin();
         i != clausures.end();
         i++) {
        i->first->reach();
        i->second->reach();
    }
}

