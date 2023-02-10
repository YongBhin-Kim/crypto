# Makefile for
# 1. ARIA Block Cipher Mode - CTR mode test code
# 2. CTR mode parallel implementation test code

# CC
CC		= gcc

# Util (e.g. byte array <- hex string)
UTIL_SRCS					= ../Util/util.c
UTIL_HEADERS				= ../Util/util.h

# ARIA encryption/decryption sourcre & headers
ARIA_SRCS					= ../../ARIA-C/aria.c
ARIA_HEADERS				= ../../ARIA-C/aria.h

# ARIA CTR mode srouce & headers
ARIA_CTR_SRCS 				= aria_ctr.c
ARIA_CTR_HEADERS 			= aria_ctr.h

# ARIA CTR mode parallel implementations srouce & headers
ARIA_CTR_PARALLEL_SRCS 		= aria_ctr_parallel.c
ARIA_CTR_PARALLEL_HEADERS 	= aria_ctr_parallel.h


# ARIA CTR mode testrun
ARIA_CTR_TESTRUN			= aria_ctr_testrun
ARIA_CTR_TESTRUN_SRCS		= aria_ctr_testrun.c

# ARIA CTR mod parallel implementation test code
ARIA_CTR_PARALLEL_TESTRUN		= aria_ctr_parallel_testrun
ARIA_CTR_PARALLEL_TESTRUN_SRCS	= aria_ctr_parallel_testrun.c

all : $(ARIA_CTR_TESTRUN) $(ARIA_CTR_PARALLEL_TESTRUN)

$(ARIA_CTR_TESTRUN):	$(ARIA_CTR_TESTRUN_SRCS) $(ARIA_SRCS) $(ARIA_HEADERS) $(UTIL_SRCS) $(UTIL_HEADERS) $(ARIA_CTR_SRCS) $(ARIA_CTR_HEADERS)
	$(CC) -o $@ $(ARIA_CTR_TESTRUN_SRCS) $(ARIA_SRCS) $(ARIA_CTR_SRCS) $(UTIL_SRCS) $(LDFLAGS)

$(ARIA_CTR_PARALLEL_TESTRUN):	$(ARIA_CTR_PARALLEL_TESTRUN_SRCS) $(ARIA_SRCS) $(ARIA_HEADERS) $(UTIL_SRCS) $(UTIL_HEADERS) $(ARIA_CTR_PARALLEL_SRCS) $(ARIA_CTR_PARALLEL_HEADERS)
	$(CC) -o $@ $(ARIA_CTR_PARALLEL_TESTRUN_SRCS) $(ARIA_SRCS) $(ARIA_CTR_PARALLEL_SRCS) $(UTIL_SRCS) $(LDFLAGS)


clean:
	rm -rf $(ARIA_CTR_TESTRUN) 
	rm -rf $(ARIA_CTR_PARALLEL_TESTRUN)

# EOF