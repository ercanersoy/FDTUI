# vs.mak
#
# makefile for Visual Studio Command line tools
#
# License CC0 PUBLIC DOMAIN
#
# To the extent possible under law, Mark J. Olesen has waived all copyright
# and related or neighboring rights to FDOSTUI Library. This work is published
# from: United States.
#
# Build 32-bit Windows FDOSTUI Library
# Windows Host Operating System
# Requries Visual Studio command line tools
# 
# For example, in a command prompt and depending upon Visual Studio version
# you can type:
# 
#     > cd C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC
#     > vcvarsall.bat
# 
# This will load the command line enviroment
# 
# Change into this directory and type 'nmake /F vs.mak'
# 
# ..\obj\vs32 directory will be populated with object files
# ..\lib\fdtuivs.lib is the library that will be created
# 
# Compilation flags for external applications
# 
#     /D__NT__ /D__FLAT__

O=..\obj\vs32
B=..\bin
S=..\src
S32=..\src\win32
L=..\lib
I=..\include

CC=cl
CXX=cl

CFLAGS= /I $(I) /c /Z7 /D__NT__ /D__FLAT__
CXXFLAGS= /I $(I) /c /Z7 /D__NT__ /D__FLAT__

TUIOBJ=\
	$(O)\hw.obj \
	$(O)\screen.obj \
	$(O)\cursor.obj \
	$(O)\filedlg.obj \
	$(O)\poll.obj \
	$(O)\array.obj \
	$(O)\block.obj \
	$(O)\box.obj \
	$(O)\button.obj \
	$(O)\checkbtn.obj \
	$(O)\clip.obj \
	$(O)\combobox.obj \
	$(O)\draw.obj \
	$(O)\edit.obj \
	$(O)\entry.obj \
	$(O)\fdtuicfg.obj \
	$(O)\file.obj \
	$(O)\gapbuf.obj \
	$(O)\group.obj \
	$(O)\hash.obj \
	$(O)\ini.obj \
	$(O)\inpdlg.obj \
	$(O)\label.obj \
	$(O)\listbox.obj \
	$(O)\matrix.obj \
	$(O)\menubar.obj \
	$(O)\menuitem.obj \
	$(O)\menu.obj \
	$(O)\popup.obj \
	$(O)\radiobtn.obj \
	$(O)\scrllbar.obj \
	$(O)\scroller.obj \
	$(O)\skin.obj \
	$(O)\slider.obj  \
	$(O)\spinner.obj \
	$(O)\strings.obj \
	$(O)\tree.obj \
	$(O)\treeitem.obj \
	$(O)\valuator.obj \
	$(O)\widget.obj \
	$(O)\window.obj \
	$(O)\wmhelper.obj \
	$(O)\wmmenu.obj \
	$(O)\wm.obj 

all: $(L)\fdtuivs.lib

$(L)\fdtuivs.lib: $(TUIOBJ)
	lib /OUT:$@ @vs.lbc

$(O)\hw.obj : $(S32)\hw.c 
	$(CC) $(CFLAGS) /Fo$@ $(S32)\hw.c
    
$(O)\screen.obj : $(S32)\screen.c 
	$(CC) $(CFLAGS) /Fo$@ $(S32)\screen.c

$(O)\cursor.obj: $(S32)\cursor.c $(I)\cursor.h
	$(CC) $(CFLAGS) /Fo$@ $(S32)\cursor.c

$(O)\filedlg.obj: $(S32)\filedlg.cpp $(I)\filedlg.hpp \
 $(I)\window.hpp $(I)\group.hpp $(I)\array.h \
 $(I)\widget.hpp $(I)\box.h $(I)\event.h \
 $(I)\keyboard.h $(I)\cursor.h $(I)\label.hpp \
 $(I)\draw.h $(I)\screen.h $(I)\skin.h \
 $(I)\entry.hpp $(I)\checkbtn.hpp $(I)\button.hpp \
 $(I)\wm.hpp $(I)\listbox.hpp $(I)\scrllbar.hpp \
 $(I)\slider.hpp $(I)\valuator.hpp $(I)\combobox.hpp
	$(CXX) $(CXXFLAGS) /Fo$@ $(S32)\filedlg.cpp

