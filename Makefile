# Makefile - Build file
# Written by Ercan Ersoy.

all: dosshell.exe

dosshell.exe: main.obj 3rdparty\kitten\kitten.obj 3rdparty\fdostui\lib\fdtuid16.lib
	wlink system dos op stack=4096 file main.obj,3rdparty\kitten\kitten.obj,3rdparty\fdostui\lib\fdtuid16.lib
	mv main.exe dosshell.exe

main.obj: main.cpp
	wpp -mm -0 -bt=DOS -d3 -i=3rdparty\fdostui\include -i=3rdparty\kitten main.cpp

3rdparty\kitten\kitten.obj: 3rdparty\kitten\kitten.c
	wcc -mm -0 -bt=DOS -d3 -s -i=3rdparty\kitten 3rdparty\kitten\kitten.c -fo=3rdparty\kitten\kitten.obj

clean-obj: .SYMBOLIC
	@if exist main.obj del main.obj
