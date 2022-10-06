
CC			= g++
CFLAGS		= -W -O2
LDFLAGS		= -lm

all : AES32

# AES32 Makefile
# g++ -o AES32.exec AES32.cpp -o AES32
AES32: AES32.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	@rm -rf *.o AES32
