# Makefile for
# 1. gen_sbox testrun
# 2. aria enc/dec testrun

# CC
CC							= gcc

# Generate ARIA sbox
GENSBOX						= gen_sbox_testrun
GENSBOX_SRCS				= gen_sbox_testrun.c

# ARIA encryption/decryption
ARIA_SRCS					= aria.c
ARIA_HEADERS				= aria.h

# ARIA encryption/decryption test
ARIA_TESTRUN				= aria_testrun
ARIA_TESTRUN_SRCS			= aria_testrun.c

# ARIA parallel implementation
ARIA_PARALLEL_SRCS			= aria_parallel.c
ARIA_PARALLEL_HEADERS		= aria_parallel.h

# ARIA parallel implementation encryption/decryption test
ARIA_PARALLEL_TESTRUN		= aria_parallel_testrun
ARIA_PARALLEL_TESTRUN_SRCS  = aria_parallel_testrun.c


all : $(GENSBOX) $(ARIA_TESTRUN) $(ARIA_PARALLEL_TESTRUN)

$(GENSBOX): $(GENSBOX_SRCS)
	$(CC) -o $@ $(GENSBOX_SRCS) $(LDFLAGS)

$(ARIA_TESTRUN):	$(ARIA_TESTRUN_SRCS) $(ARIA_SRCS) $(ARIA_HEADERS)
	$(CC) -o $@ $(ARIA_TESTRUN_SRCS) $(ARIA_SRCS) $(LDFLAGS)


$(ARIA_PARALLEL_TESTRUN):	$(ARIA_PARALLEL_TESTRUN_SRCS) $(ARIA_PARALLEL_SRCS) $(ARIA_PARALLEL_HEADERS)
	$(CC) -o $@ $(ARIA_PARALLEL_TESTRUN_SRCS) $(ARIA_PARALLEL_SRCS) $(LDFLAGS)

clean:
	rm -rf $(GENSBOX) 
	rm -rf $(ARIA_TESTRUN)
	rm -rf $(ARIA_PARALLEL_TESTRUN)

# EOF