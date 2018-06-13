# ncur.mak
#
# makefile for GNU Linux or BSD ncurses
#
# License CC0 PUBLIC DOMAIN
#
# To the extent possible under law, Mark J. Olesen has waived all copyright
# and related or neighboring rights to FDOSTUI Library. This work is published
# from: United States.
#
# Compiling:
#	change into fdostui/make directory
#       type `make -f ncur.mak'
#
#       ../obj/ncur directory will be populated with object files
#       ../lib/libfdtuicur.a is the library that will be created
#
# Compilation flags for external applications
#
#       -D__FLAT__ -D__CURSES__
#
# Linking:
#	applications will need to link in ncurses (-l curses)
#
# Installing:
#	change into fdostui/make directory
#	type `make -f ncur.mak && make [VARIABLE] install -f ncur.mak'
#		where VARIABLE is optional and can be any of the following:
#			PREFIX
#			LIBDIR
#			DESTDIR	
#		installing into system directories will need proper permission
#
# Uninstalling:
#	change into fdostui/make directory
#	type `make [VARIABLE] uninstall -f ncur.mak'
#		where VARIABLE is the exact parameters used for installing

# PREFIX	root directory to install FDOSTUI library and include files
#        	modify via command line: make PREFIX=/usr/local
#        	following are typical prefix's 
#            		OpenBSD: /usr/local
#            		FreeBSD: /usr/local
#			NetBSD: /usr/pkg
PREFIX ?=/usr

# LIBDIR	directory to install FDOSTUI library
#		modify via command line: make LIBDIR=/usr/lib64
LIBDIR ?=${PREFIX}/lib

# INCDIR	diretory to install FDOSTUI include files
INCDIR =${PREFIX}/include

# DESTDIR	override root directory
#		used to install FDOSTUI locally
#		if defined, must contain a trailing slash
#			make DESTDIR=/home/auser/

# DESTDIR_LIB	directory to install library
#		should not be modified directory
DESTDIR_LIB=${DESTDIR}${LIBDIR}

# DESTDIR_INC	directory to install include files
#		should not be modified directory
DESTDIR_INC=${DESTDIR}${INCDIR}/fdostui

O=../obj/ncur
B=../bin
S=../src
SCUR=../src/ncurses
L=../lib
I=../include

RANLIB=ranlib

CFLAGS=\
	-I${I}\
	-D__FLAT__\
	-D__CURSES__\
	-Wall\
	-ansi\
	-std=c99\
	-pedantic\
	-ggdb

CXXFLAGS=\
	-I${I}\
	-D__FLAT__\
	-D__CURSES__\
	-Wall\
	-ansi\
	-pedantic\
	-ggdb

TUIOBJ=\
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
all: ${L}/libfdtuicur.a

${L}/libfdtuicur.a : ${TUIOBJ}
	${AR} rvc $@ $?
	${RANLIB} $@

${O}/cursor.o: ${SCUR}/cursor.c ${I}/cursor.h
	${CC} -c ${CFLAGS} -o $@ ${SCUR}/cursor.c 2> $@.err

${O}/filedlg.o: ${SCUR}/filedlg.cpp ${I}/filedlg.hpp \
 ${I}/window.hpp ${I}/group.hpp ${I}/array.h \
 ${I}/widget.hpp ${I}/box.h ${I}/event.h \
 ${I}/keyboard.h ${I}/cursor.h ${I}/label.hpp \
 ${I}/draw.h ${I}/screen.h ${I}/skin.h \
 ${I}/entry.hpp ${I}/checkbtn.hpp ${I}/button.hpp \
 ${I}/wm.hpp ${I}/listbox.hpp ${I}/scrllbar.hpp \
 ${I}/slider.hpp ${I}/valuator.hpp ${I}/combobox.hpp
	${CXX} -c ${CXXFLAGS} -o $@ ${SCUR}/filedlg.cpp 2> $@.err

${O}/poll.o: ${SCUR}/poll.c ${I}/poll.h ${I}/event.h \
 ${I}/keyboard.h
	${CC} -c ${CFLAGS} -o $@ ${SCUR}/poll.c 2> $@.err

