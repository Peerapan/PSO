# Define the symbols we might want to change:
CXX	:= c++
CXXFLAGS := -g -std=c++14

OBJECTS	:= build/function.o build/model.o build/test_poon.o

build/test_poon: $(OBJECTS)
	$(CXX) $(OBJECTS) -o build/test_poon

build/test_poon.o: test/test_poon_v3.cpp
	$(CXX) $(CXXFLAGS) -I./include -c  test/test_poon_v3.cpp -o build/test_poon.o
	
build/model.o: src/model.cpp
	$(CXX) $(INCLUDES) -I./include $(CXXFLAGS) -c src/model.cpp -o build/model.o
	
build/function.o: src/function.cpp
	$(CXX) $(INCLUDES) -I./include $(CXXFLAGS) -c src/function.cpp -o build/function.o

