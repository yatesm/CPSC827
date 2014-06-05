#ifndef COMPILERDATA__H
#define COMPILERDATA__H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "production.h"
#include "parseXML.h"
#include "fourTuple.h"

using std::vector;
using std::multimap;

class CompilerData {
	public:
		//Public Functions
		static CompilerData* getInstance();
    	~CompilerData();
    	//Public Accessor Functions
		int getMaxRealLength() const { return maxRealLength; }
		int getMaxIntLength() const { return maxIntLength; }
		int getMaxVarLength() const { return maxVarLength; }
		int getMaxFlags() const { return maxFlags; }
		int getMaxLineLength() const { return maxLineLength; };
		int getFlag(int flag) const { return flags[flag]; }
		void setFlag(int flag, int value);
		void addTuple(FourTuple* f);
		multimap<string, int>& getTokenData() { return tokenDataMap; }
		multimap<string, Production>& getProductionMap() { return productionDataMap; }
		multimap<string, int>& getFSymbolMap() { return fSymbolMap; }
		multimap<string, int>& getGSymbolMap() { return gSymbolMap; }
		std::vector<FourTuple*>* getTupleTable() { return &tupleTable; }
	private:	
		//Private Variables
		static CompilerData * instance;
	   ParseXML::ParseXML * xmlParser;
		HandleChars * compilerDataHandle;
		HandleChars * tokenDataHandle;
		std::multimap<string, int> compilerDataMap;		
		std::multimap<string, int> tokenDataMap;
		std::multimap<string, Production> productionDataMap;
		std::multimap<string, int> fSymbolMap;
		std::multimap<string, int> gSymbolMap;
		std::vector<FourTuple*> tupleTable;
		int maxRealLength;
		int maxIntLength;
		int maxVarLength;
		int maxFlags;
		int maxLineLength;
		int flags[40];
    
		//Private Functions
		void handleCompilerData();
		void handleTokenData();
		void handleProductionData(const string& filename);
		void handlePrecedenceData(const string& filename);
		void debug(const multimap<string, int>& xmlData);
		void debugSymbolMap(const multimap<string, int>& symbolMap);
		void debugProductions(const multimap<string, Production>& xmlData);
		void debugPrecedence();
		CompilerData();
		CompilerData(const CompilerData&);
		CompilerData& operator=(CompilerData&);
};
#endif
