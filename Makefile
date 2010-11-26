CFLAGS := -W -Wall

all: lzw

lzw:
	${CC} ${CFLAGS} src/lzw.c -o lzw

clean:
	rm -f lzw
