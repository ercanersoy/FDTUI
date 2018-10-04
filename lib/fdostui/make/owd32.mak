# owd32.mak
#
# makefile for OpenWatcom 32-bit DPMI 
#
# License CC0 PUBLIC DOMAIN
#
# To the extent possible under law, Mark J. Olesen has waived all copyright
# and related or neighboring rights to FDOSTUI Library. This work is published
# from: United States.
#
#
# Build 32-bit DPMI FDOSTUI library  
# FreeDOS or Windows Host Operating System
# Requries OpenWatcom compiler tools.
# Change into this directory and type 'wmake -f owd32.mak'
#
# ..\obj\owd32 directory will be populated with object files
# ..\lib\fdtuid32.lib is the library that will be created
#
# Compilation flags for external applications
#
#    -3r -mf -bt=DOS

O=..\obj\owd32
B=..\bin
S=..\src
S32=..\src\dos32
L=..\lib
I=..\include

AS=wasm
CC=wcc386
CXX=wpp386

AFLAGS= -3r -mf -bt=DOS -d1 -i=$(I)
CFLAGS= -3r -mf -bt=DOS -d3 -i=$(I)
CXXFLAGS= -3r -mf -bt=DOS -d3 -i=$(I)

TUIOBJ=&
	$(O)\hw.obj &
	$(O)\mouse.obj &
	$(O)\screen.obj &
	$(O)\video.obj &
	$(O)\cursor.obj &
	$(O)\filedlg.obj &
	$(O)\poll.obj &
	$(O)\array.obj &
	$(O)\block.obj &
	$(O)\box.obj &
	$(O)\button.obj &
	$(O)\checkbtn.obj &
	$(O)\clip.obj &
	$(O)\combobox.obj &
	$(O)\draw.obj &
	$(O)\edit.obj &
	$(O)\entry.obj &
	$(O)\fdtuicfg.obj &
	$(O)\file.obj &
	$(O)\gapbuf.obj &
	$(O)\group.obj &
	$(O)\hash.obj &
	$(O)\ini.obj &
	$(O)\inpdlg.obj &
	$(O)\label.obj &
	$(O)\listbox.obj &
	$(O)\matrix.obj &
	$(O)\menubar.obj &
	$(O)\menuitem.obj &
	$(O)\menu.obj &
	$(O)\popup.obj &
	$(O)\radiobtn.obj &
	$(O)\scrllbar.obj &
	$(O)\scroller.obj &
	$(O)\skin.obj &
	$(O)\slider.obj  &
	$(O)\spinner.obj &
	$(O)\strings.obj &
	$(O)\tree.obj &
	$(O)\treeitem.obj &
	$(O)\valuator.obj &
	$(O)\widget.obj &
	$(O)\window.obj &
	$(O)\wmhelper.obj &
	$(O)\wmmenu.obj &
	$(O)\wm.obj 

all: $(L)\fdtuid32.lib

$(L)\fdtuid32.lib: $(TUIOBJ)
	wlib $@ @owd32.lbc

$(O)\hw.obj : $(S32)\hw.asm 
	$(AS) $(AFLAGS) -fo=$@ $(S32)\hw.asm
    
$(O)\mouse.obj : $(S32)\mouse.asm $(I)\mouse.inc
	$(AS) $(AFLAGS) -fo=$@ $(S32)\mouse.asm
    
$(O)\screen.obj : $(S32)\screen.asm 
	$(AS) $(AFLAGS) -fo=$@ $(S32)\screen.asm
    
$(O)\video.obj : $(S32)\video.asm $(I)\video.inc
	$(AS) $(AFLAGS) -fo=$@ $(S32)\video.asm

$(O)\cursor.obj: $(S)\cursor.c $(I)\cursor.h
	$(CC) $(CFLAGS) -fo=$@ $(S)\cursor.c

