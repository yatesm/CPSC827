#include "assemblyGenerator.h"
#include <iostream>

using std::cout;
using std::endl;

#define MAX_REGISTERS 6

void AssemblyGenerator::generateAssembly() {
	FILE *filePtr;
	bool printFlag = false;
	int registerLocation, registerLocation2;
	int op3RegisterLocation, op4RegisterLocation, op1RegisterLocation;
	int arraySize, procStatus = 0, procFlag = 0, nfpFlag = 0, refFlag = 0;
	string typeString, typeString2;
	string lastComparison = "";
	string procName, 	procString, nameString, callString;
	string outputString, parameterString;
	string op1, op2, op3, op4;
	string op1Type, op3Type, op4Type;
	string fileName = tupleList->at(0)->getOperand(1);
	string tempFileName = fileName;
	int staticFlag = 0;
	tempFileName += ".hla";

	int paramNdx, nlNdx;
	if(!(filePtr = fopen(tempFileName.c_str(), "w"))) {
		cout<<"Could not open "<<tempFileName<<".  Please ensure you have write access"<<endl;
		return;
	}

	string currentOp;
	for(unsigned int i = 0; i < tupleList->size(); ++i) {
		op1 = tupleList->at(i)->getOperand(1);
		op2 = tupleList->at(i)->getOperand(2);
		op3 = tupleList->at(i)->getOperand(3);
		op4 = tupleList->at(i)->getOperand(4);
		fprintf(filePtr, "\n//(%s, %s, %s, %s)\n", op1.c_str(), op2.c_str(), op3.c_str(), op4.c_str());
		//Do register simulation
		if(printFlag)
			simulation.printToFile(filePtr);
				
		currentOp = op2;
		if(procFlag == 1 && currentOp == "MEMORY") {
			fprintf(filePtr, "var\n");
			procFlag = 0;
		}
		if(procStatus == 1 && currentOp != "MEMORY") {
			fprintf(filePtr, "begin %s;\n", procName.c_str());
			procStatus = 0;
		}
		/*************************************************************************
		**										PROGRAMBEGIN										**
						FUCKING DONE								FUCKING DONE		
		*************************************************************************/				
		if(currentOp == "PROGRAMBEGIN") {
			fprintf(filePtr, "program %s;\n", fileName.c_str());
			fprintf(filePtr, "#include ( \"stdlib.hhf\" )\n");
		}
		/*************************************************************************
		**										MEMORY  												**
						FUCKING DONE								FUCKING DONE		
		*************************************************************************/		
		else if(currentOp == "MEMORY") {
			if(staticFlag == 0) {
				fprintf(filePtr, "static\n");
				staticFlag = 1;
			}
			if ( op4 != "-" ) {
				arraySize = atoi(op3.c_str()) * atoi(op4.c_str());
				fprintf(filePtr, "%s: dword[%s];\n", op1.c_str(), op3.c_str());
			} 
			else if ( op3 != "1" ) 
				fprintf(filePtr, "%s: dword[%s];\n", op1.c_str(), op3.c_str());
			else 
				fprintf(filePtr, "%s: dword;\n", op1.c_str());
		}
		/*************************************************************************
		**										LABELs							**
		*************************************************************************/		
		else if(currentOp == "LABEL") {
			if(op1 == "MAIN")
		    fprintf(filePtr, "begin %s;\n", fileName.c_str());				
			for(unsigned int j = 0; j < op1.size(); ++j) {
				if( op1[j] != '$')
					fprintf(filePtr, "%c", op1[j]);
			}
			fprintf(filePtr, ":\n");
			
			simulation.clearRegisters();
		}
		/*************************************************************************
		**										SUBSTORE						**
		*************************************************************************/
		
		else if(op2 == "SUBSTORE") {
			op1Type = determineType(op1);
			op3Type = determineType(op3);
			op4Type = determineType(op4);	
			
			if(op3Type == "constant") {
				if(op4Type == "constant") {
					//OP4 == Constant implies OP1 == Vector/Matrix variable
					op4RegisterLocation = simulation.findFreeRegister(-1);
					fprintf(filePtr, "mov( %s, %s );\n", op4.c_str(), registerNames[op4RegisterLocation].c_str());
					fprintf(filePtr, "shl( 2, %s );\n", registerNames[op4RegisterLocation].c_str());
					fprintf(filePtr, "mov( %s, %s[ %s ]);\n", op3.c_str(), op1.c_str(), registerNames[op4RegisterLocation].c_str());
				}
				else if(op4Type == "variable") {
					//OP4 == Variable implies OP1 == Vector/Matrix variable			
					op4RegisterLocation = simulation.findValueInRegister(op4);
					if(op4RegisterLocation == -1) {
						op4RegisterLocation = simulation.findFreeRegister(-1);
						simulation.evictRegister(op4RegisterLocation);
						simulation.setValueInRegister(op4, op4RegisterLocation);
						fprintf(filePtr, "mov( %s, %s );\n", op4.c_str(), registerNames[op4RegisterLocation].c_str());
					}
					fprintf(filePtr, "shl( 2, %s );\n", registerNames[op4RegisterLocation].c_str());
					fprintf(filePtr, "mov( %s, %s[ %s ]);\n", op3.c_str(), op1.c_str(), registerNames[op4RegisterLocation].c_str());					
				}
				else if(op4Type == "IV") {
					//OP4 == IV implies OP1 == Vector/Matrix variable					
					op4RegisterLocation = simulation.findValueInRegister(op4);
					if(op4RegisterLocation == -1) {
						fprintf(filePtr, "OMG WE'VE GOT SPILLAGE\n");
					}
					fprintf(filePtr, "shl( 2, %s );\n", registerNames[op4RegisterLocation].c_str());
					simulation.setUsedInRegister(true, op4RegisterLocation);
					fprintf(filePtr, "mov( %s, %s[ %s ] );\n", op3.c_str(), op1.c_str(), registerNames[op4RegisterLocation].c_str());
				}
				else if(op4 == "-") {
					//OP4 == - implies OP1 == Scalar variable						
					fprintf(filePtr, "mov( %s, %s );\n", op3.c_str(), op1.c_str());
				}
				else {
					cout<<"Something has gone wrong: constant, unknown"<<endl;
				}
			}
			else if(op3Type == "variable") {
				//LOAD OP3 INTO A REGISTER
				op3RegisterLocation = simulation.findValueInRegister(op3);
				if(op3RegisterLocation == -1) {
					op3RegisterLocation = simulation.findFreeRegister(-1);
					simulation.evictRegister(op3RegisterLocation);
					simulation.setValueInRegister(op3, op3RegisterLocation);
					fprintf(filePtr, "mov( %s, %s );\n", op3.c_str(), registerNames[op3RegisterLocation].c_str());
				}
				//OP4 == Constant implies OP1 == Vector/Matrix variable		
				if(op4Type == "constant") {
					op4RegisterLocation = simulation.findFreeRegister(op3RegisterLocation);
					fprintf(filePtr, "mov( %s, %s );\n", op4.c_str(), registerNames[op4RegisterLocation].c_str());
					simulation.evictRegister(op4RegisterLocation);
					simulation.setValueInRegister(op4, op4RegisterLocation);
					fprintf(filePtr, "shl( 2, %s );\n", registerNames[op4RegisterLocation].c_str());					
					fprintf(filePtr, "mov( %s, %s[ %s ] );\n", op3.c_str(), op1.c_str(), registerNames[op4RegisterLocation].c_str());	
				}
				//OP4 == Variable implies OP1 == Vector/Matrix variable		
				else if(op4Type == "variable") {	
					op4RegisterLocation = simulation.findValueInRegister(op4);
					if(op4RegisterLocation == -1) {
						op4RegisterLocation = simulation.findFreeRegister(op3RegisterLocation);
						simulation.evictRegister(op4RegisterLocation);
						simulation.setValueInRegister(op4, op4RegisterLocation);				
						fprintf(filePtr, "mov( %s, %s );\n", op4.c_str(), registerNames[op4RegisterLocation].c_str());
					}
					fprintf(filePtr, "shl( 2, %s );\n", registerNames[op4RegisterLocation].c_str());					
					fprintf(filePtr, "mov( %s, %s [ %s ] );\n", registerNames[op3RegisterLocation].c_str(), op1.c_str(), registerNames[op4RegisterLocation].c_str());
				}
				else if(op4Type == "IV") {
					//OP4 == Variable implies OP1 == Vector/Matrix variable			
					op4RegisterLocation = simulation.findValueInRegister(op4);
					if(op4RegisterLocation == -1) {
						fprintf(filePtr, "OMFG WE SPILLED\n");				
					}
					fprintf(filePtr, "shl( 2, %s );\n", registerNames[op4RegisterLocation].c_str());		
					simulation.setUsedInRegister(true, op4RegisterLocation);								
					fprintf(filePtr, "mov( %s, %s[ %s ]);\n", registerNames[op3RegisterLocation].c_str(), op1.c_str(), registerNames[op4RegisterLocation].c_str());											
				}
				else if(op4 == "-") {
					//OP4 == Nothing implies OP1 == Scalar
					fprintf(filePtr, "mov( %s, %s );\n", registerNames[op3RegisterLocation].c_str(), op1.c_str());
				}
				else {
					cout<<"Something has gone wrong: constant, unknown"<<endl;
				}	
			}
			else if(op3Type == "IV") {	
				op3RegisterLocation = simulation.findValueInRegister(op3);
				if(op3RegisterLocation == -1) {
					fprintf(filePtr, "OMG WE HAVE SPILLAGE!\n");
				}
				if(op4Type == "constant") {
					op4RegisterLocation = simulation.findFreeRegister(op3RegisterLocation);
					fprintf(filePtr, "mov( %s, %s );\n", op4.c_str(), registerNames[op4RegisterLocation].c_str());
					simulation.evictRegister(op4RegisterLocation);
					simulation.setValueInRegister(op4, op4RegisterLocation);
					fprintf(filePtr, "shl( 2, %s );\n", registerNames[op4RegisterLocation].c_str());				
					fprintf(filePtr, "mov( %s, %s[ %s ] );\n", registerNames[op3RegisterLocation].c_str(), op1.c_str(), registerNames[op4RegisterLocation].c_str());
				}
				else if(op4Type == "variable") {
					op4RegisterLocation = simulation.findValueInRegister(op4);
					if(op4RegisterLocation == -1) {
						op4RegisterLocation = simulation.findFreeRegister(op3RegisterLocation);
						simulation.evictRegister(op4RegisterLocation);
						simulation.setValueInRegister(op4, op4RegisterLocation);
						fprintf(filePtr, "mov( %s, %s );\n", op4.c_str(), registerNames[op4RegisterLocation].c_str());
					}
					fprintf(filePtr, "shl( 2, %s );\n", registerNames[op4RegisterLocation].c_str());
					fprintf(filePtr, "mov( %s, %s[ %s ] );\n", registerNames[op3RegisterLocation].c_str(), op1.c_str(), registerNames[op4RegisterLocation].c_str());
				}
				else if(op4Type == "IV") {
					op4RegisterLocation = simulation.findValueInRegister(op4);
					if(op4RegisterLocation == -1) {
						fprintf(filePtr, "WE HAVE SPILLAGE\n");
					}
					fprintf(filePtr, "shl( 2, %s );\n", registerNames[op4RegisterLocation].c_str());
					simulation.setUsedInRegister(true, op4RegisterLocation);
					fprintf(filePtr, "mov( %s, %s[ %s ] );\n", registerNames[op3RegisterLocation].c_str(), op1.c_str(), registerNames[op4RegisterLocation].c_str());
				}
				else if(op4 == "-") {
					fprintf(filePtr, "mov( %s, %s );\n", registerNames[op3RegisterLocation].c_str(), op1.c_str());
				}
				else {
					cout<<"Something has gone wrong: constant, unknown"<<endl;
				}	
				simulation.setUsedInRegister(true, op3RegisterLocation);
			}
			else {
				cout<<"Something has gone terribly, terribly wrong"<<endl;
			}	
		}
		/*************************************************************************
		**										CALL     											**
		*************************************************************************/		
		else if(currentOp == "CALL") {
			if ( op3 == "PRINTF" ) {
				outputString = "stdout.put(\"";
			}
			else if ( op3 == "SCANF" ) {
				outputString = "stdin.geti32()";
			}
		}
		/*************************************************************************
		**			    			     BEGINOUTPUTPARAMETER									**
		*************************************************************************/		
		else if(currentOp == "BEGINOUTPUTPARAMETER") {
			// remove the " from beginning and end
			parameterString = op3.erase(1, 1);
			parameterString = parameterString.erase(parameterString.size()-1, 1);
			
			paramNdx = parameterString.find("%d");
			nlNdx = parameterString.substr(0, paramNdx).find("\\n");
			if(nlNdx != string::npos) {
			    outputString += parameterString.substr(0, nlNdx);
			    outputString += "\", nl";
			    nlNdx += 2;
			    if(nlNdx < paramNdx) {
			        outputString += ", \"";
			        outputString += parameterString.substr(nlNdx, paramNdx-nlNdx);
			        outputString += "\");";			                            			        			        
			    }
			}
			else if(nlNdx == 0) {
			    outputString += "nl, \"" + parameterString.substr(nlNdx, paramNdx-nlNdx) + "\");";
			}
			else {			
    			outputString += parameterString.substr(0, paramNdx) + "\");";

    		}
    		if(outputString == "stdout.put(\"\\n\");")
    			outputString = "stdout.put(nl);";
    		fprintf(filePtr, "%s\n", outputString.c_str());
    		paramNdx += 2;
			
		}
		/*************************************************************************
		**										OUTPUTPARAMETER            					**
		*************************************************************************/		
		else if(currentOp == "OUTPUTPARAMETER") {
			string type = determineType(op3);
			outputString = "stdout.puti32Size(";
			if ( type == "IV" ) {   
				registerLocation = simulation.findValueInRegister(op3);
				if ( registerLocation == -1 ) {
					registerLocation = simulation.findFreeRegister(-1);
					simulation.evictRegister(registerLocation); 
					simulation.setValueInRegister(op3, registerLocation);
					fprintf(filePtr, "mov ( %s, %s );\n", op3.c_str(), registerNames[registerLocation].c_str());
				}
				outputString += registerNames[registerLocation] + ", 1, '";	
				simulation.setUsedInRegister(true, registerLocation);
			}
			else {
				outputString += op3 + ", 1, '";
			}

			int spaceCount = 1;
			if ( parameterString[paramNdx] > 48 && parameterString[paramNdx] <= 57 ) 
				spaceCount = atoi(&(parameterString[paramNdx]));
			for(int p = 0; p < spaceCount; p++) {
			    outputString += " ";
			}
			outputString += "');";
			
			fprintf(filePtr, "%s\n", outputString.c_str());		
			int thisNdx = paramNdx + 1;
			if(thisNdx < parameterString.size()) {
			    paramNdx = parameterString.substr(thisNdx, parameterString.size()).find("%d");
			    if ( paramNdx != string::npos ) {
			        nlNdx = parameterString.substr(thisNdx, paramNdx-nlNdx).find("\\n");
			        if(nlNdx != string::npos) {
			            outputString = "stdout.put(\"" + parameterString.substr(thisNdx, nlNdx-thisNdx) + "\", nl";
			            nlNdx += 2;
			            if(nlNdx < paramNdx) {
			                outputString += ", \"" + parameterString.substr(nlNdx, paramNdx-nlNdx) + "\");";
			            }
				    		if(outputString == "stdout.put(\"\\n\");")
				    			outputString = "stdout.put(nl);";			            			            
    	               fprintf(filePtr, "%s\n", outputString.c_str());               
			        }
			        else {
			            outputString = "stdout.put(\"" + parameterString.substr(thisNdx, paramNdx-thisNdx) + "\", nl";
			            if(nlNdx < outputString.length())
			                outputString += ", \"" +parameterString.substr(nlNdx, outputString.size()) + "\");";
			            else
			                outputString += "\");";
				    		if(outputString == "stdout.put(\"\\n\");")
				    			outputString = "stdout.put(nl);";			            
			            fprintf(filePtr, "%s\n", outputString.c_str());       
			        }
			    }
			    else {
				    outputString = "stdout.put(\"" + parameterString.substr(thisNdx, outputString.size()) + "\");";
					 if(outputString == "stdout.put(\"\\n\");")
				    	outputString = "stdout.put(nl);";			            
				    fprintf(filePtr, "%s\n", outputString.c_str());				    
				}
		    }
		}
		else if(currentOp == "ENDOUTPUTPARAMETER") {
		}
		else if(currentOp == "BEGININTPUTPARAMETER") {
			fprintf(filePtr, "%s;\n", outputString.c_str());
			fprintf(filePtr, "mov( EAX, %s );\n", op3.c_str());
		}
		else if(currentOp == "INPUTPARAMETER") {
			fprintf(filePtr, "%s;\n", outputString.c_str());
			fprintf(filePtr, "mov( EAX, %s );\n", op3.c_str());		
		}
		else if(currentOp == "ENDINPUTPARAMETER") {
			fprintf(filePtr, "stdin.flushInput();\n");
		}
		/*************************************************************************
		**										ILESS													**
		*************************************************************************/		
		else if(currentOp == "ILESS") {
			//Set comparison string to opposite of ILESS
			lastComparison = "ge";
			doComparison(op3, op4, filePtr);
		}
		/*************************************************************************
		**										CJUMPF												**
		*************************************************************************/		
		else if(currentOp == "CJUMPF") {
			fprintf(filePtr, "j%s ", lastComparison.c_str());
			for(unsigned int j = 0; j < op1.size(); ++j) {
				if( op1[j] != '$')
					fprintf(filePtr, "%c", op1[j]);
			}
			fprintf(filePtr, ";\n");
			lastComparison = "";
			simulation.clearRegisters();
		}
		/*************************************************************************
		**										JUMP													**
		*************************************************************************/				
		else if(currentOp == "JUMP") {
			fprintf(filePtr, "jmp ");
			for(unsigned int j = 0; j < op1.size(); ++j) {
				if( op1[j] != '$')
					fprintf(filePtr, "%c", op1[j]);
			}
			fprintf(filePtr, ";\n");			
			simulation.clearRegisters();			
		}
		/*************************************************************************
		**										IGREATER												**
		*************************************************************************/				
		else if(currentOp == "IGREATER") {
			lastComparison = "le";
			doComparison(op3, op4, filePtr);
		}
		/*************************************************************************
		**										ISUB													**
		*************************************************************************/				
		else if(currentOp == "ISUB") {
			if(refFlag == 1) {
				registerLocation = simulation.findValueInRegister(op3);
				if(registerLocation == -1)	{
					registerLocation = simulation.findFreeRegister(-1);
				}
				fprintf(filePtr, "mov( %s, %s );\n", op3.c_str(), registerNames[registerLocation].c_str());
				fprintf(filePtr, "sub( %s, (type int32 [%s]) );\n", op4.c_str(), registerNames[registerLocation].c_str());
				registerLocation2 = simulation.findFreeRegister(registerLocation);
				fprintf(filePtr, "mov( (type int32 [%s]), %s );\n", registerNames[registerLocation].c_str(), registerNames[registerLocation2].c_str());
				simulation.setValueInRegister(op1, registerLocation);
			}
			else		
		    addSub(op1, "sub", op3, op4, filePtr);		
		}
		/*************************************************************************
		**										ISUBLOAD												**
		*************************************************************************/				
		else if(currentOp == "ISUBLOAD") {
			op4Type = determineType(op4);
			if ( op4Type == "constant" ) {
				// find free register
				op4RegisterLocation = simulation.findFreeRegister(-1);
				if ( op4RegisterLocation == -1 ) {
					fprintf(filePtr, "SPILLED\n");
				}
				// calculate the index (op4*4)
				fprintf(filePtr, "mov( %s, %s );\n", op4.c_str(), registerNames[op4RegisterLocation].c_str());
				simulation.setValueInRegister(op4, op4RegisterLocation);
				fprintf(filePtr, "shl( 2, %s );\n", registerNames[op4RegisterLocation].c_str());
				simulation.setValueInRegister("I$9999", op4RegisterLocation); 		// this is no longer op4. It is an IV.
		
				// mov ( op3[op4*4], I$* )
				op1RegisterLocation = simulation.findFreeRegister(op4RegisterLocation);
				if ( op1RegisterLocation  == -1 ) {
					fprintf(filePtr, "SPILLED\n");
				}
				fprintf(filePtr, "mov( %s[ %s ], %s );\n", op3.c_str(), registerNames[op4RegisterLocation].c_str(), registerNames[op1RegisterLocation].c_str());
				
				// set I$9999 as used, put op1 in a register
				simulation.setUsedInRegister(true, op4RegisterLocation);
				simulation.setValueInRegister(op1, op1RegisterLocation);
			}
			else if ( op4Type == "variable" ) {
				// see if op4 is in a register
				op4RegisterLocation = simulation.findValueInRegister(op4);
				if ( op4RegisterLocation == -1 ) {
					// put it in a register - mov( op4, op4RegisterLocation );
					op4RegisterLocation = simulation.findFreeRegister(-1);
					if ( op4RegisterLocation == -1 ) {
						fprintf(filePtr, "SPILLED\n");
					}
					fprintf(filePtr, "mov( %s, %s );\n", op4.c_str(), registerNames[op4RegisterLocation].c_str());
					simulation.setValueInRegister(op4, op4RegisterLocation);
				}
				// calculate the index -- shl( 2, op4 )
				fprintf(filePtr, "shl( 2, %s );\n", registerNames[op4RegisterLocation].c_str());
				simulation.setValueInRegister("I$9999", op4RegisterLocation); 		// this is no longer op4. It is an IV.					
				
				// mov ( var [ var ], I$* )
				op1RegisterLocation = simulation.findFreeRegister(op4RegisterLocation);
				if ( op1RegisterLocation  == -1 ) {
					fprintf(filePtr, "SPILLED\n");
				}
				fprintf(filePtr, "mov( %s[ %s ], %s );\n", op3.c_str(), registerNames[op4RegisterLocation].c_str(), registerNames[op1RegisterLocation].c_str());
				
				// set I$9999 as used, put op1 in a register
				simulation.setUsedInRegister(true, op4RegisterLocation);
				simulation.setValueInRegister(op1, op1RegisterLocation);
			}
			else if ( op4Type == "IV" ) {
				// get op4 register
				op4RegisterLocation = simulation.findValueInRegister(op4);
				if ( op4RegisterLocation == -1 ) {
					fprintf(filePtr, "SPILLED\n");
				}
				// calculate the index -- shl( 2, I$* )
				fprintf(filePtr, "shl(2, %s );\n", registerNames[op4RegisterLocation].c_str());
				simulation.setValueInRegister("I$9999", op4RegisterLocation);		// this is no longer op4. It is an IV.
				
			
				// mov ( var [ I$* ], I$* );
				op1RegisterLocation = simulation.findFreeRegister(op4RegisterLocation);
				if ( op1RegisterLocation  == -1 ) {
					fprintf(filePtr, "SPILLED\n");
				}
				fprintf(filePtr, "mov( %s[ %s ], %s );\n", op3.c_str(), registerNames[op4RegisterLocation].c_str(), registerNames[op1RegisterLocation].c_str());
				
				// set I$9999 as used, put op1 in a register
				simulation.setUsedInRegister(true, op4RegisterLocation);
				simulation.evictRegister(op4RegisterLocation);
				simulation.setValueInRegister(op1, op1RegisterLocation);
			}
		}

/*		else if(currentOp == "ISUBLOAD") {
            typeString = determineType(op4);
            registerLocation = simulation.findFreeRegister(0);            
            if(typeString == "constant" || typeString == "variable") {
                fprintf(filePtr, "mov( %s, EAX );\n", op4.c_str());
                fprintf(filePtr, "shl( 2, EAX );\n");            
            }
            else if(typeString == "IV") {
                registerLocation2 = simulation.findValueInRegister(op4);
                if(registerLocation2 != -1)
                    fprintf(filePtr, "mov( %s, EAX );\n", registerNames[registerLocation2].c_str());
                else                 	
                    fprintf(filePtr, "mov( %s, EAX );\n", op4.c_str());
                fprintf(filePtr, "shl( 2, EAX );\n");
					 simulation.setUsedInRegister(true, registerLocation2);
            }
            else
                cout<<"Something has gone wrong with "<<op1<<" substore "<<op3<<" "<<op4<<endl;
            fprintf(filePtr, "mov( %s[ EAX ], %s );\n", op3.c_str(), registerNames[registerLocation].c_str());
            simulation.setValueInRegister(op1, registerLocation);
		}*/
		/*************************************************************************
		**										IADD													**
		*************************************************************************/				
		else if(currentOp == "IADD") {
			if(refFlag == 1) {
				registerLocation = simulation.findValueInRegister(op3);
				if(registerLocation == -1)	{
					registerLocation = simulation.findFreeRegister(-1);
				}
				fprintf(filePtr, "mov( %s, %s );\n", op3.c_str(), registerNames[registerLocation].c_str());
				fprintf(filePtr, "add( %s, (type int32 [%s]) );\n", op4.c_str(), registerNames[registerLocation].c_str());
				registerLocation2 = simulation.findFreeRegister(registerLocation);
				fprintf(filePtr, "mov( (type int32 [%s]), %s );\n", registerNames[registerLocation].c_str(), registerNames[registerLocation2].c_str());
				simulation.setValueInRegister(op1, registerLocation);
			}
			else {
		    addSub(op1, "add", op3, op4, filePtr);
		  	}
		}
		/*************************************************************************
		**										ENDOFCALCULATIONS									**
		*************************************************************************/				
		else if(currentOp == "ENDOFCALCULATIONS") {
		
		}
		else if(currentOp == "PROGRAMEND") {
		    fprintf(filePtr, "stdout.put(nl);\n");
			fprintf(filePtr, "end %s;\n", fileName.c_str());
		}
		/********************
			END OF BUBBLESORT TUPLES
		*********************/
		else if(currentOp == "IMULT") {
			op3Type = determineType(op3);
			op4Type = determineType(op4);
			
			//find op3
			op3RegisterLocation = simulation.findValueInRegister(op3);
			if ( op3RegisterLocation == -1 ) {
				op3RegisterLocation = simulation.findFreeRegister(-1);
				if ( op3RegisterLocation == -1 ) {
					fprintf(filePtr, "SPILLED\n");
				}
				simulation.setValueInRegister(op3, op3RegisterLocation);
				// mov( op3, op3Register )
				fprintf(filePtr, "mov( %s, %s );\n", op3.c_str(), registerNames[op3RegisterLocation].c_str());
			}
			
			if ( op3RegisterLocation != 0 ) { // put it in EAX
				registerLocation = simulation.findFreeRegister(0, op3RegisterLocation);		// find somewhere to put the current EAX
				if ( registerLocation == -1 ) {
					fprintf(filePtr, "SPILLED\n");
				}
				simulation.setValueInRegister(simulation.getRegisterValue(0), registerLocation);
				fprintf(filePtr, "mov( EAX, %s );\n", registerNames[registerLocation].c_str());		// mov( EAX, registerLocation ) 
				simulation.setValueInRegister(op3, 0);
				fprintf(filePtr, "mov( %s, EAX );\n", op3.c_str());
			}
			
			// imul( op4, EAX(op3)) --> I$*
			fprintf(filePtr, "imul( %s, EAX );\n", op4.c_str());
			
			// EAX now = op4
			simulation.setValueInRegister(op1, 0);
		}		
/*		else if(currentOp == "IMULT") {
			typeString = determineType(op3);
			typeString2 = determineType(op4);
			registerLocation = simulation.findValueInRegister(op3); 
			if(registerLocation == -1) {
				registerLocation = simulation.findFreeRegister(-1);
				simulation.setValueInRegister(op3,registerLocation);
				fprintf(filePtr, "mov( %s, %s );\n", op3.c_str(), registerNames[registerLocation].c_str());			
			}
			if ( registerLocation != 0 ) {
				if ( simulation.getRegisterValue(0) != "" ) fprintf(filePtr, "push( EAX );\n");
				fprintf(filePtr, "mov( %s, EAX );\n", registerNames[registerLocation].c_str());
			}
			if(typeString2 == "constant") {
				fprintf(filePtr, "imul( %s, EAX );\n", op4.c_str());
			}
			else if(typeString2 == "variable") {
				fprintf(filePtr, "imul( %s, EAX );\n", op4.c_str());					
			}
			if ( registerLocation != 0 ) {
				if ( simulation.getRegisterValue(0) != "" ) fprintf(filePtr, "mov( EAX, %s );\n", registerNames[registerLocation].c_str());
				fprintf(filePtr, "pop( EAX );\n");
			}
			simulation.setValueInRegister(op1, registerLocation);
		}
*/		
		/********************
			END OF Matrix Addition TUPLES
		*********************/
		else if(currentOp == "PROCEDUREBEGIN") {
			procFlag = 1;
			procName = op1;
			procString = "procedure ";
			procString += procName;
		}
		else if(currentOp == "BEGINFORMALPARAMETERS") {
			procString += "(";
		}
		else if(currentOp == "FORMALREFERENCEPARAMETER"){
			refFlag = 1;
			procString += " var ";
			procString += op1;
			procString += ": int32, ";
		}
		else if(currentOp == "FORMALVALUEPARAMETER") {
			procString += " val ";
			procString += op1;
			procString += ": int32, ";
		}
		else if(currentOp == "NOFORMALPARAMETERS") {
			nfpFlag = 1;
			procString += ";";
			nfpFlag = 0;
			procStatus = 1;			
			fprintf(filePtr, "%s\n", procString.c_str());
		}
		else if(currentOp == "ENDFORMALPARAMETERS"){		
			if(nfpFlag != 1) {
				procString[procString.length() - 2] = ')';
				fprintf(filePtr, "%s;\n", procString.c_str());
				cout<<"ProcString = "<<procString<<endl;
			}
			nfpFlag = 0;
			procStatus = 1;
		}
		else if(currentOp == "ENDPROCEDURE") {
			fprintf(filePtr, "end %s;\n", procName.c_str());
			refFlag = 0;
		}
		else if(currentOp == "PROCCALL") {
			callString = op1;
		}
		else if(currentOp == "BEGINACTUALPARAMETERLIST") {
			callString += "(";
		}
		else if(currentOp == "ACTUALREFERENCEPARAMETER") {
			callString += op3;
			callString += ", ";
		}
		else if(currentOp == "ACTUALVALUEPARAMETER") {
			callString += op3;
			callString += ", ";		
		}
		else if(currentOp == "ENDACTUALPARAMETERS") {
			callString[callString.length()-2] = ')';
			fprintf(filePtr, "%s;\n", callString.c_str());
			cout<<"callString = "<<callString<<endl;
		}
		else if(currentOp == "NOACTUALPARAMETERS") {
			fprintf(filePtr, "call %s;\n", callString.c_str());
		}
		/********************
			END OF Pass By Reference and Pass By Value TUPLES
		*********************/
		else if(currentOp == "FLAG20ON") {
			printFlag = true;
			simulation.print();
		}
		else if(currentOp == "FLAG20OFF") {
			printFlag = false;
		}
		else {
			cout<<"Operation "<<currentOp<<" is not defined!"<<endl;
		}
		
	}
}

