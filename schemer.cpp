#include "schemer.h";
#include <iostream>

using namespace std;

Expression * plusBuiltIn( const list<Expression*> &arguments) throw (SchemerException) {

    if (arguments.size() < 2) {
        throw new SchemerException("(+) function should receive at least two arguments");
    }

    Token *token;
    bool isFloat = false;
    long double accum = 0;

    for (list<Expression*>::const_iterator i = arguments.begin();
         i != arguments.end();
         i ++) {
        if ((*i)->type == EXP_ATOM) {
            token = ((Atom*)(*i))->token;
            switch (token->type) {
                case TOK_FLOAT:
                    isFloat = true;
                    accum += ((FloatToken*)token)->floatValue;
                    break;
                case TOK_INT:
                    accum += ((IntToken*)token)->intValue;
                    break;
                default:
                    throw new SchemerException("Arguments for (+) should be numeric");
            }
        }
        else {
            throw new SchemerException("Invalid argument in (+) function");
        }
    }

    return (isFloat? new Atom(new FloatToken(accum) ) :
                     new Atom(new IntToken((long int)accum) ) );
}

Expression * timesBuiltIn( const list<Expression*> &arguments) throw (SchemerException) {

    if (arguments.size() < 2) {
        throw new SchemerException("(*) function should receive at least two arguments");
    }

    Token *token;
    bool isFloat = false;
    long double accum = 1;

    for (list<Expression*>::const_iterator i = arguments.begin();
         i != arguments.end();
         i ++) {
        if ((*i)->type == EXP_ATOM) {
            token = ((Atom*)(*i))->token;
            switch (token->type) {
                case TOK_FLOAT:
                    isFloat = true;
                    accum *= ((FloatToken*)token)->floatValue;
                    break;
                case TOK_INT:
                    accum *= ((IntToken*)token)->intValue;
                    break;
                default:
                    throw new SchemerException("Arguments for (*) should be numeric");
            }
        }
        else {
            throw new SchemerException("Invalid argument in (*) function");
        }
    }

    return (isFloat? new Atom(new FloatToken(accum) ) :
                     new Atom(new IntToken((long int)accum) ) );
}

Expression * divideBuiltIn( const list<Expression*> &arguments) throw (SchemerException) {
    if (arguments.size() != 2) {
        throw new SchemerException("(/) function should receive exactly two arguments");
    }

    Token *token;
    long double v, a, b;
    int count;
    list<Expression*>::const_iterator i;

    for (i = arguments.begin(), count = 0;
         i != arguments.end();
         i ++, count++) {
        if ((*i)->type == EXP_ATOM) {
            token = ((Atom*)(*i))->token;
            switch (token->type) {
                case TOK_FLOAT:
                    v = ((FloatToken*)token)->floatValue;
                    break;
                case TOK_INT:
                    v = ((IntToken*)token)->intValue;
                    break;
                default:
                    throw new SchemerException("Arguments for (/) should be numeric");
            }

            if (count == 0) { a = v; }
            else if (count == 1) { b = v; }
        }
        else {
            throw new SchemerException("Invalid argument in (/) function");
        }
    }

    return new Atom(new FloatToken(a / b) );
}

Expression * modulusBuiltIn( const list<Expression*> &arguments) throw (SchemerException) {

    if (arguments.size() != 2) {
        throw new SchemerException("(%) function should receive exactly two arguments");
    }

    Token *token;
    long int v, a, b;
    int count;
    list<Expression*>::const_iterator i;

    for (i = arguments.begin(), count = 0;
         i != arguments.end();
         i ++, count++) {
        if ((*i)->type == EXP_ATOM) {
            token = ((Atom*)(*i))->token;
            switch (token->type) {
                case TOK_INT:
                    v = ((IntToken*)token)->intValue;
                    break;
                default:
                    throw new SchemerException("Arguments for (%) should be integer");
            }

            if (count == 0) { a = v; }
            else if (count == 1) { b = v; }
        }
        else {
            throw new SchemerException("Invalid argument in (%) function");
        }
    }

    return new Atom(new IntToken(a % b) );
}

