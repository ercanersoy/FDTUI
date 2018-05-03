# vs.mak
#
# builds FDOSTUI test applications
#
# nmake /F vs.mak
#
# License CC0 PUBLIC DOMAIN
#
# To the extent possible under law, Mark J. Olesen has waived all copyright
# and related or neighboring rights to FDOSTUI Library. This work is published
# from: United States.

CXXFLAGS= /I ..\include /Z7 /D__NT__ /D__FLAT__
LIBS=..\lib\fdtuivs.lib

all: hello.exe menu.exe widget.exe scroller.exe file.exe tree.exe

hello.exe : 
	cl $(CXXFLAGS) /Fehello.exe thello.cpp /link $(LIBS)

menu.exe : 
	cl $(CXXFLAGS) /Femenu.exe tmenu.cpp /link $(LIBS)

widget.exe : 
	cl $(CXXFLAGS) /Fewidget.exe twidget.cpp /link $(LIBS)

scroller.exe : 
	cl $(CXXFLAGS) /Fescroller.exe tscrollr.cpp /link $(LIBS)

file.exe : 
	cl $(CXXFLAGS) /Fefile.exe tfile.cpp /link $(LIBS)

tree.exe : 
	cl $(CXXFLAGS) /Fetree.exe ttree.cpp /link $(LIBS)
    
clean: 
	@if exist *.exe del *.exe
	@if exist *.ilk del *.ilk
	@if exist *.obj del *.obj
	@if exist *.pdb del *.pdb

