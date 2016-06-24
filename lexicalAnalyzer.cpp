#include "lexicalAnalyzer.h"

#include <string>
#include <iostream>
#include <fstream>

#include <string.h>

#include "compilerData.h"

using std::string;
using std::cout;
using std::endl;

LexicalAnalyzer::LexicalAnalyzer(string fname) : state(0), saveState(0), startChar(0), currentChar(0), line(),
                                                 filename(fname),
                                                 tokenmultimap(CompilerData::getInstance()->getTokenData()),
                                                 in() {
    in.open(filename.c_str(), std::ios::in);
    if (!in) {
        throw std::string("Cannot open source file: ") + filename;
    }
}

Token *LexicalAnalyzer::getToken() {
    int numLength = 0, idLength = 0, flag = 0;
    string tempString = "", iString = "";
    startChar = currentChar;

    while (!in.eof()) {
        if ((unsigned) (currentChar) >= strlen(line)) {
            Token *tempToken = finishLastToken(tempString, iString);
            getNewLine();
            startChar = 0;
            currentChar = 0;
            if (tempToken != NULL)
                return (tempToken);
        }
        if (CompilerData::getInstance()->getFlag(33) == 1) {
            cout << "State: " << state << " currentChar: " << currentChar << " startChar: ";
            cout << startChar << " char: " << line[currentChar] << endl;
        }
        switch (state) {
            case 0:
                /* Begin Reserve Words */
                if (line[currentChar] == 'E') state = 1;                  //ELSE, END
                else if (line[currentChar] == 'I') state = 5;             //IF INTEGER INPUT
                else if (line[currentChar] == 'P') state = 13;            //PROCEDURE PROGRAM
                else if (line[currentChar] == 'D') state = 24;            //DO DECLARE
                else if (line[currentChar] == 'C') state = 30;            //CALL
                else if (line[currentChar] == 'V') state = 33;            //VALUE
                else if (line[currentChar] == 'R') state = 37;            //REAL REFERENCE
                else if (line[currentChar] == 'M') state = 46;            //MAIN
                else if (line[currentChar] == 'O') state = 49;            //OUTPUT
                else if (line[currentChar] == 'T') state = 54;            //THEN
                else if (line[currentChar] == 'W') state = 57;            //WHILE
                    /* End Reserve Words */

                    /* Begin Numerics and Identifiers */
                else if (line[currentChar] >= '0'
                         && line[currentChar] <= '9') {
                    state = 61;                                           //NUMERIC
                    tempString += line[currentChar];
                }
                else if (line[currentChar] == '-') {
                    state = 63;                                           //MINUS or NUMERIC
                    tempString += line[currentChar];
                }
                else if (line[currentChar] >= 'a'
                         && line[currentChar] <= 'z') {
                    state = 64;                                           //IDENTIFIER
                    tempString += line[currentChar];
                }
                    /* End Numerics and Identifiers */

                    /* Begin Single ASCII Characters */
                else if (line[currentChar] == '+')                        //PLUS_TOKEN_ID
                    return (makeASCIIToken("PLUS_TOKEN_ID", "+"));
                else if (line[currentChar] == '{')                        //LBRACE_TOKEN_ID
                    return (makeASCIIToken("LBRACE_TOKEN_ID", "{"));
                else if (line[currentChar] == '[')                        //LBRACKET_TOKEN_ID
                    return (makeASCIIToken("LBRACK_TOKEN_ID", "["));
                else if (line[currentChar] == '(')                        //LPAREN_TOKEN_ID
                    return (makeASCIIToken("LPAREN_TOKEN_ID", "("));
                else if (line[currentChar] == '}')                        //RBRACE_TOKEN_ID
                    return (makeASCIIToken("RBRACE_TOKEN_ID", "}"));
                else if (line[currentChar] == ']')                        //RBRACKET_TOKEN_ID
                    return (makeASCIIToken("RBRACK_TOKEN_ID", "]"));
                else if (line[currentChar] == ')')                        //RPAREN_TOKEN_ID
                    return (makeASCIIToken("RPAREN_TOKEN_ID", ")"));
                else if (line[currentChar] == '*')                        //MULT_TOKEN_ID
                    return (makeASCIIToken("MULT_TOKEN_ID", "*"));
                else if (line[currentChar] == ',')                        //COMMA_TOKEN_ID
                    return (makeASCIIToken("COMMA_TOKEN_ID", ","));
                else if (line[currentChar] == ';')                        //SEMI_TOKEN_ID
                    return (makeASCIIToken("SEMI_TOKEN_ID", ";"));
                else if (line[currentChar] == '_')                        //UNDER_TOKEN_ID
                    return (makeASCIIToken("UNDER_TOKEN_ID", "_"));
                /* End Single ASCII Characters */


                /* Begin Double ASCII Characters */
                else if (line[currentChar] == '|') state = 65;            // | or ||
                else if (line[currentChar] == '&') state = 66;            // && or ASCII Error
                else if (line[currentChar] == '!') state = 67;            // ! or !=
                else if (line[currentChar] == '<') state = 68;            // < or <= or <-
                else if (line[currentChar] == '>') state = 69;            // > or >=
                else if (line[currentChar] == '=') state = 70;            // == or =
                else if (line[currentChar] == ':') state = 71;            // : or ::
                /* End Double ASCII Characters */

                /* Whitespace, Comments, Flags, Strings, Miscellany */
                else if (line[currentChar] == '#') state = 72;            //FLAGS
                else if (line[currentChar] == '/') state = 76;            //DIV_TOKEN_ID or COMMENT
                else if (line[currentChar] == '"') state = 79;            //STRING_TOKEN_ID
                else if (line[currentChar] == ' ') startChar = currentChar;  //SPACEBAR, ADVANCE
                /* End Whitespace, Comments, Flags, Miscellany */

                /* End ASCII Characters */

                /* Begin Error States */
                else if (line[currentChar] <= 'Z' && line[currentChar] >= 'A')
                    state = 1234;
                else if (line[currentChar] == '\0') {
                    getNewLine();
                    startChar = 0;
                    currentChar = -1;
                }
                else return (invalidASCII(line[currentChar]));            //INVALID ASCII CHAR
                /* End Error States */
                break;

            /* BEGINNING OF RESERVE WORD STATES */
            case 1:
                if (line[currentChar] == 'N') state = 2;                  //END
                else if (line[currentChar] == 'L') state = 3;             //ELSE
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 2:
                if (line[currentChar] == 'D') {
                    state = 1224;                                         //END_TOKEN_ID
                    tempString = "END_TOKEN_ID";
                    iString = "END";
                }
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 3:
                if (line[currentChar] == 'S') state = 4;                  //ELSE
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 4:
                if (line[currentChar] == 'E') {
                    state = 1224;                                         //ELSE_TOKEN_ID
                    tempString = "ELSE_TOKEN_ID";
                    iString = "ELSE";
                }
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 5:
                if (line[currentChar] == 'F') {
                    state = 1224;                                         //IF_TOKEN_ID
                    tempString = "IF_TOKEN_ID";
                    iString = "IF";
                }
                else if (line[currentChar] == 'N') state = 6;             //INTEGER, INPUT
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 6:
                if (line[currentChar] == 'T') state = 7;                  //INTEGER
                else if (line[currentChar] == 'P') state = 11;            //INPUT
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 7:
                if (line[currentChar] == 'E') state = 8;                  //INTEGER
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 8:
                if (line[currentChar] == 'G') state = 9;                  //INTEGER
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 9:
                if (line[currentChar] == 'E') state = 10;                 //INTEGER
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 10:
                if (line[currentChar] == 'R') {
                    state = 1224;                                         //INTEGER
                    tempString = "integer";
                    iString = "INTEGER";
                }
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 11:
                if (line[currentChar] == 'U') state = 12;                 //INPUT
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 12:
                if (line[currentChar] == 'T') {
                    state = 1224;                                         //INPUT_TOKEN_ID
                    tempString = "INPUT_TOKEN_ID";
                    iString = "INPUT";
                }
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 13:
                if (line[currentChar] == 'R') state = 14;                 //PR
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 14:
                if (line[currentChar] == 'O') state = 15;                 //PRO
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 15:
                if (line[currentChar] == 'C') state = 16;                 //PROC
                else if (line[currentChar] == 'G') state = 21;            //PROG
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 16:
                if (line[currentChar] == 'E') state = 17;                  //PROCE
                else state = 1234;                                         //RESERVED WORD ERROR
                break;
            case 17:
                if (line[currentChar] == 'D') state = 18;                  //PROCED
                else state = 1234;                                         //RESERVED WORD ERROR
                break;
            case 18:
                if (line[currentChar] == 'U') state = 19;                  //PROCEDU
                else state = 1234;                                         //RESERVED WORD ERROR
                break;
            case 19:
                if (line[currentChar] == 'R') state = 20;                  //PROCEDUR
                else state = 1234;                                         //RESERVED WORD ERROR
                break;
            case 20:
                if (line[currentChar] == 'E') {
                    state = 1224;                                         //PROCEDURE_TOKEN_ID
                    tempString = "PROCEDURE_TOKEN_ID";
                    iString = "PROCEDURE";
                }
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 21:
                if (line[currentChar] == 'R') state = 22;                 //PROGR
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 22:
                if (line[currentChar] == 'A') state = 23;                 //PROGRA
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 23:
                if (line[currentChar] == 'M') {
                    state = 1224;                                         //PROGRAM_TOKEN_ID
                    tempString = "PROGRAM_TOKEN_ID";
                    iString = "PROGRAM";
                }
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 24:
                if (line[currentChar] == 'O') {
                    state = 1224;                                         //DO_TOKEN_ID
                    tempString = "DO_TOKEN_ID";
                    iString = "DO";
                }
                else if (line[currentChar] == 'E') state = 25;            //DE
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 25:
                if (line[currentChar] == 'C') state = 26;                 //DEC
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 26:
                if (line[currentChar] == 'L') state = 27;                 //DECL
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 27:
                if (line[currentChar] == 'A') state = 28;                 //DECLA
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 28:
                if (line[currentChar] == 'R') state = 29;                 //DECLAR
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 29:
                if (line[currentChar] == 'E') {
                    state = 1224;                                         //DECLARE_TOKEN_ID
                    tempString = "DECLARE_TOKEN_ID";
                    iString = "DECLARE";
                }
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 30:
                if (line[currentChar] == 'A') state = 31;                 //CA
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 31:
                if (line[currentChar] == 'L') state = 32;                 //CAL
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 32:
                if (line[currentChar] == 'L') {
                    state = 1224;                                         //CALL_TOKEN_ID
                    tempString = "CALL_TOKEN_ID";
                    iString = "CALL";
                }
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 33:
                if (line[currentChar] == 'A') state = 34;                 //VA
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 34:
                if (line[currentChar] == 'L') state = 35;                 //VAL
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 35:
                if (line[currentChar] == 'U') state = 36;                 //VALU
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 36:
                if (line[currentChar] == 'E') {
                    state = 1224;                                         //VALUE_TOKEN_ID
                    tempString = "VALUE_TOKEN_ID";
                    iString = "VALUE";
                }
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 37:
                if (line[currentChar] == 'E') state = 38;                 //RE
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 38:
                if (line[currentChar] == 'A') state = 39;                 //REA
                else if (line[currentChar] == 'F') state = 40;            //REF
                break;
            case 39:
                if (line[currentChar] == 'L') {
                    state = 1224;                                         //REAL
                    tempString = "REAL_TOKEN_ID";
                    iString = "REAL";
                }
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 40:
                if (line[currentChar] == 'E') state = 41;                 //REFE
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 41:
                if (line[currentChar] == 'R') state = 42;                 //REFER
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 42:
                if (line[currentChar] == 'E') state = 43;                 //REFERE
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 43:
                if (line[currentChar] == 'N') state = 44;                 //REFEREN
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 44:
                if (line[currentChar] == 'C') state = 45;                 //REFERENC
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 45:
                if (line[currentChar] == 'E') {
                    state = 1224;                                         //REFERENCE_TOKEN_ID
                    tempString = "REFERENCE_TOKEN_ID";
                    iString = "REFERENCE";
                }
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 46:
                if (line[currentChar] == 'A') state = 47;                 //MA
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 47:
                if (line[currentChar] == 'I') state = 48;                 //MAI
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 48:
                if (line[currentChar] == 'N') {
                    state = 1224;                                         //MAIN_TOKEN_ID
                    tempString = "MAIN_TOKEN_ID";
                    iString = "MAIN";
                }
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 49:
                if (line[currentChar] == 'U') state = 50;                 //OU
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 50:
                if (line[currentChar] == 'T') state = 51;                 //OUT
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 51:
                if (line[currentChar] == 'P') state = 52;                 //OUTP
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 52:
                if (line[currentChar] == 'U') state = 53;                 //OUTPU
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 53:
                if (line[currentChar] == 'T') {
                    state = 1224;                                         //OUTPUT_TOKEN_ID
                    tempString = "OUTPUT_TOKEN_ID";
                    iString = "OUTPUT";
                }
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 54:
                if (line[currentChar] == 'H') state = 55;                 //TH
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 55:
                if (line[currentChar] == 'E') state = 56;                 //THE
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 56:
                if (line[currentChar] == 'N') {
                    state = 1224;                                         //THEN_TOKEN_ID
                    tempString = "THEN_TOKEN_ID";
                    iString = "THEN";
                }
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 57:
                if (line[currentChar] == 'H') state = 58;                 //WH
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 58:
                if (line[currentChar] == 'I') state = 59;                 //WHI
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 59:
                if (line[currentChar] == 'L') state = 60;                 //WHIL
                else state = 1234;                                        //RESERVED WORD ERROR
                break;
            case 60:
                if (line[currentChar] == 'E') {
                    state = 1224;                                         //WHILE_TOKEN_ID
                    tempString = "WHILE_TOKEN_ID";
                    iString = "WHILE";
                }
                else state = 1234;                                        //RESERVED WORD ERROR
                break;

            case 1224:                                                    //GOOD RESERVED WORD
                if (line[currentChar] <= 'Z' && line[currentChar] >= 'A') state = 1234;
                else return (makeToken(iString, tempString));
                break;
            /* END OF RESERVE WORD STATES */

            /* BEGIN NUMERICS, IDENTIFIERS STATES */
            case 61:                                                      //NUMERIC
                if (line[currentChar] >= '0' && line[currentChar] <= '9') {
                    numLength++;
                    tempString += line[currentChar];
                }
                else if (line[currentChar] == '.') {
                    state = 62;                                           //REAL
                    tempString += line[currentChar];
                }
                else {
                    if (numLength > CompilerData::getInstance()->getMaxIntLength())
                        return (invalidInt(tempString));
                    else
                        return (makeToken("integer", tempString));
                }
                break;
            case 62:                                                      //Already seen a decimal (.)
                if (line[currentChar] >= '0' && line[currentChar] <= '9') {
                    numLength++;
                    tempString += line[currentChar];
                }
                else if (line[currentChar - 1] == '.') {
                    tempString.erase(tempString.size() - 1, 1);
                    currentChar--;
                    return (makeToken("integer", tempString));
                }
                else {
                    if (numLength > CompilerData::getInstance()->getMaxRealLength())
                        return (invalidReal(tempString));
                    else
                        return (makeToken("real", tempString));
                }
                break;
            case 63:
                if (line[currentChar] >= '0' && line[currentChar] <= '9') {
                    state = 61;
                    numLength++;
                    tempString += line[currentChar];
                }
                else
                    return (makeToken("-", "MINUS_TOKEN_ID"));
                break;
            case 64:
                if ((line[currentChar] >= 'a' && line[currentChar] <= 'z')
                    || (line[currentChar] <= '9' && line[currentChar] >= '0')
                    || line[currentChar] == '_') {
                    idLength++;
                    tempString += line[currentChar];
                }
                else {
                    if (idLength > (CompilerData::getInstance()->getMaxVarLength())) {
                        return (invalidIdentifier(tempString));
                    }
                    else {
                        return (makeToken("var", tempString));
                    }
                }
                break;
            /* END NUMERICS, IDENTIFIERS STATES */

            /* BEGIN ASCII CHARACTERS */
            case 65:
                if (line[currentChar] == '|')                             //BARBAR_TOKEN_ID
                    return (makeASCIIToken("BARBAR_TOKEN_ID", "||"));
                else                                                      //BAR_TOKEN_ID
                    return (makeToken("|", "BAR_TOKEN_ID"));
            case 66:
                if (line[currentChar] == '&')                             //AMPAMP_TOKEN_ID
                    return (makeASCIIToken("AMPAMP_TOKEN_ID", "&&"));
                else return (invalidASCII(line[currentChar]));            //INVALID ASCII
            case 67:
                if (line[currentChar] == '=')
                    return (makeASCIIToken("NEQ_TOKEN_ID", "!="));        //NEQ_TOKEN_ID
                else
                    return (makeToken("!", "EXCL_TOKEN_ID"));             //EXCL_TOKEN_ID
            case 68:
                if (line[currentChar] == '=')                             //LTEQ_TOKEN_ID
                    return (makeASCIIToken("LTEQ_TOKEN_ID", "<="));
                else if (line[currentChar] == '-')                        //ASS_TOKEN_ID
                    return (makeASCIIToken("ASS_TOKEN_ID", "<-"));
                else                                                      //LT_TOKEN_ID
                    return (makeToken("<", "LT_TOKEN_ID"));
            case 69:
                if (line[currentChar] == '=')                             //GTEQ_TOKEN_ID
                    return (makeASCIIToken("GTEQ_TOKEN_ID", ">="));

                else                                                      //GT_TOKEN_ID
                    return (makeToken(">", "GT_TOKEN_ID"));
            case 70:
                if (line[currentChar] == '=')                             //EQEQ_TOKEN_ID
                    return (makeASCIIToken("EQEQ_TOKEN_ID", "=="));
                else                                                      //EQ_TOKEN_ID
                    return (makeToken("=", "EQ_TOKEN_ID"));
            case 71:
                if (line[currentChar] == ':')                             //COLCOL_TOKEN_ID
                    return (makeASCIIToken("COLCOL_TOKEN_ID", "::"));
                else                                                      //COL_TOKEN_ID
                    return (makeToken(":", "COLON_TOKEN_ID"));
            /* BEGIN FLAG STATES */
            case 72:
                if (line[currentChar] == '#') state = 73;                 //FLAG LINE
                else return (invalidASCII(line[currentChar]));            //INVALID ASCII
                break;
            case 73:
                if (line[currentChar] == '+') state = 74;                 //FLAG ON
                else if (line[currentChar] == '-') state = 75;            //FLAG OFF
                break;
            case 74:
                /* Check for number, if number, increase flag value */
                if (line[currentChar] >= '0' && line[currentChar] <= '9')
                    flag = (flag * 10) + ((int) (line[currentChar]) - 48);
                    /* If not number, turn on flags */
                else {
                    CompilerData::getInstance()->setFlag(flag, 1);
                    if (flag == 20 || flag == 0)
                        CompilerData::getInstance()->addTuple(new FourTuple("-", "FLAG20ON", "-", "-"));
                    flag = 0;
                    /* Check to see if non-number is + or - */
                    if (line[currentChar] == '+') state = 74;             //Turn Flag On
                    else if (line[currentChar] == '-') state = 75;        //Turn Flag Off
                    else if (line[currentChar] == '#') state = 80;        //Look for end of flags
                    else state = 81;                                      //Waiting for #+-
                }
                break;
            case 75:
                /*Check for number, if number, increase flag value */
                if (line[currentChar] >= '0' && line[currentChar] <= '9')
                    flag = (flag * 10) + ((int) (line[currentChar]) - 48);
                    /* If not number, turn off flags */
                else {
                    CompilerData::getInstance()->setFlag(flag, 0);
                    if (flag == 20 || flag == 0)
                        CompilerData::getInstance()->addTuple(new FourTuple("-", "FLAG20OFF", "-", "-"));
                    flag = 0;
                    if (line[currentChar] == '+') state = 74;             //Turn Flag On
                    else if (line[currentChar] == '-') state = 75;        //Turn Flag Off
                    else if (line[currentChar] == '#') state = 80;        //Look for end of flags
                    else state = 81;                                      //Waiting for #+-
                }
                break;
            case 80:
                if (line[currentChar] == '#') {
                    state = 0;
                    startChar = currentChar + 1;
                }
                else if (line[currentChar] == '+') state = 74;            //Turn Flag On
                else if (line[currentChar] == '-') state = 75;            //Turn Flag Off
                else state = 81;                                          //Waiting for #+-
                break;
            case 81:
                if (line[currentChar] == '#') state = 80;                 //Look for end of Flags
                else if (line[currentChar] == '+') state = 74;            //Turn Flag On
                else if (line[currentChar] == '-') state = 75;            //Turn Flag Off
                break;
            /* END FLAG STATES */

            /* BEGIN COMMENT STATES */
            case 76:
                if (line[currentChar] == '*') state = 77;                 //BEGIN COMMENT
                else {
                    state = 0;                                            //DIV_TOKEN_ID
                    return (makeToken("/", "DIV_TOKEN_ID"));
                }
                break;
            case 77:
                if (line[currentChar] == '*') state = 78;                 //BEGIN END COMMENT
                else if (line[currentChar] == '\n') {
                    state = 77;                                           //END LINE CHARACTER
                    getNewLine();                                         //GET NEW LINE
                    startChar = 0;
                    currentChar = -1;                                     //Increments to 0 after switch
                }
                else state = 77;                                          //CONTINUE COMMENT
                break;
            case 78:
                if (line[currentChar] == '/') {
                    state = 0;                                            //END COMMENT
                    if ((unsigned) (currentChar) < strlen(line))
                        startChar = currentChar + 1;                      //currentChar will increment
                }
                else state = 77;                                          //KEEP LOOKING FOR COMMENT
                break;
            /* END COMMENT STATES */

            /* BEGIN STRING STATES */
            case 79:
                if (line[currentChar] == '"') {
                    state = 0;                                            //END QUOTATION
                    tempString = "";
                    for (int i = 0; i <= ((currentChar) - (startChar)); ++i) {
                        tempString += line[i + startChar];
                    }
                    currentChar++;
                    startChar = currentChar;
                    return (makeToken("string", tempString));
                }
                break;
            /* END STRING STATES */

            case 1234:
                if (line[currentChar] <= 'Z' && line[currentChar] >= 'A') state = 1234;
                else {
                    tempString = "";
                    for (int i = 0; i < (currentChar - startChar); ++i) {
                        if (line[i + startChar] != ' ')
                            tempString += line[i + startChar];
                    }
                    return (invalidReserveWord(tempString));
                }
                break;
            default:
                cout << "Something went wrong!" << endl;
                cout << "State = " << state << "currentChar = " << currentChar << "startChar = " << startChar << endl;
                state = 0;
                break;
        }
        ++currentChar;
    }
    return (NULL);
}

