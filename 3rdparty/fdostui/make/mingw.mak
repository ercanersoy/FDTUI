# mingw.mak
#
# makefile for Minimalist GNU for Windows (MINGW)
#
# License CC0 PUBLIC DOMAIN
#
# To the extent possible under law, Mark J. Olesen has waived all copyright
# and related or neighboring rights to FDOSTUI Library. This work is published
# from: United States.
#
# BUILD Windows 32-bit FDOSTUI Library 
# Requires MinGW GNU compiler collection gcc and g++ and gmake (GNUMake)
# Change into this directory and type 'make -f mingw.mak'
# 
# ../obj/mingw directory will be populated with object files
# ../lib/libfdtuimgw.a is the library that will be created
# 
# Compilation flags for external applications
# 
#     -D__NT__ -D__FLAT__

O=../obj/mingw
B=../bin
S=../src
S32=../src/win32
L=../lib
I=../include

UNAME:= $(shell uname)

ifeq ($(UNAME), Linux)
# MinGW running under Gnu/Linux (cross-compiling)
CROSSFLAGS=-D__NTCRUSTY__
CC=i686-w64-mingw32-gcc
CXX=i686-w64-mingw32-g++
AR=i686-w64-mingw32-ar
RANLIB=i686-w64-mingw32-ranlib
else
# MinGW running under Windows
CROSSFLAGS=
RANLIB=ranlib
endif

CFLAGS=\
	-I${I}\
	${CROSSFLAGS}\
	-D__NT__\
	-D__FLAT__\
	-Wall\
	-ansi\
	-std=c99\
	-pedantic\
	-ggdb

CXXFLAGS=\
	-I${I}\
	-D__NT__\
	-D__FLAT__\
	${CROSSFLAGS}\
	-Wall\
	-ansi\
	-pedantic\
	-ggdb

TUIOBJ=\
	${O}/hw.o \
	${O}/cursor.o \
	${O}/filedlg.o \
	${O}/poll.o \
	${O}/screen.o \
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
	${O}/inpdlg.o \
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
	${O}/strings.o \
	${O}/tree.o \
	${O}/treeitem.o \
	${O}/valuator.o \
	${O}/widget.o \
	${O}/window.o \
	${O}/wmhelper.o \
	${O}/wmmenu.o \
	${O}/wm.o 

.PHONY:
all: ${L}/libfdtuimgw.a

${L}/libfdtuimgw.a : ${TUIOBJ}
	${AR} rvc $@ $?
	${RANLIB} $@

${O}/hw.o : ${S32}/hw.c 
	${CC} -c ${CFLAGS} -o $@ ${S32}/hw.c 2> $@.err

${O}/cursor.o: ${S32}/cursor.c ${I}/cursor.h
	${CC} -c ${CFLAGS} -o $@ ${S32}/cursor.c 2> $@.err

${O}/filedlg.o: ${S32}/filedlg.cpp ${I}/filedlg.hpp \
 ${I}/window.hpp ${I}/group.hpp ${I}/array.h \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/keyboard.h ${I}/cursor.h ${I}/label.hpp \
 ${I}/draw.h ${I}/screen.h ${I}/skin.h \
 ${I}/entry.hpp ${I}/checkbtn.hpp ${I}/button.hpp \
 ${I}/wm.hpp ${I}/listbox.hpp ${I}/scrllbar.hpp \
 ${I}/slider.hpp ${I}/valuator.hpp ${I}/combobox.hpp
	${CXX} -c ${CXXFLAGS} -o $@ ${S32}/filedlg.cpp 2> $@.err

${O}/poll.o: ${S32}/poll.c ${I}/poll.h ${I}/event.h \
 ${I}/keyboard.h
	${CC} -c ${CFLAGS} -o $@ ${S32}/poll.c 2> $@.err

${O}/screen.o: ${S32}/screen.c ${I}/screen.h ${I}/cursor.h
	${CC} -c ${CFLAGS} -o $@ ${S32}/screen.c 2> $@.err

