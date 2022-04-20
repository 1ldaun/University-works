all:
	gcc -fPIC -shared lib.c -o lab1iasN3249.so -Wall -Werror -Wextra -O2
	LD_LIBRARY_PATH=. gcc -o lab1iasN3249 lab1iasN3249.c -ldl -lm -Wall -Werror -Wextra -O2

clean:
	rm lab1iasN3249.so lab1iasN3249
