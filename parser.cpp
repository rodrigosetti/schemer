#include "parser.h"
#include <sstream>

using namespace std;

Token *next_token(list<Token*> &tokens) {

    Token *token = tokens.front();
    tokens.pop_front();
    return token;
}

void expectOpen(list<Token*> &tokens) throw (SchemerException) {
    Token *token = next_token(tokens);
    if (token->type != TOK_OPEN) {
        throw SchemerException("Expected (", token->line, token->column);
    }
}

void expectClose(list<Token*> &tokens) throw (SchemerException) {
    Token *token = next_token(tokens);
    if (token->type != TOK_CLOSE) {
        throw SchemerException("Expected )", token->line, token->column);
    }
}

/*****************************************************************************/

BeginExpression::~BeginExpression() {
    for (std::list<Expression*>::iterator i = expressions.begin();
         i != expressions.end();
         i++) {
        delete *i;
    }
}

ApplicationExpression::~ApplicationExpression() {
    for (std::list<Expression*>::iterator i = arguments.begin();
         i != arguments.end();
         i++) {
        delete *i;
    }
}

CondExpression::~CondExpression() {
    for (std::list<Expression*>::iterator i = conditions.begin();
         i != conditions.end();
         i++) {
        delete *i;
    }
    for (std::list<Expression*>::iterator i = implications.begin();
         i != implications.end();
         i++) {
        delete *i;
    }
}

bool Atom::boolValue() {

    switch (token->type) {
        case TOK_FLOAT:
            return ((FloatToken*)token)->floatValue;
        case TOK_INT:
            return ((IntToken*)token)->intValue;
        case TOK_SYMBOL:
            return true;
        default:
            return false;
    }
}

/******************************************************************************
 * PARSING
 *****************************************************************************/

Expression* Expression::parse(list<Token*> &tokens) throw (SchemerException) {

    if ( tokens.empty() )
        throw SchemerException("Unexpected end of input.");

    Token *token;

    // consume the next token
    token = next_token(tokens);

    if (token->type == TOK_OPEN) {

        // consume the next token
        token = next_token(tokens);

        if (token->type == TOK_RESERVED) {
            switch (((ReservedWordToken*)token)->reservedWord) {
                case RES_LAMBDA :
                    return LambdaExpression::parse(tokens);
                case RES_DEFINE :
                    return DefineExpression::parse(tokens);
                case RES_COND :
                    return CondExpression::parse(tokens);
                case RES_QUOTE :
                    return QuoteExpression::parse(tokens);
                case RES_BEGIN :
                    return BeginExpression::parse(tokens);
            }
        }
        else {
            // put back next token, because it will be used by application
            tokens.push_front(token);
            return ApplicationExpression::parse(tokens);
        }
    }
    else {
        return new Atom(token);
    }
}

Expression *ApplicationExpression::parse(list<Token*> &tokens) throw (SchemerException) {
    ApplicationExpression *expression = new ApplicationExpression();

    expression->function = Expression::parse(tokens);

    while (tokens.front()->type != TOK_CLOSE) {
        expression->arguments.push_back( Expression::parse(tokens) );
        if (tokens.empty()) {
            throw SchemerException("Unexpected end of input (expecting ')')");
        }
    }
    next_token(tokens);

    return expression;
}

Expression *LambdaExpression::parse(list<Token*> &tokens) throw (SchemerException) {
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
            throw SchemerException("Unexpected token");
        }
    }
    expression->lambdaExpression = Expression::parse(tokens);

    expectClose(tokens);

    return expression;
}

Expression *DefineExpression::parse(list<Token*> &tokens) throw (SchemerException) {
    Token *token;
    DefineExpression *expression = new DefineExpression();

    token = next_token(tokens);
    if (token->type != TOK_SYMBOL) {
        throw SchemerException("Expected symbol");
    }

    expression->name = (SymbolToken*)token;
    expression->defined = Expression::parse(tokens);

    expectClose(tokens);

    return expression;
}

Expression *CondExpression::parse(list<Token*> &tokens) throw (SchemerException) {
    Token *token;
    CondExpression *expression = new CondExpression();

    while (true) {
        token = tokens.front();

        if (token && token->type == TOK_CLOSE) {
            break;
        } else {
            expectOpen(tokens);

            expression->conditions.push_back(Expression::parse(tokens));
            expression->implications.push_back(Expression::parse(tokens));

            expectClose(tokens);
        }
    }

    expectClose(tokens);

    return expression;
}

Expression *QuoteExpression::parse(list<Token*> &tokens) throw (SchemerException) {
    QuoteExpression *expression = new QuoteExpression();

    expression->quoted = Expression::parse(tokens);
    expectClose(tokens);

    return expression;
}

Expression *BeginExpression::parse(list<Token*> &tokens) throw (SchemerException) {
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
    return expression;
}

/******************************************************************************
 * PRINTING
 *****************************************************************************/

