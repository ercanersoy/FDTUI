# mingw.mak
#
# builds FDOSTUI test applications (MINGW)
#
# requires GNUMake (gmake) and MinGW tools
#
# make -f mingw.mak
#
# License CC0 PUBLIC DOMAIN
#
# To the extent possible under law, Mark J. Olesen has waived all copyright
# and related or neighboring rights to FDOSTUI Library. This work is published
# from: United States.

UNAME:= $(shell uname)

ifeq ($(UNAME), Linux)
# MinGW running under GNU/Linux (cross-compiling)
CC=i686-w64-mingw32-gcc
CXX=i686-w64-mingw32-g++
endif

CCFLAGS=\
	-I../include\
	-D__FLAT__\
	-D__NT__\
	-Wall\
	-ansi\
	-pedantic\
	-ggdb

CXXFLAGS=\
	-I../include\
	-D__FLAT__\
	-D__NT__\
	-Wall\
	-ansi\
	-pedantic\
	-ggdb

LIBS= -static -static-libgcc -static-libstdc++ -L ../lib/ -l fdtuimgw

.PHONY:
all: hello.exe menu.exe widget.exe scroller.exe file.exe tree.exe

hello.exe : thello.o
	${CXX} -ggdb -o $@ $? ${LIBS}

menu.exe : tmenu.o
	${CXX} -ggdb -o $@ $? ${LIBS}

widget.exe : twidget.o
	${CXX} -ggdb -o $@ $? ${LIBS}

scroller.exe : tscrollr.o
	${CXX} -ggdb -o $@ $? ${LIBS}

file.exe : tfile.o
	${CXX} -ggdb -o $@ $? ${LIBS}

tree.exe : ttree.o
	${CXX} -ggdb -o $@ $? ${LIBS}

%.o : %.cpp 
	${CXX} -c ${CXXFLAGS} -o $@ $< 2> $@.err

%.o : %.c
	${CC} -c ${CCFLAGS} -o $@ $< 2> $@.err
    
.PHONY:
clean:
	@rm -f hello.exe
	@rm -f menu.exe
	@rm -f widget.exe
	@rm -f scroller.exe
	@rm -f file.exe
	@rm -f *.o	
	@rm -f *.o.err
