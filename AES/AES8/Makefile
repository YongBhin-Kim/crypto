
CC			= g++
CFLAGS		= -W -O2
LDFLAGS		= -lm

all : AES8

# AES8 Makefile
AES8: AES8.cpp ../Math/GF_Matrix_Math.cpp ../Math/GF_Math.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	@rm -rf *.o AES8
