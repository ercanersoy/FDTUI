# oww32.mak
#
# builds FDOSTUI test applications
#
# wmake -f oww32.mak
#
# License CC0 PUBLIC DOMAIN
#
# To the extent possible under law, Mark J. Olesen has waived all copyright
# and related or neighboring rights to FDOSTUI Library. This work is published
# from: United States.

CXXFLAGS= -i=..\include -mf -d3 -bt=NT -k4096
LIBS=..\lib\fdtuiw32.lib

all: hello.exe menu.exe widget.exe scroller.exe file.exe tree.exe

hello.exe : 
	wcl386 $(CXXFLAGS) -fe=hello.exe thello.cpp -l=NT $(LIBS)

menu.exe : 
	wcl386 $(CXXFLAGS) -fe=menu.exe tmenu.cpp -l=NT $(LIBS)

widget.exe : 
	wcl386 $(CXXFLAGS) -fe=widget.exe twidget.cpp -l=NT $(LIBS)

scroller.exe : 
	wcl386 $(CXXFLAGS) -fe=scroller.exe tscrollr.cpp -l=NT $(LIBS)

file.exe : 
	wcl386 $(CXXFLAGS) -fe=file.exe tfile.cpp -l=NT $(LIBS)

tree.exe : 
	wcl386 $(CXXFLAGS) -fe=tree.exe ttree.cpp -l=NT $(LIBS)
    
clean: .SYMBOLIC
	@if exist *.exe del *.exe
	@if exist *.lnk del *.lnk
	@if exist *.obj del *.obj
	@if exist *.map del *.map
	@if exist *.err del *.err

