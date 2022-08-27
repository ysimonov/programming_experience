appname := test_matplotlib

CC=gcc
CXX=g++
RM=rm -f

CXXFLAGS := -Wall -g -std=c++11 -pthread -O2
LDFLAGS := -I/usr/include/python3.10
LDLIBS := -lpython3.10

srcfiles := $(shell find . -maxdepth 1 -name "*.cpp")
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

all: $(appname)

$(appname):
	$(CXX) $(CXXFLAGS) $(srcfiles) -o $(appname) $(LDFLAGS) $(LDLIBS)

.PHONY: clean
clean:
	$(RM) *~ *.o test_matplotlib
