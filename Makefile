# Makefile for use with emacs and emacs's flymake mode
# Copyright (C) raodm@miamiOH.edu
#
# From emacs menu select Tools->compile
# To compile one file use the syntax:
#     make SRC=hello.cpp
# The compile many files use the syntax (change my_exe_name below):
#     make many EXE=my_exe_name
#

.PHONY: check-syntax all clean style many style-many

CXX=g++
CXXFLAGS=-std=c++11 -Wall -g -Wextra
LIBS=-lm -lpthread

# Target exectuable name if SRC is defined
ifdef SRC
OBJ=$(patsubst %.cpp, %.o, $(SRC))
EXE=$(patsubst %.cpp, %,   $(SRC))
endif

# Variables to conditionally download cpplint.py
have_lint=$(wildcard cpplint.py)
ifneq ('$(have_lint)', 'cpplint.py')
WGET=get-lint
endif

all: build style

build: $(SRC)
ifeq (,$(findstring .h, $(SRC)))
	$(CXX) $(CXXFLAGS) $(SRC) $(LIBS) -o $(EXE)
endif

compile: $(SRC)
ifeq (,$(findstring .h, $(SRC)))
	$(CXX) -c $(CXXFLAGS) $(SRC) $(LIBS) -o $(OBJ)
endif

check-syntax:
	$(CXX) $(CXXFLAGS) -fsyntax-only $(CHK_SOURCES)

style: $(WGET)
	./cpplint.py $(SRC)

get-lint:
	wget -q http://pc2lab.cec.miamiOH.edu/documents/cpplint.py
	chmod +x cpplint.py

many: compile-many style-many

compile-many:
ifndef EXE
	@echo Specify target executable name via command-line EXE=your_exe_name
	@exit 2
endif
	$(CXX) $(CXXFLAGS) $(LIBS) *.cpp -o $(EXE)

style-many: $(WGET)
	$(eval SRCS:=$(wildcard *.h *.cpp))
	./cpplint.py $(SRCS)
