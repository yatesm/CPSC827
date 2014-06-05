#ifndef COMPILER__H
#define COMPILER__H

#include "token.h"
#include "parser.h"
#include "compilerData.h"
#include "assemblyGenerator.h"

using std::map;

class Compiler {
	public:
		Compiler(string fname);
		void compile();
	private:
		CompilerData *cData;
		Parser parser;
		AssemblyGenerator assembler;
		//Private functions not for use.
		Compiler(const Compiler& c);
		Compiler& operator=(const Compiler& c);
};
#endif