$(O)\filedlg.obj: $(S)\filedlg.cpp $(I)\filedlg.hpp &
 $(I)\window.hpp $(I)\group.hpp $(I)\array.h &
 $(I)\widget.hpp $(I)\box.h $(I)\event.h &
 $(I)\keyboard.h $(I)\cursor.h $(I)\label.hpp &
 $(I)\draw.h $(I)\screen.h $(I)\skin.h &
 $(I)\entry.hpp $(I)\checkbtn.hpp $(I)\button.hpp &
 $(I)\wm.hpp $(I)\listbox.hpp $(I)\scrllbar.hpp &
 $(I)\slider.hpp $(I)\valuator.hpp $(I)\combobox.hpp
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\filedlg.cpp

$(O)\poll.obj: $(S)\poll.c $(I)\poll.h $(I)\event.h &
 $(I)\keyboard.h
	$(CC) $(CFLAGS) -fo=$@ $(S)\poll.c

$(O)\array.obj: $(S)\array.c $(I)\array.h
	$(CC) $(CFLAGS) -fo=$@ $(S)\array.c

$(O)\block.obj: $(S)\block.c $(I)\block.h $(I)\box.h &
 $(I)\screen.h
	$(CC) $(CFLAGS) -fo=$@ $(S)\block.c

$(O)\box.obj: $(S)\box.c $(I)\box.h
	$(CC) $(CFLAGS) -fo=$@ $(S)\box.c

$(O)\button.obj: $(S)\button.cpp $(I)\button.hpp $(I)\draw.h &
 $(I)\screen.h $(I)\skin.h $(I)\box.h &
 $(I)\widget.hpp $(I)\event.h $(I)\keyboard.h &
 $(I)\cursor.h $(I)\wm.hpp $(I)\window.hpp &
 $(I)\group.hpp $(I)\array.h $(I)\wmhelper.hpp
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\button.cpp

$(O)\checkbtn.obj: $(S)\checkbtn.cpp $(I)\checkbtn.hpp &
 $(I)\button.hpp $(I)\draw.h $(I)\screen.h &
 $(I)\skin.h $(I)\box.h $(I)\widget.hpp &
 $(I)\event.h $(I)\keyboard.h $(I)\cursor.h &
 $(I)\wm.hpp $(I)\window.hpp $(I)\group.hpp &
 $(I)\array.h
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\checkbtn.cpp

$(O)\clip.obj: $(S)\clip.c $(I)\clip.h $(I)\box.h &
 $(I)\screen.h
	$(CC) $(CFLAGS) -fo=$@ $(S)\clip.c

$(O)\combobox.obj: $(S)\combobox.cpp $(I)\combobox.hpp &
 $(I)\entry.hpp $(I)\widget.hpp $(I)\box.h &
 $(I)\event.h $(I)\keyboard.h $(I)\cursor.h &
 $(I)\listbox.hpp $(I)\scrllbar.hpp $(I)\slider.hpp &
 $(I)\valuator.hpp $(I)\array.h $(I)\draw.h &
 $(I)\screen.h $(I)\skin.h $(I)\skin.h &
 $(I)\cursor.h $(I)\block.h $(I)\mouse.h &
 $(I)\wm.hpp $(I)\window.hpp $(I)\group.hpp
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\combobox.cpp

$(O)\draw.obj: $(S)\draw.c $(I)\draw.h $(I)\screen.h &
 $(I)\skin.h $(I)\box.h $(I)\matrix.h $(I)\clip.h
	$(CC) $(CFLAGS) -fo=$@ $(S)\draw.c

$(O)\edit.obj: $(S)\edit.cpp $(I)\edit.hpp $(I)\cursor.h &
 $(I)\scrllbar.hpp $(I)\slider.hpp $(I)\valuator.hpp &
 $(I)\widget.hpp $(I)\box.h $(I)\event.h &
 $(I)\keyboard.h $(I)\gapbuf.h $(I)\wm.hpp &
 $(I)\window.hpp $(I)\group.hpp $(I)\array.h &
 $(I)\skin.h $(I)\screen.h $(I)\draw.h &
 $(I)\skin.h $(I)\screen.h $(I)\matrix.h
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\edit.cpp

$(O)\entry.obj: $(S)\entry.cpp $(I)\entry.hpp $(I)\widget.hpp &
 $(I)\box.h $(I)\event.h $(I)\keyboard.h &
 $(I)\cursor.h $(I)\wm.hpp $(I)\window.hpp &
 $(I)\group.hpp $(I)\array.h $(I)\draw.h &
 $(I)\screen.h $(I)\skin.h $(I)\skin.h
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\entry.cpp