${O}/array.o: ${S}/array.c ${I}/array.h
	${CC} -c ${CFLAGS} -o $@ ${S}/array.c 2> $@.err

${O}/block.o: ${S}/block.c ${I}/block.h ${I}/box.h \
 ${I}/screen.h
	${CC} -c ${CFLAGS} -o $@ ${S}/block.c 2> $@.err

${O}/box.o: ${S}/box.c ${I}/box.h
	${CC} -c ${CFLAGS} -o $@ ${S}/box.c 2> $@.err

${O}/button.o: ${S}/button.cpp ${I}/button.hpp ${I}/draw.h \
 ${I}/screen.h ${I}/skin.h ${I}/box.h \
 ${I}/widget.hpp ${I}/event.h ${I}/keyboard.h \
 ${I}/cursor.h ${I}/wm.hpp ${I}/window.hpp \
 ${I}/group.hpp ${I}/array.h ${I}/wmhelper.hpp
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/button.cpp 2> $@.err

${O}/checkbtn.o: ${S}/checkbtn.cpp ${I}/checkbtn.hpp \
 ${I}/button.hpp ${I}/draw.h ${I}/screen.h \
 ${I}/skin.h ${I}/box.h ${I}/widget.hpp \
 ${I}/event.h ${I}/keyboard.h ${I}/cursor.h \
 ${I}/wm.hpp ${I}/window.hpp ${I}/group.hpp \
 ${I}/array.h
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/checkbtn.cpp 2> $@.err

${O}/clip.o: ${S}/clip.c ${I}/clip.h ${I}/box.h \
 ${I}/screen.h
	${CC} -c ${CFLAGS} -o $@ ${S}/clip.c 2> $@.err

${O}/combobox.o: ${S}/combobox.cpp ${I}/combobox.hpp \
 ${I}/entry.hpp ${I}/widget.hpp ${I}/box.h \
 ${I}/event.h ${I}/keyboard.h ${I}/cursor.h \
 ${I}/listbox.hpp ${I}/scrllbar.hpp ${I}/slider.hpp \
 ${I}/valuator.hpp ${I}/array.h ${I}/draw.h \
 ${I}/screen.h ${I}/skin.h ${I}/skin.h \
 ${I}/cursor.h ${I}/block.h ${I}/mouse.h \
 ${I}/wm.hpp ${I}/window.hpp ${I}/group.hpp
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/combobox.cpp 2> $@.err

${O}/draw.o: ${S}/draw.c ${I}/draw.h ${I}/screen.h \
 ${I}/skin.h ${I}/box.h ${I}/matrix.h ${I}/clip.h
	${CC} -c ${CFLAGS} -o $@ ${S}/draw.c 2> $@.err

${O}/edit.o: ${S}/edit.cpp ${I}/edit.hpp ${I}/cursor.h \
 ${I}/scrllbar.hpp ${I}/slider.hpp ${I}/valuator.hpp \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/keyboard.h ${I}/gapbuf.h ${I}/wm.hpp \
 ${I}/window.hpp ${I}/group.hpp ${I}/array.h \
 ${I}/skin.h ${I}/screen.h ${I}/draw.h \
 ${I}/skin.h ${I}/screen.h ${I}/matrix.h
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/edit.cpp 2> $@.err

${O}/entry.o: ${S}/entry.cpp ${I}/entry.hpp ${I}/widget.hpp \
 ${I}/box.h ${I}/event.h ${I}/keyboard.h \
 ${I}/cursor.h ${I}/wm.hpp ${I}/window.hpp \
 ${I}/group.hpp ${I}/array.h ${I}/draw.h \
 ${I}/screen.h ${I}/skin.h ${I}/skin.h
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/entry.cpp 2> $@.err

