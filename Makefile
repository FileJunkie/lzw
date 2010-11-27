CFLAGS := -W -Wall -g -O0

all: lzw

common:
	LC_ALL="C" ${CC} ${CFLAGS} -c src/common.c -o common.o 

lzw: common
	LC_ALL="C" ${CC} ${CFLAGS} src/lzw.c common.o -o lzw

clean:
	rm -f lzw *.o
