
CC			= g++
CFLAGS		= -W -O2

SOURCES= ECBMCT.cpp AES32.cpp HexByte.cpp ../../../Math/GF_Matrix_Math.cpp ../../../Math/GF_Math.c
HEADERS= ECBMCT.h AES32.h HexByte.h ../../../Math/GF_Matrix_Math.h ../../../Math/GF_Math.h
ECBMCT: $(HEADERS) $(SOURCES)
	$(CC) -o $@ $(SOURCES) $(CPPFLAGS)

clean:
	@rm -rf *.o ECBMCT