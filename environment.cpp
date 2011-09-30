#include "environment.h"

using namespace std;

Environment::Environment() {
    parent = NULL;
}

Environment::Environment (const map<string,Expression*> bindings, 
                          Environment *parent) {
    this->bindings = bindings;
    this->parent = parent;
}

Expression *Environment::find(const string &name) {

    map<string,Expression*>::const_iterator i;

    i = bindings.find( name );

    if (i != bindings.end() ) {
        return i->second;
    }
    else if ( parent != NULL ) {
        return parent->find( name );
    }
    return NULL;
}

void Environment::insert(const string &name, Expression *expression) {

    bindings.insert(pair<string,Expression*>(name, expression));
}

