#include "quote.h"
#include <sstream>

using namespace std;

Expression *QuoteExpression::parse(list<Token*> &tokens) throw (SchemerException*) {
    QuoteExpression *expression = new QuoteExpression();

    expression->quoted = Expression::parse(tokens);
    expectClose(tokens);

    return expression;
}

ostream & QuoteExpression::print(ostream &output) const {

    output << "(QUOTE " << quoted << ')';
    return output;
}

Expression* QuoteExpression::evaluate(Environment *env) throw (SchemerException*) {
    return quoted;
}

void QuoteExpression::reach() {
    GarbageCollectable::reach();
    quoted->reach();
}