string AssemblyGenerator::determineType (string value) {
	if(value.find("$") != string::npos)
		return "IV";
	else if(value == "-")
		return "-";		
	else if((value[0] >= 48 && value[0] <= 57) || value[0] == '-')
		return "constant";	
	else
		return "variable";
}

void AssemblyGenerator::doComparison(string op3, string op4, FILE* filePtr) {
	string typeString, typeString2;
	int registerLocation, registerLocation2;
	typeString = determineType(op3);
	typeString2 = determineType(op4);
	//OP3 is Constant
	if(typeString == "constant") {
		//OP 4 is constant, easy
		if(typeString2 == "constant") {
			fprintf(filePtr, "cmp( %s, %s );\n", op3.c_str(), op4.c_str());
		}
		//OP 4 is Variable
		else if(typeString2 == "variable") {
			//Find OP4 variable in register
			registerLocation = simulation.findValueInRegister(op4);
			//If OP4 in register, use register
			if(registerLocation != -1)
				fprintf(filePtr, "cmp( %s, %s );\n", op3.c_str(), registerNames[registerLocation].c_str());
			else  //Else, use OP4 memory address
				fprintf(filePtr, "cmp( %s, %s );\n", op3.c_str(), op4.c_str());
		}
		//OP4 is IV
		else if(typeString2 == "IV") {
			//Find OP4 in Register
			registerLocation = simulation.findValueInRegister(op4);
			if(registerLocation != -1) { //If OP4 in Register, use register
				simulation.setUsedInRegister(true, registerLocation);  //Set IV OP4 Used
				fprintf(filePtr, "cmp( %s, %s );\n", op3.c_str(), registerNames[registerLocation].c_str());
			}						
		}
	}
	//OP3 is variable
	else if(typeString == "variable") {
		//Find OP3 register location
		registerLocation = simulation.findValueInRegister(op3);
		if(typeString2 == "constant") {
			if(registerLocation != -1)  //If OP3 in Register, use Register
				fprintf(filePtr, "cmp( %s, %s );\n", registerNames[registerLocation].c_str(), op4.c_str());
			else	//OP3 not in Register, use Memory address
				fprintf(filePtr, "cmp( %s, %s );\n", op3.c_str(), op4.c_str());				
		}
		else if(typeString2 == "variable") {
			//Find OP4 Register location
			registerLocation2 = simulation.findValueInRegister(op4);
			//Both variables in registers
			if(registerLocation != -1 && registerLocation2 != -1)
				fprintf(filePtr, "cmp( %s, %s );\n", registerNames[registerLocation].c_str(), registerNames[registerLocation2].c_str());
			else if (registerLocation != -1 && registerLocation2 == -1)  
			//OP3 in Register, OP4 in Memory					
				fprintf(filePtr, "cmp( %s, %s );\n", registerNames[registerLocation].c_str(), op4.c_str());
			else if (registerLocation == -1 && registerLocation2 != -1)
			//OP3 in Memory, OP4 in Register 
				fprintf(filePtr, "cmp( %s, %s );\n", op3.c_str(), registerNames[registerLocation2].c_str());		
			else if (registerLocation == -1 && registerLocation2 == -1) {
			//Both in memory, load one into register 
				//Find free register
				registerLocation2 = simulation.findFreeRegister(-1);
				//Evict found register
				simulation.evictRegister(registerLocation2);	
				//Load new value in register
				simulation.setValueInRegister(op4, registerLocation2);	
				fprintf(filePtr, "mov( %s, %s );\n", op3.c_str(), registerNames[registerLocation2].c_str());
				fprintf(filePtr, "cmp( %s, %s );\n", registerNames[registerLocation2].c_str(), op4.c_str());
			}
		}
		else if(typeString2 == "IV") {
			registerLocation2 = simulation.findValueInRegister(op4.c_str());
			if(registerLocation2 == -1)
				cout<<"SPILLED MILK!"<<endl;			
			fprintf(filePtr, "cmp( %s, %s );\n", registerNames[registerLocation2].c_str(), op3.c_str());
			//Set IV OP4 Used
			simulation.setUsedInRegister(true, registerLocation2);						
		}
	}
	else if(typeString == "IV") {
		registerLocation = simulation.findValueInRegister(op3);
		registerLocation2 = simulation.findValueInRegister(op4);
		if(registerLocation == -1) 
			cout<<"SPILLED MILK!"<<endl;
		if(typeString2 == "constant") {
			fprintf(filePtr, "cmp( %s, %s );\n", registerNames[registerLocation].c_str(), op4.c_str());				
		}
		else if(typeString2 == "variable") {
			fprintf(filePtr, "cmp( %s, %s );\n", registerNames[registerLocation].c_str(), op4.c_str());
		}
		else if(typeString2 == "IV") {
			if(registerLocation2 != -1)
				fprintf(filePtr, "cmp( %s, %s );\n", registerNames[registerLocation].c_str(), registerNames[registerLocation2].c_str());
			else
				cout<<"SPILLED MILK!"<<endl;
			simulation.setUsedInRegister(true, registerLocation2);			
			//Set IV OP3 Used
			simulation.setUsedInRegister(true, registerLocation);							
		}
	}
}