$(O)\poll.obj: $(S32)\poll.c $(I)\poll.h $(I)\event.h \
 $(I)\keyboard.h
	$(CC) $(CFLAGS) /Fo$@ $(S32)\poll.c

$(O)\array.obj: $(S)\array.c $(I)\array.h
	$(CC) $(CFLAGS) /Fo$@ $(S)\array.c

$(O)\block.obj: $(S)\block.c $(I)\block.h $(I)\box.h \
 $(I)\screen.h
	$(CC) $(CFLAGS) /Fo$@ $(S)\block.c

$(O)\box.obj: $(S)\box.c $(I)\box.h
	$(CC) $(CFLAGS) /Fo$@ $(S)\box.c

$(O)\button.obj: $(S)\button.cpp $(I)\button.hpp $(I)\draw.h \
 $(I)\screen.h $(I)\skin.h $(I)\box.h \
 $(I)\widget.hpp $(I)\event.h $(I)\keyboard.h \
 $(I)\cursor.h $(I)\wm.hpp $(I)\window.hpp \
 $(I)\group.hpp $(I)\array.h $(I)\wmhelper.hpp
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\button.cpp

$(O)\checkbtn.obj: $(S)\checkbtn.cpp $(I)\checkbtn.hpp \
 $(I)\button.hpp $(I)\draw.h $(I)\screen.h \
 $(I)\skin.h $(I)\box.h $(I)\widget.hpp \
 $(I)\event.h $(I)\keyboard.h $(I)\cursor.h \
 $(I)\wm.hpp $(I)\window.hpp $(I)\group.hpp \
 $(I)\array.h
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\checkbtn.cpp

$(O)\clip.obj: $(S)\clip.c $(I)\clip.h $(I)\box.h \
 $(I)\screen.h
	$(CC) $(CFLAGS) /Fo$@ $(S)\clip.c

$(O)\combobox.obj: $(S)\combobox.cpp $(I)\combobox.hpp \
 $(I)\entry.hpp $(I)\widget.hpp $(I)\box.h \
 $(I)\event.h $(I)\keyboard.h $(I)\cursor.h \
 $(I)\listbox.hpp $(I)\scrllbar.hpp $(I)\slider.hpp \
 $(I)\valuator.hpp $(I)\array.h $(I)\draw.h \
 $(I)\screen.h $(I)\skin.h $(I)\skin.h \
 $(I)\cursor.h $(I)\block.h $(I)\mouse.h \
 $(I)\wm.hpp $(I)\window.hpp $(I)\group.hpp
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\combobox.cpp

$(O)\draw.obj: $(S)\draw.c $(I)\draw.h $(I)\screen.h \
 $(I)\skin.h $(I)\box.h $(I)\matrix.h $(I)\clip.h
	$(CC) $(CFLAGS) /Fo$@ $(S)\draw.c

$(O)\edit.obj: $(S)\edit.cpp $(I)\edit.hpp $(I)\cursor.h \
 $(I)\scrllbar.hpp $(I)\slider.hpp $(I)\valuator.hpp \
 $(I)\widget.hpp $(I)\box.h $(I)\event.h \
 $(I)\keyboard.h $(I)\gapbuf.h $(I)\wm.hpp \
 $(I)\window.hpp $(I)\group.hpp $(I)\array.h \
 $(I)\skin.h $(I)\screen.h $(I)\draw.h \
 $(I)\skin.h $(I)\screen.h $(I)\matrix.h
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\edit.cpp

$(O)\entry.obj: $(S)\entry.cpp $(I)\entry.hpp $(I)\widget.hpp \
 $(I)\box.h $(I)\event.h $(I)\keyboard.h \
 $(I)\cursor.h $(I)\wm.hpp $(I)\window.hpp \
 $(I)\group.hpp $(I)\array.h $(I)\draw.h \
 $(I)\screen.h $(I)\skin.h $(I)\skin.h
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\entry.cpp

