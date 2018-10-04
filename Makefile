# Makefile - Build FreeDOS TUI Shell
# Written by Ercan Ersoy.

# Directory variables

LIB=lib
INCLUDE=include
SRC=src
OBJ=bin
BIN=bin

# Command variables

CPP=wpp
LINK=wlink

# Command flags

CPPFLAGS=-mm -0 -bt=DOS
LINKFLAGS=system dos op stack=4096 file 

# Objects

OBJECTS=$(OBJ)\dosshell.obj &
$(LIB)\kitten\kitten.obj &
$(LIB)\fdostui\lib\fdtuid16.lib

# Directives

all: $(BIN)\dosshell.exe

$(BIN)\dosshell.exe: $(OBJECTS)
	$(LINK) $(LINKFLAGS) &
$(OBJ)\dosshell.obj, &
$(LIB)\kitten\kitten.obj, &
$(LIB)\fdostui\lib\fdtuid16.lib
 

$(OBJ)\dosshell.obj: $(SRC)\dosshell.cpp
	$(CPP) $(CPPFLAGS) &
-i=$(LIB)\fdostui\include &
-i=$(LIB)\kitten &
-i=$(INCLUDE) &
$? &
-fo=$@

clean-obj: .SYMBOLIC
	@if exist $(OBJ)\*.obj del $(OBJ)\*.obj
