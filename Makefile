.PHONY: all test clean

test:
	g++ -g -lm -std=c++11 -I./src test/test_poon_v3.cpp -o build/test_poon

fin:
	g++ -O3 -lm -std=c++11 -I./src test/test_poon_v3.cpp -o build/test_poon
	
clean:
	-rm -rf build/