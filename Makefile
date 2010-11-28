CFLAGS := -W -Wall

all: lzw unlzw

common:
	LC_ALL="C" ${CC} ${CFLAGS} -c src/common.c -o common.o 

lzw: common
	LC_ALL="C" ${CC} ${CFLAGS} src/lzw.c common.o -o lzw

unlzw: common
	LC_ALL="C" ${CC} ${CFLAGS} src/unlzw.c common.o -o unlzw

clean:
	rm -f lzw unlzw *.o
