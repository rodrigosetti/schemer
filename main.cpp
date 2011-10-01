#include "schemer.h"
#include <iostream>
#include <sstream>

using namespace std;

int main() {
    stringstream stream (stringstream::in | stringstream::out);
    string line;
    list<Token*> tokens;
    Expression* expression;
    Environment *globalEnvironment = getGlobalEnvironment();

    while (true) {

        stream.clear();

        cout << ">>> ";
        getline( cin, line );
        stream << line;

        stream.flush();

        if (cin.eof()) {
            break;
        }

        try {
            tokens = Token::tokenize(stream);
            expression = Expression::parse(tokens);

            cout << "=> " << expression->evaluate( globalEnvironment ) << endl;
        }
        catch (SchemerException *e) {
            cout << e << endl;
        }
    }
    cout << endl;

    delete globalEnvironment;
    return 0;
}

