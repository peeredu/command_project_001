BUILD_PATH=../bin/
CC=gcc
CFLAGS=-g -Wall -Wextra -Werror
EXTRAFLAGS=#-fsanitize=leak -fsanitize=address -fsanitize=undefined -fsanitize=unreachable -fno-sanitize-recover -fstack-protector 

all: rebuild check 

rebuild: socket_server json_export_test clean

socket_server: clean socket_server.c
	$(CC) $(CFLAGS) $(EXTRAFLAGS) socket_server.c socket_util.c json_export.c ../../data/src/db_api.c common/logger.c -o $(BUILD_PATH)socket_server  -lpthread -lmysqlclient

json_export_test: clean tests/json_export_test.c
	$(CC) $(CFLAGS) $(EXTRAFLAGS) tests/json_export_test.c json_export.c -o $(BUILD_PATH)json_export_test

clean_all:
	rm -rf *.o *.a *.so $(BUILD_PATH)*
	
clean:
	rm -rf *.o *.a

check:
	clang-format *.c   -i
	cppcheck --enable=all --force --suppress=missingIncludeSystem *.c  # common/*.c common/*.h real_db/*.c real_db/*.h