Token *LexicalAnalyzer::invalidASCII(char c) {
    state = 0;
    currentChar++;
    if (c == ' ')
        return (NULL);
    for (unsigned int i = 0; i < (80 - 43); ++i)
        cout << " ";
    if ((int) (c) < 10)
        cout << " ";
    else if ((int) (c) < 100)
        cout << "  ";
    else
        cout << "   ";
    cout << "INVALID ASCII CHARACTER " << c << " ASCII Value: " << (int) (c) << endl;
    return (NULL);
}

Token *LexicalAnalyzer::invalidReal(string s) {
    //13 + s.size() + 28
    for (unsigned int i = 0; i < (80 - (41 + s.size())); ++i)
        cout << " ";
    cout << "INVALID REAL " << s << " Too many significant digits" << endl;
    state = 0;
    return (NULL);
}

Token *LexicalAnalyzer::invalidReserve(char c) {
    for (unsigned int i = 0; i < (80 - 23); ++i)
        cout << " ";
    cout << "INVALID RESERVE WORD: " << c << endl;
    state = 0;
    currentChar++;
    return (NULL);
}

Token *LexicalAnalyzer::invalidReserveWord(string tempString) {
    for (unsigned int i = 0; i < (80 - (22 + tempString.size())); ++i)
        cout << " ";
    cout << "INVALID RESERVE WORD: " << tempString << endl;
    state = 0;
    return (NULL);
}

