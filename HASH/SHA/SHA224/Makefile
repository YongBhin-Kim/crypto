# Makefile for
# 1. SHA224

CC							= gcc
LDFLAGS						= -g -O2

SHA224_TEST = sha224_test
SHA224_HEADERS = sha224.h
SHA224_SOURCES = sha224.c
SHA224_TEST_SOURCES = main.c

all : clean $(SHA224_TEST)

$(SHA224_TEST):	$(SHA224_TEST_SOURCES) $(SHA224_SOURCES) $(SHA224_HEADERS)
	$(CC) -o $@ $(SHA224_TEST_SOURCES) $(SHA224_SOURCES) $(LDFLAGS)

run: clean sha224_test
	./sha224_test

clean:
	rm -rf $(SHA224_TEST)

# EOF