# cham makefile

CC			= gcc
CFLAGS		= -W -O2
LDFLAGS		= -lm

all : cham

SOURCES =  cham64.c
SOURCES += main.c
HEADERS= cham.h
cham: $(HEADERS) $(SOURCES)
	$(CC) -o $@ $(SOURCES) $(CFLAGS)

clean:
	@rm -rf *.o cham