Token *LexicalAnalyzer::invalidInt(string s) {
    //19 + s.size() + 28
    for (unsigned int i = 0; i < (80 - (44 + s.size())); ++i)
        cout << " ";
    cout << "INVALID INTEGER " << s << " Too many significant digits" << endl;
    state = 0;
    return (NULL);
}

Token *LexicalAnalyzer::invalidIdentifier(string s) {
    //19 + s.size() + 28
    for (unsigned int i = 0; i < (80 - (47 + s.size())); ++i)
        cout << " ";
    cout << "INVALID IDENTIFIER " << s << " Too many significant digits" << endl;
    state = 0;
    return (NULL);
}

Token *LexicalAnalyzer::finishLastToken(string tempString, string iString) {
    if (state == 65) return (makeASCIIToken("BAR_TOKEN_ID", "|"));
    else if (state == 66) return (invalidASCII('&'));
    else if (state == 67) return (makeASCIIToken("!", "EXCL_TOKEN_ID"));
    else if (state == 68) return (makeASCIIToken("<", "LT_TOKEN_ID"));
    else if (state == 69) return (makeASCIIToken(">", "GT_TOKEN_ID"));
    else if (state == 70) return (makeASCIIToken("=", "EQ_TOKEN_ID"));
    else if (state == 71) return (makeASCIIToken(":", "COL_TOKEN_ID"));
    else if (state == 61) return (makeToken("integer", tempString));
    else if (state == 62) return (makeToken("REAL_TOKEN_ID", tempString));
    else if (state == 64) return (makeToken("var", tempString));
    else if (state == 1224) return (makeToken(iString, tempString));
    return (NULL);
}

