# djgpp.mak
#
# builds FDOSTUI test applications
#
# make -f djgpp.mak
#
# License CC0 PUBLIC DOMAIN
#
# To the extent possible under law, Mark J. Olesen has waived all copyright
# and related or neighboring rights to FDOSTUI Library. This work is published
# from: United States.

CCFLAGS=\
	-I../include\
	-D__FLAT__\
	-Wall\
	-pedantic\
	-ggdb

CXXFLAGS=\
	-I../include\
	-D__FLAT__\
	-Wall\
	-pedantic\
	-ggdb

LIBS= -L ../lib/ -l fdtui 

.PHONY:
all: hello menu widget scroller file  tree

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
	${CXX} -c ${CXXFLAGS} -o $@ $<

%.o : %.c
	${CC} -c ${CCFLAGS} -o $@ $< 
    
.PHONY:
clean:
	@rm -f tini
	@rm -f hello
	@rm -f menu
	@rm -f widget
	@rm -f scroller
	@rm -f file
	@rm -f *.o	
	@rm -f *.o.err
