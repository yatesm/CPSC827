#ifndef FourTuple__H
#define FourTuple__H

#include<string>
#include<string.h>
#include<ostream>
using std::ostream;
using std::string;

class FourTuple {
	public:
		FourTuple();
		FourTuple(const FourTuple&);
		FourTuple(string o1, string o2, string o3, string o4);

		friend ostream& operator<<(ostream& out, FourTuple& s);
		FourTuple& operator=(const FourTuple&);	

		string getOperand(int i);
		void setOperand(string s, int i);

	private:	
		string operand1;
		string operand2;
		string operand3;
		string operand4;
};
#endif
