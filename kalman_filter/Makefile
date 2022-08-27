appname := main

PYTHON_VERSION=3.10

CC=gcc
CXX=g++
RM=rm -f

CXXFLAGS := -Wall -g -std=c++11 -pthread -O2
LDFLAGS := -I/usr/include/python$(PYTHON_VERSION)
LDLIBS := -lpython$(PYTHON_VERSION)

srcfiles := $(shell find . -maxdepth 1 -name "*.cpp")
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

.PHONY: clean
clean:
	$(RM) *~ *.o main

.PHONY: build
build: clean
	$(CXX) $(CXXFLAGS) $(srcfiles) -o $(appname) $(LDFLAGS) $(LDLIBS)


