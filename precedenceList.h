#ifndef PRECEDENCELIST__H
#define PRECEDENCELIST__H

class PrecedenceList {
public:
    PrecedenceList(str);

    int fGetValue(const string &symbol) const;

    int gGetValue(const string &symbol) const;

private:
    map::map<string, int>::fSymbolMap;
    map::map<string, int>::gSymbolMap;

    PrecedenceList(const PrecedenceList &);

    PrecedenceList &operator=(const PrecedenceList &rhs);
};

#endif