$(O)\fdtuicfg.obj: $(S)\fdtuicfg.c $(I)\fdtuicfg.h $(I)\ini.h &
 $(I)\hash.h $(I)\skin.h $(I)\screen.h
	$(CC) $(CFLAGS) -fo=$@ $(S)\fdtuicfg.c

$(O)\file.obj: $(S)\file.cpp $(I)\file.hpp $(I)\filedlg.hpp &
 $(I)\window.hpp $(I)\group.hpp $(I)\array.h &
 $(I)\widget.hpp $(I)\box.h $(I)\event.h &
 $(I)\keyboard.h $(I)\cursor.h $(I)\label.hpp &
 $(I)\draw.h $(I)\screen.h $(I)\skin.h &
 $(I)\entry.hpp $(I)\checkbtn.hpp $(I)\button.hpp &
 $(I)\wm.hpp $(I)\listbox.hpp $(I)\scrllbar.hpp &
 $(I)\slider.hpp $(I)\valuator.hpp $(I)\combobox.hpp &
 $(I)\screen.h $(I)\wm.hpp $(I)\strings.h
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\file.cpp

$(O)\gapbuf.obj: $(S)\gapbuf.c $(I)\gapbuf.h
	$(CC) $(CFLAGS) -fo=$@ $(S)\gapbuf.c

$(O)\group.obj: $(S)\group.cpp $(I)\group.hpp $(I)\array.h &
 $(I)\widget.hpp $(I)\box.h $(I)\event.h &
 $(I)\keyboard.h $(I)\cursor.h $(I)\skin.h &
 $(I)\screen.h $(I)\matrix.h $(I)\clip.h &
 $(I)\draw.h $(I)\skin.h $(I)\wm.hpp &
 $(I)\window.hpp $(I)\group.hpp
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\group.cpp

$(O)\hash.obj: $(S)\hash.c $(I)\hash.h
	$(CC) $(CFLAGS) -fo=$@ $(S)\hash.c

$(O)\ini.obj: $(S)\ini.c $(I)\ini.h $(I)\hash.h
	$(CC) $(CFLAGS) -fo=$@ $(S)\ini.c

$(O)\inpdlg.obj: $(S)\inpdlg.cpp $(I)\inpdlg.hpp $(I)\window.hpp &
 $(I)\group.hpp $(I)\array.h $(I)\widget.hpp &
 $(I)\box.h $(I)\event.h $(I)\keyboard.h &
 $(I)\cursor.h $(I)\label.hpp $(I)\draw.h &
 $(I)\screen.h $(I)\skin.h $(I)\entry.hpp &
 $(I)\button.hpp
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\inpdlg.cpp

$(O)\label.obj: $(S)\label.cpp $(I)\label.hpp $(I)\draw.h &
 $(I)\screen.h $(I)\skin.h $(I)\box.h &
 $(I)\widget.hpp $(I)\event.h $(I)\keyboard.h &
 $(I)\cursor.h
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\label.cpp

$(O)\listbox.obj: $(S)\listbox.cpp $(I)\listbox.hpp &
 $(I)\scrllbar.hpp $(I)\slider.hpp $(I)\valuator.hpp &
 $(I)\widget.hpp $(I)\box.h $(I)\event.h &
 $(I)\keyboard.h $(I)\cursor.h $(I)\array.h &
 $(I)\skin.h $(I)\screen.h $(I)\draw.h &
 $(I)\skin.h $(I)\clip.h $(I)\matrix.h &
 $(I)\wm.hpp $(I)\window.hpp $(I)\group.hpp &
 $(I)\mouse.h
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\listbox.cpp

$(O)\matrix.obj: $(S)\matrix.c $(I)\matrix.h
	$(CC) $(CFLAGS) -fo=$@ $(S)\matrix.c

