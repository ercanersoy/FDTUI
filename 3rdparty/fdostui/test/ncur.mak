# ncur.mak
#
# builds FDOSTUI test applications
#
# make -f ncur.mak
#
# License CC0 PUBLIC DOMAIN
#
# To the extent possible under law, Mark J. Olesen has waived all copyright
# and related or neighboring rights to FDOSTUI Library. This work is published
# from: United States.

CCFLAGS=\
	-I../include\
	-D__FLAT__\
	-D__CURSES__\
	-Wall\
	-ansi\
	-pedantic\
	-ggdb

CXXFLAGS=\
	-I../include\
	-D__FLAT__\
	-D__CURSES__\
	-Wall\
	-ansi\
	-pedantic\
	-ggdb

LIBS= -L ../lib/ -l fdtuicur -l curses 

.PHONY:
all: hello menu widget scroller file tree ercan

ercan : ercan.o
	${CXX} -ggdb -o $@ $? ${LIBS}

hello : thello.o
	${CXX} -ggdb -o $@ $? ${LIBS}

menu : tmenu.o
	${CXX} -ggdb -o $@ $? ${LIBS}

widget : twidget.o
	${CXX} -ggdb -o $@ $? ${LIBS}

scroller : tscrollr.o
	${CXX} -ggdb -o $@ $? ${LIBS}

file : tfile.o
	${CXX} -ggdb -o $@ $? ${LIBS}

tree : ttree.o
	${CXX} -ggdb -o $@ $? ${LIBS}

%.o : %.cpp 
	${CXX} -c ${CXXFLAGS} -o $@ $< 2> $@.err

%.o : %.c
	${CC} -c ${CCFLAGS} -o $@ $< 2> $@.err
    
.PHONY:
clean:
	@rm -f ercan
	@rm -f tini
	@rm -f hello
	@rm -f menu
	@rm -f widget
	@rm -f scroller
	@rm -f file
	@rm -f tree
	@rm -f *.o	
	@rm -f *.o.err
