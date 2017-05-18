UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	CXX = clang++
else
	CXX = g++
endif

all: p5

p1: Test.o Value.o Tuple.o Relation.o Database.o DBManage.o Condition.o
	$(CXX) -std=c++11 -g  Test.o Value.o Tuple.o Relation.o Database.o DBManage.o Condition.o -o main

p2: TestParser.o Parser.o TokenStream.o Evaluator.o DBManage.o Condition.o Value.o Tuple.o Relation.o Database.o
	$(CXX) -std=c++11 -g TestParser.o Parser.o TokenStream.o Evaluator.o DBManage.o Condition.o Value.o Tuple.o Relation.o Database.o -o main

p3: TestIntegration.o Parser.o TokenStream.o Evaluator.o DBManage.o Condition.o Value.o Tuple.o Relation.o Database.o
	$(CXX) -std=c++11 -g TestIntegration.o Parser.o TokenStream.o Evaluator.o DBManage.o Condition.o Value.o Tuple.o Relation.o Database.o -o main

p4: DBMShell.o Parser.o TokenStream.o Evaluator.o DBManage.o Condition.o Value.o Tuple.o Relation.o Database.o
	$(CXX) -std=c++11 -g DBMShell.o Parser.o TokenStream.o Evaluator.o DBManage.o Condition.o Value.o Tuple.o Relation.o Database.o -o main

p5: AppRunner.o App.o Parser.o TokenStream.o Evaluator.o DBManage.o Condition.o Value.o Tuple.o Relation.o Database.o
	$(CXX) -std=c++11 -g AppRunner.o App.o Parser.o TokenStream.o Evaluator.o DBManage.o Condition.o Value.o Tuple.o Relation.o Database.o -o main


DBMShell.o: DBMShell.cpp Parser.H
	$(CXX) -std=c++11 -c -g DBMShell.cpp

TestParser.o: TestParser.cpp Parser.H
	$(CXX) -std=c++11 -c -g TestParser.cpp

TestIntegration.o: TestIntegration.cpp Parser.H
	$(CXX) -std=c++11 -c -g TestIntegration.cpp

App.o: App.H App.C Parser.H
	$(CXX) -std=c++11 -c -g App.C

AppRunner.o: AppRunner.cpp
	$(CXX) -std=c++11 -c -g AppRunner.cpp

Parser.o: Parser.C Parser.H TokenStream.H Evaluator.H DBManage.H
	$(CXX) -std=c++11 -c -g Parser.C

TokenStream.o: TokenStream.C TokenStream.H
	$(CXX) -std=c++11 -c -g TokenStream.C

Evaluator.o: Evaluator.C Evaluator.H TokenStream.H Value.H Condition.H Relation.H DBManage.H
	$(CXX) -std=c++11 -c -g Evaluator.C

Test.o: Test.C Value.H Tuple.H Relation.H Database.H DBManage.H Condition.H
	$(CXX) -std=c++11 -c -g Test.C

Value.o: Value.C Value.H
	$(CXX) -std=c++11 -c -g Value.C

Condition.o: Condition.C Condition.H Value.H
	$(CXX) -std=c++11 -c -g Condition.C

Tuple.o: Tuple.C Tuple.H Value.H
	$(CXX) -std=c++11 -c -g  Tuple.C

Relation.o: Relation.C Relation.H Tuple.H
	$(CXX) -std=c++11 -c -g Relation.C

Database.o: Database.C Database.H Relation.H Value.H
	$(CXX) -std=c++11 -c -g Database.C

DBManage.o: DBManage.C DBManage.H Database.H Relation.H Tuple.H Value.H Condition.H
	$(CXX) -std=c++11 -c -g  DBManage.C

clean: Test.C
	rm *.o main
