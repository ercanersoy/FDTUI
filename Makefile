all: dosshell.exe

dosshell.exe: main.obj
	wlink system dos op stack=4096 file main.obj,fdostui\lib\fdtuid16.lib name dosshell.exe

main.obj: main.cpp
	wpp -mm -0 -bt=DOS -d3 -i=fodstui\source\fdostui\include main.cpp
