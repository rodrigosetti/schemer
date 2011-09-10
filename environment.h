#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include <string>
#include <map>

class Expression;
class Environment {

    public:

        Environment ();
        Environment (const std::map<std::string,Expression*> &bindings, 
                     Environment *parent = NULL);

        Expression *find(const std::string &name);

    private:

        Environment *parent;
        std::map<std::string,Expression*> bindings;
};

#endif