${O}/fdtuicfg.o: ${S}/fdtuicfg.c ${I}/fdtuicfg.h ${I}/ini.h \
 ${I}/hash.h ${I}/skin.h ${I}/screen.h
	${CC} -c ${CFLAGS} -o $@ ${S}/fdtuicfg.c 2> $@.err

${O}/file.o: ${S}/file.cpp ${I}/file.hpp ${I}/filedlg.hpp \
 ${I}/window.hpp ${I}/group.hpp ${I}/array.h \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/keyboard.h ${I}/cursor.h ${I}/label.hpp \
 ${I}/draw.h ${I}/screen.h ${I}/skin.h \
 ${I}/entry.hpp ${I}/checkbtn.hpp ${I}/button.hpp \
 ${I}/wm.hpp ${I}/listbox.hpp ${I}/scrllbar.hpp \
 ${I}/slider.hpp ${I}/valuator.hpp ${I}/combobox.hpp \
 ${I}/screen.h ${I}/wm.hpp ${I}/strings.h
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/file.cpp 2> $@.err

${O}/gapbuf.o: ${S}/gapbuf.c ${I}/gapbuf.h
	${CC} -c ${CFLAGS} -o $@ ${S}/gapbuf.c 2> $@.err

${O}/group.o: ${S}/group.cpp ${I}/group.hpp ${I}/array.h \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/keyboard.h ${I}/cursor.h ${I}/skin.h \
 ${I}/screen.h ${I}/matrix.h ${I}/clip.h \
 ${I}/draw.h ${I}/skin.h ${I}/wm.hpp \
 ${I}/window.hpp ${I}/group.hpp
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/group.cpp 2> $@.err

${O}/hash.o: ${S}/hash.c ${I}/hash.h
	${CC} -c ${CFLAGS} -o $@ ${S}/hash.c 2> $@.err

${O}/ini.o: ${S}/ini.c ${I}/ini.h ${I}/hash.h
	${CC} -c ${CFLAGS} -o $@ ${S}/ini.c 2> $@.err

${O}/inpdlg.o: ${S}/inpdlg.cpp ${I}/inpdlg.hpp ${I}/window.hpp \
 ${I}/group.hpp ${I}/array.h ${I}/widget.hpp \
 ${I}/box.h ${I}/event.h ${I}/keyboard.h \
 ${I}/cursor.h ${I}/label.hpp ${I}/draw.h \
 ${I}/screen.h ${I}/skin.h ${I}/entry.hpp \
 ${I}/button.hpp
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/inpdlg.cpp 2> $@.err

${O}/label.o: ${S}/label.cpp ${I}/label.hpp ${I}/draw.h \
 ${I}/screen.h ${I}/skin.h ${I}/box.h \
 ${I}/widget.hpp ${I}/event.h ${I}/keyboard.h \
 ${I}/cursor.h
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/label.cpp 2> $@.err

${O}/listbox.o: ${S}/listbox.cpp ${I}/listbox.hpp \
 ${I}/scrllbar.hpp ${I}/slider.hpp ${I}/valuator.hpp \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/keyboard.h ${I}/cursor.h ${I}/array.h \
 ${I}/skin.h ${I}/screen.h ${I}/draw.h \
 ${I}/skin.h ${I}/clip.h ${I}/matrix.h \
 ${I}/wm.hpp ${I}/window.hpp ${I}/group.hpp \
 ${I}/mouse.h
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/listbox.cpp 2> $@.err

${O}/matrix.o: ${S}/matrix.c ${I}/matrix.h
	${CC} -c ${CFLAGS} -o $@ ${S}/matrix.c 2> $@.err

${O}/menubar.o: ${S}/menubar.cpp ${I}/menubar.hpp \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/keyboard.h ${I}/cursor.h ${I}/menuitem.h \
 ${I}/array.h ${I}/menu.hpp ${I}/window.hpp \
 ${I}/group.hpp ${I}/wm.hpp ${I}/skin.h \
 ${I}/screen.h ${I}/draw.h ${I}/skin.h \
 ${I}/mouse.h ${I}/block.h ${I}/poll.h
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/menubar.cpp 2> $@.err