${O}/screen.o: ${SCUR}/screen.c ${I}/screen.h ${I}/cursor.h
	${CC} -c ${CFLAGS} -o $@ ${SCUR}/screen.c 2> $@.err

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
install:
	test -d ${DESTDIR_LIB} || mkdir -p ${DESTDIR_LIB}
	test -d ${DESTDIR_INC} || mkdir -p ${DESTDIR_INC}
	install -s -m 0755 ${L}/libfdtuicur.a ${DESTDIR_LIB}/libfdtuicur.a
	install -m 0644 ${I}/array.h ${DESTDIR_INC}/array.h
	install -m 0644 ${I}/block.h ${DESTDIR_INC}/block.h
	install -m 0644 ${I}/box.h ${DESTDIR_INC}/box.h
	install -m 0644 ${I}/button.hpp ${DESTDIR_INC}/button.hpp
	install -m 0644 ${I}/checkbtn.hpp ${DESTDIR_INC}/checkbtn.hpp
	install -m 0644 ${I}/clip.h ${DESTDIR_INC}/clip.h
	install -m 0644 ${I}/combobox.hpp ${DESTDIR_INC}/combobox.hpp
	install -m 0644 ${I}/cursor.h ${DESTDIR_INC}/cursor.h
	install -m 0644 ${I}/draw.h ${DESTDIR_INC}/draw.h
	install -m 0644 ${I}/edit.hpp ${DESTDIR_INC}/edit.hpp
	install -m 0644 ${I}/entry.hpp ${DESTDIR_INC}/entry.hpp
	install -m 0644 ${I}/event.h ${DESTDIR_INC}/event.h
	install -m 0644 ${I}/fdostui.hpp ${DESTDIR_INC}/fdostui.hpp
	install -m 0644 ${I}/filedlg.hpp ${DESTDIR_INC}/filedlg.hpp
	install -m 0644 ${I}/file.hpp ${DESTDIR_INC}/file.hpp
	install -m 0644 ${I}/gapbuf.h ${DESTDIR_INC}/gapbuf.h
	install -m 0644 ${I}/group.hpp ${DESTDIR_INC}/group.hpp
	install -m 0644 ${I}/hash.h ${DESTDIR_INC}/hash.h
	install -m 0644 ${I}/hw.h ${DESTDIR_INC}/hw.h
	install -m 0644 ${I}/ini.h ${DESTDIR_INC}/ini.h
	install -m 0644 ${I}/keyboard.h ${DESTDIR_INC}/keyboard.h
	install -m 0644 ${I}/label.hpp ${DESTDIR_INC}/label.h
	install -m 0644 ${I}/listbox.hpp ${DESTDIR_INC}/listbox.hpp
	install -m 0644 ${I}/matrix.h ${DESTDIR_INC}/matrix.h
	install -m 0644 ${I}/menubar.hpp ${DESTDIR_INC}/menubar.hpp
	install -m 0644 ${I}/menu.hpp ${DESTDIR_INC}/menu.hpp
	install -m 0644 ${I}/menuitem.h ${DESTDIR_INC}/menuitem.h
	install -m 0644 ${I}/mouse.h ${DESTDIR_INC}/mouse.h
	install -m 0644 ${I}/poll.h ${DESTDIR_INC}/poll.h
	install -m 0644 ${I}/popup.hpp ${DESTDIR_INC}/popup.hpp
	install -m 0644 ${I}/radiobtn.hpp ${DESTDIR_INC}/radiobtn.hpp
	install -m 0644 ${I}/screen.h ${DESTDIR_INC}/screen.h
	install -m 0644 ${I}/scrllbar.hpp ${DESTDIR_INC}/scrllbar.hpp
	install -m 0644 ${I}/scroller.hpp ${DESTDIR_INC}/scroller.hpp
	install -m 0644 ${I}/skin.h ${DESTDIR_INC}/skin.h
	install -m 0644 ${I}/slider.hpp ${DESTDIR_INC}/slider.hpp
	install -m 0644 ${I}/spinner.hpp ${DESTDIR_INC}/spinner.hpp
	install -m 0644 ${I}/tree.hpp ${DESTDIR_INC}/tree.hpp
	install -m 0644 ${I}/treeitem.hpp ${DESTDIR_INC}/treeitem.hpp
	install -m 0644 ${I}/valuator.hpp ${DESTDIR_INC}/valuator.hpp
	install -m 0644 ${I}/widget.hpp ${DESTDIR_INC}/widget.hpp
	install -m 0644 ${I}/window.hpp ${DESTDIR_INC}/window.hpp
	install -m 0644 ${I}/wmhelper.hpp ${DESTDIR_INC}/wmhelper.hpp
	install -m 0644 ${I}/wm.hpp ${DESTDIR_INC}/wm.hpp
	install -m 0644 ${I}/wmmenu.hpp ${DESTDIR_INC}/wmmenu.hpp

.PHONY:
uninstall:
	rm -f ${DESTDIR}${LIBDIR}/libfdtuicur.a
	rm -rf ${DESTDIR}${INCDIR}/fdostui

.PHONY:
clean:
	@rm -f ${O}/*.o
	@rm -f ${O}/*.err
	@rm -f ${L}/*.a
