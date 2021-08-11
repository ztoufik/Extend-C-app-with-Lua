SHELL := /bin/bash

#your output program
outputexe=LuaInt
#your source file
src=$(shell ls *.c)
#your ob file
obj=$(shell for file in $$(ls *.c);do echo $$file |awk -F '.' '{print $$1".o"}';done;)
#download lua source code from https://www.lua.org/ftp/lua-5.4.3.tar.gz
lua_source_path=/home/toufik/.src/lua-5.4.3/src/
#include lua source path in include dirs path search during compiling process
bflags=-iquote$(lua_source_path) -c
#include lua source path in library dirs path search during linking process
lflags=-L$(lua_source_path) -ldl
#library name for gcc to link
lualibs=-llua -lm
DEPS=$(shell ls *.h)
CC=gcc

%.o: %.c $(DEPS)
	$(CC)  $(bflags) $(src) -o $@
$(outputexe): $(obj)
	$(CC) $(lflags) $^ -o $(outputexe) $(lualibs)

clean:
	rm  *.o $(outputexe)