void AssemblyGenerator::addSub(string op1, string op2, string op3, string op4, FILE *filePtr) {
	string op3Type = determineType(op3);
	string op4Type = determineType(op4);
	int op3RegisterLocation, op4RegisterLocation;

   op3RegisterLocation = simulation.findValueInRegister(op3);
   if(op3Type == "IV") {
	   if(op3RegisterLocation == -1) 
	   	op3RegisterLocation = retrieveSpilled(filePtr);
	   	
  	}
  	else if(op3RegisterLocation == -1) {
  		op3RegisterLocation = simulation.findFreeRegister(-1);
  		if(op3RegisterLocation == -1) {
  			//Time to spill
  			op3RegisterLocation = spill(filePtr);
 		}
  		simulation.setValueInRegister(op3, op3RegisterLocation);
  		fprintf(filePtr, "mov( %s, %s );\n", op3.c_str(), registerNames[op3RegisterLocation].c_str());
  	}
   if(op4Type == "constant")
       fprintf(filePtr, "%s( %s, %s );\n", op2.c_str(), op4.c_str(), registerNames[op3RegisterLocation].c_str());
   else if(op4Type == "variable")
       fprintf(filePtr, "%s( %s, %s );\n", op2.c_str(), op4.c_str(), registerNames[op3RegisterLocation].c_str());
   else if(op4Type == "IV") {
       op4RegisterLocation = simulation.findValueInRegister(op4);
       if(op4RegisterLocation == -1) {
			fprintf(filePtr, "SPILLED WHILE ADDING/SUBBING\n");
       }
       fprintf(filePtr, "%s( %s, %s );\n", op2.c_str(), registerNames[op4RegisterLocation].c_str(), registerNames[op3RegisterLocation].c_str());
       simulation.setUsedInRegister(true, op4RegisterLocation);
   }        
   simulation.setValueInRegister(op1, op3RegisterLocation);
}

