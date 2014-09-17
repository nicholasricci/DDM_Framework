# SortMatching
# Copyright 2012 Marco Mandrioli
#
# This file is part of SortMatching.
#
# SortMatching is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# SortMatching is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with SortMatching.  If not, see <http://www.gnu.org/licenses/>.


INCDIR		= ./include
SRCDIR		= ./src
OBJDIR		= ./obj
PROGDIR		= ./bin

PROG		= sort_matching_standard

CC		= gcc

CFLAGS		= -Wall
LFLAGS		=
X86FLAGS	= -m32
X64FLAGS	= -m64
OPTFLAGS	= -O3
DBGFLAGS	= -g

X86		= x86
X64		= amd64
DEBUG		= Debug
RELEASE		= Release

RM		= rm -f



all:
	$(MAKE) $(MAKEFILE) x86_release
	$(MAKE) $(MAKEFILE) x86_debug
	$(MAKE) $(MAKEFILE) x64_release
	$(MAKE) $(MAKEFILE) x64_debug
	@echo
	@echo Success.
	@echo
	@echo To launch the program type:
	@echo $(PROGDIR)/$(PROG).$(X86).$(RELEASE) for the x86 release version
	@echo $(PROGDIR)/$(PROG).$(X86).$(DEBUG) for the x86 debug version
	@echo $(PROGDIR)/$(PROG).$(X64).$(RELEASE) for the x64 release version
	@echo $(PROGDIR)/$(PROG).$(X64).$(DEBUG) for the x64 debug version
	@echo


x86:
	$(MAKE) $(MAKEFILE) x86_release
	$(MAKE) $(MAKEFILE) x86_debug
	@echo
	@echo Success.
	@echo
	@echo To launch the program type:
	@echo $(PROGDIR)/$(PROG).$(X86).$(RELEASE) for the x86 release version
	@echo $(PROGDIR)/$(PROG).$(X86).$(DEBUG) for the x86 debug version
	@echo


x64:
	$(MAKE) $(MAKEFILE) x64_release
	$(MAKE) $(MAKEFILE) x64_debug
	@echo
	@echo Success.
	@echo
	@echo To launch the program type:
	@echo $(PROGDIR)/$(PROG).$(X64).$(RELEASE) for the x64 release version
	@echo $(PROGDIR)/$(PROG).$(X64).$(DEBUG) for the x64 debug version
	@echo


release:
	$(MAKE) $(MAKEFILE) x86_release
	$(MAKE) $(MAKEFILE) x64_release
	@echo
	@echo Success.
	@echo
	@echo To launch the program type:
	@echo $(PROGDIR)/$(PROG).$(X86).$(RELEASE) for the x86 release version
	@echo $(PROGDIR)/$(PROG).$(X64).$(RELEASE) for the x64 release version
	@echo


debug:
	$(MAKE) $(MAKEFILE) x86_debug
	$(MAKE) $(MAKEFILE) x64_debug
	@echo
	@echo Success.
	@echo
	@echo To launch the program type:
	@echo $(PROGDIR)/$(PROG).$(X86).$(DEBUG) for the x86 debug version
	@echo $(PROGDIR)/$(PROG).$(X64).$(DEBUG) for the x64 debug version
	@echo



x86_release: PLFNAME = $(X86)
x86_release: CFGNAME = $(RELEASE)
x86_release: FLAGS = $(OPTFLAGS) $(X86FLAGS)
x86_release: OBJDIRFULL = $(OBJDIR)/$(PLFNAME)/$(CFGNAME)
x86_release: title $(PROG) out


x86_debug: PLFNAME = $(X86)
x86_debug: CFGNAME = $(DEBUG)
x86_debug: FLAGS = $(DBGFLAGS) $(X86FLAGS)
x86_debug: OBJDIRFULL = $(OBJDIR)/$(PLFNAME)/$(CFGNAME)
x86_debug: title $(PROG) out


x64_release: PLFNAME = $(X64)
x64_release: CFGNAME = $(RELEASE)
x64_release: FLAGS = $(OPTFLAGS) $(X64FLAGS)
x64_release: OBJDIRFULL = $(OBJDIR)/$(PLFNAME)/$(CFGNAME)
x64_release: title $(PROG) out


