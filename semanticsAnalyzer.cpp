#include "semanticsAnalyzer.h"
#include "compilerData.h"
#include "semanticObject.h"
#include "fourTuple.h"

#include<stack>
#include<string>
#include<sstream>
#include<iostream>

using std::stack;
using namespace std;

SemanticsAnalyzer::SemanticsAnalyzer(std::stack<string> &s) : intermediateInteger(0), intermediateReal(0), intermediateBool(0), intermediateLabel(0), cData(CompilerData::getInstance()), labelStack(new stack<string>), semanticStack(&s),
   localTable(), globalTable(), localFlag(false) {}

bool SemanticsAnalyzer::checkDoubleDecl(string varString) {
    if(localFlag) {
        if(localTable.findObject(varString)) {
            cout<<"Entry "<<varString<<" already exists in local symbol table!"<<endl;
            return true;
        }
    }
   else {
	   if(globalTable.findObject(varString)) {
         cout<<"Entry "<<varString<<" already exists in global symbol table!"<<endl;
         return true;
      }
   }
	return false;
}

bool SemanticsAnalyzer::checkGlobalDecl(string varString) {
        if(globalTable.findObject(varString)) {
           cout<<"Entry "<<varString<<" already exists in global symbol table!"<<endl;
           return true;
        }
	return false;
}

void SemanticsAnalyzer::clearLocalSymbolTable() {
   localTable.clearTable();
}

