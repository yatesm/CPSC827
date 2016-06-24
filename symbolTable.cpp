#include "symbolTable.h"
#include "compilerData.h"
#include <iostream>
#include <iomanip>

using std::setw;
using std::cout;
using std::endl;

SemanticObject *SymbolTable::getObject(string str) {
    for (unsigned int i = 0; i < table.size(); ++i) {
        if ((table[i])->getIdentifier() == str)
            return table[i];
    }
    return NULL;
}


bool SymbolTable::findObject(string str) {
    for (unsigned int i = 0; i < table.size(); ++i) {
        if (table[i]->getIdentifier() == str)
            return true;
    }
    return false;
}

SemanticObject &SymbolTable::getObjectAt(int i) {
    return *table[i];
}

void SymbolTable::addObject(SemanticObject &s, bool localFlag) {
    if (CompilerData::getInstance()->getFlag(15) == 1) {
        if (localFlag)
            cout << "-------------------LOCAL SYMBOL TABLE-----------------" << endl;
        else
            cout << "-------------------GLOBAL SYMBOL TABLE----------------" << endl;
        s.print();
    }
    table.push_back(&s);
}

void SymbolTable::clearTable() {
    vector<SemanticObject *>::iterator it = table.begin();
    while (it != table.end()) {
        delete *it;
        ++it;
    }
}

void SymbolTable::empty() {
    table.clear();
}

void SymbolTable::print() {
    cout << "| " << setw(11) << "Identifier  | " << setw(8) << "  Type    | " << setw(8) << "  Shape  | " << setw(5) <<
    " Rows | " << setw(5) << " Cols |" << endl;
    cout << "------------------------------------------------------" << endl;
    for (unsigned int i = 0; i < table.size(); ++i) {
        cout << "| " << setw(11) << table[i]->getIdentifier() << " | " << setw(9) << table[i]->getType()
        << " | " << setw(8) << table[i]->getShape() << " | " << setw(5) << table[i]->getSize() << " | " << setw(5) <<
        table[i]->getSize2() << " | " << endl;
    }

}

void SymbolTable::printLocalTable() {
    cout << "| " << setw(11) << "Identifier  | " << setw(8) << "  Type    | " << setw(8) << "  Shape  | " << setw(5) <<
    " Rows | " << setw(5) << " Cols | " << setw(9) << "Call Type |" << endl;
    cout << "------------------------------------------------------------------" << endl;
    for (unsigned int i = 0; i < table.size(); ++i) {
        cout << "| " << setw(11) << table[i]->getIdentifier() << " | " << setw(9) << table[i]->getType()
        << " | " << setw(8) << table[i]->getShape() << " | " << setw(5) << table[i]->getSize() << " | " << setw(5) <<
        table[i]->getSize2() << " | " << setw(9) << table[i]->getCallType() << " |" << endl;
    }
}
