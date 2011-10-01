#include "schemer.h"
#include <iostream>

using namespace std;

int main() {
    list<Token*> tokens;
    Expression* expression;
    Environment *globalEnvironment = getGlobalEnvironment();

    try {
        tokens = Token::tokenize(cin);
        expression = Expression::parse(tokens);
        cout << expression->evaluate( globalEnvironment ) << endl;
    }
    catch (SchemerException *e) {
        cout << e << endl;
    }

    delete globalEnvironment;
    return 0;
}

