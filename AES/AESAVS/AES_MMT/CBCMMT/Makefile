
CC			= g++
CFLAGS		= -W -O2

SOURCES= ECB_CBC.cpp AES32.cpp ../Math/GF_Matrix_Math.cpp ../Math/GF_Math.c
HEADERS= ECB_CBC.h AES32.h ../Math/GF_Matrix_Math.h ../Math/GF_Math.h
ECB_CBC: $(HEADERS) $(SOURCES)
	$(CC) -o $@ $(SOURCES) $(CPPFLAGS)

clean:
	@rm -rf *.o ECB_CBC