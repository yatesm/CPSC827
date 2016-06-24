#include "precedenceList.h"
#include <map>


PrecedenceList::fGetValue(const string &s) const {
    map<string, int>::iterator it = fSymbolMap.find(s);
    return (it->second);
}

PrecedenceList::gGetValue(const string &s) const {
    map<string, int>::iterator it = gSymbolMap.find(s);
    return (it->second);
}