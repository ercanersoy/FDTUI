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
MAKE=wmake

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

EXE=$(BIN)\fdtui.exe

# Directives

all: $(EXE)

$(EXE): $(OBJECTS)
	$(LINK) $(LINKFLAGS) name $@ file @link.lbc

$(LIB)\kitten\kitten.obj: $(LIB)\kitten\kitten.c
	$(CPP) $(CPPFLAGS) &
-i=$(LIB)\kitten &
$? &
-fo=$@

$(LIB)\fdostui\lib\fdtuid16.lib: $(LIB)\fdostui\make\owd16.mak
	cd $(LIB)\fdostui\make
	$(MAKE) -f owd16.mak
	cd ../../..

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
	@if exist $(EXE) del $(EXE)
	@if exist $(OBJ)\*.obj del $(OBJ)\*.obj
	@if exist $(LIB)\fdostui\obj\owd16\*.obj del $(LIB)\fdostui\obj\owd16\*.obj
	@if exist $(LIB)\fdostui\lib\*.lib del $(LIB)\fdostui\lib\*.lib
	@if exist $(LIB)\kitten\*.obj del $(LIB)\kitten\*.obj
