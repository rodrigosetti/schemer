#include "schemer.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char **argv) {
    stringstream stream (stringstream::in | stringstream::out);
    ifstream file;
    string line;
    Expression* expression;
    Environment *globalEnvironment = getGlobalEnvironment();

    /* For each file given as parameter, open and evaluate it */
    for (int c = 1; c < argc; c++) {
        try {
            file.open(argv[c], ifstream::in);

            file >> &expression;

            expression->evaluate( globalEnvironment );
        }
        catch (SchemerException *e) {
           cerr << "Error in file " << argv[c] << ": " << e << endl;
           cerr << "aborting." << endl;
           exit(1);
        }
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
            expression = NULL;
            stream >> &expression;

            if (expression != NULL) {
                cout << "=> " << expression->evaluate( globalEnvironment ) << endl;
            }
        }
        catch (SchemerException *e) {
           cerr << "Error: " << e << endl;
        }
    }
    cout << endl;

    delete globalEnvironment;
    return 0;
}