$(O)\fdtuicfg.obj: $(S)\fdtuicfg.c $(I)\fdtuicfg.h $(I)\ini.h \
 $(I)\hash.h $(I)\skin.h $(I)\screen.h
	$(CC) $(CFLAGS) /Fo$@ $(S)\fdtuicfg.c

$(O)\file.obj: $(S)\file.cpp $(I)\file.hpp $(I)\filedlg.hpp \
 $(I)\window.hpp $(I)\group.hpp $(I)\array.h \
 $(I)\widget.hpp $(I)\box.h $(I)\event.h \
 $(I)\keyboard.h $(I)\cursor.h $(I)\label.hpp \
 $(I)\draw.h $(I)\screen.h $(I)\skin.h \
 $(I)\entry.hpp $(I)\checkbtn.hpp $(I)\button.hpp \
 $(I)\wm.hpp $(I)\listbox.hpp $(I)\scrllbar.hpp \
 $(I)\slider.hpp $(I)\valuator.hpp $(I)\combobox.hpp \
 $(I)\screen.h $(I)\wm.hpp $(I)\strings.h
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\file.cpp

$(O)\gapbuf.obj: $(S)\gapbuf.c $(I)\gapbuf.h
	$(CC) $(CFLAGS) /Fo$@ $(S)\gapbuf.c

$(O)\group.obj: $(S)\group.cpp $(I)\group.hpp $(I)\array.h \
 $(I)\widget.hpp $(I)\box.h $(I)\event.h \
 $(I)\keyboard.h $(I)\cursor.h $(I)\skin.h \
 $(I)\screen.h $(I)\matrix.h $(I)\clip.h \
 $(I)\draw.h $(I)\skin.h $(I)\wm.hpp \
 $(I)\window.hpp $(I)\group.hpp
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\group.cpp

$(O)\hash.obj: $(S)\hash.c $(I)\hash.h
	$(CC) $(CFLAGS) /Fo$@ $(S)\hash.c

$(O)\ini.obj: $(S)\ini.c $(I)\ini.h $(I)\hash.h
	$(CC) $(CFLAGS) /Fo$@ $(S)\ini.c

$(O)\inpdlg.obj: $(S)\inpdlg.cpp $(I)\inpdlg.hpp $(I)\window.hpp \
 $(I)\group.hpp $(I)\array.h $(I)\widget.hpp \
 $(I)\box.h $(I)\event.h $(I)\keyboard.h \
 $(I)\cursor.h $(I)\label.hpp $(I)\draw.h \
 $(I)\screen.h $(I)\skin.h $(I)\entry.hpp \
 $(I)\button.hpp
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\inpdlg.cpp

$(O)\label.obj: $(S)\label.cpp $(I)\label.hpp $(I)\draw.h \
 $(I)\screen.h $(I)\skin.h $(I)\box.h \
 $(I)\widget.hpp $(I)\event.h $(I)\keyboard.h \
 $(I)\cursor.h
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\label.cpp

$(O)\listbox.obj: $(S)\listbox.cpp $(I)\listbox.hpp \
 $(I)\scrllbar.hpp $(I)\slider.hpp $(I)\valuator.hpp \
 $(I)\widget.hpp $(I)\box.h $(I)\event.h \
 $(I)\keyboard.h $(I)\cursor.h $(I)\array.h \
 $(I)\skin.h $(I)\screen.h $(I)\draw.h \
 $(I)\skin.h $(I)\clip.h $(I)\matrix.h \
 $(I)\wm.hpp $(I)\window.hpp $(I)\group.hpp \
 $(I)\mouse.h
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\listbox.cpp

$(O)\matrix.obj: $(S)\matrix.c $(I)\matrix.h
	$(CC) $(CFLAGS) /Fo$@ $(S)\matrix.c

$(O)\menubar.obj: $(S)\menubar.cpp $(I)\menubar.hpp \
 $(I)\widget.hpp $(I)\box.h $(I)\event.h \
 $(I)\keyboard.h $(I)\cursor.h $(I)\menuitem.h \
 $(I)\array.h $(I)\menu.hpp $(I)\window.hpp \
 $(I)\group.hpp $(I)\wm.hpp $(I)\skin.h \
 $(I)\screen.h $(I)\draw.h $(I)\skin.h \
 $(I)\mouse.h $(I)\block.h $(I)\poll.h
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\menubar.cpp