x64_debug: PLFNAME = $(X64)
x64_debug: CFGNAME = $(DEBUG)
x64_debug: FLAGS = $(DBGFLAGS) $(X64FLAGS)
x64_debug: OBJDIRFULL = $(OBJDIR)/$(PLFNAME)/$(CFGNAME)
x64_debug: title $(PROG) out
	

clean: clean_x86 clean_x64

clean_x86: clean_x86_release clean_x86_debug

clean_x64: clean_x64_release clean_x64_debug

clean_release: clean_x86_release clean_x64_release

clean_debug: clean_x86_debug clean_x64_debug


clean_x86_release:
	@echo
	@echo Cleaning $(X86) $(RELEASE) files...
	$(RM) $(OBJDIR)/$(X86)/$(RELEASE)/*.o $(PROGDIR)/$(PROG).$(X86).$(RELEASE)

clean_x86_debug:
	@echo
	@echo Cleaning $(X86) $(DEBUG) files...
	$(RM) $(OBJDIR)/$(X86)/$(DEBUG)/*.o $(PROGDIR)/$(PROG).$(X86).$(DEBUG)
	
clean_x64_release:
	@echo
	@echo Cleaning $(X64) $(RELEASE) files...
	$(RM) $(OBJDIR)/$(X64)/$(RELEASE)/*.o $(PROGDIR)/$(PROG).$(X64).$(RELEASE)

clean_x64_debug:
	@echo
	@echo Cleaning $(X64) $(DEBUG) files...
	$(RM) $(OBJDIR)/$(X64)/$(DEBUG)/*.o $(PROGDIR)/$(PROG).$(X64).$(DEBUG)
	

newdir:
	@echo Creating folder $(OBJDIR)/$(PLFNAME)/$(CFGNAME)...
	mkdir -p $(OBJDIR)/$(PLFNAME)
	mkdir -p $(OBJDIR)/$(PLFNAME)/$(CFGNAME)
	@echo Creating folder $(PROGDIR)...
	mkdir -p $(PROGDIR)


title:
	@echo
	@echo ======================================================
	@echo Making $(PROG).$(PLFNAME).$(CFGNAME) executables
	@echo ======================================================


out:
	@echo
	@echo Success compiling $(PLFNAME) $(CFGNAME).
	@echo To launch the program type $(PROGDIR)/$(PROG).$(PLFNAME).$(CFGNAME)
	@echo


$(PROG): newdir linker

linker: error main matching test_generator utils
	@echo linking $(CFGNAME) version...
	$(CC) $(CFLAGS) $(FLAGS) -o $(PROGDIR)/$(PROG).$(PLFNAME).$(CFGNAME) $(OBJDIRFULL)/error.o $(OBJDIRFULL)/main.o \
	$(OBJDIRFULL)/matching.o $(OBJDIRFULL)/test_generator.o $(OBJDIRFULL)/utils.o $(LFLAGS)


error: $(SRCDIR)/error.c
	@echo compiling error.c....
	$(CC) $(CFLAGS) $(FLAGS) -o $(OBJDIRFULL)/error.o -c $(SRCDIR)/error.c


main: $(SRCDIR)/main.c $(INCDIR)/matching.h $(INCDIR)/test_generator.h $(INCDIR)/utils.h
	@echo compiling main.c....
	$(CC) $(CFLAGS) $(FLAGS) -o $(OBJDIRFULL)/main.o -c $(SRCDIR)/main.c


matching: $(SRCDIR)/matching.c $(INCDIR)/utils.h
	@echo compiling matching.c....
	$(CC) $(CFLAGS) $(FLAGS) -o $(OBJDIRFULL)/matching.o -c $(SRCDIR)/matching.c


test_generator: $(SRCDIR)/test_generator.c
	@echo compiling test_generator.c....
	$(CC) $(CFLAGS) $(FLAGS) -o $(OBJDIRFULL)/test_generator.o -c $(SRCDIR)/test_generator.c


utils: $(SRCDIR)/utils.c
	@echo compiling utils.c....
	$(CC) $(CFLAGS) $(FLAGS) -o $(OBJDIRFULL)/utils.o -c $(SRCDIR)/utils.c


error main matching test_generator utils: $(INCDIR)/types.h
