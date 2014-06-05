#ifndef PRODUCTION__H
#define PRODUCTION__H

#include <string>

using std::string;

class Production {
	public:
		Production() : lhs(""), rhs(""), num(0) {}
		Production(string l, string r, int n) : lhs(l), rhs(r), num(n) {}
		string getLHS() const { return lhs; }
		string getRHS() const { return rhs; }
		int 	 getNum() const { return num; }
		
		void setRHS(string s) { rhs = s; }
		void setLHS(string s) { lhs = s; }
		void setNum(int i) 	 { num = i; }
	private:
		Production& operator=(const Production& p);
		string lhs;
		string rhs;
		int num;
};
#endif