$(O)\menubar.obj: $(S)\menubar.cpp $(I)\menubar.hpp &
 $(I)\widget.hpp $(I)\box.h $(I)\event.h &
 $(I)\keyboard.h $(I)\cursor.h $(I)\menuitem.h &
 $(I)\array.h $(I)\menu.hpp $(I)\window.hpp &
 $(I)\group.hpp $(I)\wm.hpp $(I)\skin.h &
 $(I)\screen.h $(I)\draw.h $(I)\skin.h &
 $(I)\mouse.h $(I)\block.h $(I)\poll.h
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\menubar.cpp

$(O)\menu.obj: $(S)\menu.cpp $(I)\menu.hpp $(I)\window.hpp &
 $(I)\group.hpp $(I)\array.h $(I)\widget.hpp &
 $(I)\box.h $(I)\event.h $(I)\keyboard.h &
 $(I)\cursor.h $(I)\menuitem.h $(I)\mouse.h &
 $(I)\wm.hpp $(I)\wmhelper.hpp $(I)\screen.h &
 $(I)\block.h $(I)\skin.h $(I)\screen.h
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\menu.cpp

$(O)\menuitem.obj: $(S)\menuitem.c $(I)\menuitem.h $(I)\array.h &
 $(I)\keyboard.h $(I)\screen.h
	$(CC) $(CFLAGS) -fo=$@ $(S)\menuitem.c

$(O)\popup.obj: $(S)\popup.cpp $(I)\popup.hpp $(I)\menu.hpp &
 $(I)\window.hpp $(I)\group.hpp $(I)\array.h &
 $(I)\widget.hpp $(I)\box.h $(I)\event.h &
 $(I)\keyboard.h $(I)\cursor.h $(I)\menuitem.h &
 $(I)\wm.hpp $(I)\label.hpp $(I)\draw.h &
 $(I)\screen.h $(I)\skin.h $(I)\button.hpp &
 $(I)\mouse.h $(I)\block.h $(I)\screen.h
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\popup.cpp

$(O)\radiobtn.obj: $(S)\radiobtn.cpp $(I)\radiobtn.hpp &
 $(I)\checkbtn.hpp $(I)\button.hpp $(I)\draw.h &
 $(I)\screen.h $(I)\skin.h $(I)\box.h &
 $(I)\widget.hpp $(I)\event.h $(I)\keyboard.h &
 $(I)\cursor.h $(I)\wm.hpp $(I)\window.hpp &
 $(I)\group.hpp $(I)\array.h
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\radiobtn.cpp

$(O)\scrllbar.obj: $(S)\scrllbar.cpp $(I)\scrllbar.hpp &
 $(I)\slider.hpp $(I)\valuator.hpp $(I)\widget.hpp &
 $(I)\box.h $(I)\event.h $(I)\keyboard.h &
 $(I)\cursor.h $(I)\draw.h $(I)\screen.h &
 $(I)\skin.h
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\scrllbar.cpp

$(O)\scroller.obj: $(S)\scroller.cpp $(I)\scroller.hpp &
 $(I)\group.hpp $(I)\array.h $(I)\widget.hpp &
 $(I)\box.h $(I)\event.h $(I)\keyboard.h &
 $(I)\cursor.h $(I)\scrllbar.hpp $(I)\slider.hpp &
 $(I)\valuator.hpp $(I)\wm.hpp $(I)\window.hpp &
 $(I)\skin.h $(I)\screen.h $(I)\draw.h &
 $(I)\skin.h $(I)\matrix.h
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\scroller.cpp

$(O)\skin.obj: $(S)\skin.c $(I)\skin.h $(I)\screen.h
	$(CC) $(CFLAGS) -fo=$@ $(S)\skin.c

$(O)\slider.obj: $(S)\slider.cpp $(I)\slider.hpp $(I)\valuator.hpp &
 $(I)\widget.hpp $(I)\box.h $(I)\event.h &
 $(I)\keyboard.h $(I)\cursor.h $(I)\draw.h &
 $(I)\screen.h $(I)\skin.h $(I)\window.hpp &
 $(I)\group.hpp $(I)\array.h
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\slider.cpp

$(O)\spinner.obj: $(S)\spinner.cpp $(I)\spinner.hpp &
 $(I)\valuator.hpp $(I)\widget.hpp $(I)\box.h &
 $(I)\event.h $(I)\keyboard.h $(I)\cursor.h &
 $(I)\entry.hpp $(I)\draw.h $(I)\screen.h &
 $(I)\skin.h $(I)\skin.h
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\spinner.cpp

