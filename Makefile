.PHONY: all test clean

test:
	g++ -O2 -lm -std=c++11 -I./src test/test_simple.cpp -o build/test_simple
	g++ -O2 -lm -std=c++11 -I./src test/test_poon_v3.cpp -o build/test_poon

clean:
	-rm -rf build/