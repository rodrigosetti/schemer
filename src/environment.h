#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include "gc.h"
#include "exceptions.h"
#include <string>
#include <map>

class Expression;

class Environment : public GarbageCollectable {

    public:

        Environment ();
        Environment (const std::map<std::string,Expression*> bindings, 
                     Environment *parent = NULL);

        Expression *find(const std::string &name);
        void insert(const std::string &name, Expression *expression) throw (SchemerException*);
        void deepReach();

        static Environment *globalEnvironment;

    private:

        Environment *parent;
        std::map<std::string,Expression*> bindings;
};

#endif

