#include "application.h"
#include "procedure.h"
#include "builtin.h"
#include <sstream>

using namespace std;

Expression *ApplicationExpression::parse(list<Token*> &tokens) throw (SchemerException*) {
    ApplicationExpression *expression = new ApplicationExpression();

    expression->function = Expression::parse(tokens);

    while (tokens.front()->type != TOK_CLOSE) {
        expression->arguments.push_back( Expression::parse(tokens) );
        if (tokens.empty()) {
            throw new SchemerException("Unexpected end of input (expecting ')')");
        }
    }
    next_token(tokens);

    return expression;
}

ostream & ApplicationExpression::print(ostream &output) const {

    output << '(' << function << ' ';
    bool separate = false;
    for (list<Expression*>::const_iterator i = arguments.begin();
         i != arguments.end();
         i++) {
        if (separate) output << ' ';
        output << *i;
        separate = true;
    }
    output << ')';
    return output;
}

Expression* ApplicationExpression::evaluate(Environment *env, set<Expression*> &callers) throw (SchemerException*) {
    Expression *functor = function->evaluate(env);
    Expression *evaluated;

    if (functor->type == EXP_PROCEDURE) {

        if ( ((Procedure*)functor)->formalParameters.size() != arguments.size() ) {
            throw new SchemerException("Parameters number does not match");
        }

        map<string,Expression*> parametersBindings;

        list<SymbolToken*>::const_iterator i;
        list<Expression*>::const_iterator j;

        for (j = arguments.begin(), 
             i = ((Procedure*)functor)->formalParameters.begin();
             j != arguments.end() && 
             i != ((Procedure*)functor)->formalParameters.end();
             i++, j++) {
             parametersBindings.insert(pair<string,Expression*>(
                    (*i)->symbolValue,
                    (*j)->evaluate(env)));
        }

        Environment *lambdaEnvironment = ((Procedure*)functor)->environment;
        Environment *procedureEnv = new Environment(parametersBindings, lambdaEnvironment);

        while (true) {
            callers.insert( functor );
            try {
                evaluated = ((Procedure*)functor)->procedureExpression
                    ->evaluate(procedureEnv, callers);
                callers.erase( functor );
                break;
            } catch (TailCallException *e) {
                callers.erase( functor );
                if (e->functor != functor) {
                    throw e;
                } else {
                    procedureEnv = e->environment;
                    delete e;
                }
            }
        }

    }
    else if (functor->type == EXP_BUILTIN) {

        list<Expression*> evaluatedArgs;
        for (list<Expression*>::const_iterator i = arguments.begin();
             i != arguments.end();
             i++) {
            evaluatedArgs.push_back( (*i)->evaluate(env) );
        }

        evaluated = ((BuiltInProcedure*)functor)->function( evaluatedArgs );
    }

    return evaluated;
}

void ApplicationExpression::deepReach() {
    GarbageCollectable::deepReach();
    function->reach();
    for (std::list<Expression*>::iterator i = arguments.begin();
         i != arguments.end();
         i++) {
        (*i)->reach();
    }
}

