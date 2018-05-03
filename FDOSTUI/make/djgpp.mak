# djgpp.mak
#
# EXPERIMENTAL (work in progress)
# makefile for DJ's GNU Programming Platform (DJGPP)
#
# License CC0 PUBLIC DOMAIN
#
# To the extent possible under law, Mark J. Olesen has waived all copyright
# and related or neighboring rights to FDOSTUI Library. This work is published
# from: United States.

O=../obj/djgpp
B=../bin
S=../src
SCUR=../src/djgpp
L=../lib
I=../include

RANLIB=ranlib

CFLAGS=\
	-I${I}\
	-D__FLAT__\
	-Wall\
	-pedantic\
	-ggdb


CXXFLAGS=\
	-I${I}\
	-D__FLAT__\
	-Wall\
	-pedantic\
	-ggdb
	
TUIOBJ=\
	${O}/hw.o \
	${O}/keyboard.o \
	${O}/mouse.o \
	${O}/screen.o \
	${O}/video.o \
	${O}/cursor.o \
	${O}/filedlg.o \
	${O}/poll.o \
	${O}/array.o \
	${O}/block.o \
	${O}/box.o \
	${O}/button.o \
	${O}/checkbtn.o \
	${O}/clip.o \
	${O}/combobox.o \
	${O}/draw.o \
	${O}/edit.o \
	${O}/entry.o \
	${O}/fdtuicfg.o \
	${O}/file.o \
	${O}/gapbuf.o \
	${O}/group.o \
	${O}/hash.o \
	${O}/ini.o \
	${O}/label.o \
	${O}/listbox.o \
	${O}/matrix.o \
	${O}/menubar.o \
	${O}/menuitem.o \
	${O}/menu.o \
	${O}/popup.o \
	${O}/radiobtn.o \
	${O}/scrllbar.o \
	${O}/scroller.o \
	${O}/skin.o \
	${O}/slider.o  \
	${O}/spinner.o \
	${O}/tree.o \
	${O}/treeitem.o \
	${O}/valuator.o \
	${O}/widget.o \
	${O}/window.o \
	${O}/wmhelper.o \
	${O}/wmmenu.o \
	${O}/wm.o 

.PHONY:
all: ${L}/libfdtui.a

${L}/libfdtui.a : ${TUIOBJ}
	${AR} rvc $@ $?
	${RANLIB} $@
	
${O}/hw.o : ${SCUR}/hw.c 
	redir -o $*.err -eo ${CC} -c ${CFLAGS} -o $@ ${SCUR}/hw.c
	
${O}/keyboard.o : ${SCUR}/keyboard.c  ${I}/keyboard.h
	redir -o $*.err -eo ${CC} -c ${CFLAGS} -o $@ ${SCUR}/keyboard.c
    
${O}/mouse.o : ${SCUR}/mouse.c ${I}/mouse.h
	redir -o $*.err -eo ${CC} -c ${CFLAGS} -o $@ ${SCUR}/mouse.c
    
${O}/video.o: ${SCUR}/video.c ${SCUR}/video.h
	redir -o $*.err -eo ${CC} -c ${CFLAGS} -o $@ ${SCUR}/video.c

${O}/cursor.o: ${SCUR}/cursor.c ${I}/cursor.h
	redir -o $*.err -eo ${CC} -c ${CFLAGS} -o $@ ${SCUR}/cursor.c 

${O}/filedlg.o: ${S}/filedlg.cpp ${I}/filedlg.hpp \
 ${I}/window.hpp ${I}/group.hpp ${I}/array.h \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/keyboard.h ${I}/cursor.h ${I}/label.hpp \
 ${I}/draw.h ${I}/screen.h ${I}/skin.h \
 ${I}/entry.hpp ${I}/checkbtn.hpp ${I}/button.hpp \
 ${I}/wm.hpp ${I}/listbox.hpp ${I}/scrllbar.hpp \
 ${I}/slider.hpp ${I}/valuator.hpp ${I}/combobox.hpp
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/filedlg.cpp
	
${O}/poll.o: ${S}/poll.c ${I}/poll.h ${I}/event.h \
 ${I}/keyboard.h
	redir -o $*.err -eo ${CC} -c ${CFLAGS} -o $@ ${S}/poll.c 

${O}/screen.o: ${SCUR}/screen.c ${I}/screen.h ${I}/cursor.h
	redir -o $*.err -eo ${CC} -c ${CFLAGS} -o $@ ${SCUR}/screen.c 

${O}/array.o: ${S}/array.c ${I}/array.h
	redir -o $*.err -eo ${CC} -c ${CFLAGS} -o $@ ${S}/array.c 

${O}/block.o: ${S}/block.c ${I}/block.h ${I}/box.h \
 ${I}/screen.h
	redir -o $*.err -eo ${CC} -c ${CFLAGS} -o $@ ${S}/block.c

