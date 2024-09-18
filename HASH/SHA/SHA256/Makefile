# Makefile for
# 1. SHA256

CC							= gcc
LDFLAGS						= -g -O2

SHA256_TEST = sha256_test
SHA256_HEADERS = sha256.h
SHA256_SOURCES = sha256.c
SHA256_TEST_SOURCES = sha256_test.c

all : clean $(SHA256_TEST)

$(SHA256_TEST):	$(SHA256_TEST_SOURCES) $(SHA256_SOURCES) $(SHA256_HEADERS)
	$(CC) -o $@ $(SHA256_TEST_SOURCES) $(SHA256_SOURCES) $(LDFLAGS)

run: clean sha256_test
	./sha256_test

clean:
	rm -rf $(SHA256_TEST)

# EOF