void SemanticsAnalyzer::goSemanticsGo(int productionNumber) {
	SemanticObject *obj, *tempObject;   
	
   string varString, callString, typeString, rowString, colString;
   string procString, constString, tempVar, stringString, iv1, iv2, iv3;
   string bv1, ivLoad, numCols, actualCallString, rhsString, typeString1, typeString2;
   if(productionNumber == 25)
      localFlag = true;
   if(productionNumber == 21 || productionNumber == 22)
      localFlag = false;
	switch(productionNumber) {
		//   start --> prog body END
      case 1:
         semanticStack->pop();
         semanticStack->pop();
         varString = semanticStack->top();
         cData->addTuple(new FourTuple(varString, "PROGRAMEND", "-", "-"));
		   if(cData->getFlag(16) == 1)
		      printGlobalSymbolTable();
		break;
		//   prog --> PROGRAM var
		case 2:
			// store s-i in symboltable, set type "program"
			varString = semanticStack->top();
			semanticStack->pop();
			semanticStack->pop();			
			obj = new SemanticObject(varString, "", "", "", "PROGRAM");
            
			// add to symbol table
			globalTable.addObject(*obj, localFlag);

			//set s-i-1 <-- location
			semanticStack->push(varString);
			cData->addTuple(new FourTuple(varString, "PROGRAMBEGIN", "", ""));
		break;
		//   body --> declpart procpart execpart
		case 3:
		    //Lambda Production, pop two guys off semantic stack.
		    semanticStack->pop();
		    semanticStack->pop();
		break;
		//   body --> declpart execpart
		case 4:
		    //Lambda Production
		    //Pop a guy off the semantic Stack.
		    semanticStack->pop();
		break;
		//   declpart --> DECLARE decllist END
		case 5:
            //Lambda Production
            //Pop 2 guys off the stack
            semanticStack->pop();
            semanticStack->pop();
		break;
		//   decllist --> decllist-
		case 6:
		    //Lambda Production
		    //|LHS| == |RHS| Pop Nothing
		break;
		//   decllist- --> decllist- declstat ;
		case 7:
		    //Lambda Production
		    //Pop 2 guys off semantic Stack
		    semanticStack->pop();
		    semanticStack->pop();
		break;
		//   decllist- --> declstat ;
		case 8:
		    //Lambda Production
		    //Pop 2 guys off semantic Stack
		    semanticStack->pop();		    
		break;
		//   declstat --> declstat-
		case 9:
		    //Lambda Production
		    //|RHS| == |LHS| Pop nothing
		break;
		//   declstat- --> declstat- , var
		case 10:
		    varString = semanticStack->top();
		    semanticStack->pop();
		    semanticStack->pop(); //pop the ,
		    if(checkDoubleDecl(varString)) {
		        semanticStack->push("-");
		        break;
		    }		    tempVar = semanticStack->top();
		    if(localFlag)
		        tempObject = localTable.getObject(tempVar);
		    else
		        tempObject = globalTable.getObject(tempVar);
		    obj = new SemanticObject(varString, tempObject->getShape(), tempObject->getSize(), tempObject->getSize2(), tempObject->getType());
          if(localFlag)
            localTable.addObject(*obj, localFlag);
          else
            globalTable.addObject(*obj, localFlag);
		    cData->addTuple(new FourTuple(varString, "MEMORY", obj->getSize(), obj->getSize2()));
	   break;	
		//   declstat- --> declstat- , var = constant
		case 11:
		    constString = semanticStack->top();
		    semanticStack->pop();
		    semanticStack->pop(); //Pop the =
		    varString = semanticStack->top();
		    semanticStack->pop();
		    semanticStack->pop(); //Pop the ,
		    
		    if(checkDoubleDecl(varString)) {
		        semanticStack->push("-");
		        break;
		    }
		    tempVar = semanticStack->top();
		    if(localFlag)
		       tempObject = localTable.getObject(tempVar);
		    else
   		    tempObject = globalTable.getObject(tempVar);
		    obj = new SemanticObject(varString, tempObject->getShape(), tempObject->getSize(), tempObject->getSize2(), tempObject->getType());
          if(localFlag)
            localTable.addObject(*obj, localFlag);
          else
            globalTable.addObject(*obj, localFlag);
		    cData->addTuple(new FourTuple(varString, "MEMORY", "1", "-"));
		break;
		//   declstat- --> type var
		case 12:
         varString = semanticStack->top();
         semanticStack->pop();
         typeString = semanticStack->top();
         semanticStack->pop();
		   if(checkDoubleDecl(varString)) {
		        semanticStack->push("-");
		        break;
		   }
          obj = new SemanticObject(varString, "SCALAR", "1", "-", typeString);
          if(localFlag)
            localTable.addObject(*obj, localFlag);
          else
            globalTable.addObject(*obj, localFlag);
         semanticStack->push(varString);
         cData->addTuple(new FourTuple(varString, "MEMORY", "1", "-"));
         tempObject = localTable.getObject(tempVar);
	    break;
		//   declstat- --> type var = constant
		case 13:	
		    constString = semanticStack->top();
		    semanticStack->pop();
		    semanticStack->pop(); //Pop the =
		    varString = semanticStack->top();
		    semanticStack->pop();
		    typeString = semanticStack->top();
		    semanticStack->pop();
		    if(checkDoubleDecl(varString)) {
		      semanticStack->push("-");
		      break;
		    }
		    
          obj = new SemanticObject(varString, "SCALAR", "1", "-", typeString);
          if(localFlag)
            localTable.addObject(*obj, localFlag);
          else
            globalTable.addObject(*obj, localFlag);
		    cData->addTuple(new FourTuple(varString, "MEMORYASSIGN", "1", constString));
            semanticStack->push(varString);		    
		break;
		//   declstat- --> type var integer
		case 14:
		    rowString = semanticStack->top();
		    semanticStack->pop();
		    varString = semanticStack->top();
		    semanticStack->pop();
		    typeString = semanticStack->top();
		    semanticStack->pop();
		    if(checkDoubleDecl(varString)) {
		        semanticStack->push("-");
		        break;
		    }		    
		    obj = new SemanticObject(varString, "VECTOR", rowString, "-", typeString);
          if(localFlag)
            localTable.addObject(*obj, localFlag);
          else
            globalTable.addObject(*obj, localFlag);
		    cData->addTuple(new FourTuple(varString, "MEMORY", rowString, "-"));
		    semanticStack->push(varString); 
		break;
		//   declstat- --> type var integer :: integer
		case 15:
		    colString = semanticStack->top();
		    semanticStack->pop();
		    semanticStack->pop(); //Pop the ::
		    rowString = semanticStack->top();
		    semanticStack->pop();
		    varString = semanticStack->top();
		    semanticStack->pop();
		    typeString = semanticStack->top();
		    semanticStack->pop();
		    if(checkDoubleDecl(varString)) {
		        semanticStack->push("-");
		        break;
		    }		    
		    obj = new SemanticObject(varString, "MATRIX", rowString, colString, typeString);
          if(localFlag)
            localTable.addObject(*obj, localFlag);
          else
            globalTable.addObject(*obj, localFlag);
		    cData->addTuple(new FourTuple(varString, "MEMORY", rowString, colString));
		    semanticStack->push(varString);
		break;
		//   type --> INTEGER
		case 16:
		    semanticStack->pop();
		    semanticStack->push("int");
		break;
		//   type --> REAL
		case 17:
		    semanticStack->pop();
		    semanticStack->push("real");
		break;
		//   procpart --> proclist
		case 18:
		    //Lambda Production
		    //|RHS| == |LHS| Pop Nothing
		break;
		//   proclist --> proclist proc
		case 19:
		    //Lambda Production
		    //Pop 1 guy off Semantic Stack
		    semanticStack->pop();
		break;
		//   proclist --> proc
		case 20:
		    //Lambda Production
		    //|RHS| == |LHS| Pop Nothing
		break;
		//   proc --> prochead declpart statlist END
		case 21:
		    semanticStack->pop(); //Pop END
		    semanticStack->pop(); //Pop statlist
		    semanticStack->pop(); //Pop declpart
		    procString = semanticStack->top();
		    cData->addTuple(new FourTuple(procString, "ENDPROCEDURE", "-", "-"));
		    if(cData->getFlag(14) == 1)
		       printLocalSymbolTable();
		    localTable.empty();
		break;
		//   proc --> prochead statlist END
		case 22:
		    semanticStack->pop(); //Pop END
		    semanticStack->pop(); //Pop statlist
		    procString = semanticStack->top();
		    cData->addTuple(new FourTuple(procString, "ENDPROCEDURE", "-", "-"));
		    if(cData->getFlag(14) == 1)
		      printLocalSymbolTable();
		    localTable.empty();
		break;
		//   prochead --> procname null-list
		case 23:
		    semanticStack->pop();  //Pop null-list
		    cData->addTuple(new FourTuple("-", "NOFORMALPARAMETERS", "-", "-"));
		break;
		//   prochead --> procname fparmlist }
		case 24:
		    semanticStack->pop();  //Pop }
		    semanticStack->pop();  //Pop fparmlist   
		    cData->addTuple(new FourTuple("-", "ENDFORMALPARAMETERS", "-", "-"));
		break;
		//   procname --> PROCEDURE var
		case 25:
			varString = semanticStack->top();
			semanticStack->pop();
			semanticStack->pop();
		    if(checkDoubleDecl(varString)) {
		        semanticStack->push("-");
		        break;
		   }
		   obj = new SemanticObject(varString, "-", "-", "-", "PROCEDURE");
			globalTable.addObject(*obj, localFlag);
			if(localTable.isEmpty())
				localTable.empty();
			localTable.addObject(*obj, localFlag);
			semanticStack->push(varString);
			cData->addTuple(new FourTuple(varString, "PROCEDUREBEGIN", "-", "-"));
		break;
		//   null-list --> { }
		case 26:
		    semanticStack->pop();
		    semanticStack->pop();
		    semanticStack->push("-");
		break;
		//   fparmlist --> fparmlist-
		case 27:
		    //Lambda ProductionInput Line:
		    //|RHS| == |LHS| No Popping Necessary    
		break;
		//   fparmlist- --> fparmlist- , calltype type var
		case 28:
          varString = semanticStack->top();
          semanticStack->pop();
          typeString = semanticStack->top();
          semanticStack->pop();
          callString = semanticStack->top();
          semanticStack->pop();
          semanticStack->pop();  //pop ,
    	    if(localTable.findObject(varString)) {
		        cout<<"Entry "<<varString<<" already exists in local symbol table!"<<endl;
		        break;
		    }
		    obj = new SemanticObject(varString, "SCALAR", "1", "-", typeString, callString);
		    localTable.addObject(*obj, localFlag);
	       if(callString == "val")
	          cData->addTuple(new FourTuple(varString, "FORMALVALUEPARAMETER", "1", "-"));
	       else if(callString == "ref")	    
	          cData->addTuple(new FourTuple(varString, "FORMALREFERENCEPARAMETER", "1", "-"));
	       else
	          cout<<"Something has gone wrong!  Incorrect call-type!"<<endl;		
		break;
		//   fparmlist- --> fparmlist- , calltype type var integer
		case 29:
		    rowString = semanticStack->top();
		    semanticStack->pop();
		    varString = semanticStack->top();
		    semanticStack->pop();
		    typeString = semanticStack->top();
		    semanticStack->pop();
		    callString = semanticStack->top();
		    semanticStack->pop();  //pop calltype
			semanticStack->pop();  //pop ,
		    if(localTable.findObject(varString)) {
		        cout<<"Entry "<<varString<<" already exists in local symbol table!"<<endl;
		        break;
		    }
		    obj = new SemanticObject(varString, "VECTOR", rowString, "-", typeString, callString);
		    localTable.addObject(*obj, localFlag);

	       if(callString == "val")
	          cData->addTuple(new FourTuple(varString, "FORMALVALUEPARAMETER", rowString, "-"));
          else if(callString == "ref")	    
	          cData->addTuple(new FourTuple(varString, "FORMALREFERENCEPARAMETER", rowString, "-"));
	       else
	          cout<<"Something has one wrong!  Incorrect call-type!"<<endl;		
		break;
		//   fparmlist- --> fparmlist- , calltype type var integer :: integer
		case 30:
		    colString = semanticStack->top();
		    semanticStack->pop();  //Pop integer @ Si
		    semanticStack->pop();  //Pop the ::
		    rowString = semanticStack->top();
		    semanticStack->pop();  //Pop integer @ Si-2
		    varString = semanticStack->top();
		    semanticStack->pop();  //Pop var @ Si-3
		    typeString = semanticStack->top();
		    semanticStack->pop();  //Pop type @ Si-4
		    callString = semanticStack->top();
		    semanticStack->pop();  //Pop callType @ Si-5
		    semanticStack->pop();
		    if(localTable.findObject(varString)) {
		    	cout<<"Entry "<<varString<<" already exists in local symbol table!"<<endl;
		    	break;
		    }
		    obj = new SemanticObject(varString, "MATRIX", rowString, colString, typeString, callString);
		    localTable.addObject(*obj, localFlag);
	       if(callString == "val")
	          cData->addTuple(new FourTuple(varString, "FORMALVALUEPARAMETER", rowString, colString));
	       else if(callString == "ref")	    
	          cData->addTuple(new FourTuple(varString, "FORMALREFERENCEPARAMETER", rowString, colString));
	       else
	        	 cout<<"Something has gone wrong!  Incorrect call-type!"<<endl;
		break;
		//   fparmlist- --> { calltype type var
		case 31:
          varString = semanticStack->top();
          semanticStack->pop();
          typeString = semanticStack->top();
          semanticStack->pop();
          callString = semanticStack->top();
          semanticStack->pop();
//			 semanticStack->pop();  //Pop the { off          
    	    if(localTable.findObject(varString)) {
		        cout<<"Entry "<<varString<<" already exists in local symbol table!"<<endl;
		        semanticStack->push("-");
		        break;
		    }
		    obj = new SemanticObject(varString, "SCALAR", "1", "-", typeString, callString);
		    localTable.addObject(*obj, localFlag);
          cData->addTuple(new FourTuple("-", "BEGINFORMALPARAMETERS", "-", "-"));
	       if(callString == "val")
	          cData->addTuple(new FourTuple(varString, "FORMALVALUEPARAMETER", "1", "-"));
	       else if(callString == "ref")	    
	          cData->addTuple(new FourTuple(varString, "FORMALREFERENCEPARAMETER", "1", "-"));
	       else
	          cout<<"Something has gone wrong!  Incorrect call-type!"<<endl;
//	        semanticStack->push(varString); 
	    break;
		//   fparmlist- --> { calltype type var integer		    
		case 32:
		    rowString = semanticStack->top();
		    semanticStack->pop();
		    varString = semanticStack->top();
		    semanticStack->pop();
		    typeString = semanticStack->top();
		    semanticStack->pop();
		    callString = semanticStack->top();
		    semanticStack->pop();
//			 semanticStack->pop();  //Pop the { off
		    if(localTable.findObject(varString)) {
		        cout<<"Entry "<<varString<<" already exists in local symbol table!"<<endl;
		        semanticStack->push("-");
		        break;
		    }
		    obj = new SemanticObject(varString, "VECTOR", rowString, "-", typeString, callString);
		    localTable.addObject(*obj, localFlag);

          cData->addTuple(new FourTuple("-", "BEGINFORMALPARAMETERS", "-", "-"));
	       if(callString == "val")
	          cData->addTuple(new FourTuple(varString, "FORMALVALUEPARAMETER", rowString, "-"));
          else if(callString == "ref")	    
	          cData->addTuple(new FourTuple(varString, "FORMALREFERENCEPARAMETER", rowString, "-"));
	       else
	          cout<<"Something has one wrong!  Incorrect call-type!"<<endl;
//	       semanticStack->push(varString);  
		break;
		//   fparmlist- --> { calltype type var integer :: integer
		case 33:
		    colString = semanticStack->top();
		    semanticStack->pop();  //Pop integer @ Si
		    semanticStack->pop();  //Pop the ::
		    rowString = semanticStack->top();
		    semanticStack->pop();  //Pop integer @ Si-2
		    varString = semanticStack->top();
		    semanticStack->pop();  //Pop var @ Si-3
		    typeString = semanticStack->top();
		    semanticStack->pop();  //Pop type @ Si-4
		    callString = semanticStack->top();
		    semanticStack->pop();  //Pop callType @ Si-5
//		    semanticStack->pop();  //Pop the { off the stack
		    if(localTable.findObject(varString)) {
		        cout<<"Entry "<<varString<<" already exists in local symbol table!"<<endl;
		        semanticStack->push("-");
		        break;
		    }
		    obj = new SemanticObject(varString, "MATRIX", rowString, colString, typeString, callString);
		    localTable.addObject(*obj, localFlag);
	       cData->addTuple(new FourTuple("-", "BEGINFORMALPARAMETERS", "-", "-"));
	        if(callString == "val")
	            cData->addTuple(new FourTuple(varString, "FORMALVALUEPARAMETER", rowString, colString));
	        else if(callString == "ref")	    
	            cData->addTuple(new FourTuple(varString, "FORMALREFERENCEPARAMETER", rowString, colString));
	        else
	            cout<<"Something has one wrong!  Incorrect call-type!"<<endl;
//	       semanticStack->push(varString);	            
		break;
		//   calltype --> VALUE
		case 34:
		    semanticStack->pop();
		    semanticStack->push("val");
		break;
		//   calltype --> REFERENCE
		case 35:
		    semanticStack->pop();
		    semanticStack->push("ref");
		break;
		//   execpart --> exechead statlist END
		case 36:
			semanticStack->pop();
			semanticStack->pop();			
			cData->addTuple(new FourTuple("-", "ENDOFCALCULATIONS", "-", "-"));
		break;
		//   exechead --> MAIN
		case 37:
			cData->addTuple(new FourTuple("MAIN", "LABEL", "-", "-"));
		break;
		//   statlist --> statlist-
		case 38:
			//Lambda Production
			//|RHS| == |LHS| Don't Pop Anything
		break;
		//   statlist- --> statlist- stat
		case 39:
		//Lambda Production
			semanticStack->pop();			
		break;
		//   statlist- --> stat
		case 40:
			//Lambda Production
			//|RHS| == |LHS| Don't Pop Anything
		break;
		//   stat --> whilestat ;
		case 41:
			//Lambda Production
			semanticStack->pop();
		break;
		//   stat --> ifstat ;
		case 42:
			//Lambda Prodcution
			semanticStack->pop();
		break;
		//   stat --> assignstat
		case 43:
			//Lambda Production
			//|RHS| == |LHS| Don't Pop Anything
		break;
		//   stat --> inputstat- ;
		case 44:
			//Lambda Production
			semanticStack->pop();
		break;
		//   stat --> outputstat- ;
		case 45:
			//Lambda Production
			semanticStack->pop();
		break;
		//   stat --> callstat ;
		case 46:
			//Lambda Production		
			semanticStack->pop();
		break;
		//   inputstat- --> inputstat
 		case 47:
			cData->addTuple(new FourTuple("-", "ENDINPUTPARAMETER", "-", "-"));
			//|RHS| == |LHS| Don't Pop Anything 		
		break;
		//   inputstat --> inputstat , var
		case 48:
			varString = semanticStack->top();
			semanticStack->pop();  //Pop var
			semanticStack->pop();  //Pop comma
			//Check to see whether variable exists
			checkExistsAndShape(varString, "SCALAR");
			cData->addTuple(new FourTuple("-", "INPUTPARAMETER", varString, "1"));
		break;
		//   inputstat --> inputstat , var [ astat ]
		case 49:
			semanticStack->pop();  //Pop ]
			rowString = semanticStack->top();
			semanticStack->pop();  //Pop astat
			semanticStack->pop();  //Pop [
			varString = semanticStack->top();
			semanticStack->pop();  //Pop var
			semanticStack->pop();  //Pop comma
			//Check to see whether variable exists
			checkExistsAndShape(varString, "VECTOR");
			checkIndex(rowString);
			cData->addTuple(new FourTuple("-", "INPUTPARAMETER", varString, rowString));
		break;
		//   inputstat --> inputstat , var [ astat : astat ]
		case 50:
			semanticStack->pop();  //Pop ]
			colString = semanticStack->top();
			semanticStack->pop();  //Pop astat
			semanticStack->pop();  //Pop :
			rowString = semanticStack->top();
			semanticStack->pop();  //Pop astat
			semanticStack->pop();  //Pop [
			varString = semanticStack->top();
			semanticStack->pop();  //Pop var
			semanticStack->pop();  //Pop comma
			//Check to see whether variable exists			
			numCols = checkExistsAndShape(varString, "MATRIX");
			//Compute offset
			checkIndex(colString);
			checkIndex(rowString);
			iv1 = getIntermediateVariableInteger();
			iv2 = getIntermediateVariableInteger();
			cData->addTuple(new FourTuple(iv1, "IMULT", rowString, numCols));
			cData->addTuple(new FourTuple(iv2, "IADD", iv1, colString));
			cData->addTuple(new FourTuple("-", "INPUTPARAMETER", varString, iv2));		
		break;
		//   inputstat --> INPUT string , var
		case 51:
			varString = semanticStack->top();
			semanticStack->pop();  //Pop var
			semanticStack->pop();  //Pop comma
			stringString = semanticStack->top();
			semanticStack->pop();  //Pop string
			//Check to see whether variable exists
			checkExistsAndShape(varString, "SCALAR");			
			cData->addTuple(new FourTuple("-", "CALL", "SCANF", "-"));
			cData->addTuple(new FourTuple("-", "BEGININPUTPARAMETER", stringString, "-"));
			cData->addTuple(new FourTuple("-", "INPUTPARAMETER", varString, "1"));
		break;
		//   inputstat --> INPUT string , var [ astat ]
		case 52:
			semanticStack->pop();  //Pop ]
			rowString = semanticStack->top();
			semanticStack->pop();  //Pop astat
			semanticStack->pop();  //Pop [
			varString = semanticStack->top();
			semanticStack->pop();  //Pop var
			semanticStack->pop();  //Pop comma			
			stringString = semanticStack->top();
			semanticStack->pop();  //Pop string
			//Check to see whether variable exists
			checkExistsAndShape(varString, "VECTOR");
			checkIndex(colString);
			cData->addTuple(new FourTuple("-", "CALL", "SCANF", "-"));
			cData->addTuple(new FourTuple("-", "BEGININPUTPARAMETER", stringString, "-"));
			cData->addTuple(new FourTuple("-", "INPUTPARAMETER", varString, rowString));
		break;
		//   inputstat --> INPUT string , var [ astat : astat ]
		case 53:
			semanticStack->pop();  //Pop ]
			colString = semanticStack->top();
			semanticStack->pop();  //Pop astat
			semanticStack->pop();  //Pop :
			rowString = semanticStack->top();
			semanticStack->pop();  //Pop astat
			semanticStack->pop();  //Pop [
			varString = semanticStack->top();
			semanticStack->pop();  //Pop var
			semanticStack->pop();  //Pop ,
			stringString = semanticStack->top();
			semanticStack->pop();  //Pop string
			//Check to see whether variable exists			
			numCols = checkExistsAndShape(varString, "MATRIX");				
			checkIndex(rowString);			
			checkIndex(colString);
			cData->addTuple(new FourTuple("-", "CALL", "SCANF", "-"));
			cData->addTuple(new FourTuple("-", "BEGININPUTPARAMETER", stringString, "-"));
			//Compute offset
			iv1 = getIntermediateVariableInteger();
			iv2 = getIntermediateVariableInteger();
			cData->addTuple(new FourTuple(iv1, "IMULT", rowString, numCols));
			cData->addTuple(new FourTuple(iv2, "IADD", iv1, colString));
			cData->addTuple(new FourTuple("-", "INPUTPARAMETER", varString, iv2));			
		break;
		//   outputstat- --> outputstat
		case 54:
			//Lambda Production
			cData->addTuple(new FourTuple("-", "ENDOUTPUTPARAMETER", "-", "-"));
		break;
		//   outputstat --> outputstat , var
		case 55:
			varString = semanticStack->top();
			semanticStack->pop();
			semanticStack->pop();
			checkExistsAndShape(varString, "SCALAR");			
			cData->addTuple(new FourTuple("-", "OUTPUTPARAMETER", varString, "1"));			
		break;
		//   outputstat --> outputstat , var [ astat ]
		case 56:
			semanticStack->pop();  //Pop ]
			rowString = semanticStack->top();
			semanticStack->pop();  //Pop astat
			semanticStack->pop();  //Pop [
			varString = semanticStack->top();
			semanticStack->pop();  //Pop var
			semanticStack->pop();  //Pop comma
			checkExistsAndShape(varString, "VECTOR");			
			typeString2 = determineType(rowString);
			if(typeString2 != "int") {
				cout<<varString<<" is not of type INTEGER!"<<endl;
				break;
			}				
			iv1 = getIntermediateVariableInteger();
			cData->addTuple(new FourTuple(iv1, "ISUBLOAD", varString, rowString));
			cData->addTuple(new FourTuple("-", "OUTPUTPARAMETER", iv1, "-"));
		break;
		//   outputstat --> outputstat , var [ astat : astat ]
		case 57:
			semanticStack->pop();  //Pop ]
			colString = semanticStack->top();
			semanticStack->pop();  //Pop astat
			semanticStack->pop();  //Pop :
			rowString = semanticStack->top();
			semanticStack->pop();  //Pop astat
			semanticStack->pop();  //Pop [
			varString = semanticStack->top();
			semanticStack->pop();  //Pop var
			semanticStack->pop();  //Pop ,
			numCols = checkExistsAndShape(varString, "MATRIX");			
			checkIndex(colString);
			checkIndex(rowString);
			//Compute offset
			iv1 = getIntermediateVariableInteger();
			iv2 = getIntermediateVariableInteger();
			iv3 = getIntermediateVariableInteger();
			cData->addTuple(new FourTuple(iv1, "IMULT", rowString, numCols));
			cData->addTuple(new FourTuple(iv2, "IADD", iv1, colString));
			cData->addTuple(new FourTuple(iv3, "ISUBLOAD", varString, iv2));
			cData->addTuple(new FourTuple("-", "OUTPUTPARAMETER", iv3, "-"));			
		break;
		//   outputstat --> OUTPUT string , var
		case 58:
			varString = semanticStack->top();
			semanticStack->pop();  //Pop var
			semanticStack->pop();  //Pop comma
			stringString = semanticStack->top();
			semanticStack->pop();  //Pop string
			//Check to see whether variable exists
			checkExistsAndShape(varString, "SCALAR");
			cData->addTuple(new FourTuple("-", "CALL", "PRINTF", "-"));
			cData->addTuple(new FourTuple("-", "BEGINOUTPUTPARAMETER", stringString, "-"));
			cData->addTuple(new FourTuple("-", "OUTPUTPARAMETER", varString, "1"));
		break;
		//   outputstat --> OUTPUT string , var [ astat ]
		case 59:
			semanticStack->pop();  //Pop ]
			rowString = semanticStack->top();
			semanticStack->pop();  //Pop astat
			semanticStack->pop();  //Pop [
			varString = semanticStack->top();
			semanticStack->pop();  //Pop var
			semanticStack->pop();  //Pop comma
			stringString = semanticStack->top();
			semanticStack->pop();  //Pop String
			//Check to see whether variable exists
			checkExistsAndShape(varString, "VECTOR");			
			cData->addTuple(new FourTuple("-", "CALL", "PRINTF", "-"));
			cData->addTuple(new FourTuple("-", "BEGINOUTPUTPARAMETER", stringString, "-"));
			checkIndex(colString);								
			iv1 = getIntermediateVariableInteger();
			cData->addTuple(new FourTuple(iv1, "ISUBLOAD", varString, rowString));
			cData->addTuple(new FourTuple("-", "OUTPUTPARAMETER", iv1, "-"));
		break;
		//   outputstat --> OUTPUT string , var [ astat : astat ]
		case 60:
			semanticStack->pop();
			colString = semanticStack->top();
			semanticStack->pop(); //Pop first astat
			semanticStack->pop(); //Pop colon
			rowString = semanticStack->top();
			semanticStack->pop(); //Pop second astat
			semanticStack->pop();
			varString = semanticStack->top();
			semanticStack->pop(); //Pop var
			semanticStack->pop(); //Pop comma
			stringString = semanticStack->top();
			semanticStack->pop(); //Pop String
			//Check to see whether variable exists			
			numCols = checkExistsAndShape(varString, "MATRIX");			
			cData->addTuple(new FourTuple("-", "CALL", "PRINTF", "-"));
			cData->addTuple(new FourTuple("-", "BEGINOUTPUTPARAMETER", stringString, "-"));
			//Compute offset
			checkIndex(colString);
			checkIndex(rowString);								
			iv1 = getIntermediateVariableInteger();
			iv2 = getIntermediateVariableInteger();
			iv3 = getIntermediateVariableInteger();
			cData->addTuple(new FourTuple(iv1, "IMULT", rowString, numCols));
			cData->addTuple(new FourTuple(iv2, "IADD", iv1, colString));
			cData->addTuple(new FourTuple(iv3, "ISUBLOAD", varString, iv2));
			cData->addTuple(new FourTuple("-", "OUTPUTPARAMETER", iv3, "-"));		
		break;
		//   callstat --> callname null-list
		case 61:
			semanticStack->pop();
			cData->addTuple(new FourTuple("-", "NOACTUALPARAMETERS", "-", "-"));
		break;
		//   callstat --> callname aparmlist }
		case 62:
			semanticStack->pop();
			semanticStack->pop();
			cData->addTuple(new FourTuple("-", "ENDACTUALPARAMETERS", "-", "-"));
		break;
		//   callname --> CALL var
		case 63:
			varString = semanticStack->top();
			semanticStack->pop();
			//Make sure var is type procedure
			if(globalTable.findObject(varString)) {
				tempObject = globalTable.getObject(varString);
				if(tempObject->getType() != "PROCEDURE") {
					cout<<varString<<"is not of type PROCEDURE!"<<endl;
					break;
				}
			}
			else {
				cout<<varString<<"has not been declared!"<<endl;
				break;
			}
			cData->addTuple(new FourTuple(varString, "PROCCALL", "-", "-"));
		break;
		//   aparmlist --> aparmlist-
		case 64:
			//Lambda Production
			//|RHS| == |LHS|, Do Nothing
		break;
		//   aparmlist- --> aparmlist- , calltype var
		case 65:
			varString = semanticStack->top();
			semanticStack->pop();  //Pop var
			callString = semanticStack->top();
			semanticStack->pop();  //Pop calltype
			semanticStack->pop();  //Pop ,
			checkExistsAndShape(varString, "SCALAR");
			if(callString == "ref")
				cData->addTuple(new FourTuple("-", "ACTUALREFERENCEPARAMETER", varString, "1"));				
			else if(callString == "val")
				cData->addTuple(new FourTuple("-", "ACTUALVALUEPARAMETER", varString, "1"));						
			else {
				cout<<"Something has gone terribly, terribly wrong."<<endl;								
				cout<<"CallString = "<<callString<<"\tVarString = "<<varString<<endl;
			}			
		break;
		//   aparmlist- --> aparmlist- , calltype var [ astat ]
		case 66:
			semanticStack->pop();  //Pop ]
			rowString = semanticStack->top();
			semanticStack->pop();  //Pop astat
			semanticStack->pop();  //Pop [
			varString = semanticStack->top();
			semanticStack->pop();  //Pop var
			callString = semanticStack->top();
			semanticStack->pop();  //Pop calltype
			semanticStack->pop();  //Pop ,
			checkExistsAndShape(varString, "VECTOR");
			checkIndex(rowString);					
			if(callString == "ref")
				cData->addTuple(new FourTuple("-", "ACTUALREFERENCEPARAMETER", varString, rowString));				
			else if(callString == "val")
				cData->addTuple(new FourTuple("-", "ACTUALVALUEPARAMETER", varString, rowString));						
			else
				cout<<"Something has gone terribly, terribly wrong."<<endl;
		break;
		//   aparmlist- --> aparmlist- , calltype var [ astat : astat ]
		case 67:
			semanticStack->pop();  //Pop ]
			colString = semanticStack->top();  
			semanticStack->pop();  //Pop astat
			semanticStack->pop();  //Pop :
			rowString = semanticStack->top();
			semanticStack->pop();  //Pop astat
			semanticStack->pop();  //Pop [
			varString = semanticStack->top();
			semanticStack->pop();  //Pop var
			callString = semanticStack->top();
			semanticStack->pop();  //Pop calltype
			semanticStack->pop(); //Pop Comma
			//Check Local Table for parameter
			numCols = checkExistsAndShape(varString, "MATRIX");
			checkIndex(rowString);
			checkIndex(colString);
			iv1 = getIntermediateVariableInteger();			
			iv2 = getIntermediateVariableInteger();
			cData->addTuple(new FourTuple(iv1, "IMULT", rowString, numCols));
			cData->addTuple(new FourTuple(iv2, "IADD", iv1, colString));
			if(callString == "ref")
				cData->addTuple(new FourTuple("-", "ACTUALREFERENCEPARAMETER", varString, iv2));				
			else if(callString == "val")
				cData->addTuple(new FourTuple("-", "ACTUALVALUEPARAMETER", varString, iv2));						
			else
				cout<<"Something has gone terribly, terribly wrong."<<endl;
		break;
		//   aparmlist- --> { calltype var
		case 68:
			varString = semanticStack->top();
			semanticStack->pop();
			callString = semanticStack->top();
			semanticStack->pop();
			cData->addTuple(new FourTuple("-", "BEGINACTUALPARAMETERLIST", "-", "-"));
			if(callString == "ref")
				cData->addTuple(new FourTuple("-", "ACTUALREFERENCEPARAMETER", varString, "1"));				
			else if(callString == "val")
				cData->addTuple(new FourTuple("-", "ACTUALVALUEPARAMETER", varString, "1"));						
			else {
				cout<<"Something has gone terribly, terribly wrong."<<endl;			
				cout<<"CallString = "<<callString<<"\tVarString = "<<varString<<endl;
			}
		break;
		//   aparmlist- --> { calltype var [ astat ]
		case 69:
			semanticStack->pop();  //Pop ]
			rowString = semanticStack->top();
			semanticStack->pop();  //Pop astat
			semanticStack->pop();  //Pop [
			varString = semanticStack->top();
			semanticStack->pop();  //Pop var
			callString = semanticStack->top();
			semanticStack->pop();  //Pop calltype
			checkExistsAndShape(varString, "VECTOR");
			checkIndex(rowString);
			cData->addTuple(new FourTuple("-", "BEGINACTUALPARAMETERLIST", "-", "-"));
			if(callString == "ref")
				cData->addTuple(new FourTuple("-", "ACTUALREFERENCEPARAMETER", varString, rowString));				
			else if(callString == "val")
				cData->addTuple(new FourTuple("-", "ACTUALVALUEPARAMETER", varString, rowString));						
			else
				cout<<"Something has gone terribly, terribly wrong."<<endl;
		break;
		//   aparmlist- --> { calltype var [ astat : astat ]
		case 70:
			semanticStack->pop();  //Pop ]
			colString = semanticStack->top();  
			semanticStack->pop();  //Pop astat
			semanticStack->pop();  //Pop :
			rowString = semanticStack->top();
			semanticStack->pop();  //Pop astat
			semanticStack->pop();  //Pop [
			varString = semanticStack->top();
			semanticStack->pop();  //Pop var
			callString = semanticStack->top();
			semanticStack->pop();  //Pop calltype
			//Check Local Table for parameter
			numCols = checkExistsAndShape(varString, "MATRIX");
			checkIndex(rowString);
			checkIndex(colString);	
			iv1 = getIntermediateVariableInteger();
			iv2 = getIntermediateVariableInteger();			
			cData->addTuple(new FourTuple("-", "BEGINACTUALPARAMETERLIST", "-", "-"));		
			cData->addTuple(new FourTuple(iv1, "IMULT", rowString, numCols));
			cData->addTuple(new FourTuple(iv2, "IADD", iv1, colString));
			if(callString == "ref")
				cData->addTuple(new FourTuple("-", "ACTUALREFERENCEPARAMETER", varString, iv2));				
			else if(callString == "val")
				cData->addTuple(new FourTuple("-", "ACTUALVALUEPARAMETER", varString, iv2));						
			else
				cout<<"Something has gone terribly, terribly wrong."<<endl;
		break;
		
		/***************************************************************************************/
		/*			BEGIN CONTROL FLOW STATEMENTS PRODUCTS 71 - 77							   */
		/***************************************************************************************/
		
		//   ifstat --> ifhead statlist END
		case 71:
			semanticStack->pop();  //Pop END
			semanticStack->pop();  //Pop statlist
			iv1 = labelStack->top();
			labelStack->pop();
			cData->addTuple(new FourTuple(iv1, "LABEL", "-", "-"));			
		break;
		//   ifstat --> ifthen statlist END
		case 72:
			semanticStack->pop();  //Pop END
			semanticStack->pop();  //Pop statlist
			iv1 = labelStack->top();
			labelStack->pop();			
			cData->addTuple(new FourTuple(iv1, "LABEL", "-", "-"));
		break;
		//   ifthen --> ifhead statlist ELSE
		case 73:
			semanticStack->pop();  //Pop ELSE
			semanticStack->pop();  //Pop statlist
			iv1 = labelStack->top();
			labelStack->pop();			
			iv2 = getIntermediateVariableLabel();
			cData->addTuple(new FourTuple(iv2, "JUMP", "-", "-"));
			cData->addTuple(new FourTuple(iv1, "LABEL", "-", "-"));
			labelStack->push(iv2);
		break;
		//   ifhead --> IF ( bexpr ) THEN
		case 74:
			semanticStack->pop();  //Pop THEN
			semanticStack->pop();  //Pop )
			varString = semanticStack->top();
			semanticStack->pop();  //Pop bexpr
			semanticStack->pop();  //Pop (
			typeString1 = determineType(varString);
			if(typeString1 != "BOOL") {
				cout<<"ERROR: "<<varString<<" is not of type BOOL!"<<endl;
				break;
			}
			iv1 = getIntermediateVariableLabel();
			cData->addTuple(new FourTuple(iv1, "CJUMPF", varString, "-"));
			labelStack->push(iv1);
		break;
		//   whilestat --> whilecond statlist END
		case 75:
			semanticStack->pop();  //Pop END
			semanticStack->pop();  //Pop Statlist
			iv2 = labelStack->top();
			labelStack->pop();			
			iv1 = labelStack->top();
			labelStack->pop();			
			cData->addTuple(new FourTuple(iv1, "JUMP", "-", "-"));
			cData->addTuple(new FourTuple(iv2, "LABEL", "-", "-"));
		break;
		//   whilecond --> whilehead ( bexpr ) DO
		case 76:
			semanticStack->pop();  //Pop DO
			semanticStack->pop();  //Pop )
			varString = semanticStack->top();
			semanticStack->pop();  //Pop bexpr
			semanticStack->pop();  //Pop (
			typeString1 = determineType(varString);
			if(typeString1 != "BOOL") {
				cout<<"ERROR: "<<varString<<" is not of type BOOL!"<<endl;
				break;
			}
			iv2 = labelStack->top();
			cData->addTuple(new FourTuple(iv2, "CJUMPF", varString, "-"));
		break;
		//   whilehead --> WHILE
		case 77:
			iv1 = getIntermediateVariableLabel();
			iv2 = getIntermediateVariableLabel();
			cData->addTuple(new FourTuple(iv1, "LABEL", "-", "-"));			
			labelStack->push(iv1);	
			labelStack->push(iv2);		
		break;
		
		/***************************************************************************************/
		/*			END CONTROL FLOW STATEMENTS PRODUCTS 71 - 77							   */
		/***************************************************************************************/
		
		//   assignstat --> astat ;
		case 78:
			//Lambda Production
			semanticStack->pop();
		break;
		
		/***************************************************************************************/
		/*			ASSIGNMENT STATEMENTS PRODUCTS 79 - 81							   		   */
		/***************************************************************************************/
		
		//   astat --> var <- aexpr
		case 79:
			rhsString = semanticStack->top();
			semanticStack->pop();  //Pop aexpr
			semanticStack->pop();  //Pop <-
			varString = semanticStack->top();
			semanticStack->pop();  //Pop var
			checkExistsAndShape(varString, "SCALAR");	
			cData->addTuple(new FourTuple(varString, "SUBSTORE", rhsString, "-"));
			semanticStack->push(rhsString);
		break;
		//   astat --> var [ astat ] <- aexpr
		case 80:
			rhsString = semanticStack->top();
			semanticStack->pop();  //Pop aexpr
			semanticStack->pop();  //Pop <-
			semanticStack->pop();  //Pop ]
			rowString = semanticStack->top();
			semanticStack->pop();  //Pop astat
			semanticStack->pop();  //Pop [
			varString = semanticStack->top();
			semanticStack->pop();  //Pop var
			checkExistsAndShape(varString, "VECTOR");
			checkIndex(rowString);
			cData->addTuple(new FourTuple(varString, "SUBSTORE", rhsString, rowString));			
			semanticStack->push(rhsString);
		break;
		//   astat --> var [ astat : astat ] <- aexpr
		case 81:
			rhsString = semanticStack->top();
			semanticStack->pop();  //Pop aexpr
			semanticStack->pop();  //Pop <-
			semanticStack->pop();  //Pop ]
			colString = semanticStack->top();
			semanticStack->pop();  //Pop astat
			semanticStack->pop();  //Pop :
			rowString = semanticStack->top();
			semanticStack->pop();  //Pop astat
			semanticStack->pop();  //Pop [
			varString = semanticStack->top();
			semanticStack->pop();  //Pop var
			numCols = checkExistsAndShape(varString, "MATRIX");

			checkIndex(rowString);
			checkIndex(colString);			
						
			iv1 = getIntermediateVariableInteger();
			iv2 = getIntermediateVariableInteger();
			cData->addTuple(new FourTuple(iv1, "IMULT", rowString, numCols));
			cData->addTuple(new FourTuple(iv2, "IADD", iv1, colString));
			cData->addTuple(new FourTuple(varString, "SUBSTORE", rhsString, iv2));
			semanticStack->push(rhsString);
		break;
		
		/***************************************************************************************/
		/*			BOOLEAN EXPRESSION PRODUCTS 71 - 77							   			   */
		/***************************************************************************************/		
		
		//   astat --> bexpr
		case 82:
			//Lambda Production
			//|RHS| == |LHS|, Do Nothing
		break;
		//   bexpr --> bexpr-
		case 83:
			//Lambda Production
			//|RHS| == |LHS|, Do Nothing		
		break;
		//   bexpr- --> bexpr- || andexpr
		case 84:
			varString = semanticStack->top();
			semanticStack->pop();  //Pop notexpr
			semanticStack->pop();  //Pop &&
			rhsString = semanticStack->top();
			semanticStack->pop();  //Pop andexpr-
			typeString1 = determineType(varString);
			if(typeString1 != "BOOL") {
				cout<<"ERROR: "<<varString<<" is not of type BOOL!"<<endl;
				break;
			}
			typeString2 = determineType(rhsString);
			if(typeString2 != "BOOL") {
				cout<<"ERROR: "<<varString<<" is not of type BOOL!"<<endl;
				break;
			}			
			bv1 = getIntermediateVariableBool();
			cData->addTuple(new FourTuple(bv1, "OR", rhsString, varString));
			semanticStack->push(bv1);
		break;
		//   bexpr- --> andexpr
		case 85:
			//Lambda Production
			//|RHS| == |LHS|, Do Nothing		
		break;
		//   andexpr --> andexpr-
		case 86:
			//Lambda Production
			//|RHS| == |LHS|, Do Nothing				
		break;
		//   andexpr- --> andexpr- && notexpr
		case 87:
			varString = semanticStack->top();
			semanticStack->pop();  //Pop notexpr
			semanticStack->pop();  //Pop &&
			rhsString = semanticStack->top();
			semanticStack->pop();  //Pop andexpr-
			typeString1 = determineType(varString);
			if(typeString1 != "BOOL") {
				cout<<"ERROR: "<<varString<<" is not of type BOOL!"<<endl;
				break;
			}
			typeString2 = determineType(rhsString);
			if(typeString2 != "BOOL") {
				cout<<"ERROR: "<<varString<<" is not of type BOOL!"<<endl;
				break;
			}			
			bv1 = getIntermediateVariableBool();
			cData->addTuple(new FourTuple(bv1, "AND", rhsString, varString));
			semanticStack->push(bv1);
		break;
		//   andexpr- --> notexpr
		case 88:
			//Lambda Productionkcatsass
			//|RHS| == |LHS|, Do Nothing				
		break;
		//   notexpr --> notexpr-
		case 89:
			//Lambda Production
			//|RHS| == |LHS|, Do Nothing				
		break;
		//   notexpr- --> ! relexpr
		case 90:
			varString = semanticStack->top();
			semanticStack->pop();  //Pop relexpr
			semanticStack->pop();  //Pop !
			bv1 = getIntermediateVariableBool();
			typeString1 = determineType(varString);
			if(typeString1 != "BOOL") {
				cout<<"ERROR: "<<varString<<" is not of type BOOL!"<<endl;
				break;
			}
			cData->addTuple(new FourTuple(bv1, "NOT", varString, "-"));
			semanticStack->push(bv1);			
		break;
		//   notexpr- --> relexpr
		case 91:
			//Lambda Production
			//|RHS| == |LHS|, Do Nothing				
		break;
		//   relexpr --> relexpr-
		case 92:
			//Lambda Production
			//|RHS| == |LHS|, Do Nothing		
		break;
		//   relexpr- --> aexpr < aexpr
		case 93:
			rhsString = semanticStack->top();
			semanticStack->pop();  //Pop aexpr
			semanticStack->pop();  //Pop <
			varString = semanticStack->top();
			semanticStack->pop();  //Pop aexpr
			bv1 = getIntermediateVariableBool();
			typeString1 = determineType(rhsString);
			typeString2 = determineType(varString);
			if(typeString1 == typeString2) {
				if(typeString1 == "int")
					cData->addTuple(new FourTuple(bv1, "ILESS", varString, rhsString));
				else if(typeString1 == "real")
					cData->addTuple(new FourTuple(bv1, "RLESS", varString, rhsString));
				else {
					cout<<"Something has gone terribly, terribly wrong."<<endl;
					cout<<"TypeString1 = "<<typeString1<<"\tTypeString2 = "<<typeString2<<endl;
				}
			}
			else {
				if(typeString1 == "real") {
					if(typeString2 == "int") {
						iv1 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv1, "CONVERTITOR", rhsString, "-"));
						cData->addTuple(new FourTuple(bv1, "RLESS", varString, iv1));
					}
					else
						cout<<"Something has gone terribly, terribly wrong."<<endl;					
				}
				else if(typeString2 == "real") {
					if(typeString1 == "int") {
						iv1 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv1, "CONVERTITOR", varString, "-"));
						cData->addTuple(new FourTuple(bv1, "RLESS", iv1, rhsString));
					}
				}
			}			
			semanticStack->push(bv1);						
		break;
		//   relexpr- --> aexpr <= aexpr
		case 94:
			rhsString = semanticStack->top();
			semanticStack->pop();  //Pop aexpr
			semanticStack->pop();  //Pop <=
			varString = semanticStack->top();
			semanticStack->pop();  //Pop aexpr
			bv1 = getIntermediateVariableBool();
		   typeString1 = determineType(varString);
			typeString2 = determineType(rhsString);
			if(typeString1 == typeString2) {
				if(typeString1 == "int")
					cData->addTuple(new FourTuple(bv1, "ITLEQUAL", varString, rhsString));
				else if(typeString1 == "real")
					cData->addTuple(new FourTuple(bv1, "RTLEQUAL", varString, rhsString));
				else if(typeString1 == "BOOLEAN")
					cData->addTuple(new FourTuple(bv1, "BTLEQUAL", varString, rhsString));					
				else {
					cout<<"Something has gone terribly, terribly wrong."<<endl;
						cout<<"TypeString1 = "<<typeString1<<"\tTypeString2 = "<<typeString2<<endl;					
				}
			}
			else {
			    cout<<"OH NOES TYPES ARE DIFFERENT"<<endl;
				if(typeString1 == "real") {
					if(typeString2 == "int") {
						iv1 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv1, "CONVERTITOR", rhsString, "-"));
						cData->addTuple(new FourTuple(bv1, "RTLEQUAL", varString, iv1));
					}
					else {
						cout<<"Something has gone terribly, terribly wrong."<<endl;					
						cout<<"TypeString1 = "<<typeString1<<"\tTypeString2 = "<<typeString2<<endl;
				    }
				}
				else if(typeString2 == "real") {
					if(typeString1 == "int") {
						iv1 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv1, "CONVERTITOR", varString, "-"));
						cData->addTuple(new FourTuple(bv1, "RTLEQUAL", iv1, rhsString));
					}
				}
				else {
				    cout<<"Something has gone terribly, terribly wrong."<<endl;
						cout<<"TypeString1 = "<<typeString1<<"\tTypeString2 = "<<typeString2<<endl;				    
			    }
			}			
			semanticStack->push(bv1);						
		break;
		//   relexpr- --> aexpr > aexpr
		case 95:
			rhsString = semanticStack->top();
			semanticStack->pop();  //Pop aexpr
			semanticStack->pop();  //Pop >
			varString = semanticStack->top();
			semanticStack->pop();  //Pop aexpr
			typeString1 = determineType(rhsString);
			typeString2 = determineType(varString);			
			bv1 = getIntermediateVariableBool();
			if(typeString1 == typeString2) {
				if(typeString1 == "int")
					cData->addTuple(new FourTuple(bv1, "IGREATER", varString, rhsString));
				else if(typeString1 == "real")
					cData->addTuple(new FourTuple(bv1, "RGREATER", varString, rhsString));
				else if(typeString1 == "BOOLEAN")
					cData->addTuple(new FourTuple(bv1, "BGREATER", varString, rhsString));					
				else
					cout<<"Something has gone terribly, terribly wrong."<<endl;
			}
			else {
				if(typeString1 == "real") {
					if(typeString2 == "int") {
						iv1 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv1, "CONVERTITOR", rhsString, "-"));
						cData->addTuple(new FourTuple(bv1, "RGREATER", varString, iv1));
					}
					else
						cout<<"Something has gone terribly, terribly wrong."<<endl;					
				}
				else if(typeString2 == "real") {
					if(typeString1 == "int") {
						iv1 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv1, "CONVERTITOR", varString, "-"));
						cData->addTuple(new FourTuple(bv1, "RGREATER", iv1, rhsString));
					}
				}
			}			
			semanticStack->push(bv1);						
		break;
		//   relexpr- --> aexpr >= aexpr
		case 96:
			rhsString = semanticStack->top();
			semanticStack->pop();  //Pop aexpr
			semanticStack->pop();  //Pop >=
			varString = semanticStack->top();
			semanticStack->pop();  //Pop aexpr
			typeString1 = determineType(rhsString);
			typeString2 = determineType(varString);			
			bv1 = getIntermediateVariableBool();
			if(typeString1 == typeString2) {
				if(typeString1 == "int")
					cData->addTuple(new FourTuple(bv1, "IGTEQUAL", varString, rhsString));
				else if(typeString1 == "real")
					cData->addTuple(new FourTuple(bv1, "RGTEQUAL", varString, rhsString));
				else if(typeString1 == "BOOLEAN")
					cData->addTuple(new FourTuple(bv1, "BGTEQUAL", varString, rhsString));					
				else
					cout<<"Something has gone terribly, terribly wrong."<<endl;
			}
			else {
				if(typeString1 == "real") {
					if(typeString2 == "int") {
						iv1 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv1, "CONVERTITOR", rhsString, "-"));
						cData->addTuple(new FourTuple(bv1, "RGTEQUAL", varString, iv1));
					}
					else
						cout<<"Something has gone terribly, terribly wrong."<<endl;					
				}
				else if(typeString2 == "real") {
					if(typeString1 == "int") {
						iv1 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv1, "CONVERTITOR", varString, "-"));
						cData->addTuple(new FourTuple(bv1, "RGTEQUAL", iv1, rhsString));
					}
				}
			}			
			semanticStack->push(bv1);						
		break;
		//   relexpr- --> aexpr == aexpr
		case 97:
			rhsString = semanticStack->top();
			semanticStack->pop();  //Pop aexpr
			semanticStack->pop();  //Pop ==
			varString = semanticStack->top();
			semanticStack->pop();  //Pop aexpr
			bv1 = getIntermediateVariableBool();
			typeString1 = determineType(varString);
			typeString2 = determineType(rhsString);
			if(typeString1 == typeString2) {
				if(typeString1 == "int")
					cData->addTuple(new FourTuple(bv1, "IEQUAL", varString, rhsString));
				else if(typeString1 == "real")
					cData->addTuple(new FourTuple(bv1, "REQUAL", varString, rhsString));
				else if(typeString1 == "BOOLEAN")
					cData->addTuple(new FourTuple(bv1, "BEQUAL", varString, rhsString));					
				else
					cout<<"Something has gone terribly, terribly wrong."<<endl;
			}
			else {
				if(typeString1 == "real") {
					if(typeString2 == "int") {
						iv1 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv1, "CONVERTITOR", rhsString, "-"));
						cData->addTuple(new FourTuple(bv1, "REQUAL", varString, iv1));
					}
					else
						cout<<"Something has gone terribly, terribly wrong."<<endl;					
				}
				else if(typeString2 == "real") {
					if(typeString1 == "int") {
						iv1 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv1, "CONVERTITOR", varString, "-"));
						cData->addTuple(new FourTuple(bv1, "REQUAL", iv1, rhsString));
					}
				}
			}
			semanticStack->push(bv1);						
		break;
		//   relexpr- --> aexpr != aexpr
		case 98:
			rhsString = semanticStack->top();
			semanticStack->pop();  //Pop aexpr
			semanticStack->pop();  //Pop !=
			varString = semanticStack->top();
			semanticStack->pop();  //Pop aexpr
			bv1 = getIntermediateVariableBool();
			typeString1 = determineType(varString);
			typeString2 = determineType(rhsString);
			if(typeString1 == typeString2) {
				if(typeString1 == "int")
					cData->addTuple(new FourTuple(bv1, "INOTEQUAL", varString, rhsString));
				else if(typeString1 == "real")
					cData->addTuple(new FourTuple(bv1, "RNOTEQUAL", varString, rhsString));
				else if(typeString1 == "BOOLEAN")
					cData->addTuple(new FourTuple(bv1, "BNOTEQUAL", varString, rhsString));					
				else
					cout<<"Something has gone terribly, terribly wrong."<<endl;
			}
			else {
				if(typeString1 == "real") {
					if(typeString2 == "int") {
						iv1 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv1, "CONVERTITOR", rhsString, "-"));
						cData->addTuple(new FourTuple(bv1, "RNOTEQUAL", varString, iv1));
					}
					else
						cout<<"Something has gone terribly, terribly wrong."<<endl;					
				}
				else if(typeString2 == "real") {
					if(typeString1 == "int") {
						iv1 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv1, "CONVERTITOR", varString, "-"));
						cData->addTuple(new FourTuple(bv1, "RNOTEQUAL", iv1, rhsString));
					}
				}
			}
			semanticStack->push(bv1);						
		break;
			//Lambda Production
			//|RHS| == |LHS|, Do Nothing
		case 99:
			break;
			//Lambda Production
			//|RHS| == |LHS|, Do Nothing
		case 100:
			break;
		//   aexpr- --> aexpr- + term
		case 101:
		    rhsString = semanticStack->top();
			semanticStack->pop();  //Pop term
			semanticStack->pop();  //Pop +
			varString = semanticStack->top();
			semanticStack->pop();  //Pop aexpr-
			typeString1 = determineType(varString);
			typeString2 = determineType(rhsString);
			
			if(typeString1 == typeString2) {
				if(typeString1 == "int") {
				    iv1 = getIntermediateVariableInteger();
					cData->addTuple(new FourTuple(iv1, "IADD", varString, rhsString));
        			semanticStack->push(iv1);					
				}
				else if(typeString1 == "real") {
				    iv1 = getIntermediateVariableInteger();
					cData->addTuple(new FourTuple(iv1, "RADD", varString, rhsString));
        			semanticStack->push(iv1);					
				}
				else
					cout<<"Incorrect Type Error"<<endl;
			}
			else {
				if(typeString1 == "real") {
					if(typeString2 == "int") {
						iv1 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv1, "CONVERTITOR", rhsString, "-"));
						iv2 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv2, "RADD", varString, iv1));
            			semanticStack->push(iv2);						
					}
				}
				else if(typeString2 == "real") {
					if(typeString1 == "int") {
						iv1 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv1, "CONVERTITOR", varString, "-"));
						iv2 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv2, "RADD", iv1, rhsString));			
			            semanticStack->push(iv2);						
					}
				}
			}
		break;
		//   aexpr- --> aexpr- - term
		case 102:
		    rhsString = semanticStack->top();
			semanticStack->pop();  //Pop term
			semanticStack->pop();  //Pop -
			varString = semanticStack->top();
			semanticStack->pop();  //Pop aexpr-
			typeString1 = determineType(varString);
			typeString2 = determineType(rhsString);
			
			if(typeString1 == typeString2) {
				if(typeString1 == "int") {
				    iv1 = getIntermediateVariableInteger();
					cData->addTuple(new FourTuple(iv1, "ISUB", varString, rhsString));
        			semanticStack->push(iv1);					
				}
				else if(typeString1 == "real") {
				    iv1 = getIntermediateVariableInteger();
					cData->addTuple(new FourTuple(iv1, "RSUB", varString, rhsString));
        			semanticStack->push(iv1);					
				}
				else
					cout<<"Incorrect Type Error"<<endl;
			}
			else {
				if(typeString1 == "real") {
					if(typeString2 == "int") {
						iv1 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv1, "CONVERTITOR", rhsString, "-"));
						iv2 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv2, "RSUB", varString, iv1));
            			semanticStack->push(iv2);						
					}
				}
				else if(typeString2 == "real") {
					if(typeString1 == "int") {
						iv1 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv1, "CONVERTITOR", varString, "-"));
						iv2 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv2, "RSUB", iv1, rhsString));			
            			semanticStack->push(iv2);						
			        }
			    }
			}
		break;
		//   aexpr- --> - term
		case 103:
		   varString = semanticStack->top();
			semanticStack->pop();  //Pop term
			semanticStack->pop();  //Pop -
			typeString1 = determineType(varString);
			if(typeString1 == "int") {
			    iv1 = getIntermediateVariableInteger();
			    cData->addTuple(new FourTuple(iv1, "IMULT", varString, "-1"));
			}
			else if(typeString1 == "real") {
			    iv1 = getIntermediateVariableReal();
			    cData->addTuple(new FourTuple(iv1, "RMULT", varString, "-1.0"));
			}
			else {
			   cout<<"Negation Type Error"<<endl;
			   break;
		    }
		    semanticStack->push(iv1);
		break;
		//   aexpr- --> term
		case 104:
			break;
		//   term --> term-
		case 105:
		break;
		//   term- --> term- * primary
		case 106:
		    rhsString = semanticStack->top();
			semanticStack->pop();  //Pop Primary
			semanticStack->pop();  //Pop *
			varString = semanticStack->top();
			semanticStack->pop();  //Pop term-
			typeString1 = determineType(varString);
			typeString2 = determineType(rhsString);
			if(typeString1 == typeString2) {
				if(typeString1 == "int") {
				    iv1 = getIntermediateVariableInteger();
					cData->addTuple(new FourTuple(iv1, "IMULT", varString, rhsString));
            	semanticStack->push(iv1);														
				}
				else if(typeString1 == "real") {
				    iv1 = getIntermediateVariableReal();
					cData->addTuple(new FourTuple(iv1, "RMULT", varString, rhsString));
            	semanticStack->push(iv1);														
				}
				else
					cout<<"Incorrect Type Error"<<endl;
			}
			else {
				if(typeString1 == "real") {
					if(typeString2 == "int") {
						iv1 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv1, "CONVERTITOR", rhsString, "-"));
						iv2 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv2, "RMULT", varString, iv1));
            		semanticStack->push(iv2);									
					}
				}
				else if(typeString2 == "real") {
					if(typeString1 == "int") {
						iv1 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv1, "CONVERTITOR", varString, "-"));
						iv2 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv2, "RMULT", iv1, rhsString));
            		semanticStack->push(iv2);									
					}
			    }
			}
		break;
		//   term- --> term- / primary
		case 107:
			rhsString = semanticStack->top();
			semanticStack->pop();  //Pop primary
			semanticStack->pop();  //Pop /
			varString = semanticStack->top();
			semanticStack->pop();  //Pop term-			
			typeString1 = determineType(varString);
			typeString2 = determineType(rhsString);
			
			if(typeString1 == typeString2) {
				if(typeString1 == "int") {
				    iv1 = getIntermediateVariableInteger();
					cData->addTuple(new FourTuple(iv1, "IDIV", varString, rhsString));
        			semanticStack->push(iv1);							
				}
				else if(typeString1 == "real") {
				    iv1 = getIntermediateVariableInteger();
					cData->addTuple(new FourTuple(iv1, "RDIV", varString, rhsString));
			        semanticStack->push(iv1);							
				}
				else
					cout<<"Incorrect Type Error"<<endl;
			}
			else {
				if(typeString1 == "real") {
					if(typeString2 == "int") {
						iv1 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv1, "CONVERTITOR", rhsString, "-"));
						iv2 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv2, "RDIV", varString, iv1));
            			semanticStack->push(iv2);								
					}
				}
				else if(typeString2 == "real") {
					if(typeString1 == "int") {
						iv1 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv1, "CONVERTITOR", varString, "-"));
						iv2 = getIntermediateVariableReal();
						cData->addTuple(new FourTuple(iv2, "RDIV", iv1, rhsString));
            			semanticStack->push(iv2);								
					}
		        }
		    }    
		break;
		//   term- --> primary
		case 108:
			break;
		//   primary --> primary-
		case 109:
			break;
		//   primary- --> ( bexpr )
		case 110:
			semanticStack->pop();  //Pop )
			varString = semanticStack->top();
			semanticStack->pop();  //Pop bexpr
			semanticStack->pop();  //Pop (
			semanticStack->push(varString);
		break;
		//   primary- --> var [ astat : astat ]
		case 111:
			semanticStack->pop();  //Pop ]
			colString = semanticStack->top();
			semanticStack->pop();  //Pop astat
			semanticStack->pop();  //Pop :
			rowString = semanticStack->top();
			semanticStack->pop();  //Pop astat
			semanticStack->pop();  //Pop [
			varString = semanticStack->top();
			semanticStack->pop();  //Pop var
			numCols = checkExistsAndShape(varString, "MATRIX");
			checkIndex(rowString);			
			checkIndex(colString);
			
			iv1 = getIntermediateVariableInteger();
			iv2 = getIntermediateVariableInteger();
			cData->addTuple(new FourTuple(iv1, "IMULT", rowString, numCols));
			cData->addTuple(new FourTuple(iv2, "IADD", iv1, colString));
			typeString1 = determineType(varString);
			if(typeString1 == "int") {
				ivLoad = getIntermediateVariableInteger();
				cData->addTuple(new FourTuple(ivLoad, "ISUBLOAD", varString, iv2));
			}
			else if(typeString1 == "real") {
				ivLoad = getIntermediateVariableReal();
				cData->addTuple(new FourTuple(ivLoad, "RSUBLOAD", varString, iv2));			
			}
			semanticStack->push(ivLoad);
		break;
		//   primary- --> var [ astat ]
		case 112:
			semanticStack->pop();  //Pop ]
			rowString = semanticStack->top();
			semanticStack->pop();  //Pop astat
			semanticStack->pop();  //Pop [
			varString = semanticStack->top();
			semanticStack->pop();  //Pop var
			checkExistsAndShape(varString, "VECTOR");
			checkIndex(rowString);				
			typeString1 = determineType(varString);
			if(typeString1 == "int") {
				ivLoad = getIntermediateVariableInteger();
				cData->addTuple(new FourTuple(ivLoad, "ISUBLOAD", varString, rowString));
			}
			else if(typeString1 == "real") {
				ivLoad = getIntermediateVariableReal();
				cData->addTuple(new FourTuple(ivLoad, "RSUBLOAD", varString, rowString));			
			}
			semanticStack->push(ivLoad);
		break;
		//   primary- --> var
		case 113:
			varString = semanticStack->top();
			checkExistsAndShape(varString, "SCALAR");
		break;
		//   primary- --> constant
		case 114:
		//Already pushed whatever on the stack
		break;
		//   constant --> integer
		case 115:
		//Already pushed whatever on the stack
		break;
		//   constant --> real
		case 116:
		//Already pushed whatever on the stack
		break;

		default:
			cout<<"Production Number "<<productionNumber<<"is not yet implemented"<<endl;
			break;
	}
	
	return;
}

