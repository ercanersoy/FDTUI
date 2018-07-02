# Makefile - Build file
# Written by Ercan Ersoy.

all: dosshell.exe

dosshell.exe: main.obj 3rdparty\fdostui\lib\fdtuid16.lib
	wlink system dos op stack=4096 file main.obj,3rdparty\fdostui\lib\fdtuid16.lib name dosshell.exe

main.obj: main.cpp
	wpp -mm -0 -bt=DOS -d3 -i=3rdparty\fdostui\include main.cpp

clean-obj: .SYMBOLIC
	@if exist main.obj del main.obj
