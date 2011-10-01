#include "schemer.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char **argv) {
    stringstream stream (stringstream::in | stringstream::out);
    ifstream file;
    string line;
    list<Token*> tokens;
    Expression* expression;
    Environment *globalEnvironment = getGlobalEnvironment();

    /* For each file given as parameter, open and evaluate it */
    for (int c = 1; c < argc; c++) {
        file.open(argv[c], ifstream::in);

        tokens = Token::tokenize(file);
        expression = Expression::parse(tokens);

        expression->evaluate( globalEnvironment );
    }

    /* Eval print loop */
    while (true) {

        stream.clear();

        cout << ">>> ";
        getline( cin, line );
        stream << line;

        stream.flush();

        /* if end of user input: exit */
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

