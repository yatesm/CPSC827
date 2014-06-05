#include "compilerData.h"

#include <map>
#include <iostream>
#include <fstream>
using std::multimap;
using std::cout;
using std::endl;
using std::cin;	

CompilerData* CompilerData::getInstance() {
	if(!instance)
		instance = new CompilerData();
	return instance;
}

CompilerData::~CompilerData() {
	delete xmlParser;
	delete compilerDataHandle;
	delete tokenDataHandle;
}

CompilerData::CompilerData() : xmlParser(ParseXML::getInstance()), 
                              compilerDataHandle(new HandleChars),
                              tokenDataHandle(new HandleChars),
                              compilerDataMap(), tokenDataMap(),
                              productionDataMap(), fSymbolMap(),
                              gSymbolMap(), tupleTable(),
                              maxRealLength(0), 
                              maxIntLength(0), maxVarLength(0), maxFlags(0),
                              maxLineLength(0) 
{
	   xmlParser->parseXML("data/compilerData.xml", compilerDataHandle);
	   handleCompilerData();
//	   debug(compilerDataHandle->getData());
	   xmlParser->parseXML("data/tokenData.xml", tokenDataHandle);
	   handleTokenData();
//	   debug(tokenDataMap);
		handleProductionData("data/productionData.txt");
//		debugProductions(productionDataMap);
		handlePrecedenceData("data/precedenceData.txt");
//		debugPrecedence();
	   for(int i = 0; i < maxFlags; ++i)
	  	   flags[i] = 0;
}

void CompilerData::addTuple(FourTuple* f) { 
   if(flags[13] == 1)
      cout<<"\t\t\t\t"<<*f<<endl;
   tupleTable.push_back(f);
}

void CompilerData::handleCompilerData() {
	multimap<string, string> tempMap = compilerDataHandle->getData();
	multimap<string, string>::iterator it = tempMap.begin();
	while(it != tempMap.end()) {
		if(it->first == "MAX_REAL_LENGTH")
			maxRealLength = atoi(it->second.c_str());
		else if(it->first == "MAX_INT_LENGTH")
			maxIntLength = atoi(it->second.c_str());
		else if(it->first == "MAX_ID_LENGTH")
			maxVarLength = atoi(it->second.c_str());
		else if(it->first == "MAX_FLAGS")
			maxFlags = atoi(it->second.c_str());
		else if(it->first == "LINE_LENGTH")
			maxLineLength = atoi(it->second.c_str());
		it++;
	}
}

void CompilerData::handleProductionData(const string& filename) {
	const int BUFSIZE = 2048;
	char buff[BUFSIZE];
   std::fstream in;
   in.open(filename.c_str(), std::ios::in);
   if (!in) { // couldn't open the game file
      throw std::string("Cannot open file: ")+filename;
   }
	int i = 1;
	string lhs, rhs;
   while ( true ) {
		in.getline( buff, BUFSIZE);
		if ( in.eof() ) break;
		lhs = buff;
		
		in.getline( buff, BUFSIZE);
		if ( in.eof() ) break;
		rhs = buff;
		productionDataMap.insert(std::multimap<string, Production>::value_type(rhs, Production(lhs, rhs, i)));      
		++i;
   }	
}

void CompilerData::handlePrecedenceData(const string& filename) {
	const int BUFSIZE = 2048;
	char buff[BUFSIZE];
   std::fstream in;
   in.open(filename.c_str(), std::ios::in);
   if (!in) { // couldn't open the game file
      throw std::string("Cannot open file: ")+filename;
   }
	string lhs, rhs;
   while ( true ) {
		in.getline( buff, BUFSIZE);
		if ( in.eof() ) break;
		lhs = buff;
		
		in.getline( buff, BUFSIZE);
		if ( in.eof() ) break;
		rhs = buff;
		fSymbolMap.insert(std::multimap<string, int>::value_type(lhs, atoi(rhs.c_str())));      
		
		in.getline( buff, BUFSIZE);
		if ( in.eof() ) break;
		rhs = buff;
		gSymbolMap.insert(std::multimap<string, int>::value_type(lhs, atoi(rhs.c_str())));      
	}
}		

	
void CompilerData::handleTokenData() {
	multimap<string, string> tempMap = tokenDataHandle->getData();
	multimap<string, string>::iterator it = tempMap.begin();
	while(it != tempMap.end()) {
		tokenDataMap.insert(std::multimap<string, int>::value_type(it->first, atoi(it->second.c_str())));
		it++;
	}
}

void CompilerData::debug(const multimap<string, int>& xmlData) {
  multimap<string, int>::const_iterator ptr = xmlData.begin();
  cout << "XML Data:" << endl;
  while ( ptr != xmlData.end() ) {
    cout << "(" << ptr->first << ", " << ptr->second << ")" << endl;
    ++ptr;
  }
}

void CompilerData::debugProductions(const multimap<string, Production>& xmlData) {
	multimap<string, Production>::const_iterator ptr = xmlData.begin();
   cout << "XML Data:" << endl;
   while ( ptr != xmlData.end() ) {
      cout << ptr->second.getNum() << ". " << ptr->second.getLHS() << " --> " << ptr->second.getRHS() << endl;
      ++ptr;
   }
}

void CompilerData::debugPrecedence() {
	multimap<string, int>::const_iterator fPtr = fSymbolMap.begin();
	multimap<string, int>::const_iterator gPtr = gSymbolMap.begin();	
	cout << "Precedence Data:" <<endl;
	while( fPtr != fSymbolMap.end() || gPtr != gSymbolMap.end()) {
		cout<< "( "<<fPtr->first <<", " << fPtr->second << ", " << gPtr->second << ")" << endl;
		fPtr++;
		gPtr++;
	}
}

void CompilerData::debugSymbolMap(const multimap<string, int>& symbolMap) {
	multimap<string, int>::const_iterator ptr = symbolMap.begin();
   while ( ptr != symbolMap.end() ) {
      cout << "(" << ptr->first << " --> " << ptr->second << ")" << endl;
      ++ptr;
   }	
}

void CompilerData::setFlag(int flag, int value) {
   if(flag == 0) {
      for(int i = 0; i < maxFlags; ++i)
         flags[i] = value;
   }
   else
      flags[flag] = value;
}
