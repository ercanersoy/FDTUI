# owd32.mak
#
# builds FDOSTUI test applications
#
# wmake -f owd32.mak
#
# License CC0 PUBLIC DOMAIN
#
# To the extent possible under law, Mark J. Olesen has waived all copyright
# and related or neighboring rights to FDOSTUI Library. This work is published
# from: United States.

CXXFLAGS= -i=..\include -mf -d3 -bt=DOS -k4096
LIBS=..\lib\fdtuid32.lib

all: hello.exe menu.exe widget.exe scroller.exe file.exe tree.exe

hello.exe : 
	wcl386 $(CXXFLAGS) -fe=hello.exe thello.cpp -l=causeway $(LIBS)

menu.exe : 
	wcl386 $(CXXFLAGS) -fe=menu.exe tmenu.cpp -l=causeway $(LIBS)

widget.exe : 
	wcl386 $(CXXFLAGS) -fe=widget.exe twidget.cpp -l=causeway $(LIBS)

scroller.exe : 
	wcl386 $(CXXFLAGS) -fe=scroller.exe tscrollr.cpp -l=causeway $(LIBS)

file.exe : 
	wcl386 $(CXXFLAGS) -fe=file.exe tfile.cpp -l=causeway $(LIBS)

tree.exe : 
	wcl386 $(CXXFLAGS) -fe=tree.exe ttree.cpp -l=causeway $(LIBS)
    
clean: .SYMBOLIC
	@if exist *.exe del *.exe
	@if exist *.lnk del *.lnk
	@if exist *.obj del *.obj
	@if exist *.map del *.map
	@if exist *.err del *.err
