
CC			= g++
CFLAGS		= -W -O2
# LDFLAGS		= -lm

# AES32 Makefile
# g++ -o AES32.exec AES32.cpp -o AES32
SOURCES= AES32.cpp ../Math/GF_Matrix_Math.cpp ../Math/GF_Math.c
HEADERS= AES32.h ../Math/GF_Matrix_Math.h ../Math/GF_Math.h
AES32: $(HEADERS) $(SOURCES)
	$(CC) -o $@ $(SOURCES) $(CPPFLAGS)

clean:
	@rm -rf *.o AES32