Token *LexicalAnalyzer::makeASCIIToken(string id, string name) {
    ++currentChar;
    return (makeToken(name, id));
}

Token *LexicalAnalyzer::makeToken(string id, string name) {
    state = 0;
    multimap<string, int>::iterator it = tokenmultimap.find(name);
    Token *tempToken = new Token(0, name, id);
    if (CompilerData::getInstance()->getFlag(2) == 1) {
        int offset = 7 + 11 + tempToken->getTokenIdentifier().size() + 2;
        for (int i = 0; i < (80 - offset); ++i)
            cout << " ";
        cout << "Token: " << tempToken->getTokenIdentifier() << " Token ID: " << tempToken->getTokenID() << endl;
    }
    return (tempToken);
}

void LexicalAnalyzer::getNewLine() {
    in.getline(line, LINE_LENGTH, '\n');
    if (CompilerData::getInstance()->getFlag(1) == 1) cout << "Input Line: " << line << endl;
}

void LexicalAnalyzer::panicReset() {
    in.getline(line, LINE_LENGTH, '\n');
    if (CompilerData::getInstance()->getFlag(1) == 1) cout << "Input Line: " << line << endl;
    currentChar = 0;
    startChar = 0;
    state = 0;
}

void LexicalAnalyzer::go() {
    getNewLine();
    while (!in.eof()) {
        getToken();
    }
}
