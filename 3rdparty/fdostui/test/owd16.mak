# owd16.mak
#
# builds FDOSTUI test applications
#
# wmake -f owd16.mak
#
# License CC0 PUBLIC DOMAIN
#
# To the extent possible under law, Mark J. Olesen has waived all copyright
# and related or neighboring rights to FDOSTUI Library. This work is published
# from: United States.

CXXFLAGS= -i=..\include -mm -d3 -bt=DOS -k4096
LIBS=..\lib\fdtuid16.lib

all: hello.exe menu.exe widget.exe scroller.exe file.exe tree.exe

hello.exe : 
	wcl $(CXXFLAGS) -fe=hello.exe thello.cpp -l=DOS $(LIBS)

menu.exe : 
	wcl $(CXXFLAGS) -fe=menu.exe tmenu.cpp -l=DOS $(LIBS)

widget.exe : 
	wcl $(CXXFLAGS) -fe=widget.exe twidget.cpp -l=DOS $(LIBS)

scroller.exe : 
	wcl $(CXXFLAGS) -fe=scroller.exe tscrollr.cpp -l=DOS $(LIBS)

file.exe : 
	wcl $(CXXFLAGS) -fe=file.exe tfile.cpp -l=DOS $(LIBS)

tree.exe : 
	wcl $(CXXFLAGS) -fe=tree.exe ttree.cpp -l=DOS $(LIBS)
    
clean: .SYMBOLIC
	@if exist *.exe del *.exe
	@if exist *.lnk del *.lnk
	@if exist *.obj del *.obj
	@if exist *.map del *.map
	@if exist *.err del *.err
