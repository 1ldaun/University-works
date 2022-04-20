all: main

run: main
	./main

main: src/main.cpp src/util.cpp src/lexer/expression.lexer.c src/lexer/expression.tab.c
	g++ $^ -std=c++14 -O2 -o main
	
src/lexer/%.lexer.c src/lexer/%.lexer.h: src/lexer/%.lex
	flex src/lexer/$*.lex
	mv lex.c src/lexer/$*.lexer.c
	mv lex.h src/lexer/$*.lexer.h

src/lexer/%.tab.c src/lexer/%.tab.h: src/lexer/%.y
	bison -d -v $^ -o src/lexer/$*.tab.c

clean:
	rm -rf main.o
	rm -f src/lexer/*.c src/lexer/*.h
