#include "parseXML.h"
#include "compilerData.h"
#include "handleChars.h"
#include "compiler.h"

#include <time.h>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

CompilerData *CompilerData::instance = NULL;
ParseXML *ParseXML::instance = NULL;

int main(int argc, char **argv) {
    string tempString;

    if (argc < 2) {
        cout << "\t\tUsage: " << argv[0] << " [ input file ] " << endl;
        exit(0);
    }

    /* Header Info */
    cout << "Name: "
    << "Yates Monteith and Anthony McDougal" << endl;
    cout << "Email "
    << "JYMONTE@Clemson.edu, MCOUDGL@Clemson.edu" << endl;
    /* Timestamp Info */

    time_t rawtime;
    struct tm *theTime;
    time(&rawtime);
    theTime = localtime(&rawtime);
    cout << "Timestamp " << asctime(theTime) << endl;
    /* End Header Info */

    Compiler compiler(argv[1]);
    compiler.compile();
}
