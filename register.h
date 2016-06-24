#ifndef REGISTER__H
#define REGISTER__H

#include <string>

using std::string;

class Register {
public:
    Register() : value(""), ivUsed(false) { }

    string getContentType();

    string getValue() { return value; }

    void setValue(string &s) { value = s; }

    bool getIVUsed() { return ivUsed; }

    void setIVUsed(bool b) { ivUsed = b; }

    void toggleIVUsed() { ivUsed = !ivUsed; }

    void evict() {
        value = "";
        ivUsed = false;
    }

private:
    Register(const Register &r);

    Register &operator=(const Register &r);

    string value;
    bool ivUsed;
};

#endif