$(O)\menu.obj: $(S)\menu.cpp $(I)\menu.hpp $(I)\window.hpp \
 $(I)\group.hpp $(I)\array.h $(I)\widget.hpp \
 $(I)\box.h $(I)\event.h $(I)\keyboard.h \
 $(I)\cursor.h $(I)\menuitem.h $(I)\mouse.h \
 $(I)\wm.hpp $(I)\wmhelper.hpp $(I)\screen.h \
 $(I)\block.h $(I)\skin.h $(I)\screen.h
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\menu.cpp

$(O)\menuitem.obj: $(S)\menuitem.c $(I)\menuitem.h $(I)\array.h \
 $(I)\keyboard.h $(I)\screen.h
	$(CC) $(CFLAGS) /Fo$@ $(S)\menuitem.c

$(O)\popup.obj: $(S)\popup.cpp $(I)\popup.hpp $(I)\menu.hpp \
 $(I)\window.hpp $(I)\group.hpp $(I)\array.h \
 $(I)\widget.hpp $(I)\box.h $(I)\event.h \
 $(I)\keyboard.h $(I)\cursor.h $(I)\menuitem.h \
 $(I)\wm.hpp $(I)\label.hpp $(I)\draw.h \
 $(I)\screen.h $(I)\skin.h $(I)\button.hpp \
 $(I)\mouse.h $(I)\block.h $(I)\screen.h
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\popup.cpp

$(O)\radiobtn.obj: $(S)\radiobtn.cpp $(I)\radiobtn.hpp \
 $(I)\checkbtn.hpp $(I)\button.hpp $(I)\draw.h \
 $(I)\screen.h $(I)\skin.h $(I)\box.h \
 $(I)\widget.hpp $(I)\event.h $(I)\keyboard.h \
 $(I)\cursor.h $(I)\wm.hpp $(I)\window.hpp \
 $(I)\group.hpp $(I)\array.h
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\radiobtn.cpp

$(O)\scrllbar.obj: $(S)\scrllbar.cpp $(I)\scrllbar.hpp \
 $(I)\slider.hpp $(I)\valuator.hpp $(I)\widget.hpp \
 $(I)\box.h $(I)\event.h $(I)\keyboard.h \
 $(I)\cursor.h $(I)\draw.h $(I)\screen.h \
 $(I)\skin.h
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\scrllbar.cpp

$(O)\scroller.obj: $(S)\scroller.cpp $(I)\scroller.hpp \
 $(I)\group.hpp $(I)\array.h $(I)\widget.hpp \
 $(I)\box.h $(I)\event.h $(I)\keyboard.h \
 $(I)\cursor.h $(I)\scrllbar.hpp $(I)\slider.hpp \
 $(I)\valuator.hpp $(I)\wm.hpp $(I)\window.hpp \
 $(I)\skin.h $(I)\screen.h $(I)\draw.h \
 $(I)\skin.h $(I)\matrix.h
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\scroller.cpp

$(O)\skin.obj: $(S)\skin.c $(I)\skin.h $(I)\screen.h
	$(CC) $(CFLAGS) /Fo$@ $(S)\skin.c

$(O)\slider.obj: $(S)\slider.cpp $(I)\slider.hpp $(I)\valuator.hpp \
 $(I)\widget.hpp $(I)\box.h $(I)\event.h \
 $(I)\keyboard.h $(I)\cursor.h $(I)\draw.h \
 $(I)\screen.h $(I)\skin.h $(I)\window.hpp \
 $(I)\group.hpp $(I)\array.h
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\slider.cpp

$(O)\spinner.obj: $(S)\spinner.cpp $(I)\spinner.hpp \
 $(I)\valuator.hpp $(I)\widget.hpp $(I)\box.h \
 $(I)\event.h $(I)\keyboard.h $(I)\cursor.h \
 $(I)\entry.hpp $(I)\draw.h $(I)\screen.h \
 $(I)\skin.h $(I)\skin.h
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\spinner.cpp

