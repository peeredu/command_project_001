BUILD_PATH=../bin/
CC=gcc
CFLAGS=-g -Wall -Wextra -Werror
EXTRAFLAGS=#-fsanitize=leak -fsanitize=address -fsanitize=undefined -fsanitize=unreachable -fno-sanitize-recover -fstack-protector 

all: rebuild check 

rebuild: clean_all socket_server clean

socket_server: clean socket_server.c
	$(CC) $(CFLAGS) $(EXTRAFLAGS) socket_server.c socket_util.c -o $(BUILD_PATH)socket_server  -lpthread

clean_all:
	rm -rf *.o *.a *.so $(BUILD_PATH)*
	
clean:
	rm -rf *.o *.a

check:
	clang-format *.c   -i
	cppcheck --enable=all --force --suppress=missingIncludeSystem *.c  # common/*.c common/*.h real_db/*.c real_db/*.h
