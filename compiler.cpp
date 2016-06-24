#include "compiler.h"

Compiler::Compiler(string fname) : cData(CompilerData::getInstance()), parser(fname), assembler() {
}

void Compiler::compile() {
    parser.beginParse();
    assembler.generateAssembly();
}
