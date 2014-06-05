#ifndef LEXICALANALYZER__H
#define LEXICALANALYZER__H

#include <map>
#include <fstream>

#include "token.h"

using std::multimap;
using std::fstream;

const int LINE_LENGTH = 81;

class LexicalAnalyzer {
	public:
		LexicalAnalyzer(string fname);
	
		
		void tokenCopy(Token* tempToken);	
		void panicReset();
		
		Token* getToken();
		Token* invalidInt(string s);
		Token* invalidReal(string s);
		Token* invalidReserve(char c);
		Token* invalidASCII(char c);
		Token* invalidReserveWord(string tempString);
		Token* invalidIdentifier(string s);
		Token* makeToken(string id, string name);
		Token* makeASCIIToken(string id, string name);
		Token *makeReserveToken(string id, string name);
		Token* finishLastToken(string tempString, string iString);
		void getNewLine();	
		void go();
		const char* getLine() const { return line; }
		~LexicalAnalyzer() { in.close(); }
      
	private:
		int state;
		int saveState;
		int startChar;
		int currentChar;
		char line[LINE_LENGTH];
		string filename;
		multimap<string, int> tokenmultimap;
		fstream in;
		//Private functions not for use
		LexicalAnalyzer(const LexicalAnalyzer& l);
		LexicalAnalyzer& operator=(const LexicalAnalyzer& l);
};
#endif
