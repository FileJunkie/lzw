CFLAGS := -W -Wall -g -O0

all: lzw

lzw:
	LC_ALL="C" ${CC} ${CFLAGS} src/lzw.c -o lzw

clean:
	rm -f lzw
