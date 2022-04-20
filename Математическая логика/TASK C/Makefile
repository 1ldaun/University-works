all: main

run: main
	./main

main: main.cpp express.tab.c  express.lexer.c 
	g++ -std=c++14 -g -O2 -o main main.cpp express.tab.c  express.lexer.c 

