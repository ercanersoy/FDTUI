# dmw32.mak
#
# builds FDOSTUI test applications
#
# make -f dmw32.mak
#
# License CC0 PUBLIC DOMAIN
#
# To the extent possible under law, Mark J. Olesen has waived all copyright
# and related or neighboring rights to FDOSTUI Library. This work is published
# from: United States.

CXX=dmc
LINK=optlink
CXXFLAGS= -I..\include -D__NT__ -D__FLAT__
LIBS=..\lib\fdtuidm.lib 

all: hello.exe menu.exe widget.exe scroller.exe file.exe tree.exe

hello.exe : thello.obj
	$(LINK) $?, $@, ,$(LIBS)

menu.exe : tmenu.obj
	$(LINK) $?, $@, ,$(LIBS)

widget.exe : twidget.obj
	$(LINK) $?, $@, ,$(LIBS)

scroller.exe : tscrollr.obj
	$(LINK) $?, $@, ,$(LIBS)

file.exe : tfile.obj
	$(LINK) $?, $@, ,$(LIBS)

tree.exe : ttree.obj
	$(LINK) $?, $@, ,$(LIBS)

.cpp.obj:
	$(CXX) $(CXXFLAGS) -c $<
    
clean: 
	@if exist *.exe del *.exe
	@if exist *.obj del *.obj
	@if exist *.map del *.map
