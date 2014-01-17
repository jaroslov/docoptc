all:
	clang -g -I. docopt.c -o docc -Wall -Werror

clean:
	rm -rf docc docc.dSYM