void AssemblyGenerator::printToFile(FILE *filePtr) {
	fprintf(filePtr, "\n");
	cout<<"We're here"<<endl;
	for(int i = 0; i < MAX_REGISTERS; ++i) {
		if(i == 0)
			fprintf(filePtr, "//EAX: %s\n", simulation.getRegisterValue(i).c_str());
		else if(i == 1)
			fprintf(filePtr, "//EBX: %s\n", simulation.getRegisterValue(i).c_str());		
		else if(i == 2)
			fprintf(filePtr, "//ECX: %s\n", simulation.getRegisterValue(i).c_str());		
		else if(i == 3)
			fprintf(filePtr, "//EDX: %s\n", simulation.getRegisterValue(i).c_str());				
		else if(i == 4)
			fprintf(filePtr, "//ESI: %s\n", simulation.getRegisterValue(i).c_str());
		else if(i == 5)
			fprintf(filePtr, "//EDI: %s\n", simulation.getRegisterValue(i).c_str());			
	}
	fprintf(filePtr, "\n");	
}

int AssemblyGenerator::spill(FILE* filePtr) {
	int spilled = simulation.spillRegister();
	fprintf(filePtr, "push( %s );\n", registerNames[spilled].c_str());
	return spilled;
}

int AssemblyGenerator::retrieveSpilled(FILE* filePtr) {
	int spilled = simulation.findFreeRegister(-1);
	string spilledVal = simulation.popSpilled();
	fprintf(filePtr, "pop( %s );\n", registerNames[spilled].c_str());
	simulation.setValueInRegister(spilledVal, spilled);
	return spilled;
}
