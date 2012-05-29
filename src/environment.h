#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include "gc.h"
#include "exceptions.h"
#include <string>
#include <utility>
#include <map>

class Expression;

class Environment : public GarbageCollectable {

    public:

        Environment ();
        Environment (const std::map<std::string,std::pair<Expression*,Environment*> > bindings,
                     Environment *parent = NULL);

        std::pair<Expression*,Environment*> find(const std::string &name);
        Expression* findEvaluated(const std::string &name);
        void insert(const std::string &name, Expression *expression, Environment *environment, bool replace=false) throw (SchemerException*);
        void deepReach();

        void insert(const std::string &name, Expression *expression) throw (SchemerException*) {
            return insert(name, expression, globalEnvironment);
        }

        static Environment *globalEnvironment;

    private:

        Environment *parent;
        std::map<std::string,std::pair<Expression*,Environment*> > bindings;
};

#endif