ostream & operator << (ostream &output, const Expression *expression) {
    switch (expression->type) {
        case EXP_ATOM:
            output << (Atom*)expression;
            break;
        case EXP_DEFINE:
            output << (DefineExpression*)expression;
            break;
        case EXP_LAMBDA:
            output << (LambdaExpression*)expression;
            break;
        case EXP_BEGIN:
            output << (BeginExpression*)expression;
            break;
        case EXP_COND:
            output << (CondExpression*)expression;
            break;
        case EXP_QUOTE:
            output << (QuoteExpression*)expression;
            break;
        case EXP_APPLICATION:
            output << (ApplicationExpression*)expression;
            break;
        case EXP_PROCEDURE:
            output << (Procedure*)expression;
            break;
        case EXP_BUILTIN:
            output << (BuiltInProcedure*)expression;
            break;
    }

    return output;
}

ostream & operator << (ostream &output, const Atom *expression) {

    output << expression->token;
    return output;
}

ostream & operator << (ostream &output, const DefineExpression *expression) {

    output << "(DEFINE " << expression->name << " " << expression->defined << ')';
    return output;
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

ostream & operator << (ostream &output, const CondExpression *expression) {

    output << "(COND ";

    bool separate = false;
    list<Expression*>::const_iterator i,j;
    for (i = expression->conditions.begin(),
         j = expression->implications.begin();
         i != expression->conditions.end() &&
         j != expression->implications.end();
         i++, j++) {
        if (separate) output << ' ';
        output << '(' << *i << ' ' << *j << ')';
        separate = true;
    }
    return output;
}

ostream & operator << (ostream &output, const QuoteExpression *expression) {

    output << "(QUOTE " << expression->quoted << ')';
    return output;
}

ostream & operator << (ostream &output, const BeginExpression *expression) {

    output << "(BEGIN ";
    bool separate = false;
    for (list<Expression*>::const_iterator i = expression->expressions.begin();
         i != expression->expressions.end();
         i++) {
        output << *i;
        if (separate) output << ' ';
        separate = true;
    }
    output << ')';
    return output;
}

ostream & operator << (ostream &output, const ApplicationExpression *expression) {

    output << '(' << expression->function << ' ';
    bool separate = false;
    for (list<Expression*>::const_iterator i = expression->arguments.begin();
         i != expression->arguments.end();
         i++) {
        if (separate) output << ' ';
        output << *i;
        separate = true;
    }
    output << ')';
    return output;
}

ostream & operator << (ostream &output, const Procedure *expression) {

    output << "<PROCEDURE>";
    return output;
}

ostream & operator << (ostream &output, const BuiltInProcedure *expression) {

    output << "<BUILTIN>";
    return output;
}

/******************************************************************************
 * EVALUATION
 *****************************************************************************/


Expression* Atom::evaluate(Environment *env) throw (SchemerException) {

    Expression *evaluated;

    switch (token->type) {
        case TOK_FLOAT:
        case TOK_INT:
            return this;
        case TOK_SYMBOL:
            evaluated = env->find(((SymbolToken*)token)->symbolValue);
            if (evaluated != NULL) {
                return evaluated;
            }
            else {
                throw SchemerException("Symbol not defined in scope",
                        token->line, token->column);
            }
        default:
            throw SchemerException("Invalid Atom token",
                        token->line, token->column);
    }
}

Expression* DefineExpression::evaluate(Environment *env) throw (SchemerException) {

    env->insert(name->symbolValue, defined);

    return new Atom(new IntToken(0));
}

Expression* LambdaExpression::evaluate(Environment *env) throw (SchemerException) {

    Procedure *procedure = new Procedure();

    procedure->formalParameters = formalParameters;
    procedure->procedureExpression = lambdaExpression;
    procedure->environment = env;

    return procedure;
}

Expression* CondExpression::evaluate(Environment *env) throw (SchemerException) {

    Expression *cond;
    list<Expression*>::const_iterator i,j;

    for (i = conditions.begin(), j = implications.begin();
         i !=conditions.end() && j !=implications.end();
         i++, j++) {

        cond = (*i)->evaluate(env);
        if (cond->boolValue()) {
            return (*j)->evaluate(env);
        }
    }

    return new Atom(new IntToken(0));
}

Expression* QuoteExpression::evaluate(Environment *env) throw (SchemerException) {
    return quoted;
}

Expression* BeginExpression::evaluate(Environment *env) throw (SchemerException) {

    Expression *lastEvaluated =  new Atom(new IntToken(0));

    for (list<Expression*>::const_iterator i = expressions.begin();
         i != expressions.end();
         i++) {
        lastEvaluated = (*i)->evaluate(env);
    }
    return lastEvaluated;
}

Expression* ApplicationExpression::evaluate(Environment *env) throw (SchemerException) {

    Expression *functor = function->evaluate(env);
    Expression *evaluated;

    if (functor->type == EXP_PROCEDURE) {

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

        Environment *procedureEnv = new Environment(parametersBindings, env);
        evaluated = ((Procedure*)functor)->procedureExpression->evaluate(procedureEnv);
        delete procedureEnv;
    }
    else if (functor->type == EXP_BUILTIN) {
        evaluated = ((BuiltInProcedure*)functor)->apply( arguments );
    }

    return evaluated;
}

