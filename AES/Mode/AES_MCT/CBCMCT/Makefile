
CC			= g++
CFLAGS		= -W -O2

SOURCES= CBCMCT.cpp AES32.cpp HexByte.cpp ../../../Math/GF_Matrix_Math.cpp ../../../Math/GF_Math.c
HEADERS= CBCMCT.h AES32.h HexByte.h ../../../Math/GF_Matrix_Math.h ../../../Math/GF_Math.h
CBCMCT: $(HEADERS) $(SOURCES)
	$(CC) -o $@ $(SOURCES) $(CPPFLAGS)

clean:
	@rm -rf *.o CBCMCT