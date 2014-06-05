#ifndef SEMANTICOBJECT__H
#define SEMANTICOBJECT__H

#include <string>
#include <iostream>
#include <iomanip>
using std::cout; 
using std::endl;
using std::setw;
using std::string;

class SemanticObject {
	public:
		SemanticObject();
		SemanticObject(const SemanticObject&);
		SemanticObject (string id, string shp, string siz, string siz2, 
		   			    string typ);
		SemanticObject (string id, string shp, string siz, string siz2, 
							 string typ, string call);
							 
//		friend ostream& operator<<(ostream& out, SemanticObject& s);
		SemanticObject& operator=(SemanticObject const&);

		string getIdentifier() { return identifier; }
		string getSize2() 	  { return size2; }
		string getSize() 		  { return size;  }
		string getShape()		  { return shape; }
		string getType()       { return type;  }
		string getCallType()   { return calltype; }

		void setIdentifier(string str) { identifier = str; }
		void setSize2(string siz) { size2 = siz; }
		void setSize(string siz) { size = siz; }
		void setShape(string shp) { shape = shp; }
		void setType(string t) { type = t; }
		void setCallType(string call) { calltype = call; }
		
		void print ();

	private:
		string type;		
		string identifier;
		string shape;
		string size;
		string size2;
		string calltype;
};
#endif