$(O)\strings.obj: $(S)\strings.c $(I)\strings.h
	$(CC) $(CFLAGS) -fo=$@ $(S)\strings.c

$(O)\tree.obj: $(S)\tree.cpp $(I)\tree.hpp $(I)\treeitem.hpp &
 $(I)\array.h $(I)\box.h $(I)\scrllbar.hpp &
 $(I)\slider.hpp $(I)\valuator.hpp $(I)\widget.hpp &
 $(I)\event.h $(I)\keyboard.h $(I)\cursor.h &
 $(I)\wm.hpp $(I)\window.hpp $(I)\group.hpp &
 $(I)\skin.h $(I)\screen.h $(I)\matrix.h &
 $(I)\clip.h $(I)\mouse.h $(I)\draw.h $(I)\skin.h
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\tree.cpp

$(O)\treeitem.obj: $(S)\treeitem.cpp $(I)\treeitem.hpp &
 $(I)\array.h $(I)\box.h
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\treeitem.cpp

$(O)\valuator.obj: $(S)\valuator.cpp $(I)\valuator.hpp &
 $(I)\widget.hpp $(I)\box.h $(I)\event.h &
 $(I)\keyboard.h $(I)\cursor.h $(I)\wm.hpp &
 $(I)\window.hpp $(I)\group.hpp $(I)\array.h
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\valuator.cpp

$(O)\widget.obj: $(S)\widget.cpp $(I)\group.hpp $(I)\array.h &
 $(I)\widget.hpp $(I)\box.h $(I)\event.h &
 $(I)\keyboard.h $(I)\cursor.h
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\widget.cpp

$(O)\window.obj: $(S)\window.cpp $(I)\window.hpp $(I)\group.hpp &
 $(I)\array.h $(I)\widget.hpp $(I)\box.h &
 $(I)\event.h $(I)\keyboard.h $(I)\cursor.h &
 $(I)\wmhelper.hpp $(I)\screen.h $(I)\window.hpp
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\window.cpp

$(O)\wm.obj: $(S)\wm.cpp $(I)\wm.hpp $(I)\window.hpp &
 $(I)\group.hpp $(I)\array.h $(I)\widget.hpp &
 $(I)\box.h $(I)\event.h $(I)\keyboard.h &
 $(I)\cursor.h $(I)\wmhelper.hpp $(I)\screen.h &
 $(I)\hw.h $(I)\mouse.h $(I)\cursor.h &
 $(I)\event.h $(I)\array.h $(I)\screen.h &
 $(I)\clip.h $(I)\matrix.h $(I)\poll.h &
 $(I)\skin.h
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\wm.cpp

$(O)\wmhelper.obj: $(S)\wmhelper.cpp $(I)\wmhelper.hpp &
 $(I)\screen.h $(I)\window.hpp $(I)\group.hpp &
 $(I)\array.h $(I)\widget.hpp $(I)\box.h &
 $(I)\event.h $(I)\keyboard.h $(I)\cursor.h &
 $(I)\wm.hpp $(I)\wmmenu.hpp $(I)\menuitem.h &
 $(I)\menu.hpp $(I)\mouse.h $(I)\block.h &
 $(I)\skin.h
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\wmhelper.cpp

$(O)\wmmenu.obj: $(S)\wmmenu.cpp $(I)\wmmenu.hpp $(I)\menuitem.h &
 $(I)\array.h $(I)\keyboard.h $(I)\wmhelper.hpp &
 $(I)\screen.h $(I)\window.hpp $(I)\group.hpp &
 $(I)\widget.hpp $(I)\box.h $(I)\event.h &
 $(I)\cursor.h
	$(CXX) $(CXXFLAGS) -fo=$@ $(S)\wmmenu.cpp

clean: .SYMBOLIC
    @if exist $(O)\*.obj del $(O)\*.obj
    @if exist $(L)\fdtuid32.lib del $(L)\fdtuid32.lib
    @if exist $(L)\*.bak del $(L)\*.bak
    @if exist *.err del *.err
