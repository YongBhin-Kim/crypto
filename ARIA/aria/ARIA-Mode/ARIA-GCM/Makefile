# Makefile for
# 1. ARIA Block Cipher Mode - GCM mode testrun

# CC
CC		= gcc

# Util (e.g. byte array <- hex string)
UTIL_SRCS		= ../Util/util.c
UTIL_HEADERS	= ../Util/util.h

# ARIA encryption/decryption sourcre & headers
ARIA_SRCS		= ../../ARIA-C/aria.c
ARIA_HEADERS	= ../../ARIA-C/aria.h

# ARIA CTR mode source & headers (gcm needs ctr)
ARIA_CTR_SRCS = ../ARIA-CTR/aria_ctr.c
ARIA_CTR_HEADERS = ../ARIA-CTR/aria_ctr.h

# ARIA GCM mode srouce & headers
ARIA_GCM_SRCS = aria_gcm.c ghash.c
ARIA_GCM_HEADERS = aria_gcm.h ghash.h

# ARIA CTR mode testrun
ARIA_GCM_TESTRUN			= aria_gcm_testrun
ARIA_GCM_TESTRUN_SRCS		= aria_gcm_testrun.c 


all : $(ARIA_GCM_TESTRUN) 

$(ARIA_GCM_TESTRUN):	$(ARIA_GCM_TESTRUN_SRCS) $(ARIA_SRCS) $(ARIA_HEADERS) $(UTIL_SRCS) $(UTIL_HEADERS) $(ARIA_CTR_SRCS) $(ARIA_CTR_HEADERS) $(ARIA_GCM_SRCS) $(ARIA_GCM_HEADERS) $(ARIA_GCM_TESTRUN_HEADERS)
	$(CC) -o $@ $(ARIA_GCM_TESTRUN_SRCS) $(ARIA_SRCS) $(UTIL_SRCS) $(ARIA_CTR_SRCS) $(ARIA_GCM_SRCS) $(LDFLAGS)


clean:
	rm -rf $(ARIA_GCM_TESTRUN) 

# EOF