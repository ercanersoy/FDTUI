# Makefile - Build FDTUI
# Written by Ercan Ersoy.

# Directory variables

LIB=lib
INCLUDE=include
SRC=src
OBJ=src
BIN=bin

# Command variables

CPP=wpp
LINK=wlink

# Command flags

CPPFLAGS=-mm -0 -bt=DOS
LINKFLAGS=system dos op stack=4096

# Objects

OBJECTS=$(OBJ)\common.obj &
$(OBJ)\fm.obj &
$(OBJ)\run.obj &
$(OBJ)\main.obj &
$(LIB)\kitten\kitten.obj &
$(LIB)\fdostui\lib\fdtuid16.lib

# Executable File

EXE=$(BIN)\dosshell.exe

# Directives

all: $(BIN)\dosshell.exe

$(BIN)\dosshell.exe: $(OBJECTS)
	$(LINK) $(LINKFLAGS) name $@ file @link.lbc
 
$(OBJ)\common.obj: $(SRC)\common.cpp
	$(CPP) $(CPPFLAGS) &
-i=$(LIB)\fdostui\include &
-i=$(LIB)\kitten &
-i=$(INCLUDE) &
$? &
-fo=$@

$(OBJ)\fm.obj: $(SRC)\fm.cpp
	$(CPP) $(CPPFLAGS) &
-i=$(LIB)\fdostui\include &
-i=$(LIB)\kitten &
-i=$(INCLUDE) &
$? &
-fo=$@

$(OBJ)\run.obj: $(SRC)\run.cpp
	$(CPP) $(CPPFLAGS) &
-i=$(LIB)\fdostui\include &
-i=$(LIB)\kitten &
-i=$(INCLUDE) &
$? &
-fo=$@

$(OBJ)\main.obj: $(SRC)\main.cpp
	$(CPP) $(CPPFLAGS) &
-i=$(LIB)\fdostui\include &
-i=$(LIB)\kitten &
-i=$(INCLUDE) &
$? &
-fo=$@

clean: .SYMBOLIC
	@if exist $(BIN)\dosshell.exe del $(BIN)\dosshell.exe
	@if exist $(OBJ)\*.obj del $(OBJ)\*.obj