${O}/menu.o: ${S}/menu.cpp ${I}/menu.hpp ${I}/window.hpp \
 ${I}/group.hpp ${I}/array.h ${I}/widget.hpp \
 ${I}/box.h ${I}/event.h ${I}/keyboard.h \
 ${I}/cursor.h ${I}/menuitem.h ${I}/mouse.h \
 ${I}/wm.hpp ${I}/wmhelper.hpp ${I}/screen.h \
 ${I}/block.h ${I}/skin.h ${I}/screen.h
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/menu.cpp 2> $@.err

${O}/menuitem.o: ${S}/menuitem.c ${I}/menuitem.h ${I}/array.h \
 ${I}/keyboard.h ${I}/screen.h
	${CC} -c ${CFLAGS} -o $@ ${S}/menuitem.c 2> $@.err

${O}/popup.o: ${S}/popup.cpp ${I}/popup.hpp ${I}/menu.hpp \
 ${I}/window.hpp ${I}/group.hpp ${I}/array.h \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/keyboard.h ${I}/cursor.h ${I}/menuitem.h \
 ${I}/wm.hpp ${I}/label.hpp ${I}/draw.h \
 ${I}/screen.h ${I}/skin.h ${I}/button.hpp \
 ${I}/mouse.h ${I}/block.h ${I}/screen.h
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/popup.cpp 2> $@.err

${O}/radiobtn.o: ${S}/radiobtn.cpp ${I}/radiobtn.hpp \
 ${I}/checkbtn.hpp ${I}/button.hpp ${I}/draw.h \
 ${I}/screen.h ${I}/skin.h ${I}/box.h \
 ${I}/widget.hpp ${I}/event.h ${I}/keyboard.h \
 ${I}/cursor.h ${I}/wm.hpp ${I}/window.hpp \
 ${I}/group.hpp ${I}/array.h
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/radiobtn.cpp 2> $@.err

${O}/scrllbar.o: ${S}/scrllbar.cpp ${I}/scrllbar.hpp \
 ${I}/slider.hpp ${I}/valuator.hpp ${I}/widget.hpp \
 ${I}/box.h ${I}/event.h ${I}/keyboard.h \
 ${I}/cursor.h ${I}/draw.h ${I}/screen.h \
 ${I}/skin.h
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/scrllbar.cpp 2> $@.err

${O}/scroller.o: ${S}/scroller.cpp ${I}/scroller.hpp \
 ${I}/group.hpp ${I}/array.h ${I}/widget.hpp \
 ${I}/box.h ${I}/event.h ${I}/keyboard.h \
 ${I}/cursor.h ${I}/scrllbar.hpp ${I}/slider.hpp \
 ${I}/valuator.hpp ${I}/wm.hpp ${I}/window.hpp \
 ${I}/skin.h ${I}/screen.h ${I}/draw.h \
 ${I}/skin.h ${I}/matrix.h
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/scroller.cpp 2> $@.err

${O}/skin.o: ${S}/skin.c ${I}/skin.h ${I}/screen.h
	${CC} -c ${CFLAGS} -o $@ ${S}/skin.c 2> $@.err

${O}/slider.o: ${S}/slider.cpp ${I}/slider.hpp ${I}/valuator.hpp \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/keyboard.h ${I}/cursor.h ${I}/draw.h \
 ${I}/screen.h ${I}/skin.h ${I}/window.hpp \
 ${I}/group.hpp ${I}/array.h
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/slider.cpp 2> $@.err

${O}/spinner.o: ${S}/spinner.cpp ${I}/spinner.hpp \
 ${I}/valuator.hpp ${I}/widget.hpp ${I}/box.h \
 ${I}/event.h ${I}/keyboard.h ${I}/cursor.h \
 ${I}/entry.hpp ${I}/draw.h ${I}/screen.h \
 ${I}/skin.h ${I}/skin.h
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/spinner.cpp 2> $@.err

