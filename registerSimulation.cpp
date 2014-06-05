#include "registerSimulation.h"

#include<stdio.h>
int RegisterSimulation::findValueInRegister(string v) {
	for(int i = 0; i < MAX_REGISTERS; ++i) {
		if(registers[i].getValue() == v)
			return i;
	}
	return -1;
}

void RegisterSimulation::evictRegister(int i) { 
	if((i < MAX_REGISTERS) && (i > -1))
		registers[i].evict();
	else
		cout<<"Something has gone wrong, you've tried to evict a non-existent register"<<endl;
	return;
}

void RegisterSimulation::setValueInRegister(string v, int i) { 
	if(i < MAX_REGISTERS && i > -1) {
		registers[i].setValue(v);
		registers[i].setIVUsed(false);
	}
	else
		cout<<"Something has gone wrong, you've tried to evict and set a non-existent register"<<endl;
	return;
}	

string RegisterSimulation::getRegisterValue(int i) { 
	if(i < MAX_REGISTERS && i > -1)
		return registers[i].getValue(); 
	else
		cout<<"Something has gone terribly wrong, you've tried to retrieve a non-existent register"<<endl;
	return NULL;
}

void RegisterSimulation::setRegisterUsed(int i) { 
	if(i < MAX_REGISTERS && i > -1)
		registers[i].setIVUsed(true);
	else
		cout<<"Something has gone terribly wrong, you've tried to retrieve a non-existent register"<<endl;
	return;
}
	
int RegisterSimulation::findFreeRegister(int doNotUse) {
	for(int i = 0; i < MAX_REGISTERS; ++i) {
		if(registers[i].getValue() == "" && i != doNotUse)
			return i;
	}
	//No Free Registers	
	for(int i = 0; i < MAX_REGISTERS; ++i) {
		if(registers[i].getContentType() == "IV" && registers[i].getIVUsed() == true && i != doNotUse) {
			registers[i].evict();
			return i;
		}
	}
	for(int i = 0; i < MAX_REGISTERS; ++i) {
		if(registers[i].getContentType() == "constant" && i != doNotUse) {
			registers[i].evict();
			return i;
		}
	}
	//No IVs or Constants, remove Variable, push to
	for(int i = 0; i < MAX_REGISTERS; ++i) {
		if(registers[i].getContentType() == "variable" && i != doNotUse) {
			registers[i].evict();
			return i;
		}
	}
	//Unused intermediate variables		
	return -1;
}

int RegisterSimulation::spillRegister() {
	addToStack(registers[0].getValue());
	return(0);
}

string RegisterSimulation::popSpilled() {
	string popped = stack[stack.size() - 1];
	stack.pop_back();
	return popped;
}
	

int RegisterSimulation::findFreeRegister(int doNotUse, int alsoDoNotUse) {
	for(int i = 0; i < MAX_REGISTERS; ++i) {
		if(registers[i].getValue() == "" && i != doNotUse && i != alsoDoNotUse)
			return i;
	}
	//No Free Registers	
	for(int i = 0; i < MAX_REGISTERS; ++i) {
		if(registers[i].getContentType() == "IV" && registers[i].getIVUsed() == true && i != doNotUse && i != alsoDoNotUse) {
			registers[i].evict();
			return i;
		}
	}
	for(int i = 0; i < MAX_REGISTERS; ++i) {
		if(registers[i].getContentType() == "constant" && i != doNotUse && i != alsoDoNotUse) {
			registers[i].evict();
			return i;
		}
	}
	//No IVs or Constants, remove Variable, push to
	for(int i = 0; i < MAX_REGISTERS; ++i) {
		if(registers[i].getContentType() == "variable" && i != doNotUse && i != alsoDoNotUse) {
			registers[i].evict();
			return i;
		}
	}
	//Unused intermediate variables
	for(int i = 0; i < MAX_REGISTERS; ++i) {
		if(registers[i].getContentType() == "IV" && i != doNotUse && i != alsoDoNotUse) {
			registers[i].evict();
			return i;
		}
	}
		
	cout<<"Something has gone wrong"<<endl;
	return -1;
}


void RegisterSimulation::setUsedInRegister(bool b, int i) {
	if(i > -1  && i < MAX_REGISTERS)
		registers[i].setIVUsed(b);
}

void RegisterSimulation::clearRegisters() {
	for(int i = 0; i < MAX_REGISTERS; ++i)
		registers[i].evict();
}

void RegisterSimulation::print() {
	for(int i = 0; i < MAX_REGISTERS; ++i) {
		if(i == 0)
			cout<<"EAX: "<<registers[i].getValue()<<endl;
		else if(i == 1)
			cout<<"EBX: "<<registers[i].getValue()<<endl;		
		else if(i == 2)
			cout<<"ECX: "<<registers[i].getValue()<<endl;		
		else if(i == 3)
			cout<<"EDX: "<<registers[i].getValue()<<endl;				
		else if(i == 4)
			cout<<"ESI: "<<registers[i].getValue()<<endl;
		else if(i == 5)
			cout<<"EDI: "<<registers[i].getValue()<<endl;
	}
	cout<<"Stack (Top):";
	for(int i = stack.size() - 1; i >= 0; --i)
		cout<<" "<<stack[i]<<" ";
	cout<<endl;
}
	
void RegisterSimulation::printToFile(FILE *filePtr) {
	fprintf(filePtr, "\n");
	for(int i = 0; i < MAX_REGISTERS; ++i) {
		if(i == 0)
			fprintf(filePtr, "EAX: %s\n", registers[i].getValue().c_str());
		else if(i == 1)
			fprintf(filePtr, "EBX: %s\n", registers[i].getValue().c_str());		
		else if(i == 2)
			fprintf(filePtr, "ECX: %s\n", registers[i].getValue().c_str());		
		else if(i == 3)
			fprintf(filePtr, "EDX: %s\n", registers[i].getValue().c_str());				
		else if(i == 4)
			fprintf(filePtr, "ESI: %s\n", registers[i].getValue().c_str());
		else if(i == 5)
			fprintf(filePtr, "EDI: %s\n", registers[i].getValue().c_str());			
	}
	fprintf(filePtr, "Stack (Top): ");
	for(int i = stack.size() - 1; i >= 0; --i)
		fprintf(filePtr, " %s ", stack[i].c_str());
	fprintf(filePtr, "\n");	
}
