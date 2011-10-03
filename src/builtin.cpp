#include "builtin.h"
#include <sstream>

using namespace std;

BuiltInProcedure::BuiltInProcedure(const string name, 
                Expression* (*function)(const list<Expression*> &arguments)) :
        Expression(EXP_BUILTIN) {
    this->name = name;
    this->function = function;
}

ostream & operator << (ostream &output, const BuiltInProcedure *expression) {

    output << "<BUILTIN>";
    return output;
}

