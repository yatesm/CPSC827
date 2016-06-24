#include "parser.h"
#include "token.h"
#include "semanticsAnalyzer.h"

#include <iostream>
#include <iomanip>

using std::setw;
using std::cout;
using std::endl;
using std::stack;

Parser::Parser(string fname) : productionMap(), fSymbolMap(), gSymbolMap(),
                               syntaxStack(), semanticsStack(), lex(fname),
                               semantics(semanticsStack),
                               cData(CompilerData::getInstance()) {
    syntaxStack.push(new Token(100, "###", "###"));
    semanticsStack.push(string("###"));
    productionMap = cData->getProductionMap();
    fSymbolMap = cData->getFSymbolMap();
    gSymbolMap = cData->getGSymbolMap();
}

void Parser::beginParse() {
    bool reductionFound = false;
    string handle = "", tempString = "";
    Token *stackToken, *inputToken, *saveToken, *handleToken;
    multimap<string, int>::iterator fPrecIter, gPrecIter;
    multimap<string, string>::iterator prodIter;
    size_t veryTemp;
    /* get first token to put onto stack */

    do {
        stackToken = lex.getToken();
    } while (stackToken == NULL);
    syntaxStack.push(stackToken);
    veryTemp = stackToken->getTokenString().find("TOKEN_ID");
    if (veryTemp != string::npos)
        semanticsStack.push(stackToken->getTokenIdentifier());
    else
        semanticsStack.push(stackToken->getTokenString());

    while (true) {
        /* Get a new token */
        if (reductionFound) {
            inputToken = saveToken;
            reductionFound = false;
        }
        else {
            inputToken = lex.getToken();
            if (inputToken == NULL)
                inputToken = new Token(0, "###", "###");
        }

        fPrecIter = fSymbolMap.find(syntaxStack.top()->getTokenIdentifier());
        gPrecIter = gSymbolMap.find(inputToken->getTokenIdentifier());
        if (fPrecIter->second < gPrecIter->second) {
            if (cData->getFlag(9) == 1)
                cout << "\t\t" << setw(11) << syntaxStack.top()->getTokenIdentifier() << "\t<\t" << setw(11) <<
                inputToken->getTokenIdentifier() << endl;
            syntaxStack.push(inputToken);
            veryTemp = inputToken->getTokenString().find("TOKEN_ID");
            if (veryTemp != string::npos)
                semanticsStack.push(inputToken->getTokenIdentifier());
            else
                semanticsStack.push(inputToken->getTokenString());

        }
        else if (fPrecIter->second == gPrecIter->second) {
            if (cData->getFlag(9) == 1)
                cout << "\t\t" << setw(11) << syntaxStack.top()->getTokenIdentifier() << "\t=\t" <<
                inputToken->getTokenIdentifier() << endl;
            syntaxStack.push(inputToken);
            veryTemp = inputToken->getTokenString().find("TOKEN_ID");
            if (veryTemp != string::npos)
                semanticsStack.push(inputToken->getTokenIdentifier());
            else
                semanticsStack.push(inputToken->getTokenString());
        }
        else {
            if (cData->getFlag(9) == 1)
                cout << "\t\t" << setw(11) << syntaxStack.top()->getTokenIdentifier() << "\t>\t" <<
                inputToken->getTokenIdentifier() << endl;
            saveToken = inputToken;
            handleToken = syntaxStack.top();
            syntaxStack.pop();
            handle = handleToken->getTokenIdentifier();
            while (true) {
                fPrecIter = fSymbolMap.find(syntaxStack.top()->getTokenIdentifier());
                gPrecIter = gSymbolMap.find(handleToken->getTokenIdentifier());

                if (fPrecIter->second == gPrecIter->second) {
                    if (cData->getFlag(9) == 1) {
                        if (fPrecIter->second < gPrecIter->second)
                            cout << "\t\t" << setw(11) << syntaxStack.top()->getTokenIdentifier() << "\t<\t" <<
                            setw(11) << handleToken->getTokenIdentifier() << endl;
                        else
                            cout << "\t\t" << setw(11) << syntaxStack.top()->getTokenIdentifier() << "\t=\t" <<
                            handleToken->getTokenIdentifier() << endl;
                    }
                    stackToken = syntaxStack.top();

                    syntaxStack.pop();
                    //Stack token (LHS) becomes next token
                    tempString += stackToken->getTokenIdentifier();
                    tempString += " ";
                    tempString += handle;
                    handle = tempString;
                    tempString.clear();
                    handleToken = stackToken;
                }
                else {
                    if (cData->getFlag(9) == 1) {
                        if (fPrecIter->second < gPrecIter->second)
                            cout << "\t\t" << setw(11) << syntaxStack.top()->getTokenIdentifier() << "\t<\t" <<
                            setw(11) << handleToken->getTokenIdentifier() << endl;
                        else
                            cout << "\t\t" << setw(11) << syntaxStack.top()->getTokenIdentifier() << "\t>\t" <<
                            handleToken->getTokenIdentifier() << endl;
                    }
                    if (cData->getFlag(8) == 1) {
                        cout << "Syntax  before reduction: " << "\t";
                        printSyntaxStack();
                        cout << handle << endl;
                    }
                    if (cData->getFlag(12) == 1) {
                        cout << "SEMANTICS BEFORE REDUCTION";
                        printSemanticStack();
                        cout << endl;
                    }
                    if (cData->getFlag(10) == 1) {
                        cout << "\t\tHandle is: " << handle << endl;
                    }

                    multimap<string, Production>::iterator it = productionMap.find(handle);
                    if (it == productionMap.end()) {
                        cout << endl << "Reduceability Error!" << endl
                        << "START TO PANIC!!!" << endl;
                        panic(handle);
                        break;
                    }

                    semantics.goSemanticsGo(it->second.getNum());

                    if (cData->getFlag(12) == 1) {
                        cout << "SEMANTICS AFTER REDUCTION";
                        printSemanticStack();
                        cout << endl;
                    }

                    if (cData->getFlag(7) == 1) {
                        if (it != productionMap.end())
                            cout << "\tReduction " << it->second.getNum() << ".\t" << it->second.getLHS() << " --> " <<
                            it->first << endl;
                    }

                    fPrecIter = fSymbolMap.find(syntaxStack.top()->getTokenIdentifier());
                    gPrecIter = fSymbolMap.find(it->second.getLHS());
                    syntaxStack.push(new Token(0, it->second.getLHS(), it->second.getLHS()));

                    if (cData->getFlag(8) == 1) {
                        cout << "Syntax after reduction:" << "\t";
                        printSyntaxStack();
                        cout << endl;
                    }

                    if (it->second.getNum() == 1) {
                        cout << "Succesful parse" << endl;
                        cout << "Go have a beer!" << endl;
                        return;
                    }

                    if (syntaxStack.size() != semanticsStack.size()) {
                        cout << "OMFG YATES THE STACKS ARE NOT THE SAME SIZE" << endl;
                        cout << "Size of syntax stack: " << syntaxStack.size();
                        cout << endl << "Size of semantics stack: " << semanticsStack.size() << endl;
                    }

                    handle.clear();
                    tempString.clear();
                    reductionFound = true;
                    break;
                }
            }//Ends Greater than While Loop
        }
    }
}

