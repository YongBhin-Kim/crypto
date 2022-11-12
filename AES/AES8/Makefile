
CC			= g++
CFLAGS		= -W -O2
LDFLAGS		= -lm

all : AES8

# AES8 Makefile
SOURCES= AES8.cpp ../Math/GF_Matrix_Math.cpp ../Math/GF_Math.c
HEADERS= AES8.h ../Math/GF_Matrix_Math.h ../Math/GF_Math.h
AES8: $(HEADERS) $(SOURCES)
	$(CC) -o $@ $(SOURCES) $(CPPFLAGS)

clean:
	@rm -rf *.o AES8
