#include "fourTuple.h"

FourTuple::FourTuple() :
   operand1(""), operand2(""), operand3(""), operand4("") { }

FourTuple::FourTuple(const FourTuple& s) :
   operand1(s.operand1), operand2(s.operand2),
   operand3(s.operand3), operand4(s.operand4) { }

FourTuple::FourTuple(string o1, string o2, string o3, string o4) :
   operand1(o1), operand2(o2), operand3(o3), operand4(o4) { }

FourTuple& FourTuple::operator=(const FourTuple& s) {
	operand1 = s.operand1;
	operand2 = s.operand2;
	operand3 = s.operand3;
	operand4 = s.operand4;

	return *this;
}

ostream& operator<< ( ostream& out, FourTuple& s ) {
	out << "(" << s.getOperand(1) << ", " << s.getOperand(2) << ", " << s.getOperand(3) <<
	   ", " << s.getOperand(4) << ")";
	return out;
}

string FourTuple::getOperand(int i) {
	switch(i) {
		case 1:
			return operand1;
		case 2:
			return operand2;
		case 3:
			return operand3;
		case 4:
			return operand4;	
	}

	return "";
}

void FourTuple::setOperand(string s, int i) {
	switch(i) {
		case 1:
			operand1 = s;
		case 2:
			operand2 = s;
		case 3:
			operand3 = s;
		case 4:
			operand4 = s;	
	}
}