string SemanticsAnalyzer::determineType(string& s) {
	size_t found;
	//Check in Symbol table
	if(localFlag) {
		if(localTable.findObject(s))
			return (localTable.getObject(s)->getType());
		else if(globalTable.findObject(s))
			return (globalTable.getObject(s)->getType());
	}
	else {
		if(globalTable.findObject(s))
			return (globalTable.getObject(s)->getType());
	}
	//Not found in symbol tables
	
	//Check for intermediate variable
	found = s.find('$');
	if(found != string::npos) {
		if(s[0] == 'I')
			return "int";
		else if(s[0] == 'R')
			return "real";
		else if(s[0] == 'B')
			return "BOOL";
		else
			cout<<"Invalid intermediate variable "<<s<<endl;
			return "";
	}
	//Not an intermediate variable
	found = s.find('.');
	if(found != string::npos)
		return "real";
	//Return integer by default / process of elmination
	return "int";
}
	
string SemanticsAnalyzer::getIntermediateVariableReal() {	
	string iv = "R$";
	iv += ivGetNumber(intermediateReal);
	intermediateReal++;
	return iv;
}

string SemanticsAnalyzer::getIntermediateVariableInteger() {
	string iv = "I$";
	iv += ivGetNumber(intermediateInteger);
	intermediateInteger++;
	return iv;
}

