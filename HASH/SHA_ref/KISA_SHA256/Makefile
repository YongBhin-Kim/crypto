# # Makefile for
# # 1. KISA_SHA256

# # CC
# CC							= gcc
# LDFLAGS						= -g

# SHA256_TEST = kisa_sha256_test
# SHA256_HEADERS = SHA256_C/KISA_SHA256.h
# SHA256_SOURCES = SHA256_C/KISA_SHA256.c
# SHA256_TEST_SOURCES = main.c


# all : $(SHA256_TEST) #$()


# $(SHA256_TEST):	$(SHA256_TEST_SOURCES) $(SHA256_SOURCES) $(SHA256_HEADERS)
# 	$(CC) -o $@ $(SHA256_TEST_SOURCES) $(SHA256_SOURCES) $(LDFLAGS)
# run: kisa_sha256_test
#     ./kisa_sha256_test
# clean:
# 	rm -rf $(SHA256_TEST) 
# #rm -rf $(ARIA_TESTRUN)


# # EOF

CC							= gcc
LDFLAGS						= -g

SHA256_TEST = kisa_sha256_test
SHA256_HEADERS = SHA256_C/KISA_SHA256.h
SHA256_SOURCES = SHA256_C/KISA_SHA256.c
SHA256_TEST_SOURCES = main.c

all : $(SHA256_TEST)

$(SHA256_TEST):	$(SHA256_TEST_SOURCES) $(SHA256_SOURCES) $(SHA256_HEADERS)
	$(CC) -o $@ $(SHA256_TEST_SOURCES) $(SHA256_SOURCES) $(LDFLAGS)

run: kisa_sha256_test
	./kisa_sha256_test

clean:
	rm -rf $(SHA256_TEST)
