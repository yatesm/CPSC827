#include "token.h"
#include <string>

Token &Token::operator=(const Token &t) {
    tokenID = t.tokenID;
    tokenString = t.tokenString;
    return *this;
}