string SemanticsAnalyzer::getIntermediateVariableBool() {
	string iv = "B$";
	iv += ivGetNumber(intermediateBool);
	intermediateBool++;
	return iv;
}

string SemanticsAnalyzer::getIntermediateVariableLabel() {
	string iv = "L$";
	iv += ivGetNumber(intermediateLabel);
	intermediateLabel++;
	return iv;
}

string SemanticsAnalyzer::ivGetNumber(int number) {
    stringstream s;    
    string temp;
    s << number;
    s >> temp;
    return temp;
}

void SemanticsAnalyzer::printLocalSymbolTable() {
   cout<<"-------------------LOCAL SYMBOL TABLE-----------------------------"<<endl;

   localTable.printLocalTable();
   cout<<"------------------------------------------------------------------"<<endl;
}																					 

void SemanticsAnalyzer::printGlobalSymbolTable() {
   cout<<"-------------------GLOBAL SYMBOL TABLE----------------"<<endl;
   globalTable.print();
   cout<<"------------------------------------------------------"<<endl;
}

void SemanticsAnalyzer::checkIndex(string indexString) {
	string typeString = determineType(indexString);
	if(typeString != "int")
		cout<<"ERROR: Array Index "<<indexString<<" is not of type int!"<<endl;
}

string SemanticsAnalyzer::checkExistsAndShape(string varString, string shapeString) {
	SemanticObject *tempObject;
	string numCols;
	if(localFlag) {
		if(!(localTable.findObject(varString))) {
			if(!(globalTable.findObject(varString))) {
				cout<<"ERROR: "<<varString<<" does not exist in global or local symbol table!"<<endl;
			}
			else {
				tempObject = globalTable.getObject(varString);
				if(tempObject->getShape() != shapeString) {
					cout<<"ERROR: "<<varString<<", in the global table, is not a "<<shapeString<<"!"<<endl;
				}
			}				
		}	
		else {
			tempObject = localTable.getObject(varString);
			if(tempObject->getShape() != shapeString) {
				cout<<"ERROR: "<<varString<<", in the local table, is not a "<<shapeString<<"!"<<endl;
			}
		}
	}
	else {
		if(!(globalTable.findObject(varString))) {
			cout<<"ERROR: "<<varString<<" does not exist in global symbol table!"<<endl;
		}
		else {
			tempObject = globalTable.getObject(varString);
			if(tempObject->getShape() != shapeString) {
				cout<<"ERROR: "<<varString<<" is not a "<<shapeString<<"!"<<endl;
			}
		}				
	}
	
	if(shapeString == "MATRIX") {
		if(tempObject != NULL)
			numCols = tempObject->getSize2();
		else
			numCols = "-999";
	}
	else
		numCols = "-999";
	return numCols;
}
