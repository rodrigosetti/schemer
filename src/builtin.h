#ifndef __BUILTIN_H__
#define __BUILTIN_H__

#include "expression.h"

class BuiltInProcedure : public Expression {

    public:
        std::string name;
        Expression* (*function)(const std::list<Expression*> &arguments);

        BuiltInProcedure() : Expression(EXP_BUILTIN) {}
        BuiltInProcedure(const std::string name, 
                Expression* (*function)(const std::list<Expression*> &arguments));

        std::ostream & print(std::ostream &output) const;
};

#endif