${O}/box.o: ${S}/box.c ${I}/box.h
	redir -o $*.err -eo ${CC} -c ${CFLAGS} -o $@ ${S}/box.c 

${O}/button.o: ${S}/button.cpp ${I}/button.hpp ${I}/draw.h \
 ${I}/screen.h ${I}/skin.h ${I}/box.h \
 ${I}/widget.hpp ${I}/event.h ${I}/keyboard.h \
 ${I}/cursor.h ${I}/wm.hpp ${I}/window.hpp \
 ${I}/group.hpp ${I}/array.h ${I}/wmhelper.hpp
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/button.cpp 

${O}/checkbtn.o: ${S}/checkbtn.cpp ${I}/checkbtn.hpp \
 ${I}/button.hpp ${I}/draw.h ${I}/screen.h \
 ${I}/skin.h ${I}/box.h ${I}/widget.hpp \
 ${I}/event.h ${I}/keyboard.h ${I}/cursor.h \
 ${I}/wm.hpp ${I}/window.hpp ${I}/group.hpp \
 ${I}/array.h
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/checkbtn.cpp

${O}/clip.o: ${S}/clip.c ${I}/clip.h ${I}/box.h \
 ${I}/screen.h
	redir -o $*.err -eo ${CC} -c ${CFLAGS} -o $@ ${S}/clip.c 

${O}/combobox.o: ${S}/combobox.cpp ${I}/combobox.hpp \
 ${I}/entry.hpp ${I}/widget.hpp ${I}/box.h \
 ${I}/event.h ${I}/keyboard.h ${I}/cursor.h \
 ${I}/listbox.hpp ${I}/scrllbar.hpp ${I}/slider.hpp \
 ${I}/valuator.hpp ${I}/array.h ${I}/draw.h \
 ${I}/screen.h ${I}/skin.h ${I}/skin.h \
 ${I}/cursor.h ${I}/block.h ${I}/mouse.h \
 ${I}/wm.hpp ${I}/window.hpp ${I}/group.hpp
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/combobox.cpp 

${O}/draw.o: ${S}/draw.c ${I}/draw.h ${I}/screen.h \
 ${I}/skin.h ${I}/box.h ${I}/matrix.h ${I}/clip.h
	redir -o $*.err -eo ${CC} -c ${CFLAGS} -o $@ ${S}/draw.c 

${O}/edit.o: ${S}/edit.cpp ${I}/edit.hpp ${I}/cursor.h \
 ${I}/scrllbar.hpp ${I}/slider.hpp ${I}/valuator.hpp \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/keyboard.h ${I}/gapbuf.h ${I}/wm.hpp \
 ${I}/window.hpp ${I}/group.hpp ${I}/array.h \
 ${I}/skin.h ${I}/screen.h ${I}/draw.h \
 ${I}/skin.h ${I}/screen.h ${I}/matrix.h
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/edit.cpp 

${O}/entry.o: ${S}/entry.cpp ${I}/entry.hpp ${I}/widget.hpp \
 ${I}/box.h ${I}/event.h ${I}/keyboard.h \
 ${I}/cursor.h ${I}/wm.hpp ${I}/window.hpp \
 ${I}/group.hpp ${I}/array.h ${I}/draw.h \
 ${I}/screen.h ${I}/skin.h ${I}/skin.h
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/entry.cpp

${O}/fdtuicfg.o: ${S}/fdtuicfg.c ${I}/fdtuicfg.h ${I}/ini.h \
 ${I}/hash.h ${I}/skin.h ${I}/screen.h
	redir -o $*.err -eo ${CC} -c ${CFLAGS} -o $@ ${S}/fdtuicfg.c

${O}/file.o: ${S}/file.cpp ${I}/file.hpp ${I}/filedlg.hpp \
 ${I}/window.hpp ${I}/group.hpp ${I}/array.h \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/keyboard.h ${I}/cursor.h ${I}/label.hpp \
 ${I}/draw.h ${I}/screen.h ${I}/skin.h \
 ${I}/entry.hpp ${I}/checkbtn.hpp ${I}/button.hpp \
 ${I}/wm.hpp ${I}/listbox.hpp ${I}/scrllbar.hpp \
 ${I}/slider.hpp ${I}/valuator.hpp ${I}/combobox.hpp \
 ${I}/screen.h ${I}/wm.hpp
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/file.cpp

${O}/gapbuf.o: ${S}/gapbuf.c ${I}/gapbuf.h
	redir -o $*.err -eo ${CC} -c ${CFLAGS} -o $@ ${S}/gapbuf.c

