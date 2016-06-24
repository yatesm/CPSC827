#ifndef TOKEN__H
#define TOKEN__H

#include <string>

using std::string;

class Token {
public:
    //Public Canonical Form
    Token() : tokenID(-1), tokenString(""), tokenIdentifier("") { }

    Token(const Token &t) : tokenID(t.tokenID), tokenString(t.tokenString), tokenIdentifier(t.tokenIdentifier) { }

    Token(const int &tID, const string &tStr, const string &iStr) : tokenID(tID), tokenString(tStr),
                                                                    tokenIdentifier(iStr) { }

    Token &operator=(const Token &t);

    //Public Accessors
    int getTokenID() const { return tokenID; }

    string getTokenString() const { return tokenString; }

    string getTokenIdentifier() const { return tokenIdentifier; }

    //Public Mutators
    void setTokenID(int t) { tokenID = t; }

    void setTokenString(string tStr) { tokenString = tStr; }

    void setTokenIdentifier(string iStr) { tokenString = iStr; }

private:
    //Private Data Members
    int tokenID;
    string tokenString;
    string tokenIdentifier;
};

#endif
