make:
	g++ -Wall -g -O0 -o src/main src/main.cpp

run: src/main
	./src/main
