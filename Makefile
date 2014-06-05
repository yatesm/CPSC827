#Makefile for CPSC 827 Compiler Project
#Spring, 2011
#Yates Monteith, Anthony McDougle

CFLAGS= -pedantic -Wextra -Wall -Weffc++ -ggdb -O0 -lexpat
CCC=g++

OBJECTS = handleChars.o parseXML.o compilerData.o token.o compiler.o lexicalAnalyzer.o parser.o semanticObject.o semanticsAnalyzer.o symbolTable.o fourTuple.o assemblyGenerator.o registerSimulation.o register.o

run: main.o $(OBJECTS)
	$(CCC) $(CFLAGS) main.cpp -o run $(OBJECTS)

main.o: main.cpp parseXML.cpp handleChars.cpp
	$(CCC) $(CFLAGS) -c main.cpp
	
handleChars.o: handleChars.cpp handleChars.h
	$(CCC) $(CFLAGS) -c handleChars.cpp

parseXML.o: parseXML.cpp parseXML.h
	$(CCC) $(CFLAGS) -c -lexpat parseXML.cpp

compilerData.o: compilerData.cpp compilerData.h
	$(CCC) $(CFLAGS) -c compilerData.cpp	
	
token.o: token.cpp token.h
	$(CCC) $(CFLAGS) -c token.cpp	

compiler.o: compiler.cpp compiler.h lexicalAnalyzer.cpp
	$(CCC) $(CFLAGS) -c compiler.cpp

lexicalAnalyzer.o: lexicalAnalyzer.cpp lexicalAnalyzer.h
	$(CCC) $(CFLAGS) -c lexicalAnalyzer.cpp

parser.o: parser.cpp parser.h
	$(CCC) $(CFLAGS) -c parser.cpp

semanticObject.o: semanticObject.cpp semanticObject.h
	$(CCC) $(CFLAGS) -c semanticObject.cpp

semanticsAnalyzer.o: semanticsAnalyzer.cpp semanticsAnalyzer.h
	$(CCC) $(CFLAGS) -c semanticsAnalyzer.cpp

symbolTable.o: symbolTable.cpp symbolTable.h
	$(CCC) $(CFLAGS) -c symbolTable.cpp
	
fourTuple.o: fourTuple.cpp fourTuple.h
	$(CCC) $(CFLAGS) -c fourTuple.cpp

assemblyGenerator.o: assemblyGenerator.cpp assemblyGenerator.h
	$(CCC) $(CFLAGS) -c assemblyGenerator.cpp

register.o: register.cpp register.h
	$(CCC) $(CFLAGS) -c register.cpp	

registerSimulation.o: registerSimulation.cpp registerSimulation.h
	$(CCC) $(CFLAGS) -c registerSimulation.cpp
	
clean:
	rm -f run *.o
	rm -f *~

valid:
	@echo
	@echo Validating Loaded Compiler Data
	@xmllint -noout data/tokenData.xml
	@xmllint -noout data/compilerData.xml
	@echo
