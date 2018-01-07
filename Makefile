# Define the symbols we might want to change:
CXX	:= c++
CXXFLAGS := -g -std=c++14

OBJECTS	:= build/function.o build/test_1.o
OBJECTS2 := build/function.o build/model.o build/ss_model.o build/test_2.o

all: build/test_1 build/test_2

build/test_1: $(OBJECTS)
	$(CXX) $(OBJECTS) -o build/test_1

build/test_1.o: test/test_1.cpp
	$(CXX) $(CXXFLAGS) -I./include -c  test/test_1.cpp -o build/test_1.o

build/test_2: $(OBJECTS2)
	$(CXX) $(OBJECTS2) -o build/test_2

build/test_2.o: test/test_2.cpp
	$(CXX) $(CXXFLAGS) -I./include -c  test/test_2.cpp -o build/test_2.o
	
build/model.o: src/model.cpp
	$(CXX) $(INCLUDES) -I./include $(CXXFLAGS) -c src/model.cpp -o build/model.o
	
build/ss_model.o: src/ss_model.cpp
	$(CXX) $(INCLUDES) -I./include $(CXXFLAGS) -c src/ss_model.cpp -o build/ss_model.o
	
build/function.o: src/function.cpp
	$(CXX) $(INCLUDES) -I./include $(CXXFLAGS) -c src/function.cpp -o build/function.o
