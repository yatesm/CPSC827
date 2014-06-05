#ifndef ASSEMBLYGENERATOR__H
#define ASSEMBLYGENERATOR__H

#include<vector>
#include "registerSimulation.h"
#include "compilerData.h"

using std::vector;

class AssemblyGenerator {
	public:
		//Constructors
		AssemblyGenerator() : cData(CompilerData::getInstance()), tupleList(cData->getTupleTable()), simulation(), printFlag(false) {
			registerNames[0] = "EAX";
			registerNames[1] = "EBX";
			registerNames[2] = "ECX";
			registerNames[3] = "EDX";
			registerNames[4] = "ESI";
			registerNames[5] = "EDI";
			}
		
		void generateAssembly();
		int spill(FILE* filePtr);
		int retrieveSpilled(FILE* filePtr);
		
		string determineType (string value);
        void doComparison(string op3, string op4, FILE* filePtr);
        void addSub(string op1, string op2, string op3, string op4, FILE *filePtr);
        void printToFile(FILE* filePtr);
	private:
		CompilerData *cData;
		vector<FourTuple*> *tupleList;
		string registerNames[6];
		RegisterSimulation simulation;
		bool printFlag;
		//Private methods not meant to be called
		AssemblyGenerator(const AssemblyGenerator&);
		AssemblyGenerator& operator=(const AssemblyGenerator&);
};
#endif
