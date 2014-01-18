all:
	clang -g -I. test.c -o test -Wall -Werror

clean:
	rm -rf test test.dSYM

check: clean all
	./test self-test