$(O)\strings.obj: $(S)\strings.c $(I)\strings.h
	$(CC) $(CFLAGS) /Fo$@ $(S)\strings.c

$(O)\tree.obj: $(S)\tree.cpp $(I)\tree.hpp $(I)\treeitem.hpp \
 $(I)\array.h $(I)\box.h $(I)\scrllbar.hpp \
 $(I)\slider.hpp $(I)\valuator.hpp $(I)\widget.hpp \
 $(I)\event.h $(I)\keyboard.h $(I)\cursor.h \
 $(I)\wm.hpp $(I)\window.hpp $(I)\group.hpp \
 $(I)\skin.h $(I)\screen.h $(I)\matrix.h \
 $(I)\clip.h $(I)\mouse.h $(I)\draw.h $(I)\skin.h
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\tree.cpp

$(O)\treeitem.obj: $(S)\treeitem.cpp $(I)\treeitem.hpp \
 $(I)\array.h $(I)\box.h
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\treeitem.cpp

$(O)\valuator.obj: $(S)\valuator.cpp $(I)\valuator.hpp \
 $(I)\widget.hpp $(I)\box.h $(I)\event.h \
 $(I)\keyboard.h $(I)\cursor.h $(I)\wm.hpp \
 $(I)\window.hpp $(I)\group.hpp $(I)\array.h
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\valuator.cpp

$(O)\widget.obj: $(S)\widget.cpp $(I)\group.hpp $(I)\array.h \
 $(I)\widget.hpp $(I)\box.h $(I)\event.h \
 $(I)\keyboard.h $(I)\cursor.h
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\widget.cpp

$(O)\window.obj: $(S)\window.cpp $(I)\window.hpp $(I)\group.hpp \
 $(I)\array.h $(I)\widget.hpp $(I)\box.h \
 $(I)\event.h $(I)\keyboard.h $(I)\cursor.h \
 $(I)\wmhelper.hpp $(I)\screen.h $(I)\window.hpp
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\window.cpp

$(O)\wm.obj: $(S)\wm.cpp $(I)\wm.hpp $(I)\window.hpp \
 $(I)\group.hpp $(I)\array.h $(I)\widget.hpp \
 $(I)\box.h $(I)\event.h $(I)\keyboard.h \
 $(I)\cursor.h $(I)\wmhelper.hpp $(I)\screen.h \
 $(I)\hw.h $(I)\mouse.h $(I)\cursor.h \
 $(I)\event.h $(I)\array.h $(I)\screen.h \
 $(I)\clip.h $(I)\matrix.h $(I)\poll.h \
 $(I)\skin.h
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\wm.cpp

$(O)\wmhelper.obj: $(S)\wmhelper.cpp $(I)\wmhelper.hpp \
 $(I)\screen.h $(I)\window.hpp $(I)\group.hpp \
 $(I)\array.h $(I)\widget.hpp $(I)\box.h \
 $(I)\event.h $(I)\keyboard.h $(I)\cursor.h \
 $(I)\wm.hpp $(I)\wmmenu.hpp $(I)\menuitem.h \
 $(I)\menu.hpp $(I)\mouse.h $(I)\block.h \
 $(I)\skin.h
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\wmhelper.cpp

$(O)\wmmenu.obj: $(S)\wmmenu.cpp $(I)\wmmenu.hpp $(I)\menuitem.h \
 $(I)\array.h $(I)\keyboard.h $(I)\wmhelper.hpp \
 $(I)\screen.h $(I)\window.hpp $(I)\group.hpp \
 $(I)\widget.hpp $(I)\box.h $(I)\event.h \
 $(I)\cursor.h
	$(CXX) $(CXXFLAGS) /Fo$@ $(S)\wmmenu.cpp

clean:
    @if exist $(O)\*.obj del $(O)\*.obj
    @if exist $(L)\fdtuivs.lib del $(L)\fdtuivs.lib
    @if exist $(L)\*.bak del $(L)\*.bak
    @if exist *.err del *.err
