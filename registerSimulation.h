#ifndef REGISTERSIMULATION__H
#define REGISTERSIMULATION__H

#define MAX_REGISTERS 6

#include "register.h"
#include <vector>
#include <iostream>

using std::vector; using std::cout; using std::endl;

class RegisterSimulation {
	public:
		RegisterSimulation() : stack() {}
		~RegisterSimulation() {}
		int findFreeRegister(int doNotUse);
		int findFreeRegister(int doNotUse, int alsoDoNotUse);
		void print();
		void printToFile(FILE* filePtr);
		void clearRegisters();
		void setValueInRegister(string v, int i);
		void setUsedInRegister(bool b, int i);
		int findValueInRegister(string v);
		
		string getRegisterValue(int i);
		void evictRegister(int i);
		void setRegisterUsed(int i);
		void addToStack(string s) { stack.push_back(s); }
		int spillRegister();
		string popSpilled();
	private:
		Register registers[MAX_REGISTERS];
		vector<string> stack;
		RegisterSimulation(const RegisterSimulation& r);
		RegisterSimulation& operator=(const RegisterSimulation& r);
};
#endif