${O}/strings.o: ${S}/strings.c ${I}/strings.h
	${CC} -c ${CFLAGS} -o $@ ${S}/strings.c 2> $@.err

${O}/tree.o: ${S}/tree.cpp ${I}/tree.hpp ${I}/treeitem.hpp \
 ${I}/array.h ${I}/box.h ${I}/scrllbar.hpp \
 ${I}/slider.hpp ${I}/valuator.hpp ${I}/widget.hpp \
 ${I}/event.h ${I}/keyboard.h ${I}/cursor.h \
 ${I}/wm.hpp ${I}/window.hpp ${I}/group.hpp \
 ${I}/skin.h ${I}/screen.h ${I}/matrix.h \
 ${I}/clip.h ${I}/mouse.h ${I}/draw.h ${I}/skin.h
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/tree.cpp 2> $@.err

${O}/treeitem.o: ${S}/treeitem.cpp ${I}/treeitem.hpp \
 ${I}/array.h ${I}/box.h
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/treeitem.cpp 2> $@.err

${O}/valuator.o: ${S}/valuator.cpp ${I}/valuator.hpp \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/keyboard.h ${I}/cursor.h ${I}/wm.hpp \
 ${I}/window.hpp ${I}/group.hpp ${I}/array.h
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/valuator.cpp 2> $@.err

${O}/widget.o: ${S}/widget.cpp ${I}/group.hpp ${I}/array.h \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/keyboard.h ${I}/cursor.h
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/widget.cpp 2> $@.err

${O}/window.o: ${S}/window.cpp ${I}/window.hpp ${I}/group.hpp \
 ${I}/array.h ${I}/widget.hpp ${I}/box.h \
 ${I}/event.h ${I}/keyboard.h ${I}/cursor.h \
 ${I}/wmhelper.hpp ${I}/screen.h ${I}/window.hpp
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/window.cpp 2> $@.err

${O}/wm.o: ${S}/wm.cpp ${I}/wm.hpp ${I}/window.hpp \
 ${I}/group.hpp ${I}/array.h ${I}/widget.hpp \
 ${I}/box.h ${I}/event.h ${I}/keyboard.h \
 ${I}/cursor.h ${I}/wmhelper.hpp ${I}/screen.h \
 ${I}/hw.h ${I}/mouse.h ${I}/cursor.h \
 ${I}/event.h ${I}/array.h ${I}/screen.h \
 ${I}/clip.h ${I}/matrix.h ${I}/poll.h \
 ${I}/skin.h
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/wm.cpp 2> $@.err

${O}/wmhelper.o: ${S}/wmhelper.cpp ${I}/wmhelper.hpp \
 ${I}/screen.h ${I}/window.hpp ${I}/group.hpp \
 ${I}/array.h ${I}/widget.hpp ${I}/box.h \
 ${I}/event.h ${I}/keyboard.h ${I}/cursor.h \
 ${I}/wm.hpp ${I}/wmmenu.hpp ${I}/menuitem.h \
 ${I}/menu.hpp ${I}/mouse.h ${I}/block.h \
 ${I}/skin.h
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/wmhelper.cpp 2> $@.err

${O}/wmmenu.o: ${S}/wmmenu.cpp ${I}/wmmenu.hpp ${I}/menuitem.h \
 ${I}/array.h ${I}/keyboard.h ${I}/wmhelper.hpp \
 ${I}/screen.h ${I}/window.hpp ${I}/group.hpp \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/cursor.h
	${CXX} -c ${CXXFLAGS} -o $@ ${S}/wmmenu.cpp 2> $@.err

.PHONY:
clean:
	@rm -f ${O}/*.o
	@rm -f ${O}/*.err
	@rm -f ${L}/*.a
