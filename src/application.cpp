#include "application.h"
#include "procedure.h"
#include "builtin.h"
#include <sstream>

using namespace std;

unsigned int depth = 0;

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

    depth ++;
    if (functor->type == EXP_PROCEDURE) {

        if ( ((Procedure*)functor)->formalParameters.size() != arguments.size() ) {
            throw new SchemerException("Parameters number does not match");
        }

        map<string,pair<Expression*,Environment*> > parametersBindings;

        list<SymbolToken*>::const_iterator i;
        list<Expression*>::const_iterator j;

        for (j = arguments.begin(),
             i = ((Procedure*)functor)->formalParameters.begin();
             j != arguments.end() &&
             i != ((Procedure*)functor)->formalParameters.end();
             i++, j++) {
             parametersBindings.insert(pair<string,pair<Expression*,Environment*> >(
                    (*i)->symbolValue,
                    pair<Expression*,Environment*>(*j,env)));
        }

        Environment *lambdaEnvironment = ((Procedure*)functor)->environment;
        Environment *procedureEnv = new Environment(parametersBindings, lambdaEnvironment);

        ((Procedure*)functor)->tailCall = (callers.find(functor) != callers.end());

        if ( depth >= TAIL_CALL_THREESHOLD && ((Procedure*)functor)->tailCall ) {
            ((Procedure*)functor)->environment = procedureEnv;
            depth --;
            return functor;
        }

        callers.insert( functor );
        while (true) {
                evaluated = ((Procedure*)functor)->procedureExpression
                    ->evaluate(procedureEnv, callers);

                if ( evaluated->type == EXP_PROCEDURE &&
                     ((Procedure*)evaluated)->tailCall &&
                     ((Procedure*)evaluated)->procedureExpression == ((Procedure*)functor)->procedureExpression) {
                    functor = evaluated;
                    procedureEnv = ((Procedure*)evaluated)->environment;
                }
                else {
                    break;
                }

        }
        callers.erase( functor );

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

    depth --;
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

