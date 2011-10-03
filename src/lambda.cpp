#include "lambda.h"
#include "procedure.h"
#include <sstream>

using namespace std;

Expression *LambdaExpression::parse(list<Token*> &tokens) throw (SchemerException*) {
    Token *token;
    LambdaExpression *expression = new LambdaExpression();

    expectOpen(tokens);

    while (true) {
        token = next_token(tokens);
        if (token->type == TOK_CLOSE) {
            break;
        }
        else if (token->type == TOK_SYMBOL) {
            expression->formalParameters.push_back((SymbolToken*)token);
        }
        else {
            throw new SchemerException("Unexpected token");
        }
    }
    expression->lambdaExpression = Expression::parse(tokens);

    expectClose(tokens);

    return expression;
}

ostream & operator << (ostream &output, const LambdaExpression *expression) {

    output << "(LAMBDA " << '(';

    bool separate = false;
    for (list<SymbolToken*>::const_iterator i = expression->formalParameters.begin();
         i != expression->formalParameters.end();
         i++) {
        if (separate) output << ' ';
        output << *i;
        separate = true;
    }
    output << ") " << expression->lambdaExpression << ')';
    return output;
}

Expression* LambdaExpression::evaluate(Environment *env) throw (SchemerException*) {

    Procedure *procedure = new Procedure();

    procedure->formalParameters = formalParameters;
    procedure->procedureExpression = lambdaExpression;
    procedure->environment = env;

    return procedure;
}

