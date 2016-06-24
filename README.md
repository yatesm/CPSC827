# Compiler Developed for CPSC 827: Translation of Programming Languages

## Description

This repository contains the final milestone for CPSC 827: Translation of Programming Languages.  CPSC 827 was a
semester long course on the translation of programming languages, in which students developed a compiler over the course
of the semester.

The compiler was developed on a CentOS distribution of Linux, and compiled to using GNU G++.  It should work with MingW, however, I provide no guarantees.  It uses a blend of OO development and design patterns.  The compiler developed was a single-pass four-stage bottom-up compiler for a Turing-complete imperative language.  The target language for this compiler's translation is [x86 High-Level Assembly (HLA)](http://www.plantation-productions.com/Webster/).  I'll take a moment to talk about each of these terms:

1. **Single-pass** - Single pass compilers go through the stages of compilation a single time.  This means that there is very little optimization built into the compiler, as typically optimization occurs in subsequent passes.

2. **Four Stage** - The compiler completes its translation in four phases (Lexical analysis, parsing, semantic analysis and code generation).

  + **Lexical analysis** - In this phase, the compiler ensures that each of the tokens or symbols in the program belong to the language of the grammar that generates programs.
  + **Parsing** - In this phase, the compiler ensures that the program being compiled belongs to the language of the grammar that generates programs.  This is done using a simple precedence parsing algorithm.
  + **Semantic Analysis** - In this phase, syntactic reductions made during parsing produce an intermediate language, defined by a set of 1 or more tuples which describe the high level code at a lower level.
  + **Code Generation** - In this final phase, the tuples generated from semantic analysis are translated into x86 HLA.

3. **Bottom-Up Compiler** - The compiler parses programs by attempting to 'reduce' the program to an initial start symbol in the grammar by making syntactic reduction.  As opposed to a top-down compiler/parser, which seeks to begin at the grammatical start symbol and grow the program that is trying to be compiled.

4. **Turing-complete, imperative language**:
  + **Turing-complete** - The language has been shown (not included) to be Turing complete; i.e., it is capable of producing any program that other general-purpose high level languages can produce.
  + **Imperative** - The programs produced by the language are stateful and a combination of subroutines, procedures and steps to complete execution of a program.
5. **x86 High Level Assembly (HLA)** - HLA is an assembly language abstraction on the x86 instruction set.  The abstraction mainly concerns the different dialects of x86 and provides a unified and consistent syntax.  HLA also provides high level control structures (for, while, do-while, if, else, switch), but those were not used in this program.

The compiler is written in C++ and uses a OO programming and design pattersn to accomplish compilation.  In order to fulfill the requirements for the class, we implemented 7 of 8 possible extra language features:
  1. Register Spillage.
  2. Global / Local / Static scoping.
  3. One and Two-Dimensional Arrays.
  4. Procedures and Functions
  5. Pass by Value
  6. Pass by Reference
  7. Recursive Functions

Additionally, the language supports most high level language features:
  * Scalar, Vector and Matrix data structures
  * Ints, chars, strings
  * Functions and procedures with Parameter passing and scope
  * Standard control structures: for, while, do-while, if, if-else

## Installation

1. Clone the code to your computer.
2. Compile the source using the makefile.
3. [Install x86 HLA](http://www.plantation-productions.com/Webster/)

## Usage
1. Run the program with syntax ./executable <inputfile.hcg>
2. The program will produce <inputfile.hla> as output.
3. Run the .hla output with the x86 HLA assembler.

## Contributing

Please do not contribute to this project.  This compiler was a pedagogical proof of concept to illustrate the language theoretic constructs delivered in CPSC 827: Translation of Programming Languages.  This compiler and this project should not be used for anything besides an exhibition of the work I did that semester.

## Credits

Credit goes to me, myself, Yates Monteith, as well as my partner for this project, Anthony McDougal.

## License

GNU GPL v2.0.  See License
