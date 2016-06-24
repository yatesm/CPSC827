#ifndef SEMANTICSANALYZER__H
#define SEMANTICSANALYZER__H

#include<stack>
#include <sstream>
#include "compilerData.h"
#include "symbolTable.h"
#include "token.h"

using std::ostringstream;

class SemanticsAnalyzer {
public:
    SemanticsAnalyzer(std::stack <string> &s);

    void goSemanticsGo(int ProductionNumber);

    bool checkDoubleDecl(string varString);

    void clearLocalSymbolTable();

    void setLocalFlag(const bool b) { localFlag = b; }

    bool getLocalFlag() { return localFlag; }

    void printLocalSymbolTable();

    void printGlobalSymbolTable();

    bool checkGlobalDecl(string varString);

private:
    int intermediateInteger;
    int intermediateReal;
    int intermediateBool;
    int intermediateLabel;
    CompilerData::CompilerData *cData;
    std::stack <string> *labelStack;
    std::stack <string> *semanticStack;
    SymbolTable localTable;
    SymbolTable globalTable;
    bool localFlag;

    SemanticsAnalyzer(const SemanticsAnalyzer &s);

    SemanticsAnalyzer &operator=(SemanticsAnalyzer &s);

    string getIntermediateVariableLabel();

    string getIntermediateVariableBool();

    string getIntermediateVariableInteger();

    string getIntermediateVariableReal();

    string determineType(string &s);

    string ivGetNumber(int number);

    void checkIndex(string indexString);

    string checkExistsAndShape(string varString, string shapeString);
};

#endif
