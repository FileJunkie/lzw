CFLAGS := -W -Wall
WCC := i686-pc-mingw32-gcc

all: lzw unlzw all_win
all_win: lzw_win unlzw_win

common:
	LC_ALL="C" ${CC} ${CFLAGS} -c src/common.c -o common.o 

lzw: common
	LC_ALL="C" ${CC} ${CFLAGS} src/lzw.c common.o -o bin/lzw

unlzw: common
	LC_ALL="C" ${CC} ${CFLAGS} src/unlzw.c common.o -o bin/unlzw
	
common_win:
	LC_ALL="C" ${WCC} ${CFLAGS} -c src/common.c -o common.win.o 

lzw_win: common_win
	LC_ALL="C" ${WCC} ${CFLAGS} src/lzw.c common.win.o -o bin/lzw.exe

unlzw_win: common_win
	LC_ALL="C" ${WCC} ${CFLAGS} src/unlzw.c common.win.o -o bin/unlzw.exe

clean:
	rm -f bin/lzw* bin/unlzw* *.o unpacked_test packed_test
	
