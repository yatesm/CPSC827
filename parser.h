#ifndef PARSER__H
#define PARSER__H

#include <map>
#include <string>
#include <stack>
#include "lexicalAnalyzer.h"
#include "token.h"
#include "compilerData.h"
#include "semanticsAnalyzer.h"

using std::multimap;
using std::string;
using std::stack;

class Parser {
	public:
		Parser(string fname);
		void beginParse();
		void printSyntaxStack();
      void printSemanticStack();
		void panic(string handle);
	private:
		multimap<string, Production> productionMap;
		multimap<string, int> fSymbolMap;
		multimap<string, int> gSymbolMap;
		stack<Token*> syntaxStack;
		stack<string> semanticsStack;
		LexicalAnalyzer lex;
		SemanticsAnalyzer semantics;
		CompilerData* cData;
		Parser(const Parser& p);
		Parser& operator=(const Parser& p);
};
#endif