void Parser::printSyntaxStack() {
    stack <Token*> tempStack;
    while (!syntaxStack.empty()) {
        tempStack.push(syntaxStack.top());
        syntaxStack.pop();
    }

    while (!tempStack.empty()) {
        cout << tempStack.top()->getTokenIdentifier() << " ";
        syntaxStack.push(tempStack.top());
        tempStack.pop();
    }
}

void Parser::printSemanticStack() {
    stack <string> tempStack;
    while (!semanticsStack.empty()) {
        tempStack.push(semanticsStack.top());
        semanticsStack.pop();
    }
    while (!tempStack.empty()) {
        cout << tempStack.top() << " ";
        semanticsStack.push(tempStack.top());
        tempStack.pop();
    }
    cout << endl;
}

void Parser::panic(string handle) {
    Token *panicToken;
    //PANIC MODE!//
    panicToken = syntaxStack.top();
    cout << "Stack: ";
    printSyntaxStack();
    cout << " " << handle << endl;
    cout << "Symbols thrown out: ";
    cout << handle << endl;
    while (panicToken->getTokenIdentifier() != "decllist-" &&
           panicToken->getTokenIdentifier() != "statlist-" &&
           panicToken->getTokenIdentifier() != "###") {
        cout << panicToken->getTokenIdentifier() << " ";
        syntaxStack.pop();
        panicToken = syntaxStack.top();
    }
    cout << endl << "Source Line Thrown Out: " << lex.getLine() << endl;
    lex.panicReset();
    return;
}