Expression * lessThanBuiltIn( const list<Expression*> &arguments) throw (SchemerException) {

    if (arguments.size() != 2) {
        throw new SchemerException("(<) function should receive exactly two arguments");
    }

    Token *token;
    long double v, a, b;
    int count;
    list<Expression*>::const_iterator i;

    for (i = arguments.begin(), count = 0;
         i != arguments.end();
         i ++, count++) {
        if ((*i)->type == EXP_ATOM) {
            token = ((Atom*)(*i))->token;
            switch (token->type) {
                case TOK_FLOAT:
                    v = ((FloatToken*)token)->floatValue;
                    break;
                case TOK_INT:
                    v = ((IntToken*)token)->intValue;
                    break;
                default:
                    throw new SchemerException("Arguments for (<) should be numeric");
            }

            if (count == 0) { a = v; }
            else if (count == 1) { b = v; }
        }
        else {
            throw new SchemerException("Invalid argument in (<) function");
        }
    }

    return new Atom( new BoolToken( a < b ) );
}

Expression * greaterThanBuiltIn( const list<Expression*> &arguments) throw (SchemerException) {

    if (arguments.size() != 2) {
        throw new SchemerException("(>) function should receive exactly two arguments");
    }

    Token *token;
    long double v, a, b;
    int count;
    list<Expression*>::const_iterator i;

    for (i = arguments.begin(), count = 0;
         i != arguments.end();
         i ++, count++) {
        if ((*i)->type == EXP_ATOM) {
            token = ((Atom*)(*i))->token;
            switch (token->type) {
                case TOK_FLOAT:
                    v = ((FloatToken*)token)->floatValue;
                    break;
                case TOK_INT:
                    v = ((IntToken*)token)->intValue;
                    break;
                default:
                    throw new SchemerException("Arguments for (>) should be numeric");
            }

            if (count == 0) { a = v; }
            else if (count == 1) { b = v; }
        }
        else {
            throw new SchemerException("Invalid argument in (>) function");
        }
    }

    return new Atom( new BoolToken( a > b ) );
}

Expression * compareBuiltIn( const list<Expression*> &arguments) throw (SchemerException) {

    if (arguments.size() != 2) {
        throw new SchemerException("(=) function should receive exactly two arguments");
    }

    Token *token;
    long double v, a, b;
    int count;
    list<Expression*>::const_iterator i;

    for (i = arguments.begin(), count = 0;
         i != arguments.end();
         i ++, count++) {
        if ((*i)->type == EXP_ATOM) {
            token = ((Atom*)(*i))->token;
            switch (token->type) {
                case TOK_FLOAT:
                    v = ((FloatToken*)token)->floatValue;
                    break;
                case TOK_INT:
                    v = ((IntToken*)token)->intValue;
                    break;
                default:
                    throw new SchemerException("Arguments for (=) should be numeric");
            }

            if (count == 0) { a = v; }
            else if (count == 1) { b = v; }
        }
        else {
            throw new SchemerException("Invalid argument in (=) function");
        }
    }

    return new Atom( new BoolToken( a == b ) );
}

Expression * displayBuiltIn( const list<Expression*> &arguments) throw (SchemerException) {

    if (arguments.size() != 1) {
        throw new SchemerException("display function should receive exactly one argument");
    }

    cout << arguments.front();

    return new Atom( new NilToken());
}

Environment *getGlobalEnvironment() {

    Environment *env = new Environment();

    env->insert("+", new BuiltInProcedure("+", &plusBuiltIn));
    env->insert("*", new BuiltInProcedure("*", &timesBuiltIn));
    env->insert("/", new BuiltInProcedure("/", &divideBuiltIn));
    env->insert("%", new BuiltInProcedure("/", &modulusBuiltIn));
    env->insert("<", new BuiltInProcedure("<", &lessThanBuiltIn));
    env->insert(">", new BuiltInProcedure(">", &greaterThanBuiltIn));
    env->insert("=", new BuiltInProcedure("=", &compareBuiltIn));
    env->insert("display", new BuiltInProcedure("display", &displayBuiltIn));

    return env;
}

