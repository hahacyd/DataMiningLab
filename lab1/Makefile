
SimpleLoader: 
	g++ -c SimpleLoader.cpp

build:SimpleLoader.cpp main.cpp SimpleLoader.h
	g++ -o analysize  main.cpp SimpleLoader.cpp -std=c++11
run:$(build)
	./analysize
debug:SimpleLoader.cpp main.cpp SimpleLoader.h
	g++ -o  analysize  -g main.cpp SimpleLoader.cpp -std=c++11

clean:
	rm analysize SimpleLoader.o