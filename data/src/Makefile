CFLAGS=-Werror -Wextra -Wall 
CC=gcc
FSAN=-fsanitize=leak -fsanitize=address -fsanitize=undefined -fsanitize=unreachable -fno-sanitize-recover -fstack-protector

all: main

rebuild: clean

main: main.c
	$(CC) main.c -o ./main -lm -lmysqlclient

clean:
	rm -rf *.o *.out

check:
	clang-format -style='{BasedOnStyle: Google, IndentWidth: 4, ColumnLimit: 110}' -i *.c
	# clang-format -style='{BasedOnStyle: Google, IndentWidth: 4, ColumnLimit: 110}' -i *.h
	cppcheck --enable=all --suppress=missingIncludeSystem *.c
	# valgrind --tool=memcheck --leak-check=yes ../bin/find_duplicates
