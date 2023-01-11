# Makefile for
# 1. gen_sbox
# 2. aria enc/dec testrun

# CC
CC		= gcc

# Generate ARIA sbox
GENSBOX			= gen_sbox
GENSBOX_SRCS	= gen_sbox.c

# ARIA encryption/decryption
ARIA_SRCS		= aria.c
ARIA_HEADERS	= aria.h

# ARIA encryption/decryption test
ARIA_TESTRUN			= aria_testrun
ARIA_TESTRUN_SRCS		= aria_testrun.c
ARIA_TESTRUN_HEADERS	= aria_testrun.h


all : $(GENSBOX) $(ARIA_TESTRUN)

$(GENSBOX): $(GENSBOX_SRCS)
	$(CC) -o $@ $(GENSBOX_SRCS) $(LDFLAGS)

$(ARIA_TESTRUN):	$(ARIA_TESTRUN_SRCS) $(ARIA_TESTRUN_HEADERS) $(ARIA_SRCS) $(ARIA_HEADERS)
	$(CC) -o $@ $(ARIA_TESTRUN_SRCS) $(ARIA_SRCS) $(LDFLAGS)


clean:
	rm -rf $(GENSBOX) 
	rm -rf $(ARIA_TESTRUN)

# EOF