${O}/group.o: ${S}/group.cpp ${I}/group.hpp ${I}/array.h \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/keyboard.h ${I}/cursor.h ${I}/skin.h \
 ${I}/screen.h ${I}/matrix.h ${I}/clip.h \
 ${I}/draw.h ${I}/skin.h ${I}/wm.hpp \
 ${I}/window.hpp ${I}/group.hpp
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/group.cpp 

${O}/hash.o: ${S}/hash.c ${I}/hash.h
	redir -o $*.err -eo ${CC} -c ${CFLAGS} -o $@ ${S}/hash.c 

${O}/ini.o: ${S}/ini.c ${I}/ini.h ${I}/hash.h
	redir -o $*.err -eo ${CC} -c ${CFLAGS} -o $@ ${S}/ini.c 

${O}/label.o: ${S}/label.cpp ${I}/label.hpp ${I}/draw.h \
 ${I}/screen.h ${I}/skin.h ${I}/box.h \
 ${I}/widget.hpp ${I}/event.h ${I}/keyboard.h \
 ${I}/cursor.h
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/label.cpp 

${O}/listbox.o: ${S}/listbox.cpp ${I}/listbox.hpp \
 ${I}/scrllbar.hpp ${I}/slider.hpp ${I}/valuator.hpp \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/keyboard.h ${I}/cursor.h ${I}/array.h \
 ${I}/skin.h ${I}/screen.h ${I}/draw.h \
 ${I}/skin.h ${I}/clip.h ${I}/matrix.h \
 ${I}/wm.hpp ${I}/window.hpp ${I}/group.hpp \
 ${I}/mouse.h
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/listbox.cpp

${O}/matrix.o: ${S}/matrix.c ${I}/matrix.h
	redir -o $*.err -eo ${CC} -c ${CFLAGS} -o $@ ${S}/matrix.c 

${O}/menubar.o: ${S}/menubar.cpp ${I}/menubar.hpp \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/keyboard.h ${I}/cursor.h ${I}/menuitem.h \
 ${I}/array.h ${I}/menu.hpp ${I}/window.hpp \
 ${I}/group.hpp ${I}/wm.hpp ${I}/skin.h \
 ${I}/screen.h ${I}/draw.h ${I}/skin.h \
 ${I}/mouse.h ${I}/block.h ${I}/poll.h
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/menubar.cpp 

${O}/menu.o: ${S}/menu.cpp ${I}/menu.hpp ${I}/window.hpp \
 ${I}/group.hpp ${I}/array.h ${I}/widget.hpp \
 ${I}/box.h ${I}/event.h ${I}/keyboard.h \
 ${I}/cursor.h ${I}/menuitem.h ${I}/mouse.h \
 ${I}/wm.hpp ${I}/wmhelper.hpp ${I}/screen.h \
 ${I}/block.h ${I}/skin.h ${I}/screen.h
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/menu.cpp 

${O}/menuitem.o: ${S}/menuitem.c ${I}/menuitem.h ${I}/array.h \
 ${I}/keyboard.h ${I}/screen.h
	redir -o $*.err -eo ${CC} -c ${CFLAGS} -o $@ ${S}/menuitem.c 

${O}/popup.o: ${S}/popup.cpp ${I}/popup.hpp ${I}/menu.hpp \
 ${I}/window.hpp ${I}/group.hpp ${I}/array.h \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/keyboard.h ${I}/cursor.h ${I}/menuitem.h \
 ${I}/wm.hpp ${I}/label.hpp ${I}/draw.h \
 ${I}/screen.h ${I}/skin.h ${I}/button.hpp \
 ${I}/mouse.h ${I}/block.h ${I}/screen.h
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/popup.cpp

${O}/radiobtn.o: ${S}/radiobtn.cpp ${I}/radiobtn.hpp \
 ${I}/checkbtn.hpp ${I}/button.hpp ${I}/draw.h \
 ${I}/screen.h ${I}/skin.h ${I}/box.h \
 ${I}/widget.hpp ${I}/event.h ${I}/keyboard.h \
 ${I}/cursor.h ${I}/wm.hpp ${I}/window.hpp \
 ${I}/group.hpp ${I}/array.h
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/radiobtn.cpp

${O}/scrllbar.o: ${S}/scrllbar.cpp ${I}/scrllbar.hpp \
 ${I}/slider.hpp ${I}/valuator.hpp ${I}/widget.hpp \
 ${I}/box.h ${I}/event.h ${I}/keyboard.h \
 ${I}/cursor.h ${I}/draw.h ${I}/screen.h \
 ${I}/skin.h
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/scrllbar.cpp

