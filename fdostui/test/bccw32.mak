# bccw32.mak
#
# builds FDOSTUI test applications
#
# make /f bccw32.mak
#
# License CC0 PUBLIC DOMAIN
#
# To the extent possible under law, Mark J. Olesen has waived all copyright
# and related or neighboring rights to FDOSTUI Library. This work is published
# from: United States.

CXX=bcc32
LINK=tlink32 
LINKFLAGS=-ap -x
CXXFLAGS= -I..\include -D__NT__ 
LIBPATH="C:\BORLAND C++\BorlandC\BC5\LIB"
LIBS=..\lib\fdtuibcc.lib import32.lib cw32.lib
RUNOBJ=C0X32.OBJ

all: hello.exe menu.exe widget.exe scroller.exe file.exe tree.exe

hello.exe : thello.obj
	$(LINK) $(LINKFLAGS) -L$(LIBPATH) $(RUNOBJ) $?, $@, ,$(LIBS)

menu.exe : tmenu.obj
	$(LINK) $(LINKFLAGS) -L$(LIBPATH) $(RUNOBJ) $?, $@, ,$(LIBS)

widget.exe : twidget.obj
	$(LINK) $(LINKFLAGS) -L$(LIBPATH) $(RUNOBJ) $?, $@, ,$(LIBS)

scroller.exe : tscrollr.obj
	$(LINK) $(LINKFLAGS) -L$(LIBPATH) $(RUNOBJ) $?, $@, ,$(LIBS)

file.exe : tfile.obj
	$(LINK) $(LINKFLAGS) -L$(LIBPATH) $(RUNOBJ) $?, $@, ,$(LIBS)

tree.exe : ttree.obj
	$(LINK) $(LINKFLAGS) -L$(LIBPATH) $(RUNOBJ) $?, $@, ,$(LIBS)

.cpp.obj:
	$(CXX) $(CXXFLAGS) -c $<
    
clean: 
	@if exist *.exe del *.exe
	@if exist *.obj del *.obj
	@if exist *.map del *.map
