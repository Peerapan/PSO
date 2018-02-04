# Define the symbols we might want to change:

CXX	:= c++
CXXFLAGS := -g -std=c++14

OBJECTS	:= build/function.o build/test_1.o
OBJECTS2 := build/function.o build/model.o build/ss_model.o build/test_2.o
OBJECTS3 := build/function.o build/model.o build/all_model.o build/test_3.o
OBJECTS_GRAPH := test/test_graph.cpp

all: build/test_1 build/test_2 build/test_3 build/test_graph

build/test_1: $(OBJECTS)
	$(CXX) $(OBJECTS) -o build/test_1

build/test_1.o: test/test_1.cpp
	$(CXX) $(CXXFLAGS) -I./include -c  test/test_1.cpp -o build/test_1.o

build/test_2: $(OBJECTS2)
	$(CXX) $(OBJECTS2) -o build/test_2

build/test_2.o: test/test_2.cpp
	$(CXX) $(CXXFLAGS) -I./include -c  test/test_2.cpp -o build/test_2.o
	
build/test_3: $(OBJECTS3)
	$(CXX) $(OBJECTS3) -o build/test_3

build/test_3.o: test/test_3.cpp
	$(CXX) $(CXXFLAGS) -I./include -c  test/test_3.cpp -o build/test_3.o
	
build/test_graph: $(OBJECTS_GRAPH)
	$(CXX) $(OBJECTS_GRAPH) -I./include -o build/test_graph $(CXXFLAGS)
	
build/model.o: src/model.cpp
	$(CXX) $(INCLUDES) -I./include $(CXXFLAGS) -c src/model.cpp -o build/model.o
	
build/ss_model.o: src/ss_model.cpp
	$(CXX) $(INCLUDES) -I./include $(CXXFLAGS) -c src/ss_model.cpp -o build/ss_model.o

build/all_model.o: src/all_model.cpp
	$(CXX) $(INCLUDES) -I./include $(CXXFLAGS) -c src/all_model.cpp -o build/all_model.o
	
build/function.o: src/function.cpp
	$(CXX) $(INCLUDES) -I./include $(CXXFLAGS) -c src/function.cpp -o build/function.o
