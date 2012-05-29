#include "environment.h"
#include "expression.h"

using namespace std;

Environment *Environment::globalEnvironment = NULL;

Environment::Environment() : GarbageCollectable() {
    parent = NULL;
}

Environment::Environment (const map<string,pair<Expression*,Environment*> > bindings,
                          Environment *parent) : GarbageCollectable() {
    this->bindings = bindings;
    this->parent = parent;
}

pair<Expression*,Environment*> Environment::find(const string &name) {

    map<string,pair<Expression*,Environment*> >::const_iterator i;

    i = bindings.find( name );

    if (i != bindings.end() ) {
        return i->second;
    }
    else if ( parent != NULL ) {
        return parent->find( name );
    }
    return pair<Expression*,Environment*>(NULL, NULL);
}

Expression* Environment::findEvaluated(const string &name) {
    pair<Expression*,Environment*> result = find(name);
    return (result.first != NULL? result.first->evaluate( result.second ) : NULL);
}

void Environment::insert(const string &name, Expression *expression, Environment *environment) throw (SchemerException*) {

    pair<map<string,pair<Expression*,Environment*> >::iterator,bool> result =
        bindings.insert(pair<string,pair<Expression*,Environment*> >(name, pair<Expression*,Environment*>(expression, environment)));

    if (!result.second) {
        throw new SchemerException("Name \"" + name + "\" is already bond in the environment");
    }
}

void Environment::deepReach() {
    GarbageCollectable::deepReach();

    if (parent) {
        parent->reach();
    }
    for (map<string,pair<Expression*,Environment*> >::iterator i = bindings.begin();
         i != bindings.end();
         i++) {
        i->second.first->reach();
        i->second.second->reach();
    }
}

