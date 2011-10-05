#include "schemer.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int evaluate_file(char *filename) {
    ifstream file;
    Expression* expression;

    file.open(filename, ifstream::in);

    try {
        file >> &expression;
        expression->evaluate( Environment::globalEnvironment );
    }
    catch (SchemerException *e) {
       cerr << "Error in file " << filename << ": " << e << endl;
       cerr << "aborting." << endl;
    }

    return 0;
}

int eval_print_loop() {
    stringstream stream (stringstream::in | stringstream::out);
    string line;
    Expression* expression;

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
                cout << "=> " << expression->evaluate( Environment::globalEnvironment ) << endl;
            }
        }
        catch (SchemerException *e) {
           cerr << "Error: " << e << endl;
        }

        gc_run( Environment::globalEnvironment );
    }
    cout << endl;

    return 0;
}

int main(int argc, char **argv) {
    int code = 0;

    setup_schemer();

    if (argc == 1) {
        code = eval_print_loop();
    }
    else if (argc == 2) {
        code = evaluate_file(argv[1]);
    }
    else if (argc >= 2) {
        cout << "usage: " << endl;
        cout << argv[0] << " [<source.scm>]" << endl;
    }

    gc_delete_all();

    return code;
}

