#ifndef SYMBOLTABLE__H
#define SYMBOLTABLE__H

#include <vector>
#include "semanticObject.h"

using std::vector;

class SymbolTable {
public:
    SymbolTable() : table() { }

    bool findObject(string str);

    SemanticObject &getObjectAt(int i);

    SemanticObject *getObject(string s);

    void addObject(SemanticObject &s, bool localFlag);

    void clearTable();

    void print();

    bool isEmpty() { return table.empty(); }

    void empty();

    void printLocalTable();

private:
    vector<SemanticObject *> table;
};

#endif
