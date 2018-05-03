# depend.mak
#
# makefile to create FDOSTUI dependencies
#
# requires g++ and gmake (GNUMake)
# output is not required to build FDOSTUI
#
# License CC0 PUBLIC DOMAIN
#
# To the extent possible under law, Mark J. Olesen has waived all copyright
# and related or neighboring rights to FDOSTUI Library. This work is published
# from: United States.

O=../obj/ncur
B=../bin
S=../src
SCUR=../src/ncurses
SW32=../src/win32
L=../lib
I=../include

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

CORE=\
	${S}/array.c\
	${S}/block.c\
	${S}/box.c\
	${S}/button.cpp\
	${S}/checkbtn.cpp\
	${S}/clip.c\
	${S}/combobox.cpp\
	${S}/draw.c\
	${S}/edit.cpp\
	${S}/entry.cpp\
	${S}/fdtuicfg.c\
	${S}/file.cpp\
	${S}/gapbuf.c\
	${S}/group.cpp\
	${S}/hash.c\
	${S}/ini.c\
	${S}/label.cpp\
	${S}/listbox.cpp\
	${S}/matrix.c\
	${S}/menubar.cpp\
	${S}/menu.cpp\
	${S}/menuitem.c\
	${S}/popup.cpp\
	${S}/radiobtn.cpp\
	${S}/scrllbar.cpp\
	${S}/scroller.cpp\
	${S}/skin.c\
	${S}/slider.cpp\
	${S}/spinner.cpp\
	${S}/tree.cpp\
	${S}/treeitem.cpp\
	${S}/valuator.cpp\
	${S}/widget.cpp\
	${S}/window.cpp\
	${S}/wm.cpp\
	${S}/wmhelper.cpp\
	${S}/wmmenu.cpp

WIN32=\
	${SW32}/cursor.c\
	${SW32}/hw.c\
	${SW32}/poll.c\
	${SW32}/screen.c

NCUR=\
	${SCUR}/cursor.c\
	${SCUR}/filedlg.cpp\
	${SCUR}/poll.c\
	${SCUR}/screen.c

all: core.mk win32.mk ncur.mk

core.mk : ${CORE}
	rm -f core.mk
	${CXX} -c ${CXXFLAGS} -MM  $^ >> core.mk
	sed -i "s/..\/src/$$\{S\}/g" core.mk
	sed -i "s/..\/include/$$\{I\}/g" core.mk

win32.mk : ${WIN32}
	rm -f win32.mk
	${CXX} -c ${CXXFLAGS} -MM  $^ >> win32.mk
	sed -i "s/..\/src\/win32/$$\{S32\}/g" win32.mk
	sed -i "s/..\/include/$$\{I\}/g" win32.mk

ncur.mk : ${NCUR}
	rm -f ncur.mk
	${CXX} -c ${CXXFLAGS} -MM  $^ >> ncur.mk
	sed -i "s/..\/src\/ncurses/$$\{SCUR\}/g" ncur.mk
	sed -i "s/..\/include/$$\{I\}/g" ncur.mk