${O}/scroller.o: ${S}/scroller.cpp ${I}/scroller.hpp \
 ${I}/group.hpp ${I}/array.h ${I}/widget.hpp \
 ${I}/box.h ${I}/event.h ${I}/keyboard.h \
 ${I}/cursor.h ${I}/scrllbar.hpp ${I}/slider.hpp \
 ${I}/valuator.hpp ${I}/wm.hpp ${I}/window.hpp \
 ${I}/skin.h ${I}/screen.h ${I}/draw.h \
 ${I}/skin.h ${I}/matrix.h
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/scroller.cpp

${O}/skin.o: ${S}/skin.c ${I}/skin.h ${I}/screen.h
	redir -o $*.err -eo ${CC} -c ${CFLAGS} -o $@ ${S}/skin.c 

${O}/slider.o: ${S}/slider.cpp ${I}/slider.hpp ${I}/valuator.hpp \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/keyboard.h ${I}/cursor.h ${I}/draw.h \
 ${I}/screen.h ${I}/skin.h ${I}/window.hpp \
 ${I}/group.hpp ${I}/array.h
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/slider.cpp 

${O}/spinner.o: ${S}/spinner.cpp ${I}/spinner.hpp \
 ${I}/valuator.hpp ${I}/widget.hpp ${I}/box.h \
 ${I}/event.h ${I}/keyboard.h ${I}/cursor.h \
 ${I}/entry.hpp ${I}/draw.h ${I}/screen.h \
 ${I}/skin.h ${I}/skin.h
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/spinner.cpp

${O}/tree.o: ${S}/tree.cpp ${I}/tree.hpp ${I}/treeitem.hpp \
 ${I}/array.h ${I}/box.h ${I}/scrllbar.hpp \
 ${I}/slider.hpp ${I}/valuator.hpp ${I}/widget.hpp \
 ${I}/event.h ${I}/keyboard.h ${I}/cursor.h \
 ${I}/wm.hpp ${I}/window.hpp ${I}/group.hpp \
 ${I}/skin.h ${I}/screen.h ${I}/matrix.h \
 ${I}/clip.h ${I}/mouse.h ${I}/draw.h ${I}/skin.h
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/tree.cpp

${O}/treeitem.o: ${S}/treeitem.cpp ${I}/treeitem.hpp \
 ${I}/array.h ${I}/box.h
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/treeitem.cpp

${O}/valuator.o: ${S}/valuator.cpp ${I}/valuator.hpp \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/keyboard.h ${I}/cursor.h ${I}/wm.hpp \
 ${I}/window.hpp ${I}/group.hpp ${I}/array.h
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/valuator.cpp

${O}/widget.o: ${S}/widget.cpp ${I}/group.hpp ${I}/array.h \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/keyboard.h ${I}/cursor.h
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/widget.cpp 

${O}/window.o: ${S}/window.cpp ${I}/window.hpp ${I}/group.hpp \
 ${I}/array.h ${I}/widget.hpp ${I}/box.h \
 ${I}/event.h ${I}/keyboard.h ${I}/cursor.h \
 ${I}/wmhelper.hpp ${I}/screen.h ${I}/window.hpp
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/window.cpp

${O}/wm.o: ${S}/wm.cpp ${I}/wm.hpp ${I}/window.hpp \
 ${I}/group.hpp ${I}/array.h ${I}/widget.hpp \
 ${I}/box.h ${I}/event.h ${I}/keyboard.h \
 ${I}/cursor.h ${I}/wmhelper.hpp ${I}/screen.h \
 ${I}/hw.h ${I}/mouse.h ${I}/cursor.h \
 ${I}/event.h ${I}/array.h ${I}/screen.h \
 ${I}/clip.h ${I}/matrix.h ${I}/poll.h \
 ${I}/skin.h
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/wm.cpp 

${O}/wmhelper.o: ${S}/wmhelper.cpp ${I}/wmhelper.hpp \
 ${I}/screen.h ${I}/window.hpp ${I}/group.hpp \
 ${I}/array.h ${I}/widget.hpp ${I}/box.h \
 ${I}/event.h ${I}/keyboard.h ${I}/cursor.h \
 ${I}/wm.hpp ${I}/wmmenu.hpp ${I}/menuitem.h \
 ${I}/menu.hpp ${I}/mouse.h ${I}/block.h \
 ${I}/skin.h
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/wmhelper.cpp 

${O}/wmmenu.o: ${S}/wmmenu.cpp ${I}/wmmenu.hpp ${I}/menuitem.h \
 ${I}/array.h ${I}/keyboard.h ${I}/wmhelper.hpp \
 ${I}/screen.h ${I}/window.hpp ${I}/group.hpp \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/cursor.h
	redir -o $*.err -eo ${CXX} -c ${CXXFLAGS} -o $@ ${S}/wmmenu.cpp 

.PHONY:
clean:
	@rm -f ${O}/*.o
	@rm -f ${O}/**.err
	@rm -f ${L}/*.a
